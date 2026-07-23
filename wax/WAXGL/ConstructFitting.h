#pragma once

#include "ConstructRohrEle.h"

class WAXGLDLL ConstructFitting : public ConstructRohrEle  
  
{
private:
	bool showGfzeichen;

	CString gfzeichen;

	int offset;

public:
	const int get_offset() const;

	void set_offset(int value);

	const bool get_showGfzeichen() const;

	void set_showGfzeichen(bool value);

	const CString& get_gfzeichen() const;

	void set_gfzeichen(const CString& value);

	virtual void createBeschriftung();

	virtual ~ConstructFitting();

	ConstructFitting();

};
