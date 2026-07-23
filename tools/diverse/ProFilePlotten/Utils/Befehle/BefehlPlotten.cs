using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.DatabaseServices;
using AcDatabase = Autodesk.AutoCAD.DatabaseServices.Database;
using AcadLib.AcadDb;
using MxLib.Tools;
using MxLib.Basis;
using System.IO;

namespace Utils.Befehle
{
    /// <summary>
    /// Plotten des aktuellen Layouts in eine PDF Datei. Wenn der Modellbereich aktiv ist, wird in den letzten aktiven Layoutbereich umgeschaltet.
    /// Die PDF wird in dem gleichen Verzeichnis mit dem Namen der DWG abgelegt.  Es werden die Ploteinstellungen des Layouts übernommen.
    /// 
    /// Im AutoCAD sollte unter Extras - Optionen - Plotten das Hintergrund Plotten deaktiviert werden.
    /// 
    /// Wenn Fehler auftreten werden diese in der ProFilePlotten.log Datei im Temp Verzeichnis abgelegt.
    /// 
    /// Todo:
    /// Hinweis: Multiplot funktioniert noch nicht!
    /// </summary>
    public class BefehlPlotten : BefehlBase
    {
        public bool IsMultiplot { get; set; }

        protected override bool startCommand()
        {                               
            //Dateiname der Cad Datei  
            string dat = "";
            if (!DbAcTools.getFilename(ref dat, true))
            {
                Errorliste.addError("Der Dateiname der DWG konnte nicht ermittelt werden!");
                writeLog();
                return false;
            }

            //Datei ohne Endung
            string ext = "";
            string file = "";
            string verz = "";
            FileString fileTool = new FileString(dat);
            fileTool.exctract(ref verz, ref file, ref ext);

            string filename = "";
            filename = verz + file + ".pdf";            

            //auf das letzte aktuelle Layout wechseln
            AcDatabase db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
            db.TileMode = false;

            //Plotten
            bool result = true;
            DbAcPlot plotten = new DbAcPlot();
            if (IsMultiplot)
                result = plotten.plotAllLayouts(DbAcPlot.Plotter.pdf, filename);
            else
                result = plotten.plotCurrentLayout(DbAcPlot.Plotter.pdf, filename);


            //Wenn Fehler, dann Log-Datei schreiben
            if (!result)
            {
                appendErrorListe(plotten);
                Errorliste.addError("Datei: " + dat);
                writeLog();
                return false;
            }
            
            return true;
        }

        private bool writeLog()
        {
            if (Errorliste.Count == 0)
                return true;

            //Log Dateinamen
            string tmpPfad = System.IO.Path.GetTempPath();
            tmpPfad += "ProFilePlotten.log";

            string time = DateTime.Now.ToString();

            try
            {
                using (StreamWriter myFile = new StreamWriter(tmpPfad, true))
                {
                    foreach (MxError error in Errorliste)
                    {
                        myFile.WriteLine(time + ": " + error.ToString());
                    }

                    myFile.Close();
                }
            }
            catch (Exception ex)
            {
                Errorliste.addError(ex);
                return false;
            }

            return true;
        }

                   
    }
}
