#pragma once


typedef std::vector<CCGraficRohrOneAssembeldKomponenteVector*> CGraficRohrBemassungsAnbauChildKomponentenVectorVectorBase;

class WAXGLDLL CGraficRohrBemassungsAnbauChildKomponentenVectorVector :	public CGraficRohrBemassungsAnbauChildKomponentenVectorVectorBase
{
	private:
		CRohrBemassungsAnbauChildKomponentenVectorVector* m_pLogicRohrBemassungsAnbauChildKomponentenVectorVector;
		
		std::vector<int>::size_type size()
		{
			return CGraficRohrBemassungsAnbauChildKomponentenVectorVectorBase::size();;
		}

	public:
		CGraficRohrBemassungsAnbauChildKomponentenVectorVector(void);
		~CGraficRohrBemassungsAnbauChildKomponentenVectorVector(void);

		void SetLogicRohrBemassungsAnbauChildKomponentenVectorVector( CRohrBemassungsAnbauChildKomponentenVectorVector* pLogicRohrBemassungsAnbauChildKomponentenVectorVector)
		{
			  m_pLogicRohrBemassungsAnbauChildKomponentenVectorVector = pLogicRohrBemassungsAnbauChildKomponentenVectorVector;
		}

		CRohrBemassungsAnbauChildKomponentenVectorVector* GetLogicRohrBemassungsAnbauChildKomponentenVectorVector()
		{
			 return  m_pLogicRohrBemassungsAnbauChildKomponentenVectorVector;
		}

		bool HasChildTubes()
		{
			return CGraficRohrBemassungsAnbauChildKomponentenVectorVectorBase::size() > 0 ? true : false;
		};
	
		int NumberOfChildTubes()
		{
			return (int) CGraficRohrBemassungsAnbauChildKomponentenVectorVectorBase::size() ;
		};
};


