#include "stdafx.h"



#include "ArbeitSammelEingabeform.h"


IMPLEMENT_DYNCREATE(ArbeitSammelEingabeForm, ArbeitEingabeForm)

BEGIN_MESSAGE_MAP(ArbeitSammelEingabeForm, ArbeitEingabeForm)
END_MESSAGE_MAP()
	
/// Konstruktor
ArbeitSammelEingabeForm::ArbeitSammelEingabeForm()
{}

/// Destruktor
ArbeitSammelEingabeForm::~ArbeitSammelEingabeForm()
{}


void ArbeitSammelEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	if (!validWerkauftrag())
		return;

	if (!pDX->m_bSaveAndValidate)
		prepareArbeitsfolgen();
	
	ArbeitEingabeForm::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
		moveArbeitsfolgen();
}

void ArbeitSammelEingabeForm::updateSpecialAF(Arbeitsfolge* afo)
{
	SammelAuftrag *sa = dynamic_cast<SammelAuftrag*>(get_data());
	sa->setMaxLaengeUndGewicht();
	get_data()->get_werkauftrag()->get_arbeit().updateSpecialAF(afo, get_data()->get_werkauftrag()->get_beschichtung(), sa->get_gesamtgewicht(), sa->get_maxlaenge(), 
		get_data()->get_werkauftrag()->get_status().get_wanr());
}

void ArbeitSammelEingabeForm::prepareArbeitsfolgen()
{
	SammelAuftrag *sa = dynamic_cast<SammelAuftrag*>(get_data());

	if(!m_bClearArbeitsfolgen)
	{
		COIIVerteiler* pCOIIVerteiler = NULL;
		
		WaxPart* sub = sa->getFirstSubobject();
		if(sub)
		{
			pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(sub->get_werkauftrag());
		}
		if(pCOIIVerteiler == NULL)
		{
			///Afo Ruesten fuer den gesamten Auftrag ermitteln
			sa->remove_doppelteRuestzeiten();
		}
	}
	
	sa->copyAfosToSammelauftrag(m_bClearArbeitsfolgen);
	
	m_bClearArbeitsfolgen = true;
	
}




void ArbeitSammelEingabeForm::moveArbeitsfolgen()
{
	SammelAuftrag *sa = dynamic_cast<SammelAuftrag*>(get_data());
	sa->setAfosAndMoveToFirstPos();
}

