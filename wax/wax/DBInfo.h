#pragma once

/**
Zeigt Infos zu den eingestellten Datenbanken an
*/
class DBInfo : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	DBInfo();

	virtual BOOL OnInitDialog();
private:
	void setText(int id, const string &dbid);
};


