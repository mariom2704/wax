using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MxLib.Dialog;
using WaxEdit.Gp;
using MxLib.Entities;
using MxLib.Basis;
using System.Diagnostics;
using MxLib.Db;
using System.IO;
using System.Reflection;


namespace WaxEdit
{
	public partial class Form1 : DlgBaseSizeChange
	{
		EnMatrix preisMatrix = new EnMatrix();

		GpVkX vkx; 

		string waxstammDb = "";

		int minDnVk3 = 0;
		int maxDnVk3 = 300;

		int minDnVk2 = 15;
		int maxDnVk2 = 50;

        int minDnVk3Muffe = 0;
        int maxDnVk3Muffe = 0;

		enum auswertung
		{
			keine = -1,
			vk3 = 0,
			vk2 = 1,
            vk3Muffe = 2,
			festpreis = 3,
			festpreisAlle = 4,
		}

		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			//neue Positionen und Breite speichern
			saveFormSize();
			
			if (!readEinstellungen())
				buttonLoad.Enabled = false;

			this.Text += "   " + waxstammDb;

			vkx = new GpVkX(null, waxstammDb);

            setFilter();
            
            //Auswahl nach setzen der Filter setzen, damit Werte der Auswahl richtig zugeordnet werden können
            comboBoxAuswertung.SelectedIndex = 0;
		}

        /// <summary>
        /// Werte für Comboboxen laden
        /// </summary>
        private void setFilter()
        {
            //Werke
            EnComboBoxWerte werkOd = new EnComboBoxWerte("Oldesloe", 0);
            comboBoxWerk.Items.Add(werkOd);
            EnComboBoxWerte werkMd = new EnComboBoxWerte("Magdeburg", 1);
            comboBoxWerk.Items.Add(werkMd);

            comboBoxWerk.load(werkOd);

            //Muffenanzahl
            EnComboBoxWerte muffenAlle = new EnComboBoxWerte("Kein Filter", 0);
            muffenAlle.IntValue = 0;
            comboBoxMuffen.Items.Add(muffenAlle);
            
            EnComboBoxWerte muffenFilter = new EnComboBoxWerte("1", 0);
            muffenFilter.IntValue = 1;
            comboBoxMuffen.Items.Add(muffenFilter);

            muffenFilter = new EnComboBoxWerte("2", 0);
            muffenFilter.IntValue = 2;
            comboBoxMuffen.Items.Add(muffenFilter);

            muffenFilter = new EnComboBoxWerte("3", 0);
            muffenFilter.IntValue = 3;
            comboBoxMuffen.Items.Add(muffenFilter);

            muffenFilter = new EnComboBoxWerte(">3", 3);
            muffenFilter.IntValue = 0;
            comboBoxMuffen.Items.Add(muffenFilter);

            comboBoxMuffen.load(muffenAlle);

            //Rohrlängen
            EnComboBoxWerte rohrlaengeAlle = new EnComboBoxWerte("Kein Filter", 0);
            rohrlaengeAlle.IntValue = 12000;
            comboBoxRohrlaenge.Items.Add(rohrlaengeAlle);

            EnComboBoxWerte rohrlaenge = new EnComboBoxWerte("1399-1949", 1399);
            rohrlaenge.IntValue = 1949;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            rohrlaenge = new EnComboBoxWerte("1950-2000", 1949);
            rohrlaenge.IntValue = 2000;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            rohrlaenge = new EnComboBoxWerte("2001-2899", 2000);
            rohrlaenge.IntValue = 2899;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            rohrlaenge = new EnComboBoxWerte("2900-3000", 2899);
            rohrlaenge.IntValue = 3000;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            rohrlaenge = new EnComboBoxWerte("3001-5799", 3000);
            rohrlaenge.IntValue = 5799;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            rohrlaenge = new EnComboBoxWerte("5800-6000", 5799);
            rohrlaenge.IntValue = 6000;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);
            
            rohrlaenge = new EnComboBoxWerte("6001-12000", 6000);
            rohrlaenge.IntValue = 12000;
            comboBoxRohrlaenge.Items.Add(rohrlaenge);

            comboBoxRohrlaenge.load(rohrlaengeAlle);

            //Dn Liste laden
            loadDnFilter(comboBoxDNVon);
            loadDnFilter(comboBoxDnBis);

            //Elementfilter laden
            loadElementfilter();
        }

        private void loadElementfilter()
        {
            EnComboBoxWerte keinFilter = new EnComboBoxWerte("Kein Filter", 0);
            comboBoxEle.Items.Add(keinFilter); 

            List<EnComboBoxWerte>elemente = new List<EnComboBoxWerte>();
            vkx.loadElemente(elemente);

            comboBoxEle.Items.AddRange(elemente.ToArray());

            comboBoxEle.load(keinFilter);
        }

        private void loadDnFilter(ComboBoxExt cb)
        {
            EnComboBoxWerte keinFilter = new EnComboBoxWerte("Kein Filter", 0);            
            cb.Items.Add(keinFilter);

            int[] dnWerte = { 10, 15, 20, 25, 32, 40, 50, 65, 80, 100, 125, 150, 200, 250, 300, 350, 400 };

            EnComboBoxWerte dn = null;
            foreach (int dnWert in dnWerte)
            {
                dn = new EnComboBoxWerte(dnWert.ToString(), dnWert);
                cb.Items.Add(dn);
            }

            cb.load(keinFilter);
        }



		private void Form1_SizeChanged(object sender, EventArgs e)
		{
			this.resizeXY(listView1);
			this.reLocationX(buttonSave);

			//neue Positionen und Breite speichern
			saveFormSize();
		}


		private bool readEinstellungen()
		{
			GpEinstellungen einst = new GpEinstellungen();
			einst.Datei = "WaxEdit.xml";

			if (!einst.readWerte(ref waxstammDb))
			{
				MessageBox.Show("Die Einstellungsdatei WaxEdit.xml wurde nicht gefunden!");
				return false;
			}

			return true;
		}


		private auswertung getAuswertung()
		{
			return (auswertung)comboBoxAuswertung.SelectedIndex;
		}


		/**
		 * Button Laden
		 * Werte aus der DB laden
		 */
		private void button1_Click(object sender, EventArgs e)
		{
            laden();
		}

        private void laden()
        {
            //evt. Änderungen speichern
			if (!checkAndSave())
				return;

            abfrageStarten();
        }

        private void abfrageStarten()
        {
            MxErrorListe errors = new MxErrorListe();

            //Werk einlesen
            int werk = 0;
            EnComboBoxWerte cbWerk = comboBoxWerk.getSelectedComboboxWerte();
            if (cbWerk == null)
            {
                errorProvider1.SetError(comboBoxWerk, "Kein Werk definiert!");
                return;
            }
            werk = (int)cbWerk.Value;

            //Elementfilter einlesen
            int element = 0;
            EnComboBoxWerte cbele = comboBoxEle.getSelectedComboboxWerte();
            if (cbele != null && cbele.Value != null)
            {
                element = (int)cbele.Value;
            }

            //Werte aus Db lesen
            if (getAuswertung() == auswertung.vk3)
            {
                //Dn Bereich einlesen
                getDnBereich(ref minDnVk3, ref maxDnVk3);

                if (!vkx.readVk3(werk, checkBoxVerzinkt.Checked, minDnVk3, maxDnVk3, 1, GpVkX.vk2IdStart - 1, element, preisMatrix))
                {
                    showError(vkx);
                    return;
                }

                listView1.GotoNextFieldAfterEnter = ListViewExtended.gotoNextFieldAfterEnter.waagerecht;
            }
            else if (getAuswertung() == auswertung.vk2)
            {
                //Dn Bereich einlesen
                getDnBereich(ref minDnVk2, ref maxDnVk2);

                if (!vkx.readVk3(werk, checkBoxVerzinkt.Checked, minDnVk2, maxDnVk2, GpVkX.vk2IdStart, GpVkX.vk2IdEnde, element, preisMatrix))
                {
                    showError(vkx);
                    return;
                }

                listView1.GotoNextFieldAfterEnter = ListViewExtended.gotoNextFieldAfterEnter.waagerecht;
            }
            else if (getAuswertung() == auswertung.vk3Muffe)
            {
                //Dn Bereich einlesen
                getDnBereich(ref minDnVk3Muffe, ref maxDnVk3Muffe);

                int muffenVon = 0;
                int muffenBis = 100;
                if (!getMuffen(ref muffenVon, ref muffenBis))
                    return;

                int rlaengeVon = 0;
                int rlaengeBis = 12000;
                if (!getRohrlaenge(ref rlaengeVon, ref rlaengeBis))
                    return;

                if (!vkx.readVk3Muffe(werk, checkBoxVerzinkt.Checked, minDnVk3Muffe, maxDnVk3Muffe, 1, GpVkX.vk2IdStart - 1,
                                      muffenVon, muffenBis,
                                      rlaengeVon, rlaengeBis, element,
                                      preisMatrix))
                {
                    showError(vkx);
                    return;
                }

                listView1.GotoNextFieldAfterEnter = ListViewExtended.gotoNextFieldAfterEnter.waagerecht;
            }
            else if (getAuswertung() == auswertung.festpreis)
            {
                if (!vkx.readFestpreis(true, preisMatrix))
                {
                    showError(vkx);
                    return;
                }

                listView1.GotoNextFieldAfterEnter = ListViewExtended.gotoNextFieldAfterEnter.senkrecht;
            }
            else if (getAuswertung() == auswertung.festpreisAlle)
            {
                if (!vkx.readFestpreis(false, preisMatrix))
                {
                    showError(vkx);
                    return;
                }

                listView1.GotoNextFieldAfterEnter = ListViewExtended.gotoNextFieldAfterEnter.senkrecht;
            }
            else
                return;

            listView1.setMatrix(preisMatrix);

            if (getAuswertung() != auswertung.vk3)
                listView1.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);

            listView1.setBackColorWhiteGrey();
            listView1.EditColor = Color.LightPink;

            groupBoxEinstellungen.Enabled = false;
        }

        private void getDnBereich(ref int min, ref int max)
        {
            EnComboBoxWerte cb = comboBoxDNVon.getSelectedComboboxWerte();
            if (cb != null && cb.Value != null)
            {
                min = (int)cb.Value;
            }

            cb = comboBoxDnBis.getSelectedComboboxWerte();
            if (cb != null && cb.Value != null)
            {
                max = (int)cb.Value;
            }
        }

        private bool getRohrlaenge(ref int rlaengeVon, ref int rlaengeBis)
        {
            EnComboBoxWerte cb = comboBoxRohrlaenge.getSelectedComboboxWerte();
            if (cb == null)
                return false;

            rlaengeVon = (int)cb.Value;
            rlaengeBis = cb.IntValue;
            return true;
        }

        private bool getMuffen(ref int muffenVon, ref int muffenBis)
        {
            EnComboBoxWerte cbMuffen = comboBoxMuffen.getSelectedComboboxWerte();
            if (cbMuffen == null)
                return false;

            muffenVon = (int)cbMuffen.Value;
            muffenBis = cbMuffen.IntValue;
            return true;
        }

		/**
		 * Fehler anzeigen
		 */
		private void showError(IMxError error)
		{
			if (error.Errorliste.Count == 0)
				return;

			string meldung = "";
			for (int i=error.Errorliste.Count-1; i>=0; i--)
			{
				meldung += error.Errorliste[i].ToStringWithId() + "\n";
			}

			MessageBox.Show(meldung, "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);

			error.Errorliste.Clear();
		}

		/**
		 * Button speichern
		 * Werte in DB speichern
		 */
		private void buttonSave_Click(object sender, EventArgs e)
		{
			if (!saveWerte())
				return;

			listView1.setBackColorWhiteGrey();
		}

		/**
		 * geänderte Preise in die Db schreiben
		 */
		private bool saveWerte()
		{
			//Werte Speichern
			if (getAuswertung() == auswertung.vk3 || getAuswertung() == auswertung.vk2)
			{
				if (!vkx.saveVk3(checkBoxVerzinkt.Checked, preisMatrix))
				{
					showError(vkx);
					return false;
				}
			}
            if (getAuswertung() == auswertung.vk3Muffe)
            {
                if (!vkx.saveVk3Muffe(checkBoxVerzinkt.Checked, preisMatrix))
                {
                    showError(vkx);
                    return false;
                }
            }
			else if (getAuswertung() == auswertung.festpreis || getAuswertung() == auswertung.festpreisAlle)
			{
				if (!vkx.saveFestpreis(preisMatrix))
				{
					showError(vkx);
					return false;
				}
			}
			
			return true;
		}


		/**
		 * Prüfungen bevor die Anwendung geschlossen wird
		 */
		private void Form_Closing(object sender, FormClosingEventArgs e)
		{
			if (!checkAndSave())
			{
				//Beenden abbrechen
				e.Cancel = true;
			}
		}


		/**
		 * pürfen ob Werte schon gespeichert wurden.
		 * Wenn nicht erfolgt eine Abfrage und es wird gespeichert
		 */
		private bool checkAndSave()
		{
			if (vkx.hasChangedValues(preisMatrix))
			{
				DialogResult result = MessageBox.Show("Die Werte sind noch nicht gespeichert! Wollen Sie speichern?", "Hinweis", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning);

				if (result == DialogResult.Cancel || result == DialogResult.Abort)
					return false;

				if (result == DialogResult.Yes)
				{
					if (!saveWerte())
						return false;
				}
			}

			return true;
		}

		/**
		 * Werte zurücksetzen
		 */
		private void buttonReset_Click(object sender, EventArgs e)
		{
            reset();
		}

        private void reset()
        {
            if (!checkAndSave())
				return;

			preisMatrix.clearAll();
			listView1.Clear();

			groupBoxEinstellungen.Enabled = true;
        }

		private void comboBoxAuswertung_SelectedIndexChanged(object sender, EventArgs e)
		{
            labelDnVon.Text = "DN von:";
            labelDnBis.Text = "DN bis:";


			if (getAuswertung() == auswertung.vk3)
			{
				checkBoxVerzinkt.Enabled = true;

                panelFilter.Enabled = true;
                panelMuffenfilter.Enabled = false;

                comboBoxDNVon.setSelectedItem(minDnVk3, true, false);
                comboBoxDnBis.setSelectedItem(maxDnVk3, true, false);

				//comboBoxDNVon.Text = minDnVk3.ToString();
				//comboBoxDnBis.Text = maxDnVk3.ToString();				
			}
			else if (getAuswertung() == auswertung.vk2)
			{
				checkBoxVerzinkt.Checked = false;
				checkBoxVerzinkt.Enabled = false;

                panelFilter.Enabled = true;
                panelMuffenfilter.Enabled = false;

                comboBoxDNVon.setSelectedItem(minDnVk2, true, false);
                comboBoxDnBis.setSelectedItem(maxDnVk2, true, false);

				//comboBoxDNVon.Text = minDnVk2.ToString();
				//comboBoxDnBis.Text = maxDnVk2.ToString();
			}
            else if (getAuswertung() == auswertung.vk3Muffe)
            {
                checkBoxVerzinkt.Enabled = true;

                panelFilter.Enabled = true;
                panelMuffenfilter.Enabled = true;

                comboBoxDNVon.setSelectedItem(minDnVk3Muffe, true, false);
                comboBoxDnBis.setSelectedItem(maxDnVk3Muffe, true, false);

                //comboBoxDNVon.Text = minDnVk3Muffe.ToString();
                //comboBoxDnBis.Text = maxDnVk3Muffe.ToString();

                labelDnVon.Text = "Rohr DN:";
                labelDnBis.Text = "Muffen DN:";

            }
			else
			{
				checkBoxVerzinkt.Checked = false;
				checkBoxVerzinkt.Enabled = false;

                panelFilter.Enabled = false;
                panelMuffenfilter.Enabled = false;

				comboBoxDNVon.SelectedIndex = 0;
				comboBoxDnBis.SelectedIndex = 0;
			}

			setLabelAuswertung();
		}

		private void buttonExport_Click(object sender, EventArgs e)
		{
            exportExcel();            
		}

        private void exportExcel()
        {
            //Dateinamen ermitteln
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "Excel Datei|*.xlsb";
            saveFileDialog.Title = "Abfrage speichern";
            saveFileDialog.FileName = "Abfrage.xlsb";

            if (saveFileDialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                return;

            if (String.IsNullOrEmpty(saveFileDialog.FileName))
                return;

            //Matrix konvertieren
            EnMatrix mat = new EnMatrix();

            for (int i = 0; i < preisMatrix.ColumnNames.Count; i++)
            {
                mat.setColumnName(i, preisMatrix.ColumnNames[i].ToString());
            }

            for (int spalte = 0; spalte < preisMatrix.MaxColumn; spalte++)
            {
                for (int zeile = 0; zeile < preisMatrix.MaxRow; zeile++)
                {
                    object obj = preisMatrix.getAt(zeile, spalte);

                    ListViewField field = obj as ListViewField;
                    if (field != null)
                    {
                        //if (field.Tag != null)
                        //    obj = field.Tag;
                        //else
                        obj = field.Value;
                    }

                    mat.setAt(zeile, spalte, obj);
                }
            }

            DbExcel excel = new DbExcel();
            if (!excel.writeMatrixToDatei(mat, saveFileDialog.FileName, "Preise", false))
            {
                showError(excel);
                return;
            }

            //Exceldatei öffnen
            Process myProcess = new Process();
            myProcess.StartInfo.FileName = "\"" + saveFileDialog.FileName + "\"";

            try
            {
                myProcess.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


		private void setLabelAuswertung()
		{
			if (getAuswertung() == auswertung.vk3)
			{
				if (checkBoxVerzinkt.Checked)
					labelAuswertung.Text = "Vk3 verzinkt";
				else
					labelAuswertung.Text = "Vk3";
			}
			else if (getAuswertung() == auswertung.vk2)
			{
				labelAuswertung.Text = "Vk2";
			}
			else if (getAuswertung() == auswertung.festpreis)
			{
				labelAuswertung.Text = "Festpreis Artikel (Preis > 0)";
			}
			else if (getAuswertung() == auswertung.festpreisAlle)
			{
				labelAuswertung.Text = "Festpreis alle Artikel";
			}
		}

		private void checkBoxVerzinkt_CheckedChanged(object sender, EventArgs e)
		{
			setLabelAuswertung();
		}

        private void buttonNewPreis_Click(object sender, EventArgs e)
        {
            neuerPreis();
        }

        private void neuerPreis()
        {
            if (vkx.hasChangedValues(preisMatrix))
            {
                MessageBox.Show("Speichern Sie zuerst Ihre Änderungen, bevor Sie einen neuen Preis einfügen!", "Hinweis", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            FormAddPreis dlg = new FormAddPreis(waxstammDb);
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                abfrageStarten(); //Ansicht aktualisieren
        }

        private void listView1_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button != System.Windows.Forms.MouseButtons.Right)
                return;

            int zeile = 0;
            int spalte = 0;
            Rectangle rec = new Rectangle();
            ListViewItem.ListViewSubItem item = listView1.getField(e.X, e.Y, ref zeile, ref spalte, ref rec);

            if (item == null || item.Tag == null)
                return;

            ListViewField field = item.Tag as ListViewField;
            if (field == null)
                return;

            //Wenn es kein Preisfeld ist
            if (field.Tag == null || field.Value == null)
                return;

            GpVkX.Vk3Preis preis = new GpVkX.Vk3Preis();
            preis.Counter = (int)field.Tag;

            //Werte aus DB lesen
            if (!vkx.readPreis(ref preis))
            {
                //Unbekannter Preis
                MessageBox.Show(vkx.Errorliste.ToString(), "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //Preis aus Feld übernehmen
            preis.Preis = (double)field.Value;

            infoPreisToolStripMenuItem.Text = String.Format("Preis {0}€,  DB-Counter = {1}", preis.Preis.ToString("F2"), preis.Counter);
            beschreibungToolStripMenuItem.Text = preis.Beschreibung;
            
            deletePreisToolStripMenuItem.Tag = preis;
            editPreisToolStripMenuItem.Tag = preis;
                        
            //Kontextmenü anzeigen
            contextMenuStrip1.Show(listView1, e.X, e.Y + 10);
        }

        private void deletePreisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (vkx.hasChangedValues(preisMatrix))
            {
                MessageBox.Show("Speichern Sie zuerst Ihre Änderungen, bevor Sie einen Preis löschen!", "Hinweis", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            
            GpVkX.Vk3Preis preis = (GpVkX.Vk3Preis)deletePreisToolStripMenuItem.Tag;

            //Sicherheitsabfrage
            if (MessageBox.Show(String.Format("Wollen Sie wirklich den Preis {0}€ aus der Datenbank löschen?\n(Counter = {1})", preis.Preis, preis.Counter), "Sicherheitsabfrage", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) != System.Windows.Forms.DialogResult.Yes)
                return;

           
            if (!vkx.deletePreis(preis.Counter))
            {
                MessageBox.Show(vkx.Errorliste.ToString(), "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            MessageBox.Show("Der Preis wurde aus der Datenbank gelöscht!", "Info");

            //Ansicht aktualisieren
            abfrageStarten();
        }

        private void editPreisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (vkx.hasChangedValues(preisMatrix))
            {
                MessageBox.Show("Speichern Sie zuerst Ihre Änderungen, bevor Sie einen Preis ändern!", "Hinweis", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
                        
            /* ist schon eingelesen
            GpVkX.Vk3Preis preis = (GpVkX.Vk3Preis)editPreisToolStripMenuItem.Tag;
            if (!vkx.readPreis(ref preis))
            {
                MessageBox.Show(vkx.Errorliste.ToString(), "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            */

            GpVkX.Vk3Preis preis = (GpVkX.Vk3Preis)editPreisToolStripMenuItem.Tag;
            FormAddPreis dlg = new FormAddPreis(waxstammDb, preis);
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                abfrageStarten(); //Ansicht aktualisieren
        }

        private void ladenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            laden();
        }

        private void resetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            reset();
        }

        private void neuerPreisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            neuerPreis();
        }

        private void exportNachExcelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            exportExcel();
        }

        private void beendenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void dokumentationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FileInfo fi = new FileInfo(Assembly.GetEntryAssembly().Location);          
            string pfad = fi.DirectoryName + "\\WaxEditDokumentation.pdf";

            Process myProcess = new Process();
            myProcess.StartInfo.FileName = "\"" + pfad + "\"";

            try
            {
                myProcess.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Fehler");
            }
        }

        private void infoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Version: " + Assembly.GetExecutingAssembly().GetName().Version.ToString(), "WaxEdit");
        }

		

	}
}