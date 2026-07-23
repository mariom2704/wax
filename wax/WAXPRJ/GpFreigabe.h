#pragma once

class WAXPRJDLL GpFreigabe
{
public:
	GpFreigabe(void);
	virtual ~GpFreigabe(void);

	bool freigabeAngebot(WaxPart *wp);

	bool freigabePreisanfrage(WaxPart *wp);

	bool updateStatus(WaxPart *wp);

	bool checkPlausis(WaxPart *wp);

	bool freigabeAngebotspreis(WaxPart *wp);

	int countWerkauftraege(WaxPart *pt);

	bool saveWaxPart(WaxPart *wp, CMemFile *f);
	bool restoreWaxPart(CMemFile *f, WaxPart *wp);

private:
	bool checkBeschichtung(Werkauftrag *werkauftrag);

	bool checkWerte(WaxPart *wp, bool checkSDNr, bool checkPaNrIsSet, bool checkPreisIsSet, Status::STATUS stat);


};
