#include ".\dbbase.h"

#include <QMessageBox>
#include <QtCore>




DbBase::DbBase(void)
{
}

DbBase::~DbBase(void)
{
}


void DbBase::setDatenbankParameter(const EnDatenbankParameter &datenbankParameter)
{
	m_datenbankParameter = datenbankParameter;
}
