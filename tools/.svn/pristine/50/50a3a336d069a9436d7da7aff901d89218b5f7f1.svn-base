#include "StdAfx.h"

#include "RohrKomponente.h"
#include "RohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "RohrBemassungsRohrKomponentenVector.h"
#include "RohrBemassungsRohrKomponentenVectorVector.h"

#include "RohrOneAssembeldKomponenteVector.h"


CCRohrOneAssembeldKomponenteVector::CCRohrOneAssembeldKomponenteVector(void)
{
	m_iFromWhere = 0;
	m_dRelativerAbstand = 0;
}



CCRohrOneAssembeldKomponenteVector* CCRohrOneAssembeldKomponenteVector::GetNewAddedCommonChildToVector()
{
	CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVector = new CCRohrOneAssembeldKomponenteVector();
	m_AssembeldCommonTubeVectorVector.push_back(pRohrOneAssembeldKomponenteVector);
	return pRohrOneAssembeldKomponenteVector;
}



bool CCRohrOneAssembeldKomponenteVector::HasAssembeldCommonTubes()
{			
	return   m_AssembeldCommonTubeVectorVector.size() > 0 ? true : false; 
}



CRohrKomponenteVectorVector& CCRohrOneAssembeldKomponenteVector::GetAssembledCommonTubeVectorVector()
{	
	return  m_AssembeldCommonTubeVectorVector; 
}




CCRohrOneAssembeldKomponenteVector* CCRohrOneAssembeldKomponenteVector::GetNewAddedChildToBemassungsVector()
{
	CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVector = new CCRohrOneAssembeldKomponenteVector();
	
	CRohrBemassungsRohrKomponentenVector* pRohrBemassungsRohrKomponentenVector = m_BemassungsKomponentenVectorVector[0];
	CRohrBemassungsAnbauChildKomponentenVectorVector& RohrBemassungsAnbauChildKomponentenVectorVector =  pRohrBemassungsRohrKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector();
	RohrBemassungsAnbauChildKomponentenVectorVector.push_back(pRohrOneAssembeldKomponenteVector);
	
	return pRohrOneAssembeldKomponenteVector;
}



CRohrBemassungsKomponentenVectorVector& CCRohrOneAssembeldKomponenteVector::GetBemassungsKomponentenVectorVector()
{	
	return  m_BemassungsKomponentenVectorVector; 
}





bool CCRohrOneAssembeldKomponenteVector::HasChildTubes()
{
	std::vector<int>::size_type sz = m_BemassungsKomponentenVectorVector.NumberOfTheUniqueBemassungsvectoren();
	bool bHasChilds = false;
	for(int z = 0; z < (int)sz; z++)
	{
		CRohrBemassungsRohrKomponentenVector* pRohrBemassungsKomponentenVector= m_BemassungsKomponentenVectorVector[z];
		if(pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().HasChildTubes())
		{
			bHasChilds = true;
			break;
		}
	}
	return   bHasChilds; 
}



void CCRohrOneAssembeldKomponenteVector::AddComponenteForTube(CRohrKomponente* pRohrKomponente)
{
	push_back(pRohrKomponente);
}



double CCRohrOneAssembeldKomponenteVector::GetRelativerAbstandzumNaechsten()
{
	 return  m_dRelativerAbstand;
}



void CCRohrOneAssembeldKomponenteVector::SetRelativerAbstandNextNeigbour(double dRelativerAbstand )
{
	m_dRelativerAbstand = dRelativerAbstand;
}



CCRohrOneAssembeldKomponenteVector::~CCRohrOneAssembeldKomponenteVector(void)
{

	if(HasChildTubes())
	{
		int iNumberOfTheUniqueBemassungsvectoren = m_BemassungsKomponentenVectorVector.NumberOfTheUniqueBemassungsvectoren();
		for(int z = 0; z < iNumberOfTheUniqueBemassungsvectoren; z++)
		{
			CRohrBemassungsRohrKomponentenVector* pRohrBemassungsKomponentenVector = m_BemassungsKomponentenVectorVector[z];
		
			if(pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().HasChildTubes())
			{
				int iNumberOfChildTubes = pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().NumberOfChildTubes();
				
				for(int h = 0; h < iNumberOfChildTubes; h++)
				{
					CCRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVector = pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().at(h);
					delete pGraficRohrOneAssembeldKomponenteVector;
					pGraficRohrOneAssembeldKomponenteVector = NULL;
				}
			}
			pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().erase(pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().begin(), pRohrBemassungsKomponentenVector->GetRohrBemassungsAnbauChildKomponentenVectorVector().end());
			pRohrBemassungsKomponentenVector->shrink_to_fit();
			delete pRohrBemassungsKomponentenVector;
			pRohrBemassungsKomponentenVector = NULL;
		}
		m_BemassungsKomponentenVectorVector.erase(m_BemassungsKomponentenVectorVector.begin(), m_BemassungsKomponentenVectorVector.end());
		shrink_to_fit();
	}
	erase(begin(), end());
	shrink_to_fit();
}
