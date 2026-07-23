
#pragma once


class WAXUIDLL COIIVerteilerEingabe : 
public WaxPartForm  
{
private:
	DECLARE_DYNCREATE(COIIVerteilerEingabe)

	int rohrDruckstufe;

	int rohrKFarbe;

	Stationsverteiler *getStationsverteiler();

public:
	//Konstruktor
	COIIVerteilerEingabe();

	//Destruktor
	virtual ~COIIVerteilerEingabe();

	virtual bool check_plausis();


	//{{AFX_VIRTUAL(COIIVerteilerEingabe)
	//Dialog initialisieren
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//{{AFX_MSG(COIIVerteilerEingabe)
	
	DECLARE_MESSAGE_MAP()
public:
	 void OnCbnSelchangeComboVertAbg();
};

