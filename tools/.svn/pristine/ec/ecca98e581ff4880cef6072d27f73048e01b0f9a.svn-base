#pragma once

#include <string>
#include <vector>

class TestStklist
{
public:
	
	TestStklist(const std::string& db);

	bool do_test(bool fixit);

	std::vector<int> get_result() const;

	std::string get_error() const;


private:
	std::string error;
	std::string dbname;
	std::vector<int> result;
};