// Mit Microsoft Visual C++ automatisch erstellte IDispatch-Kapselungsklasse(n). 

// HINWEIS: Die Inhalte dieser Datei nicht ändern. Wenn Microsoft Visual C++
// diese Klasse erneuert, werden Ihre Änderungen überschrieben.

/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse CAdViewer 

class CAdViewer : public CWnd
{
protected:
	DECLARE_DYNCREATE(CAdViewer)
public:
	CLSID const& GetClsid();

	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attribute
public:

// Operationen
public:
	unsigned long GetBackColor();
	void SetBackColor(unsigned long newValue);
	BOOL GetEmbedSourceDocument();
	void SetEmbedSourceDocument(BOOL bNewValue);
	CString GetSourcePath();
	void SetSourcePath(LPCTSTR lpszNewValue);
	LPDISPATCH GetViewer();
	void ShowPrintDialog();
	void NavigateToUrl(LPCTSTR bstrUrl);
	void ExecuteCommand(LPCTSTR bstrCommand);
	void DrawToDC(long nDc, long nLeft, long nTop, long nRight, long nBottom);
	VARIANT Get_ActiveWindow();
	VARIANT Get_ClientWindow();
	CString Get_DocumentParams();
	CString Get_HistoryParams();
	CString Get_LocalFilePath();
	void Set_SourceStream(LPUNKNOWN newValue);
	void Set_ViewerParams(LPCTSTR lpszNewValue);
	CString Get_ViewerParams();
	void _GoBack();
	void _GoForward();
	void _SaveHistory();
	void _ShowHelp(LPCTSTR bstrTopic);
};
