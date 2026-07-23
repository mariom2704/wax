#ifndef __SONDERKOSTENEINGABEFORM_
#define __SONDERKOSTENEINGABEFORM_

#include "WaxPartForm.h"

class WAXPRJDLL SonderkostenEingabeForm : public WaxPartForm  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(SonderkostenEingabeForm)

public:
	SonderkostenEingabeForm();

	virtual ~SonderkostenEingabeForm();

	virtual BOOL OnInitDialog();

private:
	/// Die Liste zeigt die eingefügten Sonderkosten an
	CListCtrl list;

	bool onEdit();

	bool dirty;

protected:
	Preis::Sonderkosten * readSoko(int index);

	void writeSoko(Preis::Sonderkosten * soko, int index);

		/// Anordnung der Spalten
	enum COLUMNS 
	{
	/// Positionsnummer
		POSNR,
	/// Text
		TEXT,
	/// Preis
		PREIS,

		FIX
	};


	void set_dirty(bool val = true);
	bool is_dirty() const;

	virtual void DoDataExchange(CDataExchange* pDX);

	void OnClickedNew();
	
	void OnClickedEdit();
	
	void OnClickedDelete();
	
	void OnDblclkSonderkosten(NMHDR* pNMHDR, LRESULT* pResult);
	
	void OnSize(UINT nType, int cx, int cy);
	
	void OnClickedAuto();
};

#endif
