#if !defined(AFX_UPDATEASSISTDLG_H__7007C548_0F49_4D8B_BE93_24E28C8D018E__INCLUDED_)
#define AFX_UPDATEASSISTDLG_H__7007C548_0F49_4D8B_BE93_24E28C8D018E__INCLUDED_


#include <tools\stl.h>
#include <controls\listctrl.h>
#include "assistant.h"


class CUpdateAssistDlg : public CDialog
{
public:
	CUpdateAssistDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_UPDATEASSIST_DIALOG };

	ListCtrl list;
	CProgressCtrl progr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void onExecute();

	DECLARE_MESSAGE_MAP()
private:

	/// Führt nach Doppelklick ein Update aus
	void on_do_update();

	void set_actions();

	/// Füllt eine Liste mit Einträgen für eine Ini-Eintrag
	void read_ini(vector<string> &nl, vector<string> &dbs, vector<string> &exe);

	assistant assi;
	
};


#endif 
