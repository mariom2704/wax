#pragma once

class GpAnlage
{
public:
	GpAnlage(MxCObList *auftraege);
	virtual  ~GpAnlage(void);


	enum Status{
		FreigabeSegmentRohr = 0,

		InBearbeitungSaegeNut = 10,
		UnterbrochenSaegeNut = 12,
		FertigSaegeNut = 16,
		StorniertSaegeNut = 18,

		InBearbeitungVerteiler = 20,
		UnterbrochenVerteiler = 22,
		FertigVerteiler = 26,
		StorniertVerteiler = 28
	};


protected:
	MxCObList *get_auftraege();

private:
	MxCObList *auftraege;
};
