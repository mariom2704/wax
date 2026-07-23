#pragma once

class WAXPRJDLL Fortschrittanzeige
{
public:
	Fortschrittanzeige(double min, double max, const CString& txt = "");
	Fortschrittanzeige();
	virtual ~Fortschrittanzeige(void);

	///Maximalwert des Range (braucht nicht Prozentual sein)
	void set_maxWert(double max);	

	///Minimalwert des Range. Standard ist 0;
	void set_minWert(double min);	

	//Um wieviel Prozent bei nextStep() vorwärts gesprungen wird
	void set_step(int val); 

	void set_ueberschrift(const CString& txt);



	///Laufende Anzeige
	void setAnzeige1(const CString& txt);
	void setAnzeige2(const CString& txt);
	void setAnzeige3(const CString& txt);

	void updateCounterAnzeige();

	///nächster Step,(braucht nicht Prozentual sein), muß im Range liegen
	///Rückgabewert ist wert in Prozent vom MaxWert
	int nextStep(double wert);

	int nextStep();


	//Dialog starten
	bool start(CWnd* pWnd = 0);

	//Dialog schliessen
	void ende();

	CDialog m_fortschritt;


private:
	double m_max,
		   m_min;

	CString m_ueberschrift,
		    anzeige1,
			anzeige2,
			anzeige3;
		  

	int m_iProzentMerken;

	int step;

	
	CProgressCtrl *m_pCtrlProgress; 

};
