#include "StdAfx.h"


#include "GpBarcode.h"

GpBarcode::GpBarcode(void)
{
	m_pbarCode = NULL;

	barcodeType = eBC_Code128B;
	cdMethod = eCDStandard;
	ccType = eCC_None;

	m_strFormat = _T("");
	m_strModul = _T("");
	m_strRatio = _T("");
	m_nGuardWidth = 0;
	m_bAbove = FALSE;
    m_bPrint = TRUE;
	m_cbOrient = GpBarcode::winkel0;

  
}

GpBarcode::~GpBarcode(void)
{
	if (m_pbarCode)
		BCFree(m_pbarCode);
}

bool GpBarcode::alloc()
{
	if (m_pbarCode)
		return true;
	
  ////////////////////////////////////////////////////
  // the first thing you need to do:
  // allocate memory and get a handle to the bar code
  ERRCODE eCode = S_OK;
  eCode = BCAlloc(&m_pbarCode);

  ////////////////////////////////////////////////////

  if (eCode != S_OK)
       return false;
  
  // ************ LICENSING ***************
  // Do your licensing here (at startup)
  // BCLicenseMe("Licensee - as specified", eLicKindSite, 1, "License-Key (Hex-Code)", eLicProd1D);
  // You can obtain a license key from http://www.tec-it.com/order/
  // **************************************


  eCode = BCLicenseMe("Mem: Minimax GmbH & Co. KG DE-23843", eLicKindDeveloper, 1, "10E83FB2EBCC3ABF336E0B0074A7402A", eLicProd1D);
   if (eCode != S_OK)
       return false;

  return true;
}

bool GpBarcode::generateBarcode(const std::string &text)
{
  //Code festlegen
  BCSetBCType(m_pbarCode, barcodeType);

  // get the current selected check digit method
  BCSetCDMethod(m_pbarCode, cdMethod);

  // only used with EAN/UCC Composite Symbologies
  BCSet2DCompositeComponent(m_pbarCode, ccType);

  BCSetModWidth(m_pbarCode, m_strModul);      // set the module width (unit is 1/1000 mm); "200" = 0.2 mms
  BCSetFormat(m_pbarCode, m_strFormat);       // format string (optional)
  BCSetRatio(m_pbarCode, m_strRatio);         // print ratio   (optional)

  BCSetBearerBarWidth(m_pbarCode, m_nGuardWidth);         // guard bar width (optional)
  BCSetBearerBarType(m_pbarCode, eBearerBar_TopAndBottom);
  
  BCSetPrintText(m_pbarCode, m_bPrint, m_bAbove); // print text yes/no
  BCSetMirror(m_pbarCode, FALSE);                 // mirror mode yes/no

  //LOGFONTA fontdata;
  //BCSetLogFont(m_pbarCode, &fontdata);

  switch (m_cbOrient)                 // orientation of the symbol
  {
  
  case GpBarcode::winkel90: 
    BCSetRotation(m_pbarCode, deg90);  
    break;
  case GpBarcode::winkel180:
    BCSetRotation(m_pbarCode, deg180); 
    break;
  case GpBarcode::winkel270: 
    BCSetRotation(m_pbarCode, deg270); 
    break;
  case GpBarcode::winkel0: 
  default:
    BCSetRotation(m_pbarCode, deg0);   
    break;
  }

  // get text to encode into the bar code
  CString tmp;
  tmp = text.c_str();
  BCSetText(m_pbarCode, tmp, tmp.GetLength());

  ERRCODE eCode = S_OK;
  eCode = (eCode == S_OK) ? BCCheck(m_pbarCode) : eCode;    // check bar code data (optional)
  eCode = (eCode == S_OK) ? BCCreate(m_pbarCode) : eCode;   // create internal bar code structure (MUST)

  if (eCode != S_OK)
	  return false;

  return true;
}



bool GpBarcode::getGrafik(const std::string &code, bool showText, int width, int height, rotation winkel, const std::string &datei)
{
	ERRCODE eCode;
	e_IMType imageType = eIMJpg;
	m_cbOrient = winkel;
	m_bPrint = showText;
	int dpi = 300;

	//Speicher holen
	if (!alloc())
		return false;

	//Einstellungen setzen
	if (!generateBarcode(code))
		return false;

	//Datei ohne Endung
	CString tmp;
	tmp = datei.c_str();
	tmp = tmp.Left(tmp.GetLength() - 4);

	//Jpg erzeugen
	eCode = BCSaveImage(m_pbarCode, tmp, imageType, width, height, dpi, dpi );

	if (eCode != S_OK)
		return false;

	return true;
}



