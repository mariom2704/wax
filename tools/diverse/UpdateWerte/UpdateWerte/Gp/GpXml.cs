using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace UpdateWerte.Gp
{
    public class GpXml
    {
        string mXmlDatei;

		public GpXml(string datei)
		{
			mXmlDatei = datei;
		}

		public bool readWert(string id, ref string wert)
		{
			try
			{
				StreamReader stream = new StreamReader(mXmlDatei);
				XmlTextReader reader = new XmlTextReader(stream);
				
				while (reader.Read())
				{
					if (reader.LocalName.Equals(id))
					{
						wert = reader.ReadString();
						break;
					}
				}

				reader.Close();
				stream.Close();

			}
			catch
			{
				return false;
			}
			return true;
		}

		public string readWert(string id)
		{
			string wert = "";
			readWert(id, ref wert);

			return wert;
		}

		public string[] readListe(string id, char separator)
		{
			string wert = readWert(id);
			if (string.IsNullOrEmpty(wert))
				return null;

			return wert.Split(separator);
		}

		
    }
}
