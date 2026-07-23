using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;

namespace RegCopy.Gp
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
            Console.WriteLine("RegCopy ist ein Tool zum Kopieren von Registry Einträgen");
            Console.WriteLine("\nSyntax:\n");
            Console.WriteLine("RegCopy [dat.xml] [/p]   In der XML Datei stehen die Einstellungen");
            Console.WriteLine("                         /p Es wird ein Protokoll der kopierten Registry Einträge ausgegeben.");
            Console.WriteLine("                         Ohne Parameter muss eine regcopy.xml Datei im Arbeitsverzeichnis liegen");
            Console.WriteLine("\n");
            Console.WriteLine("Format der Einstellungsdatei mit Beispielwerten:\n");

            Console.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
            Console.WriteLine("<Einstellung>");
            Console.WriteLine(" <InsertKey>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles</InsertKey>");
            Console.WriteLine(" <InsertVar></InsertVar>");
            Console.WriteLine(" <ReplaceKey>*VALUE*</ReplaceKey>");
            Console.WriteLine(" <Key>HKEY_CURRENT_USER\\Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles\\*VALUE*\\General</Key>");
            Console.WriteLine(" <Area>HKEY_CURRENT_USER</Area>");
            Console.WriteLine(" <SourceKey>Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles\\*VALUE*</SourceKey>");
            Console.WriteLine(" <CopyKey>Software\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:407\\Profiles\\elcocad</CopyKey>");
            Console.WriteLine("</Einstellung>");

            Console.WriteLine("\n");
            Console.WriteLine("Die Einträge unter InsertKey, InsertVar und ReplaceKey können auch leer gelassen werden.");
        }


        public bool start(string datei)
        {
            string insertKey = "";
            string insertVar = "";
            string replaceValue = "";

            string area = "";
            string sourceKey = "";
            string copyKey = "";


            //Werte einlesen
            GpWerte werte = new GpWerte(datei);
            if (!werte.read(ref insertKey, ref insertVar, ref replaceValue, ref area, ref sourceKey, ref copyKey))
                return false;

            if (String.IsNullOrEmpty(area) || String.IsNullOrEmpty(sourceKey) || String.IsNullOrEmpty(copyKey))
                throw new Exception("Wert war für Area, SourceKey bzw. CopyKey nicht gesetzt.");


            //Prüfen ob Replacestring ermittelt werden muss
            if (!String.IsNullOrEmpty(insertKey))
            {               
                object obj = Registry.GetValue(insertKey, insertVar, null);          
                if (obj != null)
                {
                    int index = sourceKey.IndexOf(replaceValue);
                    if (index >= 0)
                        sourceKey = sourceKey.Replace(replaceValue, obj.ToString());

                    index = copyKey.IndexOf(replaceValue);
                    if (index >= 0)
                        copyKey = copyKey.Replace(replaceValue, obj.ToString());    
                }
            }

            if (sourceKey.ToLower() == copyKey.ToLower())
                throw new Exception("Der SourceKey und der CopyKey sind identisch.");

            //Bereich holen
            RegistryKey mainKey = null;
            if (area == "HKEY_CURRENT_USER")
                mainKey = Registry.CurrentUser;
            else if (area == "HKEY_LOCAL_MACHINE")
                mainKey = Registry.LocalMachine;
            else if (area == "HKEY_CLASSES_ROOT")
                mainKey = Registry.ClassesRoot;
            else if (area == "HKEY_CURRENT_CONFIG")
                mainKey = Registry.CurrentConfig;

            if (mainKey == null)
                throw new Exception(String.Format("Unbekannter Bereich {0}", area));
 
            //Schlüssel einlesen
            RegistryKey key = mainKey.OpenSubKey(sourceKey, RegistryKeyPermissionCheck.ReadSubTree);
            if (key == null)
                throw new Exception(String.Format("Der Schlüssel {0} wurden unter dem Bereich {1} nicht gefunden.", sourceKey, area));


            //Objekt für Kopie erstellen
            try
            {
                mainKey.DeleteSubKeyTree(copyKey);
            }
            catch { }
            RegistryKey newkey = mainKey.CreateSubKey(copyKey, RegistryKeyPermissionCheck.ReadWriteSubTree);

            message += "\n" + copyKey;

            //Schlüssel kopieren
            writeSubKeys(key, newkey, 0);

            newkey.Close();

            return true;
        }

        void writeSubKeys(RegistryKey key, RegistryKey newkey, int ebene)
        {

            //Werte kopieren            
            foreach (string st in key.GetValueNames())
            {
                object obj = key.GetValue(st);
                message += "\n" + getLeerstring(ebene) + st + " = " + obj.ToString();                

                if (newkey != null)
                    newkey.SetValue(st, obj);
            }

            message += "\n--";

            //Unterschlüssel kopieren
            foreach (string st in key.GetSubKeyNames())
            {
                message += "\n" + getLeerstring(ebene) + st;               

                //SubKey einlesen
                RegistryKey subkey = key.OpenSubKey(st, RegistryKeyPermissionCheck.ReadSubTree);
                if (subkey == null)
                    throw new Exception(String.Format("Der Unterschlüssel {0} konnte nicht geöffnet werden", st));
 
                //Neues Objekt erstellen
                RegistryKey subNewKey = null;
                if (newkey != null)
                    subNewKey = newkey.CreateSubKey(st);
                
                writeSubKeys(subkey, subNewKey, ebene + 1);
            }

        }

        string getLeerstring(int anz)
        {
            string st = "";
            for (int i = 0; i < anz; i++)
                st += "   ";

            return st;
        }

    }
}
