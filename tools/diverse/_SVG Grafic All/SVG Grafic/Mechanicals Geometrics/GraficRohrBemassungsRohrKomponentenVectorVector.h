#pragma once


typedef std::vector<CGraficRohrBemassungsRohrKomponentenVector*> CGraficRohrBemassungsKomponentenVectorVectorBase;

class WAXGLDLL CGraficRohrBemassungsKomponentenVectorVector : public CGraficRohrBemassungsKomponentenVectorVectorBase
{
	private:
		std::vector<int>::size_type size()
		{
			return CGraficRohrBemassungsKomponentenVectorVectorBase::size();
		}

	public:
		CGraficRohrBemassungsKomponentenVectorVector(void);
		~CGraficRohrBemassungsKomponentenVectorVector(void);

		int NumberOfTheUniqueBemassungsvectoren()
		{
			return (int) size();
		}
};

