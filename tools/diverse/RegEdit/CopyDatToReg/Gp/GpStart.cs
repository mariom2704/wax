using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;

namespace RegChange.Gp
{
    public class GpStart
    {
        string message = "";

        public string Message
        {
            get { return message; }
        }

        public void showHelp()
        {
            Console.WriteLine("\n");
            Console.WriteLine("CopyDatToReg ist ein Tool zum Kopieren von Dateien in einem Pfad der in der Registry gespeichert ist");
            Console.WriteLine("\nSyntax:\n");
            Console.WriteLine("CopyDatToReg [dat.xml]   In der XML Datei stehen die Einstellungen");
            Console.WriteLine("                         Ohne Parameter muss eine CopyDatToReg.xml Datei im Arbeitsverzeichnis liegen");
            Console.WriteLine("\n");
            Console.WriteLine("Format der Einstellungsdatei mit Beispielwerten:\n");

            Console.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
            Console.WriteLine("<Einstellung>");
            Console.WriteLine(" <InsertKey>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles</InsertKey>");
            Console.WriteLine(" <InsertVar></InsertVar>");
            Console.WriteLine(" <ReplaceKey>*VALUE*</ReplaceKey>");
           
            Console.WriteLine(" <Key>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles\\*VALUE*\\General</Key>");
            Console.WriteLine(" <Var>PrinterStyleSheetDir</Var>");

            Console.WriteLine(" <File>D:\\temp\\Test1.ctb</File>");
            Console.WriteLine(" <File>D:\\temp\\Test2.ctb</File>");
            Console.WriteLine("</Einstellung>");

            Console.WriteLine("\n");
            Console.WriteLine("Die Einträge unter InsertKey, InsertVar und ReplaceKey können auch leer gelassen werden.");
            Console.WriteLine("Die Dateien unter File werden in das Verzeichnis kopiert, dass unter Var gespeichert ist.");
        }


        public bool start(string datei)
        {
            string insertKey = "";
            string insertVar = "";
            string replaceValue = "";

            string key = "";
            string var = "";
            List<string> dats = new List<string>();

            //Werte einlesen
            GpWerte werte = new GpWerte(datei);
            if (!werte.read(ref insertKey, ref insertVar, ref replaceValue, ref key, ref var, dats))
                return false;

            if (String.IsNullOrEmpty(key))
                throw new Exception("Der Wert für Key ist nicht gesetzt.");

            //Prüfen ob Replacestring ermittelt werden muss
            object obj;
            if (!String.IsNullOrEmpty(insertKey))
            {
                obj = Registry.GetValue(insertKey, insertVar, null);
                if (obj != null)
                {
                    int index = key.IndexOf(replaceValue);
                    if (index >= 0)
                        key = key.Replace(replaceValue, obj.ToString());
                }
            }


            obj = Registry.GetValue(key, var, null);
            if (obj == null)
                throw new Exception(String.Format("Der Wert der Variablen {0} beim Key {1} wurde in der Registry nicht gefunden.", var, key));
   

            //Dateien Kopieren
            foreach (string dat in dats)
            {
                try
                {
                    //Dateiname extrahieren
                    string datname = dat.Substring(dat.LastIndexOf('\\') + 1);

                    File.Copy(dat, obj.ToString() + "\\" + datname, true);
                }
                catch (Exception e)
                {
                    message += "\n" + e.Message;
                }
            }


           
            return true;
        }

        

     
    }
}
