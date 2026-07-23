/*
I wrote an auth daemon for winders a while ago, then lost the source.
People kept bugging me about it, so in some spare time I hacked this up.
For all it counts, it's under the latest GNU license.

I used gnu-win32 beta 19.1 by Cygnus.  To build a daemon that goes in
the background, type:
gcc -Wl,--subsystem,windows -O3 -s -o identd.exe identd.c

-Ben

Modified to work through WinSock only by Gena Svarovski (svarovsk@inp.nsk.su)
09/05/98

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#include <wtypes.h>
#include <windowsx.h>
#include <winsock.h>

#define VERSION "version 3.0"

/*
fd_setsize  how many file descriptors can be help in a
file descriptor set.  We use select on a set of file descriptors to wait
for, so we don't want to open more sockets than we make room for here.
*/
#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

#define ARR_SIZE 1024   /* generic string/buffer length */
#define MAX_USERS 50    /* MAX_USERS must NOT be > FD_SETSIZE - 1 */
#define ALARM_TIME 5   /* alarm time in seconds (must not exceed 60) */
#define TIME_OUT 180     /* login timeout in sec - can't be < ALARM_TIME */
#define PORT 113        /* tcp port to listen to */

#define USERVAR         "USER" /* environment variable for userid */

typedef struct {
 void *next;            /* next connection in the list */
 char buffer[ARR_SIZE], /* input buffer */
      site[80];         /* remote site for this connection */
 int  sock,             /* socket handle */
      port,             /* remote tcp port for this connection */
      bufpos,
      last_input;       /* last input time, so we can disconnect idle connections */
} CONNECTION;

CONNECTION *head=NULL,  /* the beginning of the list of connections */
           *tail=NULL;  /* the end of the list of connections */

char mess[ARR_SIZE],    /* functions use mess to send output */
     mess2[ARR_SIZE],   /* for event functions output */
     start_time[30],    /* startup time */
     syslog_name[80],   /* syslog filename */
     localhost[80];     /* local hostname */

int  connect_count=0,   /* count of connections */
     timestamp_on=1,    /* whether to timestamp syslog entries or not */
     connect_str_size=0;/* size of CONNECTION structure */

char Name[]="Identd3";
int  TimeToQuit;

/* prototypes */
void *create_user(void);
void strtolower(char *);
void write_syslog(char *);
void check_timeout(void);
void sigcall(void);
int  user_quit(CONNECTION *);
void write_connection(CONNECTION *, char *);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int  SelectBlockingHook(void);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lprszCmdParam,
                   int nCmdShow) {

struct sockaddr_in  bind_addr,
                    acc_addr;
struct hostent      *host;
fd_set              readmask;
unsigned int        addr;
int                 listen_sock,
                    accept_sock,
                    len,
                    on,
                    i,
                    k,
                    size,
                    port1,
                    port2,
                    resolv_for_log=0;

char                inpstr[ARR_SIZE],
                    site_num[80],
                    userid[512],
                    osname[513],
                    *cp;

unsigned char       c;
CONNECTION          *user;

WORD		           wVersionRequested;
WSADATA 	           wsaData;
int 		           err;

LPTSTR		        uname[512];
DWORD               ulen=512;

struct timeval      tv;

HWND                hwnd;
WNDCLASS            W;

HICON               hIcon;
NOTIFYICONDATA      nid;

int                 argc;
char               *argv[20]; /* should be enough */

/* initiallize some variables */
memset(syslog_name, 0, sizeof(syslog_name));
strcpy(osname, "UNIX");
size=sizeof(struct sockaddr_in);
connect_str_size=sizeof(CONNECTION);

/* get username from somewhere */
if ( GetUserName(uname, &ulen) ) strncpy(userid, uname, ulen);
else if (getenv(USERVAR)!=NULL) strncpy(userid, getenv(USERVAR), 512);
     else strcpy(userid, "unknown");

/* parse command line */
argc=i=0;
while(lprszCmdParam[i]!=0) {
 argv[argc++]=(char*)lprszCmdParam[i];
 while(!isspace(lprszCmdParam[i]) && lprszCmdParam[i]!=0) i++;
 while( isspace(lprszCmdParam[i]) && lprszCmdParam[i]!=0) lprszCmdParam[i++]=0;
}

for (i=1; i<argc; i++) {
 if (i+1<argc) {
  if (!strcmp(argv[i], "--userid"))
   { strncpy(userid, argv[++i], 512); continue; }
  if (!strcmp(argv[i], "--osname"))
   { strncpy(osname, argv[++i], 512); continue; }
  if (!strcmp(argv[i], "--logfile"))
   { strncpy(syslog_name, argv[++i], 79); resolv_for_log=1; continue; }
  if (!strcmp(argv[i], "--noresolv"))
   { resolv_for_log=0; continue; }
 }
 printf("%s %s\n", argv[0], VERSION);
 puts(" --userid <userid>\tSets the userid identd will reply.");
 puts(" --osname <osname>\tSets the operating system name identd will reply.");
 puts(" --logfile <filename>\tSets logging on, and specifies which file to log to.");
 puts(" --noresolv\t\tDo not resolve the remote host for the logs.");
 exit(1);
}

if (!syslog_name[0]) resolv_for_log=0;

wVersionRequested = MAKEWORD( 1, 1 );

err = WSAStartup( wVersionRequested, &wsaData );
if ( err != 0 ) {
    /* Tell the user that we couldn't find a useable winsock.dll */
    write_syslog("FAIL: Couldn't find suitable winsock.dll\n");
    exit(1);
}

/* Confirm that the Windows Sockets DLL supports 1.1.*/
/* Note that if the DLL supports versions greater    */
/* than 1.1 in addition to 1.1, it will still return */
/* 1.1 in wVersion since that is the version we      */
/* requested.                                        */

if ( HIBYTE( wsaData.wVersion ) != 1 ||
     LOBYTE( wsaData.wVersion ) != 1 ) {
    /* Tell the user that we couldn't find a useable winsock.dll */
    write_syslog("FAIL: Version 1.1 of winsock.dll is required\n");
    WSACleanup( );
    exit(1);
}

/* The Windows Sockets DLL is acceptable.  Proceed.  */

/* get local hostname */
 gethostname(localhost, 80);
 strtolower(localhost);

 if ((listen_sock=socket(AF_INET, SOCK_STREAM, 0))==-1) {
  write_syslog("FAIL: Couldn't open listen socket\r\n");
  exit(1);
 }

 /* Allow reboots even with TIME_WAITS etc on port */
 on=1;
 setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

 bind_addr.sin_family=AF_INET;
 bind_addr.sin_addr.s_addr=INADDR_ANY;
 bind_addr.sin_port=htons(PORT);
 if (bind(listen_sock, (struct sockaddr *)&bind_addr, size)==-1) {
  write_syslog("FAIL: Couldn't bind to port\r\n");
  exit(1);
 }
 if (listen(listen_sock, 20)==-1) {
  write_syslog("FAIL: Listen error\r\n");
  exit(1);
 }

 sprintf(mess, "Turtle identd %s started on %s\r\n", VERSION, localhost);
 write_syslog(mess);

 tv.tv_sec = ALARM_TIME;
 tv.tv_usec = 0L;

/* hIcon = CreateIcon(hInst, 32, 32, 1, 1, ANDmaskIcon, XORmaskIcon);*/
 hIcon = LoadIcon(hInst, MAKEINTRESOURCE(1));

 /** time to create window **/
 memset(&W, 0, sizeof(WNDCLASS));
 W.style         = CS_HREDRAW | CS_VREDRAW;
 W.lpfnWndProc   = WndProc;
 W.hInstance     = hInst;
 W.hIcon         = hIcon;
 W.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
 W.lpszClassName = Name;
 RegisterClass(&W);

 hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW,
                     10, 10, 600, 400, NULL, NULL, hInst, NULL);
 /* we're not going to show it really */
/* ShowWindow(hwnd, SW_SHOW); */

 /* add icon to taskbar */
 memset(&nid, 0, sizeof(NOTIFYICONDATA));
 nid.cbSize           = sizeof(NOTIFYICONDATA);
 nid.hWnd             = hwnd;
 nid.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE;
 nid.hIcon            = hIcon;
 nid.uCallbackMessage = 0x1111;
 sprintf(nid.szTip, "%s : %d", userid, connect_count);
 Shell_NotifyIcon( NIM_ADD, &nid );

 /* here we will check message queue */
 WSASetBlockingHook((FARPROC)SelectBlockingHook);

 /**** Main program loop. *****/
 while ( !TimeToQuit ) {
  FD_ZERO(&readmask);

  for (user=head; user!=NULL; user=user->next)
   if (user->sock!=-1) FD_SET(user->sock, &readmask);
  FD_SET(listen_sock, &readmask);

  /* wait (assumes errors are nice and not nasty) */
  i = select(FD_SETSIZE, &readmask, 0, 0, NULL);
  if ( i == SOCKET_ERROR ) {
   i = WSAGetLastError();
   continue;
  }
  if ( i ==  0 ) { check_timeout(); continue; }

  /* check for connection to listen socket */
  if ( FD_ISSET(listen_sock, &readmask) ) {
   accept_sock=accept(listen_sock, (struct sockaddr *)&acc_addr, &size);

   if ((user=create_user())==NULL) {
    write_syslog("ERROR: Reached connection limit.\r\n");
    closesocket(accept_sock);
    continue;
   }

   /* get remote internet site */
   strcpy(site_num, inet_ntoa(acc_addr.sin_addr));
   addr=inet_addr(site_num);
   strcpy(user->site, site_num);
   if (resolv_for_log)
    if ((host=gethostbyaddr((char *)&addr, 4, AF_INET)) != NULL)
     strncpy(user->site, host->h_name, 79);

   strtolower(user->site);
   user->port=(unsigned int)ntohs(acc_addr.sin_port);

   user->sock=accept_sock;
   user->last_input=time((time_t *)0);
  }

  /* need extra cycle to be able to restart scanning if user list altered */
  user = head;
  while( user != NULL ) {
   /** cycle through connections **/
   for (user=head; user!=NULL; user=user->next) {

    /* if no data waiting on socket, go to the next socket */
    if (!FD_ISSET(user->sock, &readmask)) continue;

    /* see if client has closed socket */
    inpstr[0]=0;
    if ( (len=recv(user->sock, inpstr, sizeof(inpstr), 0)) <= 0 ) {
     user_quit(user);
     break; /* need to quit cycle anyway to restart list from beginnig */
    }

    inpstr[len]='\0';

    user->last_input=time((time_t *)0);  /* ie now */

    k=user->bufpos;
    for (i=0; i<len; ++i) {
     c=(unsigned char)inpstr[i];

     /* handle eol */
     if ( c=='\n' || c=='\r' || !c || k+5>=ARR_SIZE ) goto GOT_LINE;

     /* handle non 7-bit ascii chars (not RFC compliant) */
     /* if ( c<32 || c>126 ) continue; */

     /* copy the char into buffer */
     user->buffer[k++]=c;
    }
    user->bufpos=k;
    continue;

    GOT_LINE:
    if (k+5>ARR_SIZE) {
     sprintf(mess, "1, 1: ERROR : UNKNOWN-ERROR\r\n");
     write_connection(user, mess);
     sprintf(mess, "ERROR: %s overflowed the buffer\r\n", user->site);
     write_syslog(mess);
     user_quit(user);
     break; /* need to quit cycle anyway to restart list from beginnig */
    }

    user->buffer[k]='\0';
    port1=0;
    port2=0;
    for (cp=user->buffer; *cp && *cp!=','; cp++);
    if (*cp) {
     *cp='\0';
     port1=atoi(user->buffer);
     port2=atoi(cp+1);
    }

    if (!port1 || !port2 || (unsigned)port1>65535 || (unsigned)port2>65535) {
     sprintf(mess, "%d, %d : ERROR : INVALID-PORT\r\n", port1, port2);
     write_connection(user, mess);
     sprintf(mess, "ERROR: %s invalid port %d, %d\r\n", user->site, port1, port2);
     write_syslog(mess);
     user_quit(user);
     break; /* need to quit cycle anyway to restart list from beginnig */
    }

    /* here we could do stuff */
    sprintf(mess, "%d, %d : USERID : %s : %s\r\n", port1, port2, osname, userid);
    write_connection(user, mess);
    sprintf(mess, "GOOD: success for %s on ports %d, %d\r\n", user->site, port1, port2);
    write_syslog(mess);
    user_quit(user);

    /* update tip */
	 connect_count++;
    nid.uFlags = NIF_TIP;
    sprintf(nid.szTip, "%s : %d", userid, connect_count);
    Shell_NotifyIcon( NIM_MODIFY, &nid );

    break; /* need to quit cycle to restart list from beginnig */
   } /* end of foreach user */
  } /* end of while scanning complete*/
 } /* end of neverending whlie */
 Shell_NotifyIcon( NIM_DELETE, &nid );
 WSAUnhookBlockingHook();
 WSACleanup();
 return 1;
} /* end of main */


/* -----
 * user_quit(CONNECTION *user)
 * closes the connection and removes it from the list
 *
 */
int user_quit(CONNECTION *user) {
CONNECTION *i, *j;

 if (user==NULL) return 1;
 if (user->sock>-1) {
  closesocket(user->sock);
 }
 if (user==head) {
  if (tail==head) {
   head=NULL;
   tail=NULL;
   free(user);
   return 0; /* return 0 in only case -- empty list */
  }
  head=head->next;
  free(user);
  return 1;
 }
 for (i=head; i->next!=user && i->next!=NULL; i=i->next);
 j=i->next;
 if (j!=user) {
  write_syslog("ERROR: Programmer error: Connection list is corrupted.\r\n");
  exit(1);
 }
 if( j == tail ) tail = i;
 i->next=j->next;
 j->next=NULL;
 free(user);
 return 1;
}

/* -----
 * void *create_user()
 * allocates some space for the user and puts it on the list
 *
 */
void *create_user(void) {
CONNECTION *new_user;

 if (head==NULL) {
  head=malloc(connect_str_size);
  if (head==NULL) return NULL;
  memset(head, 0, connect_str_size);
  tail=head;
  head->next=NULL;
  return head;
 }

 new_user=malloc(connect_str_size);
 if (new_user==NULL) return NULL;
 memset(new_user, 0, connect_str_size);
 tail->next=new_user;
 new_user->next=NULL;
 tail=new_user;
 return new_user;
}

/* -----
 * strtolower(char *str)
 * strtolower() converts str to lowercase
 *
 */
void strtolower(char *str) {
 if (str==NULL) return;
 while (*str) {
  *str=tolower(*str);
  str++;
 }
}


/*** Send a string to system log ***/
void write_syslog(char *str) {
FILE *fp;
time_t tm;
char timestr[40];

 if ( syslog_name[0]==0 || (fp=fopen(syslog_name, "a"))==0 ) return;
 if (timestamp_on) {
  time(&tm);
  strcpy(timestr, ctime(&tm));
  timestr[strlen(timestr)-1]='\0';
  fprintf(fp, "%s: %s", timestr, str);
 } else fputs(str, fp);
 fclose(fp);
}


/*** send string down socket ***/
void write_connection(CONNECTION *user, char *str) {
 if (user==NULL) return;
 if (user->sock>-1) send(user->sock, str, strlen(str), 0);
}


void check_timeout(void) {
CONNECTION *user;
time_t tm;
int secs;

 tm=time((time_t *)0);

 user = head;
 while( user != NULL ) {
  for (user=head; user!=NULL; user=user->next) {
   secs=(int)tm-user->last_input;
   if (secs>=TIME_OUT) {
    sprintf(mess, "ERROR: %s timed out\r\n", user->site);
    write_syslog(mess);
    user_quit(user);
    break; /* restart user scanning */
   } /* if */
  } /* for each user */
 } /* while scanning is complete */
}

int SelectBlockingHook(void) {
     MSG msg;
     BOOL ret;
     /* get the next message if any */
     ret = (BOOL)PeekMessage(&msg,NULL,0,0,PM_REMOVE);
     /* if we got one, process it */
     if (ret) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
     }
     /* TRUE if we got a message */
     return ret;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
   	case WM_DESTROY:
	      TimeToQuit=1;
   	   if(WSAIsBlocking()) WSACancelBlockingCall();
   		PostQuitMessage(0);
	      return 0;
		default:
      	return DefWindowProc(hWnd, Message, wParam, lParam);
   }
}

