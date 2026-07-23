#pragma once


typedef std::vector<CGraficRohrKomponente*> CGraficRohrBemassungsRohrKomponentenVectorBase;

class WAXGLDLL CGraficRohrBemassungsRohrKomponentenVector :
	public CGraficRohrBemassungsRohrKomponentenVectorBase
{

	private:
		CRohrBemassungsRohrKomponentenVector* m_pRohrLogicBemassungsKomponentenVector;
		CGraficController* m_pGraficGraficController;
		CPointDouble StartPoint, EndPoint;

		std::vector<int>::size_type size()
		{
			return CGraficRohrBemassungsRohrKomponentenVectorBase::size();
		}

	public:
		void CalculateAadDrawDimension();
		void CalculateAadDrawChildDimension(int iRelativerAbstand, int i);
		CGraficRohrBemassungsRohrKomponentenVector(CGraficController* pGraficGraficController);
		~CGraficRohrBemassungsRohrKomponentenVector(void);

		void SetRohrLogicBemassungsKomponentenVector(CRohrBemassungsRohrKomponentenVector* pRohrLogicBemassungsKomponentenVector)
		{
			m_pRohrLogicBemassungsKomponentenVector = pRohrLogicBemassungsKomponentenVector;
		};

		CRohrBemassungsRohrKomponentenVector* GetRohrLogicBemassungsKomponentenVector()
		{
			return m_pRohrLogicBemassungsKomponentenVector;
		};
		
		CGraficRohrBemassungsAnbauChildKomponentenVectorVector m_GraficRohrBemassungsAnbauChildKomponentenVectorVector;

		
		bool HasChildTubes();
		CGraficRohrBemassungsAnbauChildKomponentenVectorVector& GetGraficRohrBemassungsAnbauChildKomponentenVectorVector();

		CPointDouble GetConnectionPointForAbgaenge(double RelativerAbstandzumNaechsten);

		double GetWinkelRohrZuXundYAchse();
		
		void ZeichneLinie( double x1, double y1, double x2, double y2, CString strFarbe );


};

