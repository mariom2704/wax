// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_WAXDOCMANAGER_401506D8005D_INCLUDED
#define _INC_WAXDOCMANAGER_401506D8005D_INCLUDED

/**
Irgendwie macht der CDocManager nicht alles so, wie es sein sollte. Deshalb wird er hier 
überschrieben.
*/
class WaxDocManager : public CDocManager
{
public:
	WaxDocManager();

	virtual ~WaxDocManager();

	/// Dokument öffnen
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

	/// Shell File Type registrieren
	virtual void RegisterShellFileTypes(BOOL bCompat);
};

#endif /* _INC_WAXDOCMANAGER_401506D8005D_INCLUDED */
