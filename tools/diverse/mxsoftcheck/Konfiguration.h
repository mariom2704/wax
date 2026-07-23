#pragma once
#include "stl.h"


namespace MxSoftwareCheck
{
	/**
	Konfiguration des Softwarecheck Programms
	Speichert und erzeugt die Dateinamen und Pfade für die 
	Steuerdatei und die Logdatei
	*/
	class Konfiguration
	{
	public:
		/// Konstruktor
		Konfiguration();

		/// Destruktor
		~Konfiguration();

		/// Erzeugt den Dateinamen für die Steuerungsdatei softwarestand.txt
		void init_localfiles(string localpath);
		
		/// Initialisiert den Pfad zum Serververzeichnis
		void init_logfile_server(string serverpath);

		/// Liefert den Pfad/Dateinamen zur Softwarestand.txt
		vector<string> get_configfile() const;

		/// Wrapper zur Ermittlung des Benutzernamens
		string get_username() const;

		/// Wrapper zur Ermittlung des Rechnernamens
		string get_computername() const;

		/// Liefert den Pfad auf die lokale Logdatei
		string get_logfile() const;

		/// Liefert den Pfad zur Logdatei auf dem Server
		string get_logfile_server() const;

		unsigned long get_maxage() const;
		
		static LCID lcid;

		static bool use_english();

	private:

		vector<string> strFileNames;
		/// Enthält den Pfad zur softwarestand.txt
		string configfile;

		/// Enthält den Pfad auf die lokale Logdatei, wird in init_localfiles initialisiert
		string logfile;

		/// Enthält den Pfad zum Logverzeichnis auf dem Server
		string logfile_server;

	};

}