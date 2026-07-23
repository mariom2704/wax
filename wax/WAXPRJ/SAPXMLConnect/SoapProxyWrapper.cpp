#include "stdafx.h"

#include "SoapProxyWrapper.h"
#include "soapbindingProxy.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CSoapProxyWrapper::funcZPlmMccCadCreateMatpl(const char* soap_endpoint, CString IdNodedest, CString ZplmMccPositionClass, CString ZplmMccPositionMatlDesc, CString& strReturn)
{															
	bindingProxy bindProxy;

	bindProxy.userid = "autocad";
	bindProxy.passwd = "autocad";
	
	_ns1__ZPlmMccCadCreateMatpl				ns1__ZPlmMccCadCreateMatpl;
	_ns1__ZPlmMccCadCreateMatplResponse		ns1__ZPlmMccCadCreateMatplResponse;
	
	ns1__ZPlmMccCadCreateMatpl.IdNodedest = IdNodedest; 

	ns1__ZplmMccNodeHierarchy ZplmMccNodeHierarchy;
	ZplmMccNodeHierarchy.Extident = "1";
	ZplmMccNodeHierarchy.ExtidentParent = "";
	ns1__ZplmMccNodeHierarchyT ZplmMccNodeHierarchyT;
	ns1__ZPlmMccCadCreateMatpl.ItNodeHierarchie = &ZplmMccNodeHierarchyT;
	ns1__ZPlmMccCadCreateMatpl.ItNodeHierarchie[0].item.push_back(&ZplmMccNodeHierarchy);
			
	ns1__ZplmMccPosition ZplmMccPosition;
	ZplmMccPosition.Extident  = "1";
    ZplmMccPosition.Class     = ZplmMccPositionClass; 
    ZplmMccPosition.Classtype = "001";
    ZplmMccPosition.Langu     = "D";
    ZplmMccPosition.MatlDesc  = ZplmMccPositionMatlDesc;
    ZplmMccPosition.DsnOffice = "001";
    ZplmMccPosition.ProdHier  = "AV";
    ZplmMccPosition.ItemCat   = "L";
    ZplmMccPosition.CompQty   = "1";
    ZplmMccPosition.ItemNo    ="0001";
    ZplmMccPosition.CompUnit  = "ST";
	ZplmMccPosition.ProdRel   ="X";
	ns1__ZplmMccPositionT ZplmMccPositionT;
	ns1__ZPlmMccCadCreateMatpl.ItPosition = &ZplmMccPositionT;
	ns1__ZPlmMccCadCreateMatpl.ItPosition[0].item.push_back(&ZplmMccPosition);
	
	

	bindProxy.ZPlmMccCadCreateMatpl(soap_endpoint, NULL, &ns1__ZPlmMccCadCreateMatpl, &ns1__ZPlmMccCadCreateMatplResponse);

	if(ns1__ZPlmMccCadCreateMatplResponse.EtPosition > 0)
	{
		if(ns1__ZPlmMccCadCreateMatplResponse.EtPosition[0].item.size() == 0)
		{
			strReturn = ns1__ZPlmMccCadCreateMatplResponse.EsReturn[0].Message.c_str();
			return false;
		}
		else
		{
			strReturn = ns1__ZPlmMccCadCreateMatplResponse.EtPosition[0].item[0]->Material.c_str();
			return true;
		}
	}
	else
	{
		CString strNoServer = getStammdaten().getText("IDS_SAP_SERVER_NOT_THERE").c_str();
		if( ns1__ZPlmMccCadCreateMatplResponse.EsReturn == 0)
		{
			strReturn = strNoServer;
		}
		else
		{
			strReturn = ns1__ZPlmMccCadCreateMatplResponse.EsReturn[0].Message.c_str();
		}
		return false;
	}
};
