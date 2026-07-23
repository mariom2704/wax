#pragma once

#include "MxError.h"
#include <vector>

namespace MxErrorHandling
{
	class ROHRKONSTDLL MxErrorHandler
	{
	public:
		MxErrorHandler()
		{
		}

		MxErrorHandler(const MxErrorHandler& other)
		{
			copy (other);
		}
			
		~MxErrorHandler()
		{
			clearErrorList();
		}

		void operator = (const MxErrorHandler& other)
		{
			copy(other);
		}

		/**
		Alle Attribute von other kopieren
		*/
		bool copy(const MxErrorHandler& other)
		{
			//Hier attribute kopieren
			clearErrorList();
			appendError(other.m_errorListe);

			return true;
		}

		bool isCritical() const
		{
			/**
			liefert true, wenn irgendein kritischer Fehler aufgetreten ist
			*/
			for (std::vector<MxError>::const_iterator it = m_errorListe.begin(); it != m_errorListe.end(); it++)
				if (it->getErrorTyp() == MxError::Critical)
					return true;
			return false;
		}

		int getErrorCount() const
		{
			return (int)m_errorListe.size();
		}

		void appendError(const MxError &error)
		{
			// Fehlermeldung nur einmal eintragen
			for (std::vector<MxError>::iterator it = m_errorListe.begin(); it != m_errorListe.end(); it++)
			{
				if (*it == error)
					return;
			}
			m_errorListe.push_back(MxError(error));
		}


		void appendError(const std::vector<MxError>& errorList)
		{
			//Objekte mit deepCopy kopieren
			for (std::vector<MxError>::const_iterator it = errorList.begin(); it != errorList.end(); it++)
				m_errorListe.push_back(MxError(*it));
		}

		void appendErrorId(int id, MxError::ErrorTyp typ)
		{
			MxError error;

			error.setErrorId(id,typ);
			appendError(error);
		}

		void appendErrorDescription(const std::string& error, MxError::ErrorTyp typ, const std::string& file = "", int line = 0)
		{
			MxError fehler;
			fehler.setFileInformation(file, line);

			fehler.setErrorDescription(error,typ);
			appendError(fehler);
		}


		void clearErrorList()
		{
			m_errorListe.clear();	
		}

		const std::vector<MxError>& getErrorListe() const
		{
			return m_errorListe;
		}
private:
	std::vector<MxError> m_errorListe;
	};

}