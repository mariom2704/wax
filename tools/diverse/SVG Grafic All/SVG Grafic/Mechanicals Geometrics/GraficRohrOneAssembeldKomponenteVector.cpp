#include "StdAfx.h"

#include "..\UtiltyHelper.h"

#include "..\_Tubes Logic Grafic Adapter\RohrKomponente.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsRohrKomponentenVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsRohrKomponentenVectorVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrOneAssembeldKomponenteVector.h"


#include "GraficTubeController.h"
#include "..\Mechanicals Geometrics\GraficRohrKomponente.h"
#include "..\Controller\GraficController.h"

#include "GraficRohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "GraficRohrBemassungsRohrKomponentenVector.h"
#include "GraficRohrBemassungsRohrKomponentenVectorVector.h"
#include "GraficRohrOneAssembeldKomponenteVector.h"

#include "..\SVG Grafic\Controller\GraficController.h"


CCGraficRohrOneAssembeldKomponenteVector::CCGraficRohrOneAssembeldKomponenteVector()
{	
}



void CCGraficRohrOneAssembeldKomponenteVector::AddComponenteForTube(CGraficRohrKomponente* pRohrKomponente)
{
	push_back(pRohrKomponente);
}



bool CCGraficRohrOneAssembeldKomponenteVector::HasChildTubes()
{

	std::vector<int>::size_type sz = m_GraficBemassungsKomponentenVectorVector.NumberOfTheUniqueBemassungsvectoren();
	bool bHasChilds = false;
	for(int z = 0; z < (int)sz; z++)
	{
		CGraficRohrBemassungsRohrKomponentenVector* pGraficRohrBemassungsKomponentenVector= m_GraficBemassungsKomponentenVectorVector[z];
		
		if(pGraficRohrBemassungsKomponentenVector->GetGraficRohrBemassungsAnbauChildKomponentenVectorVector().HasChildTubes())
		{
			bHasChilds = true;
			break;
		}
	}
	return   bHasChilds; 
}



CGraficRohrBemassungsKomponentenVectorVector& CCGraficRohrOneAssembeldKomponenteVector::GetGraficBemassungsKomponentenVectorVector()
{
	return m_GraficBemassungsKomponentenVectorVector;
}



CCGraficRohrOneAssembeldKomponenteVector::~CCGraficRohrOneAssembeldKomponenteVector(void)
{
	if(HasChildTubes())
	{
		int iNumberOfTheUniqueBemassungsvectoren = m_GraficBemassungsKomponentenVectorVector.NumberOfTheUniqueBemassungsvectoren();
		for(int z = 0; z < iNumberOfTheUniqueBemassungsvectoren; z++)
		{
			CGraficRohrBemassungsRohrKomponentenVector* pGraficRohrBemassungsKomponentenVector = m_GraficBemassungsKomponentenVectorVector[z];
		
			if(pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.HasChildTubes())
			{
				int iNumberOfChildTubes = pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.NumberOfChildTubes();
				for(int h = 0; h < iNumberOfChildTubes; h++)
				{
					CCGraficRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVector = pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector[h];
					delete pGraficRohrOneAssembeldKomponenteVector;
					pGraficRohrOneAssembeldKomponenteVector = NULL;
				}
			}
			pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.erase(pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.begin(), pGraficRohrBemassungsKomponentenVector->m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.end());
			//pGraficRohrBemassungsKomponentenVector->shrink_to_fit();
			delete pGraficRohrBemassungsKomponentenVector;
			pGraficRohrBemassungsKomponentenVector = NULL;

		}
		m_GraficBemassungsKomponentenVectorVector.erase(m_GraficBemassungsKomponentenVectorVector.begin(), m_GraficBemassungsKomponentenVectorVector.end());
		//m_GraficBemassungsKomponentenVectorVector.shrink_to_fit();
	}
	erase(begin(), end());
	//shrink_to_fit();
}
