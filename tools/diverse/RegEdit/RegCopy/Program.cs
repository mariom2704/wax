using System;
using System.Collections.Generic;
using System.Text;
using RegCopy.Gp;

namespace RegCopy
{
    class Program
    {
        static void Main(string[] args)
        {                           
            try
            {
                string datei = "RegCopy.xml";
                bool showProtokoll = false;   
                GpStart start = new GpStart();
                
                if (args != null && args.Length > 0)
                {
                    if (args[0] == "\\?" || args[0] == "/?" || args[0] == "-?" || args[0] == "?")
                    {                        
                        start.showHelp();
                        return;
                    }

                    string arg0 = args[0].ToLower();
                    string arg1 = "";
                    if (args.Length > 1)
                        arg1 = args[1].ToLower();

                    if (arg0 == "\\p" || arg0 == "/p" || arg0 == "-p" ||
                        (!String.IsNullOrEmpty(arg1) && (arg1 == "\\p" || arg1 == "/p" || arg1 == "-p")))
                    {
                        showProtokoll = true;
                    }

                    if (arg0 != "\\p" && arg0 != "/p" && arg0 != "-p")
                        datei = args[0];
                    else if (!String.IsNullOrEmpty(arg1) && arg1 != "\\p" && arg1 != "/p" && arg1 != "-p")
                            datei = args[1];
                }

                start.start(datei);

                //Meldungen ausgeben
                if (showProtokoll)
                    Console.WriteLine(start.Message);
            }
            catch (Exception e)
            {
                Console.WriteLine("\n" + e.Message);
            }
        }
    }
}
