#pragma once


typedef std::vector<CCRohrOneAssembeldKomponenteVector*> CRohrBemassungsAnbauChildKomponentenVectorVectorBase;

class WAXGLDLL CRohrBemassungsAnbauChildKomponentenVectorVector : public CRohrBemassungsAnbauChildKomponentenVectorVectorBase
{

	private:
		std::vector<int>::size_type size()
		{
			return CRohrBemassungsAnbauChildKomponentenVectorVectorBase::size();
		}


	public:
		CRohrBemassungsAnbauChildKomponentenVectorVector(void);
		~CRohrBemassungsAnbauChildKomponentenVectorVector(void);

		bool HasChildTubes()
		{
			return size() > 0 ? true : false;
		};
	
		int NumberOfChildTubes()
		{
			return (int) size() ;
		};
};

