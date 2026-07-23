using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.Windows.Forms;

namespace MxStat.Gp
{
    public class GpEinstellungen
    {
        string mXmlDatei;

		string idServer = "Server",
			   idDatenbank = "Datenbank",
			   idSqlDatPfad = "SqlDatPfad";

		public string Datei
		{
            set
            {
                mXmlDatei = value;
            }
		}



		public bool readWerte(ref string server, ref string datenbank, ref string sqlDatPfad)
		{
			try
			{
				StreamReader stream = new StreamReader(mXmlDatei);
				XmlTextReader reader = new XmlTextReader(stream);
				
				while (reader.Read())
				{
					if (reader.LocalName.Equals(idServer))
						server = reader.ReadString();
					else if (reader.LocalName.Equals(idDatenbank))
						datenbank = reader.ReadString();
					else if (reader.LocalName.Equals(idSqlDatPfad))
						sqlDatPfad = reader.ReadString();
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
