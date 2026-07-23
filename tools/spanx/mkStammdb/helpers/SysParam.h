#pragma once

#include <mxodbc/MXDatabase.h>

namespace MkStammDb
{
	namespace Helpers
	{
		class SysParam
		{
		public:
			SysParam(MXDatabase* server_db, const std::string& parameter)
			{
				db = server_db;
				para = parameter;
			}

			std::string load_str() const
			{

				std::string user = get_user();
				std::string select = "SELECT wert FROM Systemparameter WHERE benutzer = '" + user + "' AND name = '" + para + "'";
				std::string ret = db->exec_SQL(select);
				return ret;				
			}


		private:
			MXDatabase* db;
			std::string para;

			std::string get_user() const
			{
				char us[255];
				memset(us, 0, 255);
				DWORD size = 255;
				::GetUserName(us, &size);
				return us;
			}
		};
	}
}
