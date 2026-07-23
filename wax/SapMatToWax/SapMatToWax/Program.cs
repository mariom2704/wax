using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace SapMatToWax
{
    static class Program
    {
        /// <summary>
        /// Der Haupteinstiegspunkt für die Anwendung.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            if (args != null && args.Length == 2)
                Application.Run(new Start(args[0], args[1]));
            else
                Application.Run(new Start());
        }
    }
}