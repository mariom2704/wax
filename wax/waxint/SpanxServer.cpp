#include "stdafx.h"


#include "SpanxServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString SpanxServer::pipename("\\\\.\\pipe\\spanxserver");
const CString SpanxServer::pipename2("\\\\.\\pipe\\spanxqueue");
	
SpanxServer::SpanxServer()
{

}

void SpanxServer::start(CWnd *parent)
{
	spanx_thread_handle = AfxBeginThread(spanx_thread, (LPVOID)parent, THREAD_PRIORITY_LOWEST)->m_hThread;
}
	
SpanxServer::~SpanxServer()
{
	/**
	Beendet den Thread, indem das Kommando "STOP THREAD" gesendet wird.
	Danach wird maximal 5 Sekunden gewartet.
	*/
	writepipe("STOP THREAD");
	TRACE("Warte auf Thread Ende...\n");
	WaitForSingleObject(spanx_thread_handle, 5000);
	TRACE("Thread Ende...\n");
}


void SpanxServer::writepipe(const CString& data)
{
	writepipe(pipename, data);
}

void SpanxServer::write_return_pipe(const CString& data)
{
	writepipe(pipename2, data);
}


void SpanxServer::writepipe(CString pipename, const CString& data)
{
	
	HANDLE hPipe; 
	LPVOID lpvMessage = (LPVOID)(LPCSTR)data;  
	BOOL fSuccess; 
	DWORD cbWritten, dwMode; 

	hPipe = CreateFile( 
		pipename,   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE, 
		0,              // no sharing 
		NULL,           // no security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	// Break if the pipe handle is valid. 

	if (hPipe != INVALID_HANDLE_VALUE) 
	{
		dwMode = PIPE_READMODE_MESSAGE; 
		fSuccess = SetNamedPipeHandleState( 
			hPipe,    // pipe handle 
			&dwMode,  // new pipe mode 
			NULL,     // don't set maximum bytes 
			NULL);    // don't set maximum time 
		if (!fSuccess) 
			ASSERT(FALSE);

		// Send a message to the pipe server. 
		fSuccess = WriteFile( 
			hPipe,                  // pipe handle 
			lpvMessage,             // message 
			strlen((char*)lpvMessage) + 1, // message length 
			&cbWritten,             // bytes written 
			NULL);                  // not overlapped 
		if (! fSuccess) 
			ASSERT(FALSE);
	}
	CloseHandle(hPipe); 

}
	
UINT SpanxServer::spanx_thread(LPVOID window)
{
	/**
	The main loop creates an instance of the named pipe and 
	then waits for a client to connect to it. When the client 
	connects, a thread is created to handle communications 
	with that client, and the loop is repeated. 

	
	*/
 
	CWnd *exec_wnd = (CWnd*)window;
	
	BOOL connected; 
	HANDLE pipe; 
 	pipe = CreateNamedPipe( 
          pipename,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
          PIPE_TYPE_MESSAGE |       // message type pipe 
          PIPE_READMODE_MESSAGE |   // message-read mode 
          PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
          255,                  // output buffer size 
          255,                  // input buffer size 
          10000,             // client time-out 
          NULL);                    // no security attribute 
	
	if (pipe != INVALID_HANDLE_VALUE) 
	{
		map<string, int> cmdlst;
		cmdlst.insert(make_pair((string)"EDIT_STRANGROHR", ID_SPANXANFRAGE_STRANGROHR));
		cmdlst.insert(make_pair((string)"EDIT_VERTEILER", ID_SPANXANFRAGE_VERTEILER));
		cmdlst.insert(make_pair((string)"STOP THREAD", 0));

		for (;;) 
		{
			connected = ConnectNamedPipe(pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 

			if (connected) 
			{ 
				bool stop = false;
				DWORD len = 0;
				static char buf[4096];
				memset(buf, 0, 4096);
				ReadFile(pipe, buf, 4096, &len, NULL);
				TRACE("Daten empfangen: %s [Länge:%i]\n", ((CString)buf).Left(225), len);
				CString res = buf;

				map<string, int>::iterator it;

				for (it = cmdlst.begin(); it != cmdlst.end(); it++)
				{
					string cmd = it->first;
					string data(buf, cmd.length());
					if (cmd == data)
					{
						if (!it->second)
						{
							stop = true;
							break;
						}
						exec_wnd->PostMessage(it->second, 0, (LPARAM)&buf[cmd.length()]);
					}
				}
				DisconnectNamedPipe(pipe);
				if (stop)
					break;
			} 
		}
	}
	CloseHandle(pipe); 
	return 0;

}
