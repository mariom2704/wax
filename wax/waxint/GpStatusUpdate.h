#pragma once

/**
Statusupdate, lesend und schreibend.

Muss noch ergänzt werden
*/
namespace ServerInterface
{
	class GpStatusUpdate
	{
	public:
		bool readStatus(WaxPart* pt)
		{
			pt->updateStatus();
			for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos != NULL; )
			{
				WaxPart* upt = (WaxPart*)pt->get_subobjects().GetNext(pos);
				upt->updateStatus();					
			}
			return true;
		}
/*
		bool writeStatus(WaxPart* pt)
		{
		}
*/
	};

}