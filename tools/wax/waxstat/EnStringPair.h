#pragma once
#include <QtCore>

/**
Ein Stringpaar speichern
*/
class EnStringPair
{
public:
	EnStringPair(void);
	EnStringPair(const QString &val1,const QString &val2);
	~EnStringPair(void);

	void setString1(const QString &val);
	void setString2(const QString &val);
	void setStringPair(const QString &val1,const QString &val2);

	const QString getString1()const;
	const QString getString2()const;
	bool getStringPair(QString &val1,QString &val2);


private:
	QString m_str1,
		    m_str2;
};
