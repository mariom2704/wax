using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace WaxEdit.Gp
{

	public class GpEinstellungen
	{
		string mXmlDatei;

		string idDatenbank = "Datenbank";

		public string Datei
		{
			set
			{
				mXmlDatei = value;
			}
		}



		public bool readWerte(ref string datenbank)
		{
			try
			{
				StreamReader stream = new StreamReader(mXmlDatei);
				XmlTextReader reader = new XmlTextReader(stream);

				while (reader.Read())
				{
					if (reader.LocalName.Equals(idDatenbank))
						datenbank = reader.ReadString();
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
	}
}

