#pragma once
#include "afx.h"



/**
Klasse zur Fehlerverwaltung

Über die set Methoden kann der Fehler entweder 
	- über eine Id bestimmt werden  (setErrorId(..))
	- oder es wird ein Fehlertext zugewiesen (setErrorDescription(..))
*/
class MxError : public CObject
{
public:
	MxError(void);
	virtual ~MxError(void);

	///Fehlertypen
	enum ErrorTyp{
		Information,
		Warning,
		Critical
	};

	bool copy(const MxError &other);		///< kopiert übergebenes Objekt

	virtual MxError *deepCopy()const;		///< Gibt eine neues mit new erzeugtes Objekt zurück


	//Set Methoden
	void setErrorId(const CString &id,ErrorTyp typ = Critical);
	void setErrorDescription(const CString &error,ErrorTyp typ = Critical);

	//Get Methoden
	const CString getErrorDescription()const;
	const CString getErrorId()const;
	const MxError::ErrorTyp getErrorTyp()const;

private:
	CString m_id;	///ID des Fehlers

	CString m_bezeichnung; ///Fehlerbezeichnung

	ErrorTyp m_typ; ///Typ des Fehlers
	
};
