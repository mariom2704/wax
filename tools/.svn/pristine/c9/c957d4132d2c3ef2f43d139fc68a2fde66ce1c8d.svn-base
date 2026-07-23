using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace RegCopy.Gp
{
    public class GpWerte
    {
        string xmlDatei = "";

        public GpWerte(string datei)
        {
            xmlDatei = datei;
        }


        public bool read(ref string insertKey, ref string insertVar, ref string replaceKey,
                         ref string bereich, ref string source, ref  string copy)                         
        {


            StreamReader stream = new StreamReader(xmlDatei);
            XmlTextReader reader = new XmlTextReader(stream);

            while (reader.Read())
            {
                if (reader.LocalName.Equals("InsertKey"))
                    insertKey = reader.ReadString();
                else if (reader.LocalName.Equals("InsertVar"))
                    insertVar = reader.ReadString();
                else if (reader.LocalName.Equals("ReplaceKey"))
                    replaceKey = reader.ReadString();

                else if (reader.LocalName.Equals("SourceKey"))
                    source = reader.ReadString();
                else if (reader.LocalName.Equals("CopyKey"))
                    copy = reader.ReadString();
                else if (reader.LocalName.Equals("Area"))
                    bereich = reader.ReadString();               
            }

            reader.Close();
            stream.Close();

            return true;
        }
    }
}
