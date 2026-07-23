// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_MATERIALEINGABE_3EE84C040203_INCLUDED
#define _INC_MATERIALEINGABE_3EE84C040203_INCLUDED

#include "EditMaterial.h"
#include "EditProfile.h"

class MaterialEingabe 
: public CPropertySheet
{
public:
	EditProfile profil;

	MaterialEingabe(Material *material, bool profiltab = false);

	virtual ~MaterialEingabe();

	//{{AFX_VIRTUAL(MaterialEingabe)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(MaterialEingabe)
	 void onRebuildTabs();
	
	DECLARE_MESSAGE_MAP()

private:
	/// Schaltet den Reiter für die Profileingabe default aktiv
	bool active_profiltab;

	Material * mat;

	//Fügt die Tabs ein. Bei Profilen wird der passende Tab hinzugefügt.
	void initTabs();

	EditMaterial editmat;
};

#endif /* _INC_MATERIALEINGABE_3EE84C040203_INCLUDED */
