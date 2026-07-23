#include ".\DbArbeitsfolgen.h"

DbArbeitsfolgen::DbArbeitsfolgen(void)
{
}

DbArbeitsfolgen::~DbArbeitsfolgen(void)
{
}

///Werte aus Db lesen
bool DbArbeitsfolgen::readFromDb(QList<EnArbeitsfolge> &liste)
{
	return false;
}

///Werte in Db (Über)Schreiben
bool DbArbeitsfolgen::writeToDb(const QList<EnArbeitsfolge> &liste)
{
	return false;
}

bool DbArbeitsfolgen::writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste)
{
	return false;
}

//Werte in Db Updaten
bool DbArbeitsfolgen::updateInDb(const QList<EnArbeitsfolge> &liste)
{
	return false;
}


