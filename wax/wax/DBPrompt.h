#ifndef _INC_DBPROMPT_
#define _INC_DBPROMPT_


/**
Auswahl einer Benutzerdatenbank beim Systemstart
*/
class DBPrompt : public CDialog  
{
	DECLARE_MESSAGE_MAP()
public:
	DBPrompt();

	const CString& get_dbname() const;

private:
	virtual void OnCancel();

	virtual void PreSubclassWindow();

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	virtual BOOL OnInitDialog();
	
	//Name der ausgewählten Datenbank
	CString dbname;

	//Server-Datenbank (Datenbank1) soll geladen werden
	void onClickedServer();
	
	//Lokale Datenbank (Datenbank2) soll geladen werden
	void onClickedLokal();
};

#endif 
