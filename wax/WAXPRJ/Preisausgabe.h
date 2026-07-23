#pragma once

#include <mxcontrols/listctrl.h>

class CObList;

class WAXPRJDLL Preisausgabe : 
public CDialog  
{
public:
	enum TYP 
	{
		MATERIAL = 2,

		AFO = 3,

		KOMPLETT = 4,
	
		VK3_KOMPLETT = 6,
	
		VK3_ELEMENTE = 7
	};

	virtual ~Preisausgabe();

	Preisausgabe(Werkauftrag* value);


	int openDialog(int typ, bool gf, CObList& matlist, 
					double summeMatP, double summeMatGK, double summeMatWAP);



private:
	double gesamtpreisVK3;

	Werkauftrag* wa;

	int typ;

	bool gf;

	double summeAfoAV;

	double summeAfoGK;

	double gesamtgewicht;

	double gesamtpreis;

	int maxlaenge;

	double summeMatP;

	double summeMatGK; 
	
	double summeMatWAP; 
	
	double summeAfoP;
	
	double summeAfoWAP; 
	
	double sonderkosten;

	CObList matlist;

	CObList afolist;
	
	ListCtrl list;

	//{{AFX_VIRTUAL(Preisausgabe)
	public:
	virtual void OnOK();

	virtual BOOL OnInitDialog();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(Preisausgabe)
	
	DECLARE_MESSAGE_MAP()

public:
	int openDialog(int typ, bool gf, CObList& elelist, CObList & passlist, CObList &pulverlist, double summeEle);

	void fillVK3Elementlist(int select = -1);

	void fillVK3Komplettlist(int select = -1);

	int openDialog(int typ, bool gf, double summeEle, double sonderkosten, double gesamtpreisVK3);

};
