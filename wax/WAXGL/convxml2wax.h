#pragma once


#include <xml/dom/mxxml_dom.h>


#include "cadwerkauftrag.h"


class ConvXml2Wax
{
public:

	ConvXml2Wax();

	virtual ~ConvXml2Wax();

	bool convert(CadWerkauftrag* rohr);

};