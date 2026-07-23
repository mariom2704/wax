#include "stdafx.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TextData::TextData(void)
{
}

TextData::~TextData(void)
{
	
}
void TextData::set_bezeichnung(const std::string& val)
{
	bezeichnung = val;
}
void TextData::set_id(const std::string& val)
{
	id = val;
}


const std::string& TextData::get_bezeichnung()const
{
	return bezeichnung;
}
		
const std::string& TextData::get_id()const
{
	return id;
}



TextDataList::TextDataList(void)
{
}

TextDataList::~TextDataList(void)
{
	for (POSITION pos = this->GetHeadPosition(); pos;)
		delete this->GetNext(pos);
	this->RemoveAll();
}

void TextDataList::dispose()
{
	if (instance)
	{
		delete instance;
		instance = NULL;
	}
}


TextDataList* TextDataList::instance = 0;

/**
gibt die einzige Instance zu der Klasse zurück
*/
 TextDataList* TextDataList::getInstance()
 {
   if( !instance )
     instance = new TextDataList();
   return instance;
 }

 

const std::string& TextDataList::getText(const std::string& id, bool nullIfNotFound)
{
	TextData  *txt = NULL;
	for (POSITION apos = this->GetHeadPosition(); apos;)
	{
		txt = (TextData *)this->GetNext(apos);
		if (txt->get_id() == id)
			return txt->get_bezeichnung();
	}
	
	//Wenn Eintrag nicht vorhanden ist neuen anlegen und in der Liste speichern
	TextData* thedata = new TextData;
	thedata->set_id(id);
	
	if (!nullIfNotFound)
		thedata->set_bezeichnung("[" + id + "]");

	this->AddTail(thedata);

	return  thedata->get_bezeichnung();

}

std::string TextDataList::getTextWithColon(const std::string& id)
{
	std::string str = getText(id) + ":";
	return str;
}