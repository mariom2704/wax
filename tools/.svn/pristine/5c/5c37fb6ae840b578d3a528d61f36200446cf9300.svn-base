// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_REPLACEMENTSTATISTICS_3B1CDB920284_INCLUDED
#define _INC_REPLACEMENTSTATISTICS_3B1CDB920284_INCLUDED

//@class Statistikklasse
class ReplacementStatistics 
{
private:
	const int get_FileCounter() const;
	void set_FileCounter(int value);
	const int get_ReplaceCounter() const;
	void set_ReplaceCounter(int value);
	const int get_RenamedFileCounter() const;
	void set_RenamedFileCounter(int value);

public:
	//@cmember Attribute initialisieren
	void init();

	//@cmember,mfunc Anzeigen einer Messagebox mit den 
	//Statistikwerten
	void display();

	//@cmember Incrementiert den Dateicounter
	inline incFileCounter();

	//@cmember Incrementiert den Zähler für geänderte 
	//Dateinamen
	inline incRenamedFileCounter();

	//@cmember Incrementiert den Ersetzungszähler
	inline incReplaceCounter();

	virtual ~ReplacementStatistics();

	ReplacementStatistics();







protected:
	int FileCounter;

	int ReplaceCounter;

	int RenamedFileCounter;

};

inline ReplacementStatistics::incFileCounter()
{
	//hier nicht editieren; Code wird generiert
	set_FileCounter(get_FileCounter()+1);

}
inline ReplacementStatistics::incRenamedFileCounter()
{
	//hier nicht editieren; Code wird generiert
	set_RenamedFileCounter(get_RenamedFileCounter()+1);

}
inline ReplacementStatistics::incReplaceCounter()
{
	//hier nicht editieren; Code wird generiert
	set_ReplaceCounter(get_ReplaceCounter()+1);

}

#endif /* _INC_REPLACEMENTSTATISTICS_3B1CDB920284_INCLUDED */
