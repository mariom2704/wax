#pragma once

class ROHRKONSTDLL StringPair
{
public:
	StringPair(void);
	StringPair(MxString key, MxString value);
	virtual ~StringPair(void);


	void set_key(const MxString& val);
	const MxString &get_key()const;

	void set_value(const MxString& val);
	const MxString &get_value()const;

private:
	MxString key;
	MxString value;
};
