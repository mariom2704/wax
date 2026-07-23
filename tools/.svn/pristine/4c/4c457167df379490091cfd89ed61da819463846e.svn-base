#include "StdAfx.h"

#include "UtiltyHelper.h"

#include "WaxRohrInterfaceAdapter.h"
#include "RohrKomponente.h"
#include "RohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "RohrBemassungsRohrKomponentenVector.h"
#include "RohrBemassungsRohrKomponentenVectorVector.h"
#include "RohrOneAssembeldKomponenteVector.h"

#include "..\SVG Grafic\SVG File Operations\GraficFileController.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficTubeController.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficRohrKomponente.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficRohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficRohrBemassungsRohrKomponentenVector.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficRohrBemassungsRohrKomponentenVectorVector.h"
#include "..\SVG Grafic\Mechanicals Geometrics\GraficRohrOneAssembeldKomponenteVector.h"

#include "..\SVG Grafic\Controller\GraficController.h"


CWaxRohrInterfaceAdapter::CWaxRohrInterfaceAdapter()
{
	
}

CGraficController* CWaxRohrInterfaceAdapter::m_pGraficController = NULL;



void CWaxRohrInterfaceAdapter::AdaptedWaxToSVGGUITestI(CJScriptViewTemplate* pHTML )
{
	if(m_pGraficController == NULL)
	{
		m_pGraficController = new CGraficController((CHtmlView*)pHTML);
	}
	
	 CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVector = new CCRohrOneAssembeldKomponenteVector();


	CRohrKomponenteTeilRohr StrangRohrI, StrangRohrII;
	CRohrKomponenteBogen	RohrKomponenteBogen;
	CRohrKomponenteNut		StrangRohrnNut;
	CRohrKomponenteGlatt	StrangRohrnGlatt;


	StrangRohrnNut.SetWinkel(10);
	StrangRohrnNut.SetLaenge(10);
	StrangRohrnNut.SetFirstKomponent();
	pRohrOneAssembeldKomponenteVector->AddComponenteForTube(&StrangRohrnNut);
			
   	StrangRohrI.SetWinkel( 0 );
	StrangRohrI.SetLaenge(750);
	pRohrOneAssembeldKomponenteVector->AddComponenteForTube(&StrangRohrI);
	//AddChildren(170);

	

	CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVectorChild = pRohrOneAssembeldKomponenteVector->GetNewAddedCommonChildToVector();

	CRohrKomponenteTeilRohr RohrKomponenteTeilRohrChild;
	CRohrKomponenteNut RohrKomponenteNutChild;

	pRohrOneAssembeldKomponenteVectorChild->SetRelativerAbstandNextNeigbour(170);
	RohrKomponenteTeilRohrChild.SetLaenge(80);
	RohrKomponenteTeilRohrChild.SetDn(25);
	CString mm = _T("RohrstutzenTeilRohr");
	
	RohrKomponenteTeilRohrChild.SetName(mm);
	
	pRohrOneAssembeldKomponenteVectorChild->AddComponenteForTube(&RohrKomponenteTeilRohrChild);
	
	RohrKomponenteNutChild.SetLaenge(10);
	RohrKomponenteNutChild.SetDn(25);
	mm = _T("RohrstutzenNut");
	
	RohrKomponenteNutChild.SetName(mm);
	pRohrOneAssembeldKomponenteVectorChild->AddComponenteForTube(&RohrKomponenteNutChild);
	

	

	//AddChildren(440);

	
	pRohrOneAssembeldKomponenteVectorChild = pRohrOneAssembeldKomponenteVector->GetNewAddedCommonChildToVector();

	CRohrKomponenteTeilRohr RohrKomponenteTeilRohrChildII;
	CRohrKomponenteNut RohrKomponenteNutChildII;

	pRohrOneAssembeldKomponenteVectorChild->SetRelativerAbstandNextNeigbour(440);
	RohrKomponenteTeilRohrChildII.SetLaenge(80);
	RohrKomponenteTeilRohrChildII.SetDn(25);
	mm = _T("RohrstutzenTeilRohr");
	
	RohrKomponenteTeilRohrChildII.SetName(mm);
	
	pRohrOneAssembeldKomponenteVectorChild->AddComponenteForTube(&RohrKomponenteTeilRohrChildII);
	
	RohrKomponenteNutChildII.SetLaenge(10);
	RohrKomponenteNutChildII.SetDn(25);
	mm = _T("RohrstutzenNut");
	
	RohrKomponenteNutChildII.SetName(mm);
	pRohrOneAssembeldKomponenteVectorChild->AddComponenteForTube(&RohrKomponenteNutChildII);
		
	RohrKomponenteBogen.SetLaenge(80);
	pRohrOneAssembeldKomponenteVector->AddComponenteForTube(&RohrKomponenteBogen);

	StrangRohrII.SetWinkel(-70);
	StrangRohrII.SetLaenge(550);
	pRohrOneAssembeldKomponenteVector->AddComponenteForTube(&StrangRohrII);
	
	StrangRohrnGlatt.SetWinkel(0);
	StrangRohrnGlatt.SetLaenge(1);
	pRohrOneAssembeldKomponenteVector->AddComponenteForTube(&StrangRohrnGlatt);



	CCGraficRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVectorv = new CCGraficRohrOneAssembeldKomponenteVector();
	m_pGraficController->GenerateDrawingOfTubes(pRohrOneAssembeldKomponenteVector, pGraficRohrOneAssembeldKomponenteVectorv);
	
	delete pGraficRohrOneAssembeldKomponenteVectorv;
	pGraficRohrOneAssembeldKomponenteVectorv = NULL;
	
	delete pRohrOneAssembeldKomponenteVector;
	pRohrOneAssembeldKomponenteVector = NULL;
	
}



CWaxRohrInterfaceAdapter::~CWaxRohrInterfaceAdapter(void)
{
	if(m_pGraficController)
	{
		delete m_pGraficController;
	}
}



