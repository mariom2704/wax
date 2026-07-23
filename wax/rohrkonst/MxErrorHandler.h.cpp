#include "stdafx.h"
#include "MxError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


namespace MxErrorHandling
{
	class MxErrorHandler
	{
	public:
		MxErrorHandler()
		{
		}

		MxErrorHandler(const MxErrorHandler& other)
		{
			copy (other);
		}
			
		~MxErrorHandler(void)
		{
			clearErrorList();
		}

		/**
		Alle Attribute von other kopieren
		*/
		bool copy(const WaxBase &other)
		{
			//Hier attribute kopieren
			clearErrorList();
			appendError(other.m_errorListe);

			return true;
		}


		void appendError(const MxError &error)
		{
			m_errorListe.Add(error.deepCopy());
		}


		void appendError(const CObArray &errorList)
		{
			//Objekte mit deepCopy kopieren
			for (int i=0; i<errorList.GetSize(); i++)
			{
				const MxError* pError = (const MxError*)errorList.GetAt(i);
				m_errorListe.Add(pError->deepCopy());
			}
		}


		void appendErrorId(const CString &id,MxError::ErrorTyp typ /*= ErrorTyp::Critical*/)
		{
			MxError error;

			error.setErrorId(id,typ);
			appendError(error);
		}



		void appendErrorDescription(const CString &error,MxError::ErrorTyp typ /*= ErrorTyp::Critical*/)
		{
			MxError fehler;

			fehler.setErrorDescription(error,typ);
			appendError(fehler);
		}


		/**
		Speicher der Errorliste wieder freigeben
		*/
		void clearErrorList()
		{
			for (int i=0; i<m_errorListe.GetSize(); i++)
			{
				delete m_errorListe.GetAt(i);
			}

			m_errorListe.RemoveAll();
		}

		const CObArray& getErrorListe()const
		{
			return m_errorListe;
		}
		private:
			CObArray m_errorListe;
	};

}