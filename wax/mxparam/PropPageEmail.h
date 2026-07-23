#ifndef __PROPPAGEGEEMAIL_H__
#define __PROPPAGEGEEMAIL_H__

#include "waxproppage.h"

/// PropPage Dialogfeld
class PropPageEmail : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	PropPageEmail(MxParameter *para);

	/// Destructor
	~PropPageEmail();
};

#endif 

