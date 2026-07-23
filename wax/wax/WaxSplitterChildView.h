#pragma once

class WaxSplitterWnd;
class CWaxSplitterChildView  : public  CSplitterWnd
{

	private:
		WaxSplitterWnd* m_pMultiSpliittView;

	public:
		CWaxSplitterChildView(WaxSplitterWnd*);
		CView* SwitchStaticChildViews(UINT uiUpperView, UINT uiLowerView);
		CView* SwitchChildView(UINT pNewView,  int paneRow, int paneCol);
};


