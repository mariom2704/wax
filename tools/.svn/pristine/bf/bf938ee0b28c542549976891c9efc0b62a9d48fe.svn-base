#pragma once

#include <string>
#include <map>

class TestGruppen
{
public:
	
	TestGruppen(const std::string& db);

	bool do_test(bool fixit);

	std::multimap<std::string, std::string> get_result() const;

	std::string get_error() const;


private:
	std::string error;
	std::string dbname;
	std::multimap<std::string, std::string> result;
};