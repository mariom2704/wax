#include "stdafx.h"


#include "cadmodell_pipe.h"

#include <mxutil/tracer.h>
#include <boost/format.hpp> 

#ifdef _DEBUG
void CadModell::Pipe::dump_list() const
{
	using MxUtil::Tracer;

	Tracer::trace("Bauteil\tArtikelnr\tLaenge\tZmass\tZmass2\tZmass3");

	for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		CadElement* ele = *it;
		std::string str = boost::str(boost::format("%s\t%i\t%i\t%i\t%i\t%i") % (LPCSTR)ele->get_kurztext() % ele->get_artikelnr() % ele->get_laenge() % ele->get_zmass() % ele->get_zmass2() % ele->get_zmass3());
		Tracer::trace(str);
	}
}
#endif