
// xHMTLtestsuiteDoc.h: Schnittstelle der Klasse CxHMTLtestsuiteDoc
//


#pragma once


class CxHMTLtestsuiteDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CxHMTLtestsuiteDoc();
	DECLARE_DYNCREATE(CxHMTLtestsuiteDoc)

// Attribute
public:

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CxHMTLtestsuiteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};


