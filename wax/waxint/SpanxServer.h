#ifndef __SPANXSERVER__
#define __SPANXSERVER__

/**
Server für Spanx Anforderungen. Es wird eine Named Pipe installiert, 
über die Anfragen ans Wax gestellt werden können.
*/
class WAXINT SpanxServer
{
public:
	/// Installiert die Named Pipe
	SpanxServer();

	/// Beendet die Named Pipe
	~SpanxServer();

	void start(CWnd *parent);

	void write_return_pipe(const CString& data);	


private:
	void writepipe(const CString& data);	
	void writepipe(CString pipename, const CString& data);	


	static UINT spanx_thread(LPVOID window);

	static const CString pipename;
	static const CString pipename2;

	HANDLE spanx_thread_handle;
};


#endif
