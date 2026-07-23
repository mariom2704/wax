#pragma once


/**
Kc Auftrag aus der Schnittstellendatei lesen
*/
class WAXPRJDLL GpSapWittenbergeAuftragsdaten
{
public:
	GpSapWittenbergeAuftragsdaten(void);
	virtual ~GpSapWittenbergeAuftragsdaten(void);

	bool readDatei();

	bool getKcNummer(CString GUID, int& sdPos, CString &kcNummer, int &kcPos, CString &psp, int &nl, int &Produktline_Brandschutztechnik, COleDateTime& tDateTime);

private:
		CStringArray zeilen;
public:
		bool findEintragReadAllGUID(CStringArray& zeile);
		bool getText(const CString &text, int offset, int pos, CString &result, bool trim);
		bool findEintragDependGUIDNummer(CString sdNummer, CString &zeile);
};
