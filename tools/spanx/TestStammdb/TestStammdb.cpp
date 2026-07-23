// TestStammdb.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "TestStklist.h"
#include "TestGruppen.h"
#include <iostream>
#include <map>

using namespace std;

typedef bool (*testrt)(std::string, bool);

map<string, testrt> testroutinen;


bool teststklist(std::string db, bool dofix)
{
	TestStklist test(db);

	if (!test.do_test(dofix))
	{
		cout << "Fehler beim Testaufruf:" << endl;
		cout << test.get_error() << endl;
		return false;
	}

	vector<int> res = test.get_result();

	for (std::vector<int>::iterator it = res.begin(); it != res.end(); it++)
	{
		cout << *it << endl;
	}
	cout << "Gefunden: " << res.size() << " Positionen" << endl;
	return true;
}

bool testgruppen(std::string db, bool dofix)
{
	TestGruppen test(db);
	
	if (!test.do_test(dofix))
	{
		cout << "Fehler beim Testaufruf:" << endl;
		cout << test.get_error() << endl;
		return false;
	}

	multimap<string, string> res = test.get_result();

	for (multimap<string, string>::iterator it = res.begin(); it != res.end(); it++)
	{
		cout << it->first << " : " << it->second << endl;
	}
	cout << "Gefunden: " << res.size() << " Positionen" << endl;
	return true;
}

void print_usage()
{
	for (map<string, testrt>::const_iterator it = testroutinen.begin(); it != testroutinen.end(); it++)
		cout << "Aufruf: TestStammdb " << it->first << " Stammdbname [fix]" << endl;
}

bool check_usage(int argc, _TCHAR* argv[])
{
	if (argc != 3 && argc != 4)
	{
		print_usage();
		return false;
	}

	if (argc == 4)
	{
		std::string fixit = (std::string)argv[3];
		if (fixit != "fix")
		{
			print_usage();
			return false;
		}
	}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	testroutinen["stklist"] =  &teststklist;
	testroutinen["gruppen"] =  &testgruppen;
	
	if (!check_usage(argc, argv))
		return 1;

	bool dofix = argc == 4;
	std::string db = (std::string)argv[2];
	std::string call = (std::string)argv[1];


	map<string, testrt>::const_iterator it = testroutinen.find(call);
	if (it == testroutinen.end())
	{
		cout << "Testaufruf '" << call << "' nicht gefunden!" << endl;
		print_usage();
		return 1;
	}

	if (!(*it).second(db, dofix))
		cout << "Fehler bei Testroutine '" << call << "'" << endl;
	
	return 0;
}

