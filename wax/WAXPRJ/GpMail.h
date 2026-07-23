#pragma once

class WAXPRJDLL GpMail
{
public:
	GpMail(void);
	virtual ~GpMail(void);

	bool sendMail(const string &email, const string &betreff, const string &text, const string &file);
};
