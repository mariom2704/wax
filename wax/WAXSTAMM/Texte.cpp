#include "stdafx.h"
#include <mxodbc\mxodbc.h>




Texte::Texte(): StammDat("texte", "", "id")
{
}

Texte::~Texte()
{
	TextDataList::dispose();
}



void Texte::load(const std::string& db)
{
	//vor dem Laden Liste löschen. (Beim Sprachenwechsel ist die Liste schon gefüllt)
	TextDataList::dispose();

	//Wenn kein Text gefunden wurde, diesen aus der Db lesen
	MXTable tbl(get_database(db), get_tbl());	
	DECLARE_COL(String, Id, tbl);
	DECLARE_COL(String, Bezeichnung, tbl);

	std::string sql = "select Id, " + WaxSys::getLanguage() + " from texte";
	tbl.set_sql(sql);
	tbl.load();

	string tmpBez;
	lst.RemoveAll();

	while (!tbl.isEof())
	{
		//data* thedata = new Texte::data;

		TextData *txtData = new TextData;
		tmpBez = *Id.data;
		txtData->set_id(tmpBez);



		//thedata->id	= *Id.data;

		tmpBez = *Bezeichnung.data;

		if (tmpBez.empty())
		{
			tmpBez = "[" + *Id.data + "] ";
		}
		else
		{
			setLineFeed(tmpBez);
			setTab(tmpBez);
		}
		//thedata->bezeichnung = tmpBez;

		txtData->set_bezeichnung(tmpBez);

		//lst.AddTail(thedata);


		TextDataList::getInstance()->AddTail(txtData);

		tbl.moveNext();
	}
}


const std::string& Texte::getText(const std::string& id, bool nullIfNotFound)
{
	return TextDataList::getInstance()->getText(id, nullIfNotFound);
}

std::string Texte::getSql(const std::string& sprache, const std::string& id)
{
	std::string sql = "select " + sprache + " from texte where Id='";
	sql += id + "'";
	return sql;
}

Texte::data::data() 
{

}


const std::string& Texte::data::get_bezeichnung()const
{
	return bezeichnung;
}
		
const std::string& Texte::data::get_id()const
{
	return id;
}


void Texte::setLineFeed(std::string& val)
{
	int nr =  val.find("\\n");
	if (nr < 0)
		return;

	val.replace(nr, 2, "\n");
}


void Texte::setTab(std::string& val)
{
	int nr =  val.find("\\t");
	if (nr < 0)
		return;

	val.replace(nr, 2, "\t");
}