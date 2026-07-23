using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SapMatToWax.Gp;
using MxLib.Dialog;

namespace SapMatToWax
{
    public partial class Start : DlgBaseSizeChange
    {
        string einstDat = "SapMatToWax.xml";
        string sapdatei = "";
        string stammDb = "";

        public Start(string sapdatei, string stammDb)
        {
            this.sapdatei = sapdatei;
            this.stammDb = stammDb;

            InitializeComponent();

            saveFormSize();
        }

        public Start()
        {
            InitializeComponent();
            saveFormSize();       
        }

        private void Start_Load(object sender, EventArgs e)
        {
            buttonOk.Enabled = false;
            string pr = "";
            string preisEinlesen = "";

            if (String.IsNullOrEmpty(stammDb))
            {
                GpXml einstellungen = new GpXml(einstDat);                
                einstellungen.readWert("ShowPreise", ref pr);
                einstellungen.readWert("PreiseEinlesen", ref preisEinlesen);
                einstellungen.readWert("SapMaterialstamm", ref sapdatei);
                
                if (!einstellungen.readWert("Datenbank", ref stammDb))
                {
                    labelStatus.Text = "Der Pfad der Datenbank konnte nicht eingelesen werden!";                    
                }
            }

            bool prEinlesen = false;
            if (preisEinlesen == "1" || preisEinlesen.ToLower() == "true")
                prEinlesen = true;


            textBoxFile.Text = sapdatei;
            textBoxDb.Text = stammDb;

            if (pr == "1" || pr.ToLower() == "true")
            {
                checkBoxPreise.Visible = true;
                checkBoxPreise.Checked = prEinlesen;
                labelDb.Visible = true;
                textBoxDb.Visible = true;
                buttonWahlDb.Visible = true;
            }
            else
            {
                checkBoxPreise.Visible = false;
                checkBoxPreise.Checked = prEinlesen;
                labelDb.Visible = false;
                textBoxDb.Visible = false;
                buttonWahlDb.Visible = false;
            }
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            this.sapdatei = textBoxFile.Text;
            this.stammDb = textBoxDb.Text;

            buttonStart.Enabled = false;

            labelStatus.Text = "Artikel werden eingelesen ....";
            labelStatus.Update();

            GpSapMatToWax sapToWax = new GpSapMatToWax(sapdatei, stammDb, checkBoxPreise.Checked);
            if (!sapToWax.start())
            {
                MessageBox.Show(sapToWax.ErrorListe.ToStringWithId(), "Fehler");

                labelStatus.Text = "Es ist ein Fehler aufgetreten!" + sapToWax.ErrorListe.ToStringWithId();
                return;
            }

            buttonOk.Enabled = true;

            //Status Meldung
            string meldung = String.Format("Der Artikelstamm wurde erfolgreich aktualisiert.\n\nEs wurden {0} Artikel aktualisiert und {1} Artikel neu hinzugefügt.", sapToWax.ChangedRows, sapToWax.InsertRows);
            labelStatus.Text = meldung;
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonWahl_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.FileName = textBoxFile.Text;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                textBoxFile.Text = this.sapdatei = dlg.FileName;
            }

        }

        private void buttonWahlDb_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.FileName = textBoxDb.Text;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                textBoxDb.Text = this.stammDb = dlg.FileName;
            }
        }


        private void onSizeChanged(object sender, EventArgs e)
        {
            this.reLocationXY(buttonOk);
            
            //neue Positionen und Breite speichern
            saveFormSize();
        }

       
    }
}