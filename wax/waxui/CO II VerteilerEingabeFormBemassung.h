
#pragma once
#include "GridCtrl\WaxGridCtrl.h"
#include "StretchBitmapControl\StretchBitmapControl.h"





class WAXUIDLL COIIVerteilerBemassung : 
public WaxPartForm  
{
protected:

	BOOL	m_bDragging;
	CPoint	m_ptDragPoint;

	HCURSOR m_hCursor1, m_hCursor2;
	int		m_nCurHeight;
	CRect	m_rcOriginalRect;
	int		m_nScrollPos;

	CStretchBitmapControl m_StretchBitmapControl;
	CBrush m_WhiteBrush;
	
	virtual void InitAnfangEndeCheckBoxen(int idn = 0);

	int m_iAnzahlAnschluesse;

	void PopulateGridData();

	bool bDialogHardWorking;

	int m_CheckSicherheitsventil;
	BOOL m_bRadioSicherheitsventilRechts;

	int m_CheckEntLeerung;
	BOOL m_bRadioEntLeerungRechts;


	DECLARE_DYNCREATE(COIIVerteilerBemassung)

	int	elementID;
	int	elementlEnde;
	int	elementrEnde;
	int	rohrDN;
	int	rohrLaenge;
	int iAbstandBock[4];

	CWaxGridCtrl m_GridCtrl;
	COIIVerteilerData *rohr;
	
	int elementDN[COIIVerteiler::MAXCOUNT];
	int elementRelAbst[COIIVerteiler::MAXCOUNT];
	int elementTypAbgangEnde[COIIVerteiler::MAXCOUNT];
	int elementTypGegenFLansch[COIIVerteiler::MAXCOUNT];
	int elementLaengeGegenFLansche[COIIVerteiler::MAXCOUNT];
	
	int elementEndschalterTyp;

	void init();
	void InitializeTheGrid();
	void SetColumns();
	void setColumnComboxDn(int col);
	virtual COIIVerteilerData* getRohrData();



public:
	//Konstruktor
	COIIVerteilerBemassung();

	void SelChangeRohrDN();
	void SelChangeRohrAnfang();
	void SelChangeRohrEnde();


	//Destruktor
	virtual ~COIIVerteilerBemassung();
	
	//set rohrRestlaenge
	void set_rohrRestlaenge();

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
	//aufgerufen wird.
	virtual bool check_plausis();

	//Dialog initialisieren
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	//Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
	//Plausicheck true liefert.
	virtual void commitUpdate();

	
	void setCtrlStatus();
	
	CCellID m_PrecGridCellFocusID;
	

	
	//{{AFX_VIRTUAL(COIIVerteilerBemassung)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	LRESULT OnGridEndEditPosted(WPARAM, LPARAM);
	//{{AFX_MSG(COIIVerteilerBemassung)
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditAnzahlAnschluesse();
	afx_msg void OnEnKillfocusEditAnzahlAnschluesse();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:

};
