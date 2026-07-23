#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rtfparser.h"

#define offsetof(s,m)   (size_t)&(((s *)0)->m)

RtfParser::RtfParser() // Property descriptions
{
	cGroup = 0;
	fSkipDestIfUnk = false;
	cbBin = 0;
	lParam = 0;

	rds = rdsNorm;
	static PROP xx[] = 
	{ 
		actnByte,   propChp,    offsetof(CHP, fBold) ,       // ipropBold
		actnByte,   propChp,    offsetof(CHP, fItalic),     // ipropItalic
		actnByte,   propChp,    offsetof(CHP, fUnderline),  // ipropUnderline
		actnWord,   propPap,    offsetof(PAP, xaLeft),      // ipropLeftInd
		actnWord,   propPap,    offsetof(PAP, xaRight),     // ipropRightInd
		actnWord,   propPap,    offsetof(PAP, xaFirst),     // ipropFirstInd
		actnWord,   propSep,    offsetof(SEP, cCols),       // ipropCols
		actnWord,   propSep,    offsetof(SEP, xaPgn),       // ipropPgnX
		actnWord,   propSep,    offsetof(SEP, yaPgn),       // ipropPgnY
		actnWord,   propDop,    offsetof(DOP, xaPage),      // ipropXaPage
		actnWord,   propDop,    offsetof(DOP, yaPage),      // ipropYaPage
		actnWord,   propDop,    offsetof(DOP, xaLeft),      // ipropXaLeft
		actnWord,   propDop,    offsetof(DOP, xaRight),     // ipropXaRight
		actnWord,   propDop,    offsetof(DOP, yaTop),       // ipropYaTop
		actnWord,   propDop,    offsetof(DOP, yaBottom),    // ipropYaBottom
		actnWord,   propDop,    offsetof(DOP, pgnStart),    // ipropPgnStart
		actnByte,   propSep,    offsetof(SEP, sbk),         // ipropSbk
		actnByte,   propSep,    offsetof(SEP, pgnFormat),   // ipropPgnFormat
		actnByte,   propDop,    offsetof(DOP, fFacingp),    // ipropFacingp
		actnByte,   propDop,    offsetof(DOP, fLandscape),  // ipropLandscape
		actnByte,   propPap,    offsetof(PAP, just),        // ipropJust
		actnSp,   propPap,    0,                          // ipropPard
		actnSp,   propChp,    0,                          // ipropPlain
		actnSp,   propSep,    0	
	};

	rgprop = xx;
	// Keyword descriptions
	static SYM yy[] = 
	{
//  keyword     dflt    fPassDflt   kwd         idx
    "b",        1,      false,     kwdProp,    ipropBold,
    "u",        1,      false,     kwdProp,    ipropUnderline,
    "i",        1,      false,     kwdProp,    ipropItalic,
    "li",       0,      false,     kwdProp,    ipropLeftInd,
    "ri",       0,      false,     kwdProp,    ipropRightInd,
    "fi",       0,      false,     kwdProp,    ipropFirstInd,
    "cols",     1,      false,     kwdProp,    ipropCols,
    "sbknone",  sbkNon, true,      kwdProp,    ipropSbk,
    "sbkcol",   sbkCol, true,      kwdProp,    ipropSbk,
    "sbkeven",  sbkEvn, true,      kwdProp,    ipropSbk,
    "sbkodd",   sbkOdd, true,      kwdProp,    ipropSbk,
    "sbkpage",  sbkPg,  true,      kwdProp,    ipropSbk,
    "pgnx",     0,      false,     kwdProp,    ipropPgnX,
    "pgny",     0,      false,     kwdProp,    ipropPgnY,
    "pgnd",   pgD,  true,      kwdProp,    ipropPgnFormat,
    "pgnucrm",  pgURom, true,      kwdProp,    ipropPgnFormat,
    "pgnlcrm",  pgLRom, true,      kwdProp,    ipropPgnFormat,
    "pgnucltr", pgULtr, true,      kwdProp,    ipropPgnFormat,
    "pgnlcltr", pgLLtr, true,      kwdProp,    ipropPgnFormat,
    "qc",       justC,  true,      kwdProp,    ipropJust,
    "ql",       justL,  true,      kwdProp,    ipropJust,
    "qr",       justR,  true,      kwdProp,    ipropJust,
    "qj",       justF,  true,      kwdProp,    ipropJust,
    "paperw",   12240,  false,     kwdProp,    ipropXaPage,
    "paperh",   15480,  false,     kwdProp,    ipropYaPage,
    "margl",    1800,   false,     kwdProp,    ipropXaLeft,
    "margr",    1800,   false,     kwdProp,    ipropXaRight,
    "margt",    1440,   false,     kwdProp,    ipropYaTop,
    "margb",    1440,   false,     kwdProp,    ipropYaBottom,
    "pgnstart", 1,      true,      kwdProp,    ipropPgnStart,
    "facingp",  1,      true,      kwdProp,    ipropFacingp,
    "landscape",1,      true,      kwdProp,    ipropLandscape,
    "par",      0,      false,     kwdChar,    0x0a,
    "\0x0a",    0,      false,     kwdChar,    0x0a,
    "\0x0d",    0,      false,     kwdChar,    0x0a,
    "tab",      0,      false,     kwdChar,    0x09,
    "ldblquote",0,      false,     kwdChar,    '"',
    "rdblquote",0,      false,     kwdChar,    '"',
    "bin",      0,      false,     kwdSp,    ipfnBin,
    "*",        0,      false,     kwdSp,    ipfnSkipDest,
    "'",        0,      false,     kwdSp,    ipfnHex,
    "author",   0,      false,     kwdDest,    idestSkip,
    "buptim",   0,      false,     kwdDest,    idestSkip,
    "colortbl", 0,      false,     kwdDest,    idestSkip,
    "comment",  0,      false,     kwdDest,    idestSkip,
    "creatim",  0,      false,     kwdDest,    idestSkip,
    "doccomm",  0,      false,     kwdDest,    idestSkip,
    "fonttbl",  0,      false,     kwdDest,    idestSkip,
    "footer",   0,      false,     kwdDest,    idestSkip,
    "footerf",  0,      false,     kwdDest,    idestSkip,
    "footerl",  0,      false,     kwdDest,    idestSkip,
    "footerr",  0,      false,     kwdDest,    idestSkip,
    "footnote", 0,      false,     kwdDest,    idestSkip,
    "ftncn",    0,      false,     kwdDest,    idestSkip,
    "ftnsep",   0,      false,     kwdDest,    idestSkip,
    "ftnsepc",  0,      false,     kwdDest,    idestSkip,
    "header",   0,      false,     kwdDest,    idestSkip,
    "headerf",  0,      false,     kwdDest,    idestSkip,
    "headerl",  0,      false,     kwdDest,    idestSkip,
    "headerr",  0,      false,     kwdDest,    idestSkip,
    "info",     0,      false,     kwdDest,    idestSkip,
    "keywords", 0,      false,     kwdDest,    idestSkip,
    "operator", 0,      false,     kwdDest,    idestSkip,
    "pict",     0,      false,     kwdDest,    idestSkip,
    "printim",  0,      false,     kwdDest,    idestSkip,
    "private1", 0,      false,     kwdDest,    idestSkip,
    "revtim",   0,      false,     kwdDest,    idestSkip,
    "rxe",      0,      false,     kwdDest,    idestSkip,
    "stylesheet",   0,      false,     kwdDest,    idestSkip,
    "subjt",  0,      false,     kwdDest,    idestSkip,
    "tc",       0,      false,     kwdDest,    idestSkip,
    "title",    0,      false,     kwdDest,    idestSkip,
    "txe",      0,      false,     kwdDest,    idestSkip,
    "xe",       0,      false,     kwdDest,    idestSkip,
    "{",        0,      false,     kwdChar,    '{',
    "}",        0,      false,     kwdChar,    '}',
    "\\",       0,      false,     kwdChar,    '\\'
    };
	rgsymRtf = yy;
	isymMax = sizeof(yy) / sizeof(SYM);

}

int RtfParser::parse()
{
    unsigned int ch;
    int ex;
    int cNibble = 2;
    int b = 0;
	index = 0;
	if (!rtf.length())
		return OK;
    for (ch = rtf[index++]; index < (int)rtf.length(); ch = rtf[index++])
    {
        if (cGroup < 0)
            return StackUnderflow;
        if (ris == risBin)                      // if we're parsing binary data, handle it dirtly
        {
            if ((ex = ParseChar(ch)) != OK)
                return ex;
        }
        else
        {
            switch (ch)
            {
            case '{':
                if ((ex = PushRtfState()) != OK)
                    return ex;
                break;
            case '}':
                if ((ex = PopRtfState()) != OK)
                    return ex;
                break;
            case '\\':
                if ((ex = ParseRtfKeyword()) != OK)
                    return ex;
                break;
            case 0x0d:
            case 0x0a:          // cr and lf are noise characters...
                break;
            default:
                if (ris == risNorm)
                {
                    if ((ex = ParseChar(ch)) != OK)
                        return ex;
                }
                else
                {               // parsing hex data
                    if (ris != risHex)
                        return Assertion;
                    b = b << 4;
                    if (isdigit(ch))
                        b += (char) ch - '0';
                    else
                    {
                        if (islower(ch))
                        {
                            if (ch < 'a' || ch > 'f')
                                return InvalidHex;
                            b += (char) ch - 'a' + 10;
                        }
                        else
                        {
                            if (ch < 'A' || ch > 'F')
                                return InvalidHex;
                            b += (char) ch - 'A' + 10;
                        }                   
					}
                    cNibble--;
                    if (!cNibble)
                    {
                        if ((ex = ParseChar(b)) != OK)
                            return ex;
                        cNibble = 2;
                        b = 0;
ris = risNorm;
                    }
                }                   // end else (ris != risNorm)
                break;
            }       // switch
        }           // else (ris != risBin)
    }               // while
    if (cGroup < 0)
        return StackUnderflow;
    if (cGroup > 0)
        return UnmatchedBrace;
    return OK;
}

//
// %%Function: PushRtfState
//
// Save relevant info on a linked list of SAVE structures.
//

int RtfParser::PushRtfState()
{
    SAVE *psaveNew = (SAVE *)malloc(sizeof(SAVE));
    if (!psaveNew)
        return StackOverflow;

    psaveNew -> pNext = psave;
    psaveNew -> chp = chp;
    psaveNew -> pap = pap;
    psaveNew -> sep = sep;
    psaveNew -> dop = dop;
    psaveNew -> rds = rds;
    psaveNew -> ris = ris;
    ris = risNorm;
    psave = psaveNew;
    cGroup++;
    return OK;
}

//
// %%Function: PopRtfState
//
// If we're ending a destination (that is, the destination is changing),
// call EndGroupAction.
// Always restore relevant info from the top of the SAVE list.
//

int RtfParser::PopRtfState()
{
    SAVE *psaveOld;
    int ex;

    if (!psave)
        return StackUnderflow;

    if (rds != psave->rds)
    {
        if ((ex = EndGroupAction(rds)) != OK)
            return ex;
    }
    chp = psave->chp;
    pap = psave->pap;
    sep = psave->sep;
    dop = psave->dop;
    rds = psave->rds;
    ris = psave->ris;

    psaveOld = psave;
    psave = psave->pNext;
    cGroup--;
    free(psaveOld);
    return OK;
}

//
// %%Function: ParseRtfKeyword
//
// Step 2:
// get a control word (and its associated value) and
// call TranslateKeyword to dispatch the control.
//

int RtfParser::ParseRtfKeyword()
{
    int ch;
    bool fParam = false;
    char fNeg = 0;
    int param = 0;
    char *pch;
    char szKeyword[30];
    char szParameter[20];

    szKeyword[0] = '\0';
    szParameter[0] = '\0';
    ch = rtf[index++];
	if (index == rtf.length())
        return EndOfFile;
    if (!isalpha(ch))           // a control symbol; no delimiter.
    {
        szKeyword[0] = (char) ch;
        szKeyword[1] = '\0';
        return TranslateKeyword(szKeyword, 0, fParam);
    }
    for (pch = szKeyword; isalpha(ch); ch = rtf[index++])
        *pch++ = (char) ch;
    *pch = '\0';
    if (ch == '-')
    {
        fNeg  = true;
        ch = rtf[index++];
		if (index == rtf.length())
            return EndOfFile;
    }
    if (isdigit(ch))
    {
        fParam = true;         // a digit after the control means we have a parameter
        for (pch = szParameter; isdigit(ch); ch = rtf[index++])
            *pch++ = (char) ch;
        *pch = '\0';
        param = atoi(szParameter);
        if (fNeg)
            param = -param;
        lParam = atol(szParameter);
        if (fNeg)
            param = -param;
    }
    if (ch != ' ')
        index--;
    return TranslateKeyword(szKeyword, param, fParam);
}

//
// %%Function: ParseChar
//
// Route the character to the appropriate destination stream.
//

int RtfParser::ParseChar(int ch)
{
    if (ris == risBin && --cbBin <= 0)
        ris = risNorm;
    switch (rds)
    {
    case rdsSkip:
        // Toss this character.
        return OK;
    case rdsNorm:
        // Output a character. Properties are valid at this point.
        return PrintChar(ch);
    default:
    // handle other destinations....
        return OK;
    }
}
//
// %%Function: PrintChar
//
// Send a character to the output file.
//

int RtfParser::PrintChar(int ch)
{
    // unfortunately, we don't do a whole lot here as far as layout goes...
    if (ch == 10)
		txt += (char)13;
	txt += (char)ch;
    return OK;
}




//
// %%Function: ApplyPropChange
//
// Set the property identified by _iprop_ to the value _val_.
//
//

int RtfParser::ApplyPropChange(IPROP iprop, int val)
{
    char *pb;

    if (rds == rdsSkip)                 // If we're skipping text,
        return OK;                    // don't do anything.

    switch (rgprop[iprop].prop)
    {
    case propDop:
        pb = (char *)&dop;
        break;
    case propSep:
        pb = (char *)&sep;
        break;
    case propPap:
        pb = (char *)&pap;
        break;
    case propChp:
        pb = (char *)&chp;
        break;
    default:
        if (rgprop[iprop].actn != actnSp)
            return BadTable;
        break;
    }
    switch (rgprop[iprop].actn)
    {
    case actnByte:
        pb[rgprop[iprop].offset] = (unsigned char) val;
        break;
    case actnWord:
        (*(int *) (pb+rgprop[iprop].offset)) = val;
        break;
    case actnSp:
        return ParseSpecialProperty(iprop, val);
        break;
    default:
        return BadTable;
    }
    return OK;
}

//
// %%Function: ParseSpialProperty
//
// Set a property that requires code to evaluate.
//

int RtfParser::ParseSpecialProperty(IPROP iprop, int val)
{
	val = 0;
    switch (iprop)
    {
    case ipropPard:
        memset(&pap, 0, sizeof(pap));
        return OK;
    case ipropPlain:
        memset(&chp, 0, sizeof(chp));
        return OK;
    case ipropStd:
        memset(&sep, 0, sizeof(sep));
        return OK;
    default:
        return BadTable;
    }
    return BadTable;
}

//
// %%Function: TranslateKeyword.
//
// Step 3.
// Search rgsymRtf for szKeyword and evaluate it appropriately.
//
// Inputs:
// szKeyword:   The RTF control to evaluate.
// param:       The parameter of the RTF control.
// fParam:      true if the control had a parameter; (that is, if param is valid)
//              false if it did not.
//

int RtfParser::TranslateKeyword(char *szKeyword, int param, bool fParam)
{
    int isym;

    // search for szKeyword in rgsymRtf

    for (isym = 0; isym < isymMax; isym++)
        if (strcmp(szKeyword, rgsymRtf[isym].szKeyword) == 0)
            break;
    if (isym == isymMax)            // control word not found
    {
        if (fSkipDestIfUnk)         // if this is a new destination
            rds = rdsSkip;          // skip the destination
                                    // else just discard it
        fSkipDestIfUnk = false;
        return OK;
    }

    // found it!  use kwd and idx to determine what to do with it.

    fSkipDestIfUnk = false;
    switch (rgsymRtf[isym].kwd)
    {
    case kwdProp:
        if (rgsymRtf[isym].fPassDflt || !fParam)
            param = rgsymRtf[isym].dflt;
        return ApplyPropChange((IPROP)rgsymRtf[isym].idx, param);
    case kwdChar:
        return ParseChar(rgsymRtf[isym].idx);
    case kwdDest:
        return ChangeDest((IDEST)rgsymRtf[isym].idx);
    case kwdSp:
        return ParseSpecialKeyword((IPFN)rgsymRtf[isym].idx);
    default:
        return BadTable;
    }
    return BadTable;
}

//
// %%Function: ChangeDest
//
// Change to the destination spified by idest.
// There's usually more to do here than this...
//

int RtfParser::ChangeDest(IDEST idest)
{
	idest = idestSkip;
    if (rds == rdsSkip)             // if we're skipping text,
        return OK;                // don't do anything

    rds = rdsSkip;              // when in doubt, skip it...
    return OK;
}

//
// %%Function: EndGroupAction
//
// The destination spified by rds is coming to a close.
// If there's any cleanup that needs to be done, do it now.
//

int RtfParser::EndGroupAction(RDS rds)
{
	rds = rdsNorm;
    return OK;
}

//
// %%Function: ParseSpialKeyword
//
// Evaluate an RTF control that needs spial processing.
//

int RtfParser::ParseSpecialKeyword(IPFN ipfn)
{
    if (rds == rdsSkip && ipfn != ipfnBin)  // if we're skipping, and it's not
        return OK;                        // the \bin keyword, ignore it.
    switch (ipfn)
    {
    case ipfnBin:
        ris = risBin;
        cbBin = lParam;
        break;
    case ipfnSkipDest:
        fSkipDestIfUnk = true;
        break;
    case ipfnHex:
 ris = risHex;
 break;
    default:
        return BadTable;
    }
    return OK;
}

