#include "stdafx.h"


#include "Fortschrittanzeige.h"



Fortschrittanzeige::Fortschrittanzeige(void)
{
	m_max = 100;
	m_min = 0;
	m_iProzentMerken = 0;
	step = 1;
	m_pCtrlProgress = NULL;
}



Fortschrittanzeige::Fortschrittanzeige(double min, double max, const CString& txt)
{
	m_max = max;
	m_min = min;

	m_iProzentMerken = 0;
	step = 1;
	m_pCtrlProgress = NULL;
	m_ueberschrift = txt;
}



Fortschrittanzeige::~Fortschrittanzeige(void)
{
}



///Maximalwert des Range (braucht nicht Prozentual sein)
void Fortschrittanzeige::set_maxWert(double max)
{
	m_max = max;
}

///Minimalwert des Range. Standard ist 0;
void Fortschrittanzeige::set_minWert(double min)
{
	m_min = min;
}


void Fortschrittanzeige::set_step(int val)
{
	step = val;
}



void Fortschrittanzeige::set_ueberschrift(const CString& txt)
{
	m_ueberschrift = txt;
}


///Laufende Anzeige
void Fortschrittanzeige::setAnzeige1(const CString& txt)
{
	if (m_pCtrlProgress)
		m_fortschritt.GetDlgItem(IDC_MSG)->SetWindowText(txt);
	else
		anzeige1 = txt;
}

void Fortschrittanzeige::setAnzeige2(const CString& txt)
{
	if (m_pCtrlProgress)
		m_fortschritt.GetDlgItem(IDC_MSG2)->SetWindowText(txt);
	else
		anzeige2 = txt;
}

void Fortschrittanzeige::setAnzeige3(const CString& txt)
{
	if (m_pCtrlProgress)
		m_fortschritt.GetDlgItem(IDC_MSG3)->SetWindowText(txt);
	else
		anzeige3 = txt;
}



///nächster Step,(braucht nicht Prozentual sein), muß im Range liegen
///Rückgabewert ist wert in Prozent vom MaxWert
int Fortschrittanzeige::nextStep(double wert)
{
	if (!m_pCtrlProgress)
		return 0;

	//Fortschritt berechnen
	int iProzent = (int)(wert / m_max * 100.0);
	if (iProzent != m_iProzentMerken)
		m_pCtrlProgress->SetPos(iProzent);
	m_iProzentMerken = iProzent;

	return iProzent;
}


/**
Um einen Step vorrücken
*/
int Fortschrittanzeige::nextStep()
{
	if (!m_pCtrlProgress)
		return 0;

	//Range
	double range = m_max - m_min;

	//Anzahl Steps
	int anzStep = (int)(range / step);
	if (anzStep == 0)
		anzStep = 1;

	//Step in Prozent
	int stepprozent = (int)(100 / anzStep);
	
	//zum nächsten Step
	m_iProzentMerken += stepprozent;
	m_pCtrlProgress->SetPos(m_iProzentMerken);

	return m_iProzentMerken;
}

/**
x von ges Anzeigen. Macht nur Sinn wen min max Integerzahlen sind.
*/
void Fortschrittanzeige::updateCounterAnzeige()
{
	int ges = (int)(m_max - m_min);
	int counter = (int)((m_max - m_min) * m_iProzentMerken / 100.0);

	CString msg;
	msg.Format("(%i %s %i)", counter, getStammdaten().getText("msg_von").c_str(), ges);

	if (m_pCtrlProgress)
		m_fortschritt.GetDlgItem(IDC_MSG4)->SetWindowText(msg);
}

bool Fortschrittanzeige::start(CWnd* pWnd)
{
	//Methode verlassen, wenn schon gestartet;
	if (m_pCtrlProgress)
		return true;

	//Fortschrittsanzeige initialisieren
	m_fortschritt.Create(IDD_FREIGABEFORTSCHRITT, pWnd);
	m_fortschritt.CenterWindow();
	m_fortschritt.ShowWindow(SW_SHOW);

	//Control ermitteln
	m_pCtrlProgress = (CProgressCtrl*)m_fortschritt.GetDlgItem(IDC_PROG_FREIGABE);

	if (!m_pCtrlProgress)
		return false;

	m_pCtrlProgress->SetPos(0);
	m_pCtrlProgress->SetRange(0, 100); 
	m_pCtrlProgress->SetStep(1);

	if (!m_ueberschrift.IsEmpty())
		m_fortschritt.SetWindowText(m_ueberschrift);

	if (!anzeige1.IsEmpty())
		m_fortschritt.GetDlgItem(IDC_MSG)->SetWindowText(anzeige1);

	if (!anzeige2.IsEmpty())
		m_fortschritt.GetDlgItem(IDC_MSG2)->SetWindowText(anzeige2);

	if (!anzeige3.IsEmpty())
		m_fortschritt.GetDlgItem(IDC_MSG3)->SetWindowText(anzeige3);


	return true;
}

void Fortschrittanzeige::ende()
{
	m_fortschritt.DestroyWindow();

	m_pCtrlProgress = NULL;
}
