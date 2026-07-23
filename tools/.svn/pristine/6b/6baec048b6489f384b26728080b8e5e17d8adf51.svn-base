#ifndef __RTFPARSER__
#define __RTFPARSER__

#include "stl.h"

class RtfParser
{
	// RTF parser error codes
public:
	enum _ERROR
	{
		OK = 0,                      // Everything's fine!
		StackUnderflow = 1,       // Unmatched '}'
		StackOverflow = 2,       // Too many '{' -- memory exhausted
		UnmatchedBrace = 3,       // RTF ended during an open group.
		InvalidHex = 4,       // invalid hex character found in data
		BadTable = 5,       // RTF table (sym or prop) invalid
		Assertion = 6,       // Assertion failure
		EndOfFile = 7,       // End of file reached while reading RTF
	};

	typedef struct char_prop
	{
		char fBold;
		char fUnderline;
		char fItalic;
	} CHP;                  // CHaracter Properties

	typedef enum 
	{
		justL, 
		justR, 
		justC, 
		justF 
	} JUST;

	typedef struct para_prop
	{
		int xaLeft;                 // left indent in twips
		int xaRight;                // right indent in twips
		int xaFirst;                // first line indent in twips
		JUST just;                  // justification
	} PAP;                  // PAragraph Properties

	typedef enum {sbkNon, sbkCol, sbkEvn, sbkOdd, sbkPg} SBK;
	typedef enum {pgD, pgURom, pgLRom, pgULtr, pgLLtr} PGN;
	typedef struct st_prop
	{
		int cCols;                  // number of columns
		SBK sbk;                    // stion break type
		int xaPgn;                  // x position of page number in twips
		int yaPgn;                  // y position of page number in twips
		PGN pgnFormat;              // how the page number is formatted
	} SEP;                  // SEction Properties

	typedef struct doc_prop
	{
		int xaPage;                 // page width in twips
		int yaPage;                 // page height in twips
		int xaLeft;                 // left margin in twips
		int yaTop;                  // top margin in twips
		int xaRight;                // right margin in twips
		int yaBottom;               // bottom margin in twips
		int pgnStart;               // starting page number in twips
		char fFacingp;              // facing pages enabled?
		char fLandscape;            // landscape or portrait??
	} DOP;                  // DOcument Properties

	typedef enum 
	{ 
		rdsNorm, 
		rdsSkip 
	} RDS;              // Rtf Destination State

	typedef enum 
	{ 
		risNorm, 
		risBin, 
		risHex 
	} RIS;       // Rtf Internal State

	typedef struct save             // property save structure
	{
		struct save *pNext;         // next save
		CHP chp;
		PAP pap;
		SEP sep;
		DOP dop;
		RDS rds;
		RIS ris;
	} SAVE;

	// What types of properties are there?
	typedef enum 
	{
		ipropBold, ipropItalic, ipropUnderline, ipropLeftInd,
		ipropRightInd, ipropFirstInd, ipropCols, ipropPgnX,
		ipropPgnY, ipropXaPage, ipropYaPage, ipropXaLeft,
		ipropXaRight, ipropYaTop, ipropYaBottom, ipropPgnStart,
		ipropSbk, ipropPgnFormat, ipropFacingp, ipropLandscape,
		ipropJust, ipropPard, ipropPlain, ipropStd,
		ipropMax 
	} IPROP;

	typedef enum 
	{
		actnSp, 
		actnByte, 
		actnWord
	} ACTN;
	
	typedef enum 
	{
		propChp, 
		propPap, 
		propSep, 
		propDop
	} PROPTYPE;

	typedef struct propmod
	{
		ACTN actn;              // size of value
		PROPTYPE prop;          // structure containing value
		int  offset;            // offset of value from base of structure
	} PROP;

	typedef enum 
	{
		ipfnBin, 
		ipfnHex, 
		ipfnSkipDest 
	} IPFN;
	typedef enum 
	{
		idestPict, 
		idestSkip 
	} IDEST;
	typedef enum 
	{
		kwdChar, 
		kwdDest, 
		kwdProp, 
		kwdSp
	} KWD;

	typedef struct symbol
	{
		char *szKeyword;        // RTF keyword
		int  dflt;              // default value to use
		bool fPassDflt;         // true to use default value from this table
		KWD  kwd;               // base action to take
		int  idx;               // index into property table if kwd == kwdProp
								// index into destination table if kwd == kwdDest
								// character to print if kwd == kwdChar
	} SYM;


public:
	string txt;
	string rtf;
	int index;
	PROP *rgprop;
	SYM *rgsymRtf;
	int isymMax;
	RtfParser();
	~RtfParser(){};
	int parse();
	int PushRtfState();
	int PopRtfState();
	int ParseRtfKeyword();
	int ParseChar(int c);
	int TranslateKeyword(char *szKeyword, int param, bool fParam);
	int PrintChar(int ch);
	int EndGroupAction(RDS rds);
	int ApplyPropChange(IPROP iprop, int val);
	int ChangeDest(IDEST idest);
	int ParseSpecialKeyword(IPFN ipfn);
	int ParseSpecialProperty(IPROP iprop, int val);
	int ParseHexByte();

private:

	int cGroup;
	bool fSkipDestIfUnk;
	long cbBin;
	long lParam;

	RDS rds;
	RIS ris;

	CHP chp;
	PAP pap;
	SEP sep;
	DOP dop;

	SAVE *psave;
};

#endif
