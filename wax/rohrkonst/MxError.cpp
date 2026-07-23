#include "stdafx.h"



		#include "MxError.h"

using namespace MxErrorHandling;



std::string MxError::getErrorDescription() const
{
	if (!file.length())
		return bezeichnung;
	
	std::string ret = str(format("%1% (In %2%/%3%)") % bezeichnung % file % line);
	return ret;
}
