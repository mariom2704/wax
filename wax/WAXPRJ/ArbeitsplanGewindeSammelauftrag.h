#pragma once

class ArbeitsplanGewindeSammelauftrag
{
	WaxDruck::Maschinenlisten* m_pMaschinenlisten;
	Arbeitsplan* m_pArbeitsplan;
	Beschichtung* m_pBeschichtung;
	int m_iFertigungsstaette;
public:
	ArbeitsplanGewindeSammelauftrag(SammelAuftrag* pSammelAuftrag);
	void createExtraAutoArbeitSaegen();
	~ArbeitsplanGewindeSammelauftrag(void);
};
