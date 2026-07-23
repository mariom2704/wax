#include "stdafx.h"
#include "..\rohrkonst\Arbeitsplan.h"

	#include "ArbeitsplanGewindeSammelauftrag.h"

ArbeitsplanGewindeSammelauftrag::ArbeitsplanGewindeSammelauftrag(SammelAuftrag* pSammelAuftrag)
{
	m_pMaschinenlisten = new WaxDruck::Maschinenlisten(pSammelAuftrag);
	Arbeitsplan& arbeit = pSammelAuftrag->get_werkauftrag()->get_arbeit();
	m_pArbeitsplan = &arbeit;
	m_pBeschichtung = &pSammelAuftrag->get_werkauftrag()->get_beschichtung();
	
	m_iFertigungsstaette = pSammelAuftrag->get_werkauftrag()->Get_Fertigungsstaette();

};


void ArbeitsplanGewindeSammelauftrag::createExtraAutoArbeitSaegen()
{
	

	const int AFO_ROHR_SAEGEN = 4000;

	for (POSITION pos = m_pMaschinenlisten->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( m_pMaschinenlisten->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;

		if (pElement->isEleRohr()) 
		{

			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
					
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_iFertigungsstaette == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("38030");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));
					
					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_iFertigungsstaette,  pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}
	

	for (POSITION pos = m_pMaschinenlisten->zuschnittliste_Gewinde_ab10Stueck_32070.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( m_pMaschinenlisten->zuschnittliste_Gewinde_ab10Stueck_32070.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;
		if (pElement->isEleRohr()) 
		{
			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
		
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_iFertigungsstaette == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("32070");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));
					
					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_iFertigungsstaette, pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}

	for (POSITION pos = m_pMaschinenlisten->zuschnittliste_Gewinde_bis10Stueck_31010.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( m_pMaschinenlisten->zuschnittliste_Gewinde_bis10Stueck_31010.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;
		if (pElement->isEleRohr()) 
		{
			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
		
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_iFertigungsstaette == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("31010");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));
					
					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_iFertigungsstaette,  pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}
};


ArbeitsplanGewindeSammelauftrag::~ArbeitsplanGewindeSammelauftrag(void)
{
}
