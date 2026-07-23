using System;
using System.Collections.Generic;
using System.Text;
using RegChange.En;
using System.IO;
using System.Xml;

namespace RegChange.Gp
{
    public class GpWerte
    {
        string xmlDatei = "";

        public GpWerte(string datei)
        {
            xmlDatei = datei;
        }


        public bool read(ref string insertKey, ref string insertVar, ref string replaceKey,
                         ref string key, ref string var, ref  string separator,
                         List<string> del, List<EnRegVal> val)
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
                else if (reader.LocalName.Equals("Key"))
                    key = reader.ReadString();
                else if (reader.LocalName.Equals("Var"))
                    var = reader.ReadString();
                else if (reader.LocalName.Equals("Separator"))
                    separator = reader.ReadString();
                else if (reader.LocalName.Equals("DeleteWert"))
                    del.Add(reader.ReadString());
                else if (reader.LocalName.Equals("NewWert"))
                {
                    int ipos = 0;
                    if (reader.MoveToFirstAttribute())
                    {

                        string pos = reader.Value;

                        if (!String.IsNullOrEmpty(pos))
                        {
                            int.TryParse(pos, out ipos);
                        }

                    }

                    EnRegVal rv = new EnRegVal(reader.ReadString(), ipos);
                    val.Add(rv);
                }
            }

            reader.Close();
            stream.Close();

            return true;
        }
    }
}
