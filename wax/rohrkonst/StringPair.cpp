#include "stdafx.h"


#include "MxString.h"
#include "StringPair.h"


StringPair::StringPair(void)
{
}
StringPair::StringPair(MxString key, MxString value)
{
	this->key = key;
	this->value = value;
}

StringPair::~StringPair(void)
{
}





void StringPair::set_key(const MxString& val)
{
	key = val;
}


const MxString &StringPair::get_key()const
{
	return key;
}





void StringPair::set_value(const MxString& val)
{
	value = val;
}

const MxString &StringPair::get_value()const
{
	return value;
}

