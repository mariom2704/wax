#pragma once

/**
Geschäftsprozessklasse zum Konvertieren von alten Versionen
*/
class __declspec( dllexport ) GpConvertOldVersion /*: public GpBase*/
{
public:
	GpConvertOldVersion(void);
	virtual ~GpConvertOldVersion(void);

	//alte Id 10 und 11 in 57 und 58 umwandeln
	static int getNewFlanschId(int oldId,int dn);		
};
