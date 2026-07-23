#pragma once


#include <string>


namespace MxErrorHandling
{
	/**
	Klasse zur Fehlerverwaltung

	Über die set Methoden kann der Fehler entweder 
		- über eine Id bestimmt werden  (setErrorId(..))
		- oder es wird ein Fehlertext zugewiesen (setErrorDescription(..))
	*/
	class ROHRKONSTDLL MxError
	{
	public:
		///Fehlertypen
		enum ErrorTyp
		{
			Information,
			Warning,
			Critical,
		};

		MxError(const std::string& err = "", int id = 0, MxError::ErrorTyp typ = Information)
		{
			setErrorId(id, typ);
			setErrorDescription(err.c_str(), typ);
			line = 0;
		}

		MxError(const MxError& other)
		{
			copy(other);
		}

		bool operator == (const MxError& other)
		{
			// Wenn der Text gleich ist, muss auch die Fehlermeldung gleich sein
			return other.getErrorDescription() == getErrorDescription();
		}

		void setFileInformation(const std::string errfile, int errline)
		{
			file = errfile;
			line = errline;
		}

		/**
		Alle Attribute von other kopieren
		*/
		bool MxError::copy(const MxError &other)
		{
			//Hier attribute kopieren
			m_id  = other.m_id;
			m_typ = other.m_typ;
			bezeichnung = other.bezeichnung;
			file = other.get_file();
			line = other.get_line();

			return true;
		}

		void MxError::setErrorId(int id, ErrorTyp typ)
		{
			m_id = id;
			m_typ = typ;
			bezeichnung.clear();
		}

		void MxError::setErrorDescription(const std::string& error, ErrorTyp typ )
		{
			m_typ = typ;
			bezeichnung = error;
		}

		std::string getErrorDescription()const;
		
		int getErrorId()const
		{
			return m_id;
		}

		const ErrorTyp MxError::getErrorTyp()const
		{
			return m_typ;
		}

		const std::string get_file() const
		{
			return file;
		}

		int get_line() const
		{
			return line;
		}

	private:
		int m_id;	///ID des Fehlers

		std::string bezeichnung; ///Fehlerbezeichnung

		ErrorTyp m_typ; ///Typ des Fehlers

		std::string file;
		int line;
		
	};
}