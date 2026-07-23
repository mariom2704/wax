#pragma once


/**
Kc Auftrag aus der Schnittstellendatei lesen
*/
class WAXPRJDLL GpSapAuftragsdaten
{
public:
	GpSapAuftragsdaten(void);
	virtual ~GpSapAuftragsdaten(void);

	bool readDatei();

	bool getKcNummer(CString strIppeMatNr, __int64 sdNummer, int sdPos, CString &kcNummer, int &kcPos, CString &psp, int &nl, int &Produktline_Brandschutztechnik, COleDateTime& tDateTime);

private:
	CStringArray zeilen;

	bool findEintragDependSDNummer(__int64 sdNummer,int sdPos, CString &zeile);
	bool findEintragDependIppeMatNummer(CString strIppeMatNr, CString &zeile);
	bool getText(const CString &text, int offset, int pos, CString &result, bool trim);
};
