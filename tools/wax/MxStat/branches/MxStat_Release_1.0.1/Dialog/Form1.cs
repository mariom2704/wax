using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using MxStat.Gp;


using MxStat.Dialog;
using System.Diagnostics;

namespace MxStat
{
    public partial class Form1 : Form
    {
        

        string sqlDatExtension = ".sql";

        int mLastSelectedIndex = -1;

		Size oldFormSize;

		string server = "",
			   datenbank = "",
			   sqlDatPfad = "";

		GpSqlStatement sql = new GpSqlStatement();

		GpAuswertung gpAuswertung = new GpAuswertung();

		GpTextFile textFile = new GpTextFile();

        public Form1()
        {
            InitializeComponent();

            listView1.RowStyleWhiteGrey = true;

            textBoxExcelDat.Text = "c:\\temp\\MxStat1.xls";

            textBoxExcelDat.Enabled = false;
            buttonExcel.Enabled = false;
 

			if (readEinstelldat())
				readSqlDateien();

			saveFormSize();

        }


		private bool readEinstelldat()
		{
	
			GpEinstellungen einst = new GpEinstellungen();
			einst.Datei = "MxStat.xml";

			if (!einst.readWerte(ref server, ref datenbank, ref sqlDatPfad))
			{
				MessageBox.Show("Die Einstellungsdatei MxStat.xml wurde nicht gefunden!");
				return false;
			}

			return true;
             
		}



		private void saveFormSize()
		{
			//Positionen und Breite speichern
			oldFormSize = this.Size;
		}

        /// <summary>
        /// Auswertung starten
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
		private void button1_Click(object sender, EventArgs e)
		{
			//Werte zurücksetzen
			listView1.Clear();
			gpAuswertung.clear();

			if (comboBox1.SelectedIndex < 0)
			{
				MessageBox.Show("Bitte erst eine Auswertung auswählen!");
				return;
			}

			if (sql == null)
				return;

			if (!insertFilter())
				return;

			//Werte für neue Auswertung setzen
			gpAuswertung.Datenbank = datenbank;
			gpAuswertung.Server = server;

			gpAuswertung.IdAuswertung = GpAuswertung.Auswertung.SQL_Abfrage;
			gpAuswertung.SqlStatement = sql.Text;

			if (!gpAuswertung.start())
			{
				MessageBox.Show("Die Auswertung konnte nicht erstellt werden!");
				return;
			}

			listView1.setMatrix(gpAuswertung.ResultMatrix);
			listView1.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);

			textBoxExcelDat.Enabled = true;
			buttonExcel.Enabled = true;
		}

        private void buttonExcel_Click(object sender, EventArgs e)
        {
            string datei = textBoxExcelDat.Text;

            GpExcel excel = new GpExcel();
            excel.Datei = datei;

			if (gpAuswertung == null)
				return;

			if (!excel.writeMatrixToDatei(gpAuswertung.ResultMatrix))
            {
                MessageBox.Show("Die Exceldatei konnte nicht erstellt werden!");
                return;
            }

            //Exceldatei öffnen
            Process myProcess = new Process();
            myProcess.StartInfo.FileName = "\"" + datei + "\""; 

            try
            {
                myProcess.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }


        }

        /// <summary>
        /// Sql dateien auslesen und im PopUp anzeigen
        /// </summary>
        private void readSqlDateien()
        {
            //string dir = Directory.GetCurrentDirectory();

			if (String.IsNullOrEmpty(this.sqlDatPfad))
			{
				MessageBox.Show("Der Pfad in dem die Abfragen gespeichert sind, ist nicht gesetzt!", "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

            DirectoryInfo dirInfo = new DirectoryInfo(this.sqlDatPfad);


            FileInfo[] files = dirInfo.GetFiles("*" + this.sqlDatExtension);

            foreach (FileInfo file in files)
            {
                comboBox1.Items.Add(file.Name.Substring(0,file.Name.Length - file.Extension.Length));
            }

            if (comboBox1.Items.Count == 1)
                comboBox1.SelectedIndex = 0;
            else
            {
				disableFilter();
            }
        }


		private void disableFilter()
		{
			listView1.Clear();

			if (gpAuswertung != null)
				gpAuswertung.clear();


			labelVon.Enabled = false;
			labelBis.Enabled = false;

			dateTimePickerVon.Enabled = false;
			dateTimePickerBis.Enabled = false;

			labelWanr.Enabled = false;
			textBoxWanr.Enabled = false;

			labelSdAuftrag.Enabled = false;
			labelSdPosVon.Enabled = false;
			labelSdPosBis.Enabled = false;

			textBoxSdAuftrag.Enabled = false;
			textBoxSdPosVon.Enabled = false;
			textBoxSdPosBis.Enabled = false;

			buttonStart.Enabled = false;
			buttonExcel.Enabled = false;
			textBoxExcelDat.Enabled = false;
		}

        private void onSelectedIndexChanged(object sender, EventArgs e)
        {
			disableFilter();

            if (mLastSelectedIndex != comboBox1.SelectedIndex)
            {
                listView1.Clear();
                mLastSelectedIndex = comboBox1.SelectedIndex;
            }

            if (comboBox1.SelectedIndex < 0)
                return;
			
			
			textFile.Datei = this.sqlDatPfad + comboBox1.SelectedItem.ToString() + this.sqlDatExtension;

			if (!textFile.readFile())
				return;
			
			
			checkFilter();
        }


		/**
		 * prüfen welche Filter in der Sql datei gesetzt sind und dann Eingabefelder freischalten
		 */
		private void checkFilter()
		{	
			sql.setSqlText(textFile.getAsString()); 


			//Felder einschalten
			if (sql.hasWaxDateVon())
			{
				labelVon.Enabled = true;
				dateTimePickerVon.Enabled = true;
			}

			if (sql.hasWaxDateBis())
			{
				labelBis.Enabled = true;
				dateTimePickerBis.Enabled = true;
			}

			if (sql.hasWaid())
			{
				labelWanr.Enabled = true;
				textBoxWanr.Enabled = true;
				labelWanr.Text = "Waid:";
			}

			if (sql.hasWanr())
			{
				labelWanr.Enabled = true;
				textBoxWanr.Enabled = true;
				labelWanr.Text = "WA Nr:";
			}

			if (sql.hasSdAuftrag())
			{
				labelSdAuftrag.Enabled = true;
				textBoxSdAuftrag.Enabled = true;
			}

			if (sql.hasSdPosVon())
			{
				labelSdPosVon.Enabled = true;
				textBoxSdPosVon.Enabled = true;
			}

			if (sql.hasSdPosBis())
			{
				labelSdPosBis.Enabled = true;
				textBoxSdPosBis.Enabled = true;
			}

			buttonStart.Enabled = true;
		}


		private bool insertFilter()
		{
			sql.setSqlText(textFile.getAsString()); 

			bool result = true;

			if (sql.hasWaxDateVon())
			{
				if (!sql.insertWaxDateVon(this.dateTimePickerVon.Value))
					result = false;
			}

			if (sql.hasWaxDateBis())
			{
				if (!sql.insertWaxDateBis(this.dateTimePickerBis.Value))
					result = false;
			}

			if (sql.hasWanr())
			{
				if (!sql.insertWanr(textBoxWanr.Text))
					result = false;
			}

			if (sql.hasWaid())
			{
				if (!sql.insertWaid(textBoxWanr.Text))
					result = false;
			}

			if (sql.hasSdAuftrag())
			{
				if (!sql.insertSdAuftrag(textBoxSdAuftrag.Text))
					result = false;
			}

			if (sql.hasSdPosVon())
			{
				if (!sql.insertSdPosVon(textBoxSdPosVon.Text))
					result = false;
			}


			if (sql.hasSdPosBis())
			{
				if (!sql.insertSdPosBis(textBoxSdPosBis.Text))
					result = false;
			}

			return result;
		}


		private void onSizeChanged(object sender, EventArgs e)
		{
			int deltaX = this.Size.Width - oldFormSize.Width;
			int deltaY = this.Size.Height - oldFormSize.Height;

			listView1.Size = new Size(listView1.Size.Width + deltaX,
									  listView1.Size.Height + deltaY);

			labelStartExport.Location = new Point(labelStartExport.Location.X + deltaX,
												  labelStartExport.Location.Y);

			buttonStart.Location = new Point(buttonStart.Location.X + deltaX,
											 buttonStart.Location.Y);

			buttonExcel.Location = new Point(buttonExcel.Location.X + deltaX,
											 buttonExcel.Location.Y);

			textBoxExcelDat.Location = new Point(textBoxExcelDat.Location.X + deltaX,
												 textBoxExcelDat.Location.Y);

			//neue Positionen und Breite speichern
			saveFormSize();
		}

		
    }
}