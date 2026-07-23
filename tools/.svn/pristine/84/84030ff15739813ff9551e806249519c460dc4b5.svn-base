#pragma once

typedef std::vector<CRohrKomponente*> CRohrBemassungsKomponentenVectorBase;

class CRohrBemassungsRohrKomponentenVector : public CRohrBemassungsKomponentenVectorBase
{

	private:
	 CRohrBemassungsAnbauChildKomponentenVectorVector m_RohrBemassungsAnbauChildKomponentenVectorVector;
	 int iIndexMasterDirectionKomponente;
	 std::vector<int>::size_type size()
	 {
			return CRohrBemassungsKomponentenVectorBase::size();
	 }
	
public:
	CRohrBemassungsRohrKomponentenVector(void);
	~CRohrBemassungsRohrKomponentenVector(void);
	CRohrBemassungsAnbauChildKomponentenVectorVector& GetRohrBemassungsAnbauChildKomponentenVectorVector();
	CRohrBemassungsAnbauChildKomponentenVectorVector*GetPointerRohrBemassungsAnbauChildKomponentenVectorVector();
	void AddRohrKomponente(CRohrKomponente* pRohrKomponente)
	{
		push_back(pRohrKomponente);
		VerityMasteDirectionKomponente( pRohrKomponente, this->size()-1);
	}

	void VerityMasteDirectionKomponente(CRohrKomponente* pRohrKomponente, int i)
	{
		if(pRohrKomponente->GetKlassenName() == CRohrKomponente::RohrKomponenteTeilRohr)
		{
			iIndexMasterDirectionKomponente = i;
		}	
	}

	int GetIndexMasterDirectionKomponent()
	{
		return	iIndexMasterDirectionKomponente;
	}



	int  NumberOfComponents()
	{
		return (int) size();
	}
};

