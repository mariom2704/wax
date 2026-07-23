#pragma once
#include ".\gridctrl.h"




class CWaxGridCtrl : public CGridCtrl
{
	public:

		enum
		{
			e_GEGENFLANSCH_LAENGE = 4,
		};

		CWaxGridCtrl(void);
		~CWaxGridCtrl(void);
		void SetGegenflanschType(int i, int FlanschType);
		void SetGegenflanschTypeFromSelectionChange(int col, int iSlectedIndex);
		int GetGegenflanschType(int strText);
		BOOL SetColumnCount(int iCols);
		
		//{{AFX_MSG(CWaxGridCtrl)
		LRESULT OnLButtonDownPosted(WPARAM, LPARAM);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()


	private:
		virtual void ResetScrollBars();
		CArray<int,int> iChoosenConboBoxGegenflanschTyp;

};
