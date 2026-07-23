#pragma once

#include "MxErrorHandler.h"

/**
Basisklasse für Entityklassen der Anwendungsschicht
*/
class ROHRKONSTDLL WaxBase : public CObject
{
public:
	WaxBase();
	virtual ~WaxBase();

	bool copy(const WaxBase &other); ///kopiert übergebenes Objekt

	//virtuelle Funktionen:
	virtual WaxBase *deepCopy()const;		///Gibt eine neues mit new erzeugtes Objekt zurück

	MxErrorHandling::MxErrorHandler& getErrorHandler()
	{
		return ErrorHandler;
	}

	const MxErrorHandling::MxErrorHandler& getErrorHandler() const
	{
		return ErrorHandler;
	}

	void clearErrorList()
	{
		ErrorHandler.clearErrorList();
	}

	const std::vector<MxErrorHandling::MxError>& getErrorListe() const
	{
		return ErrorHandler.getErrorListe();
	}

	void appendError(const std::vector<MxErrorHandling::MxError>& errorList)
	{
		ErrorHandler.appendError(errorList);
	}


	void appendError(const MxErrorHandling::MxError &error)
	{
		ErrorHandler.appendError(error);
	}

	void appendErrorDescription(const std::string& error)
	{
		ErrorHandler.appendErrorDescription(error, MxErrorHandling::MxError::Critical, __FILE__, __LINE__);
	}

private:
	MxErrorHandling::MxErrorHandler ErrorHandler;
};
