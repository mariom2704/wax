using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using MxLib.Basis;

namespace SapMatToWax.Gp
{
    public class GpXml
    {
        string mXmlDatei;

        MxErrorListe errorListe = new MxErrorListe();

        public GpXml(string datei)
        {
            mXmlDatei = datei;
        }

        public bool readWert(string id, ref string wert)
        {
            bool found = false;
            try
            {
                StreamReader stream = new StreamReader(mXmlDatei);
                XmlTextReader reader = new XmlTextReader(stream);

                while (reader.Read())
                {
                    if (reader.LocalName.Equals(id))
                    {
                        wert = reader.ReadString();
                        found = true;
                        break;
                    }
                }

                reader.Close();
                stream.Close();

            }
            catch (Exception ex)
            {
                errorListe.addError(ex.Message);
                return false;
            }

            if (!found)
            {
                ErrorListe.addError(String.Format("Das Attribut '{0}' wurde nicht in der Datei {1} gefunden!", id, mXmlDatei));
            }

            return found;
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

        public MxErrorListe ErrorListe
        {
            get { return errorListe; }
            set { errorListe = value; }
        }
	
    }
}
