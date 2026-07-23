#pragma once

#include "..\waxprj\Server.h"
#include <string>
#include <vector>
#include <map>
#include "..\waxprj\GpSapWittenbergeAuftragsdaten.h"
class WaxPart;

namespace ServerInterface
{
	/**
	Lädt eine bestimmte Anzahl von Werkaufträgen aus der Datenbank, festgelegt durch eine
	Select-Anweisung. Die Aufträge werden mit load_objects erzeugt und in einem vector 
	aufgelistet
	*/

	
	class  WaIdentity
	{
		public:
			UINT		waid;
			__int64		sdauftragsnr;
			string		GUID;
	};


	class WAXINT WaLoader
	{
		
	public:
		GpSapWittenbergeAuftragsdaten sapWittenbergeAuftragsdaten;

		WaLoader() : serverdb(Server::load, NULL, NULL) 
		{
			status = 0;
		};

		void config(const std::string& selectstr, 
			const std::string& sortstr, 
			const std::string& userstr, 
			bool loadstornoval,
			int stat,			
			DWORD parentwnd = 0,
			UINT progressmsg = 0)
		{
			select = selectstr; 
			sort = sortstr; 
			set_user(userstr);
			load_storno = loadstornoval;
			progmsg = progressmsg;
			parent = parentwnd;
			status = stat;
		}
		
		enum RETVAL
		{
			OK,
			SELECT_RETURNED_NULL, /// Select lieferte nichts zurück
			NO_SERVER_CONNECT, /// Wax Server konnte nicht erreicht werden
		};

		RETVAL prepare_load(BOOL ForceWAX_GID, BOOL ReadOnlyRecordsWithGUID);
		
		vector<WaxPart*> load_objects();

		/// Setzt den User
		void set_user(const std::string& userval);
	
		/// Erzeugen eines WaxPart Objektes über den Klassennamen
		static WaxPart* dyncreate_object(const string& name);

		Server& get_serverdb() { return serverdb; }

		int get_count() const { return waidlst.size(); }
	private:

		std::string select;
		std::string sort;
		bool load_storno;
		std::string user;

		int status;

		UINT progmsg;
		DWORD parent;

		/// Liste mit Waids, die geladen werden sollen
		std::vector<WaIdentity> waidlst;

		/// Liste mit nicht geladenenen Waids
		std::map<UINT, std::string> errors;
	
		/// Erzeugen des SQL Select Strings aus den Angaben in der Maske
		Server serverdb;

		/// Lädt die Ids der Werkaufträge in waidlst
		int load_waids(BOOL ReadGUIDFile, BOOL ReadOnlyRecordsWithGUID);

		/// Lädt einen Werkauftrag aus der Datenbank
		WaxPart* load_object(int thewaid);

		/// Daten aus der Statustabelle lesen und im WaxPart setzen
		bool update_status(WaxPart* pt);

	};
}