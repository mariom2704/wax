
#ifndef __PROPPAGEGEFREIGABE_H__
#define __PROPPAGEGEFREIGABE_H__

#include "waxproppage.h"

/// PropPage Dialogfeld
class PropPageFreigabe : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstructor
	PropPageFreigabe(MxParameter *para);

	/// Destructor
	~PropPageFreigabe();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();
/// Default-Werte setzen
	virtual void set_default();

	
private:
	BOOL paraAutoFreigabe;
	
	BOOL paraSaveFreigabe;

	BOOL paraCheckStatus;

	BOOL paraCheckSap;

	
};

#endif ///  __PROPPAGEGEFREIGABE_H__

