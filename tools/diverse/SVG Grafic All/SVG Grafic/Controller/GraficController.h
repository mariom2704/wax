#pragma once


class WAXGLDLL CGraficController
{
	private:
		CHtmlView* m_pHtmlView;
		
		CGraficTubeController m_GraficRohrContainer;
		CPointDouble m_PointTubeLength;
		CFile hPostFile;
		CString m_strSVGContent;


	public:
		void SetSVGContent(CString strSVGContent)
		{
			m_strSVGContent += strSVGContent;
		}
		
		void WriteSVGContent();
		void CreateSVGFile(CString strFileName);

		CGraficController(void);
		CGraficController(CHtmlView* pHtmlView):m_GraficRohrContainer(this), m_pHtmlView(pHtmlView)
		{};

		void CalculateTubeDimisions(CPointDouble PointEndeCenterLinie);
		double GetFactorPhysicalTubeDimisionsToPixelDimison();

		void ResetSetDimisions();
	
		void GenerateDrawingOfTubes(CCRohrOneAssembeldKomponenteVector* StrangRohrVector, CCGraficRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVector);
		void ReArangeBemassungsAndAbstandsObject(CCRohrOneAssembeldKomponenteVector* pStrangRohrVector, int iFromwhere);
		void AddGraficChildassembeldTubeChildComponent(CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVector, CCGraficRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVectore);
		void AdaptLogicTubesToGraficTubes(CCRohrOneAssembeldKomponenteVector* pRohrOneAssembeldKomponenteVector, CCGraficRohrOneAssembeldKomponenteVector* pGraficRohrOneAssembeldKomponenteVector);
		~CGraficController(void);
};