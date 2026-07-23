#pragma once

class CGraficController;
class CJScriptViewTemplate;
class WAXGLDLL CWaxRohrInterfaceAdapter
{
	
	public:
		CWaxRohrInterfaceAdapter();
		virtual ~CWaxRohrInterfaceAdapter(void);
		static void AdaptedWaxToSVGGUITestI(CJScriptViewTemplate* pHTML);
		static CGraficController* m_pGraficController;
		
};

