#pragma once



typedef std::vector<CRohrBemassungsRohrKomponentenVector*> CRohrBemassungsKomponentenVectorVectorBase;

class WAXGLDLL CRohrBemassungsKomponentenVectorVector : public CRohrBemassungsKomponentenVectorVectorBase
{
	private:
		std::vector<int>::size_type size()
		{
			return CRohrBemassungsKomponentenVectorVectorBase::size();
		}

	public:
		CRohrBemassungsKomponentenVectorVector(void);
		~CRohrBemassungsKomponentenVectorVector(void);

		int NumberOfTheUniqueBemassungsvectoren()
		{
			return (int) size();
		}


	
};

