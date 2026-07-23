#pragma once

#include "waxcadmodell.h"
#include "cadxmlparser.h"

namespace CadModell
{
	class Xml2CadModell
	{
	public:
		Xml2CadModell(){};

		bool convert(const string& xmlmodell, WaxCadModell& cadmodell) const
		{
			MxXml::DomDocument doc; // Initialisiert und terminiert die XML Unterstützung

			CadXmlParser handler(cadmodell);

			try
			{
				return doc.parse(xmlmodell, handler);
			}
			catch(DOM::SAXException& x)
			{
				string msg = "Fehler bei der XML Umwandlung: " + XStr(x.getMessage()).to_string();
				TRACE("%s\n", msg.c_str());
				cadmodell.ferthinweis = msg.c_str();
			}
			catch(Element::Error e)
			{
				cadmodell.ferthinweis = "Fehler bei der Elementinitialisierung ";
				cadmodell.ferthinweis += e.get_errorText();
			}
			catch(...)
			{
				TRACE("Fehler bei der XML Umwandlung\n");
				cadmodell.ferthinweis = "Unbekannter Fehler bei der Umwandlung";
			}
			return false;
		}

	};
}