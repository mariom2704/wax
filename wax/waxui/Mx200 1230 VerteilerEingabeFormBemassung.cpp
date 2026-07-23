#include "stdafx.h"

#include "resource.h"
#include <mxutil/ddx_extension.h>

#include <mxcontrols/msgbox.h>
using namespace MxGui;

#include "..\waxui\GridCtrl\InPlaceEdit.h"
#include "..\waxui\GridCtrl\WaxGridCellCombo.h"
#include "../rohrkonst/rohrleitung.h"

	#include "Mx200 1230 VerteilerEingabeFormBemassung.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMx2001230VerteilerEingabeBemassung, COIIVerteilerBemassung)

	
	

BEGIN_MESSAGE_MAP(CMx2001230VerteilerEingabeBemassung, COIIVerteilerBemassung)
	ON_CBN_SELENDOK(IDC_EDIT_DN_VL,		SelChangeRohrDN)
END_MESSAGE_MAP()

CMx2001230VerteilerEingabeBemassung::CMx2001230VerteilerEingabeBemassung() : COIIVerteilerBemassung()
{
	int i = 0;
	
	elementID     = ROHR;
	
	for (i=0; i< COIIVerteiler::MAXCOUNT; i++) 
	{
		elementDN[i]					= 0;
		elementRelAbst[i]				= 0;
		elementTypAbgangEnde[i]			= 0;
		elementTypGegenFLansch[i]		= 0;
		elementLaengeGegenFLansche[i]	= 1230;
	}
	elementEndschalterTyp = 0;
	elementlEnde    = FLANSCH_COII;
	elementrEnde    = FLANSCH_COII; 
	
	iAbstandBock[0] = 0;
	iAbstandBock[1] = 0;
	iAbstandBock[2] = 0;

	m_CheckSicherheitsventil = -1;
	m_bRadioSicherheitsventilRechts = 0;
	m_CheckEntLeerung = -1;
	m_bRadioEntLeerungRechts = 0;
	bDialogHardWorking = false;
	rohrDN          = 100;
	rohrLaenge      = 6000;
	m_iAnzahlAnschluesse = 1;
}



void CMx2001230VerteilerEingabeBemassung::InitAnfangEndeCheckBoxen(int iDn)
{
	BauelementGruppen bg;
	CString str; str = getStammdaten().getText("IDS_LEER").c_str();
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_ANF))->ResetContent();
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_END))->ResetContent();
	
	int ilokalDN = 0;
	if(iDn != 0)
	{
		ilokalDN = iDn;
	}
	else
	{
		ilokalDN = rohrDN;
	}


	if(ilokalDN > 80)
	{
		cbinitdata3 cbinit[] =
		{
			
			//"65"				,	65			,IDC_EDIT_DN_VL,
			//"80"				,   80			,IDC_EDIT_DN_VL,
			//"100"				,   100			,IDC_EDIT_DN_VL,

	
			getStammdaten().getText("BLINDFLANSCH_COII"),			 BLINDFLANSCH_COII,	IDC_COMBO_VERT_ANF,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	 GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_ANF,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,	IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_BODEN_COII"),	 BODEN_COII,	IDC_COMBO_VERT_ANF,
		
		
			getStammdaten().getText("BLINDFLANSCH_COII"),			 BLINDFLANSCH_COII,	IDC_COMBO_VERT_END,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	 GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_END,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,	IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_BODEN_COII"),	 BODEN_COII, IDC_COMBO_VERT_END,
			
			"", 0, 0
		};
		INIT_CBDATA3(cbinit);
		return;
	}


	if(ilokalDN > 65)
	{
		cbinitdata3 cbinit[] =
		{
		
			//"65"				,	65			,IDC_EDIT_DN_VL,
			//"80"				,   80			,IDC_EDIT_DN_VL,
			//"100"				,   100			,IDC_EDIT_DN_VL,

	
			getStammdaten().getText("BLINDFLANSCH_COII"),					BLINDFLANSCH_COII,					 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	GEGENFLANSCH_COII_SCHARZ_HAUPTROHR,  IDC_COMBO_VERT_ANF,
			bg.getBezeichnung(FLANSCH_COII),								FLANSCH_COII,						 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_GEWINDE"),							GEW,								 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_GEWINDE_KAPPE"),					GEWINDE_KAPPE_TYP_D,				 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_BODEN_COII"),						BODEN_COII,							 IDC_COMBO_VERT_ANF,
	
			getStammdaten().getText("BLINDFLANSCH_COII"),					BLINDFLANSCH_COII,					 IDC_COMBO_VERT_END,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	GEGENFLANSCH_COII_SCHARZ_HAUPTROHR,  IDC_COMBO_VERT_END,
			bg.getBezeichnung(FLANSCH_COII),								FLANSCH_COII,						 IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_GEWINDE"),							GEW,								 IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_GEWINDE_KAPPE"),					GEWINDE_KAPPE_TYP_D,				 IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_BODEN_COII"),						BODEN_COII,							 IDC_COMBO_VERT_END,
	
			"", 0, 0
		};
		INIT_CBDATA3(cbinit);
		return;
	}


	if(ilokalDN > 50)
	{
		cbinitdata3 cbinit[] =
		{
		
			//"65"				,	65			,IDC_EDIT_DN_VL,
			//"80"				,   80			,IDC_EDIT_DN_VL,
			//"100"				,   100			,IDC_EDIT_DN_VL,

	
			"Flansch + BL Flansch",			 BLINDFLANSCH_COII,	  IDC_COMBO_VERT_ANF,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,		  IDC_COMBO_VERT_ANF,
			"Gewinde",						 GEW,				  IDC_COMBO_VERT_ANF,
			"Gewinde+Kappe",				 GEWINDE_KAPPE_TYP_D, IDC_COMBO_VERT_ANF,


	
			"Flansch + BL Flansch",			 BLINDFLANSCH_COII,		IDC_COMBO_VERT_END,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,			IDC_COMBO_VERT_END,
			"Gewinde",						 GEW,					IDC_COMBO_VERT_END,
			"Gewinde+Kappe"			,		 GEWINDE_KAPPE_TYP_D,	IDC_COMBO_VERT_END,

	
			"", 0, 0
		};
		INIT_CBDATA3(cbinit);

		CMx2001230Verteiler* pMx2001230Verteiler = dynamic_cast<CMx2001230Verteiler*>(rohr->get_CommonCOMXVerteiler());
		
		if( ilokalDN == 65 && !pMx2001230Verteiler->get_beschichtung().isVerzinkt())
		{
			pMx2001230Verteiler->set_rohrQuali(Rohrleitung::RQ2448NLOS_MX1230200_DN65_GEPULVERT_EXTRA_DICK);
		}
		else
		{
			pMx2001230Verteiler->set_rohrQuali(Rohrleitung::RQ2448NLOS_MX1230200);
		}

		return;
	}
}



//Destruktor
CMx2001230VerteilerEingabeBemassung::~CMx2001230VerteilerEingabeBemassung()
{
}


COIIVerteilerData* CMx2001230VerteilerEingabeBemassung::getRohrData()
{
	return  (COIIVerteilerData*) get_data();
}


//Dialog initialisieren
BOOL CMx2001230VerteilerEingabeBemassung::OnInitDialog()
{
		
	if (!COIIVerteilerBemassung::OnInitDialog())
		return false;
	
	return true;
}

void CMx2001230VerteilerEingabeBemassung::SelChangeRohrDN()
{
	COIIVerteilerBemassung::SelChangeRohrDN();

	CMx2001230Verteiler* pMx2001230Verteiler = dynamic_cast<CMx2001230Verteiler*>(rohr->get_CommonCOMXVerteiler());
	CStringConvert strText;
	int iWo = ((CComboBox*)GetDlgItem(IDC_EDIT_DN_VL))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_EDIT_DN_VL))->GetLBText(iWo,strText);
	int iDn = (int)strText;

	if( iDn == 65 && !pMx2001230Verteiler->get_beschichtung().isVerzinkt())
	{
		pMx2001230Verteiler->set_rohrQuali(Rohrleitung::RQ2448NLOS_MX1230200_DN65_GEPULVERT_EXTRA_DICK);
	}
	else
	{
		pMx2001230Verteiler->set_rohrQuali(Rohrleitung::RQ2448NLOS_MX1230200);
	}
}