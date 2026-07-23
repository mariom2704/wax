#pragma once

#include <mxodbc\mxodbc_lib.h>

class BefehlReadDb
{
public:
	BefehlReadDb(void);
	~BefehlReadDb(void);

	bool start();

	bool startJoin1();
	bool startJoin2();


	CStringArray &getWerte();


protected:
	bool openDb(const CString &dbName);

	bool stuecklistewa(int &artNr);

	bool sapmaterialstamm(int artnr);


	bool joinAbfrage();
	bool joinAbfrage2();

	void test_write();

private:
	DWORD_PTR m_dbhandle;

	MXTable* m_opentab;

	CStringArray m_werte;

	CString m_tabArtikelstamm;

};
