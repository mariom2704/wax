using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using UpdateWerte.En;
using UpdateWerte.Gp;

namespace UpdateWerte
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();


			
		}

		private void button1_Click(object sender, EventArgs e)
		{
			List<string> keys = new List<string>();
			foreach (object obj in listBoxKey.Items)
			{
				keys.Add(obj.ToString());
			}


			List<string> spalten = new List<string>();
			foreach(object obj in listBox1.Items)
			{
				spalten.Add(obj.ToString());
			}

			GpKonvertierung gp = new GpKonvertierung(textBoxServer1.Text, textBoxDb1.Text, textBoxTab1.Text,
													 textBoxServer2.Text, textBoxDb2.Text, textBoxTab2.Text,
													 keys, spalten);

			int insertRows = 0;
			int changedRows = 0;

			if (!gp.start(checkBox1.Checked, ref changedRows, ref insertRows))
				MessageBox.Show( gp.ErrorListe.ToStringWithId(), "Fehler");
			else
				MessageBox.Show(String.Format("{0} Zeilen geändert.\n{1} Zeilen hinzugefügt", changedRows, insertRows), "Ergebnis");
		}

		private void buttonAdd_Click(object sender, EventArgs e)
		{
			if (String.IsNullOrEmpty(textBoxSpalte.Text))
				return;

			listBox1.Items.Add(textBoxSpalte.Text);
		}

		private void buttonDelete_Click(object sender, EventArgs e)
		{
			if (listBox1.SelectedIndex < 0)
				return;

			listBox1.Items.RemoveAt(listBox1.SelectedIndex);
		}

		private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (listBox1.SelectedIndex < 0)
				return;

			textBoxSpalte.Text = listBox1.SelectedItem.ToString();
		}

		private void buttonAddKey_Click(object sender, EventArgs e)
		{
			if (String.IsNullOrEmpty(textBoxKey.Text))
				return;

			listBoxKey.Items.Add(textBoxKey.Text);
		}

		private void buttonDeleteKey_Click(object sender, EventArgs e)
		{
			if (listBoxKey.SelectedIndex < 0)
				return;

			listBoxKey.Items.RemoveAt(listBoxKey.SelectedIndex);
		}

		private void readDateien()
		{
			string dir = Directory.GetCurrentDirectory();

			/*
			if (String.IsNullOrEmpty(this.sqlDatPfad))
			{
				MessageBox.Show("Der Pfad in dem die Abfragen gespeichert sind, ist nicht gesetzt!", "Fehler", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			 */

			DirectoryInfo dirInfo = new DirectoryInfo(dir);


			FileInfo[] files = dirInfo.GetFiles("*.xml");

			foreach (FileInfo file in files)
			{
				comboBox1.Items.Add(new StringPair(file.Name, file.Name.Substring(0, file.Name.Length - file.Extension.Length)));
			}

			if (comboBox1.Items.Count == 1)
				comboBox1.SelectedIndex = 0;
			
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			readDateien();
		}

		private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			StringPair dat = comboBox1.SelectedItem as StringPair;
			if (dat == null)
				return;

			readXmlDatei(dat.Id);
		}

		private bool readXmlDatei(string datei)
		{
			GpXml xml = new GpXml(datei);

			//Quelle
			textBoxServer1.Text = xml.readWert("Server1");
			textBoxDb1.Text = xml.readWert("Datenbank1");
			textBoxTab1.Text = xml.readWert("Tabelle1");

			//ziel
			textBoxServer2.Text = xml.readWert("Server2");
			textBoxDb2.Text = xml.readWert("Datenbank2");
			textBoxTab2.Text = xml.readWert("Tabelle2");


			string[] keys = xml.readListe("KeySpalte", ',');

			string[] spalten = xml.readListe("Spalten", ',');

			listBox1.Items.Clear();
			if (spalten != null)
				listBox1.Items.AddRange(spalten);

			listBoxKey.Items.Clear();
			if (keys != null)
				listBoxKey.Items.AddRange(keys);


			return true;
		}

	}
}