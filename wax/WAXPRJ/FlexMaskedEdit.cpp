#include "stdafx.h"
#include "FlexMaskedEdit.h"

FlexMaskedEdit::FlexMaskedEdit(void)
{
}


void FlexMaskedEdit::actasPSPElement( int nl, const CString& psp, int produktlinie)
{
	/*
	Eingabe für PSP Elemente
	Der Formatstring ist hier als "1-[0-9][0-9][0-9][0-9][0-9][0-9]-%i-11-[0-9][0-9][1-9]" (%i = NL Nummer)
	definiert
	*/

	/*
	 * Projekt Stuktur Plan Nummer (Sap Bezeichnung)
	 * 1-123456-123-12-123
	 *					|- Lieferlos
	 *				 |---- Alt: Sparte (11=Wasser), Neu: Brandschutztechnik
	 *			  |------- Alt: Niederlassung, Neu: Region
	 *      |------------- Zählnummer
	 * |------------------ Bereich (1=Wasser, 6=Service)
	 */

	CString def;

	if(strDefFormat.IsEmpty())
	{
		strDefFormat = _T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]");
	}
	
	def.Format(strDefFormat, nl, produktlinie);
	
	init(def, psp);
	CString strProdlinie;
	strProdlinie.Format("%i", produktlinie);
	if(produktlinie < 10)
	{
		strProdlinie = CString("0") + strProdlinie;
	}
	setData(strProdlinie, 1);

	CString strNL;
	strNL.Format("%i", nl);
	

	if(nl < 10)
	{
		strNL = CString("00") + strNL;
	}
	if(9 < nl && nl < 100)
	{
		strNL = CString("0") + strNL;
	}

	setData(strNL, 2);
}

/*
//Eingabemaske setzen
bool FlexMaskedEdit::init(CString strMask, CString strInitialData)
{
   bInit = false;
    
    if (!parse(strMask))
    {
        SetWindowText("Init failed.");
        EnableWindow(FALSE);
        return false;
    }

    if (!setValidChars(strMask))
    {
        SetWindowText("SetValidChars failed.");
        EnableWindow(FALSE);
        return false;                          
    }
    
    setData(strInitialData);
    currentChar = findNextChar(0, false, true);
    SetSel(currentChar, currentChar);

	EnableWindow(IsWindowEnabled());  // this line is important!
    bInit = true;

    return true;
}
*/
/*
bool FlexMaskedEdit::setValidChars(const CString& strMask)
{
    CString strChars;
    CString strRangeLow; 
	
    bool    bInside       = false;
    bool    bInsideEscape = false;
    bool    bInsideRange  = false;
    int     iNextChar     = 0;
    int     i;
	bool valcounter = false;
	int val_count = 0;
    
    // Clear valid chars
    for (i = 0; i < MAX_CHARS; i++)
	{
        if (m_Char[i] != NULL)
        {
            delete m_Char[i];
            m_Char[i] = NULL;
        }
	}
    // Walk through mask string
    for (i = 0; i < strMask.GetLength(); i++)
    {
        switch (strMask[i])
        {
		case '#':	
					valcounter = true;
					break;
        case '[':   bInside = true;
                    break;            //done

        case ']':   bInside = false;
					m_Char[iNextChar] = new CmEditChar(strChars, false);
					m_Char[iNextChar++]->val_count = val_count; //Zugehörigkeit zum Wertx
                    strChars = "";
                    break;

        case '-':   if (bInside)
                    {
                        if (bInsideEscape)
                        {
                            strChars += "-";
                            bInsideEscape = false;  //done
                        }
                        else
                        {
                            if (strChars.GetLength() == 0)
                                return false;
                            bInsideRange = true;
                            strRangeLow = strChars.Right(1);
                            strChars = strChars.Left(strChars.GetLength() - 1); //done
                        }
                    }
                    else
                    {
                        m_Char[iNextChar] = new CmEditChar("-", false); //done
						m_Char[iNextChar++]->val_count = val_count; //Zugehörigkeit zum Wertx
                    }
                    break;

        case '\\':  if (bInside)
                    {
                        if (!bInsideEscape)
                        {
                            bInsideEscape = true;  //done
                        }
                        else
                        {
                            if (bInsideRange)
                            {
                                strChars += getRange(strRangeLow, "\\");
                                strRangeLow = "";
                                bInsideRange = false;  //done
                            }
                            else
                            {
                                strChars += "\\";
                                bInsideEscape = false;  //done
                            }
                        }
                    }
                    else
                    {
                        m_Char[iNextChar] = new CmEditChar("\\", false); //done
						m_Char[iNextChar++]->val_count = val_count; //Zugehörigkeit zum Wertx
                    }
                    break;

        default:    
					if (valcounter)
					{
						valcounter = false;
						val_count = strMask[i] - '1' + 1;
						ASSERT(val_count >= 0 && val_count < 10);
						if (val_count < 0 || val_count > 9)
							val_count = 0;
						break;
					}
					if (bInside)
                    {
                        if (bInsideEscape)
                        {
                            bInsideEscape = false;        //done
                        }

                        if (bInsideRange)
                        {
                            strChars += getRange(strRangeLow, strMask[i]);
                            strRangeLow = "";
                            bInsideRange = false;   //done
                        }
                        else
                        {
                            strChars += strMask[i]; //done
                        }
                    }
                    else
                    {
                        m_Char[iNextChar] = new CmEditChar(strMask[i], true); //done
						m_Char[iNextChar++]->val_count = val_count; //Zugehörigkeit zum Wertx
                    }
                    break;
        }
    }

    numChars = iNextChar;
    SetLimitText(numChars);
    
    return true;
}
*/

FlexMaskedEdit::~FlexMaskedEdit(void)
{
}
