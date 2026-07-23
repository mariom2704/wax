using System;
using System.Collections.Generic;
using System.Text;
using RegChange.Gp;

namespace CopyDatToReg
{
    class Program
    {
        public static void Main(string[] args)
        {
            string datei = "CopyDatToReg.xml";
            GpStart start = new GpStart();

            try
            {
                if (args != null && args.Length > 0)
                {
                    if (args[0] == "\\?" || args[0] == "/?" || args[0] == "-?" || args[0] == "?")
                    {
                        start.showHelp();
                        return;
                    }

                    datei = args[0];
                }

                start.start(datei);

                //Meldungen ausgeben
                Console.WriteLine(start.Message);
            }
            catch (Exception e)
            {
                Console.WriteLine("\n" + e.Message);
            }
        }
    }
}
