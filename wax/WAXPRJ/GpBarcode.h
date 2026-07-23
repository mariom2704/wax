#pragma once
#include <string>
#include "tbarcode.h"

class GpBarcode
{
public:
	GpBarcode(void);
	virtual ~GpBarcode(void);

	enum rotation
	{
		winkel0 = 0,
		winkel90 = 1,
		winkel180 = 2,
		winkel270 = 3
	};
	
	bool getGrafik(const std::string &code, bool showText, int width, int height, rotation winkel, const std::string &datei);

private:
	bool alloc();
	bool generateBarcode(const std::string &text);

	t_BarCode*	m_pbarCode;

	e_BarCType barcodeType;
	e_CDMethod cdMethod;
	e_CCType  ccType;

	CString	m_strFormat;
	CString	m_strModul;
	CString	m_strRatio;

	int		m_nGuardWidth;

	BOOL	m_bAbove;
	BOOL	m_bPrint;

	rotation m_cbOrient;
};
