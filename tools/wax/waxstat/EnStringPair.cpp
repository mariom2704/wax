#include ".\enstringpair.h"

EnStringPair::EnStringPair(void)
{
}

EnStringPair::EnStringPair(const QString &val1,const QString &val2)
{
	setStringPair(val1,val2);
}


EnStringPair::~EnStringPair(void)
{
}

void EnStringPair::setString1(const QString &val)
{
	m_str1 = val;
}
void EnStringPair::setString2(const QString &val)
{
	m_str2 = val;
}

void EnStringPair::setStringPair(const QString &val1,const QString &val2)
{
	m_str1 = val1;
	m_str2 = val2;
}


const QString EnStringPair::getString1()const
{
	return m_str1;
}
const QString EnStringPair::getString2()const
{
	return m_str2;
}


bool EnStringPair::getStringPair(QString &val1,QString &val2)
{
	val1 = m_str1;
	val2 = m_str2;

	return true;
}
