using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.Runtime;

[assembly: CommandClass(typeof(Utils.Befehle.Commands))]

namespace Utils.Befehle
{
    public class Commands
    {
        [CommandMethod("MxSinglePlot")]
        static public void MxPlot()
        {
            BefehlPlotten befehl = new BefehlPlotten();
            befehl.IsMultiplot = false;
            befehl.start();
        }

        [CommandMethod("MxMultiPlot")]
        static public void MxMultiPlot()
        {
            BefehlPlotten befehl = new BefehlPlotten();
            befehl.IsMultiplot = true;
            befehl.start();
        }


       
    }
}
