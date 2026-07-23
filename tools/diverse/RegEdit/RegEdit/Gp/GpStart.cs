using System;
using System.Collections.Generic;
using System.Text;
using RegChange.En;
using Microsoft.Win32;

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
            Console.WriteLine("RegChange ist ein Tool zum Bearbeiten von Registry Einträgen, bei denen der Eintrag durch einen Separator getrennt ist. Z.B. eine Liste von Verzeichnissen");
            Console.WriteLine("\nSyntax:\n");
            Console.WriteLine("RegChange [dat.xml]  In der XML Datei stehen die Einstellungen");
            Console.WriteLine("                     Ohne Parameter muss eine RegChange.xml Datei im Arbeitsverzeichnis liegen");
            Console.WriteLine("\n");
            Console.WriteLine("\nHinweis: Wenn bei DeleteWert \"*\" eingegeben wird, werden alle Einträge des Keys gelöscht.");
            Console.WriteLine("Format der Einstellungsdatei mit Beispielwerten:\n");
         
            Console.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
            Console.WriteLine("<Einstellung>");
            Console.WriteLine(" <InsertKey>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles</InsertKey>");
            Console.WriteLine(" <InsertVar></InsertVar>");
            Console.WriteLine(" <ReplaceKey>*VALUE*</ReplaceKey>");
            Console.WriteLine(" <Key>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles\\*VALUE*\\General</Key>");
            Console.WriteLine(" <Var>ACAD</Var>");
            Console.WriteLine(" <Separator>;</Separator>");
            Console.WriteLine(" <DeleteWert>help</DeleteWert>");
            Console.WriteLine(" <DeleteWert>Rohrnetzkonfigurator</DeleteWert>");
            Console.WriteLine(" <NewWert Position=\"0\">D:\\programme\\AutoCAD 2008\\help</NewWert>");
            Console.WriteLine(" <NewWert Position=\"1\">M:\\Rohrnetzkonfigurator\\Programm</NewWert>");
            Console.WriteLine(" <NewWert Position=\"100\">M:\\Rohrnetzkonfigurator</NewWert>");
            Console.WriteLine(" <NewWert Position=\"100\">M:\\Rohrnetzkonfigurator\\Menue</NewWert>");
            Console.WriteLine("</Einstellung>");

            Console.WriteLine("\n");
            Console.WriteLine("Die Einträge unter InsertKey, InsertVar und ReplaceKey können auch leer gelassen werden.");
        }

        public bool start(string datei)
        {                 
            string insertKey = "";
            string insertVar = "";
            string replaceValue = "";

            string key = "";
            string var = "";
            string separator = "";

            List<string> del = new List<string>();            
            List<EnRegVal> val = new List<EnRegVal>();

            //Werte einlesen
            GpWerte werte = new GpWerte(datei);
            if (!werte.read(ref insertKey, ref insertVar, ref replaceValue, ref key, ref var, ref separator, del, val))
                return false;                           
           
            //Prüfen ob Replacestring ermittelt werden muss
            if (!String.IsNullOrEmpty(insertKey))
            {
                string repVal = "";
                object obj = Registry.GetValue(insertKey, insertVar, null);
                if (obj != null)
                    repVal = obj as String;

                if (!String.IsNullOrEmpty(repVal))
                {
                    int index = key.IndexOf(replaceValue);
                    if (index >= 0)
                    {
                        key = key.Replace(replaceValue, repVal);
                    }
                }
            }
           
            //Werte neu in Registry schreiben
            GpRegistry reg = new GpRegistry();
            if (!reg.appendValue(key, var, separator, del, val))
                return false;

            //Meldung übernehmen
            message = reg.Message;

            return true;
        }


    }
}
