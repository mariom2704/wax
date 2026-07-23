#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace MkStammDb
{
	namespace Helpers
	{
		class TextFile
		{
		public:
			TextFile(const std::string& fname)
			{
				fn = fname;
			}

			std::vector<std::string> load() const
			{
				std::fstream f;
				f.open(fn.c_str());
				
				std::vector<std::string> filecontent;
				if (!f.is_open())
					return filecontent;

				while (!f.eof())
				{
					char line[2048];
					f.getline(line, 2048);
					string str = line;
					if (str.length())
						filecontent.push_back(str);
				}
				return filecontent;
			}

			std::string load_str() const
			{
				std::string ret;
				std::vector<std::string> content = load();
				for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
					ret += *it + "\r\n";
				return ret;
			}

		private:
			std::string fn;

		};
	}
}