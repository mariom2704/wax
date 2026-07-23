// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_EDITPROFILE_3EE872870119_INCLUDED
#define _INC_EDITPROFILE_3EE872870119_INCLUDED

class EditProfile 
: public CPropertyPage
{
public:
	EditProfile(Material *material);

	virtual ~EditProfile();

	void set_typ(const CString& value);

	void set_typ(int typ);

	//Erzeugt aus dem Mass2 einen String zur Anzeige
	static CString encodeMass2String(int mass2) ;

	//Entschlüsselt den übergebenen String und liefert das resultierende mass2
	static int decodeMass2String(const CString& var) ;

private:
	void getsetRadio(bool set, const UINT * ids, int& var);

	int winkelLinks;

	int winkelRechts;

	int ansicht;

	int seitenansicht;

	CString typ;

	Material* mat;

	//{{AFX_VIRTUAL(EditProfile)
	public:
	virtual BOOL OnApply();

	virtual BOOL OnSetActive();

	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(EditProfile)
	//Es wurde eine Profilansicht ausgewählt. Als Reaktion müssen die beiden 
	//Winkelangaben vorbelegt/grau geschaltet werden
	 void onClickedFormx();
	
	DECLARE_MESSAGE_MAP()
};

#endif /* _INC_EDITPROFILE_3EE872870119_INCLUDED */
