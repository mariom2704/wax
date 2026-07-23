using MxLib.Dialog;
using MxLib.Entities;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WaxEdit.Gp;

namespace WaxEdit
{
    public partial class FormAddPreis : Form
    {
        GpVkX vk = null;

        bool isAdd = true;

        public FormAddPreis(string waxstamm)
        {
            InitializeComponent();
            vk = new GpVkX(null, waxstamm);
        }

        public FormAddPreis(string waxstamm, GpVkX.Vk3Preis editPreis)
        {
            Preis = editPreis;

            InitializeComponent();
            vk = new GpVkX(null, waxstamm);

            comboBoxElement.Enabled = false;
            comboBoxDn.Enabled = false;
            comboBoxDnvl.Enabled = false;

            buttonAdd.Text = "Ändern";

            this.Text = String.Format("Preis ändern (Counter = {0})", Preis.Counter);

            isAdd = false;
        }


        static public GpVkX.Vk3Preis Preis;


        private void FormAddPreis_Load(object sender, EventArgs e)
        {            
            //Elemente 
            List<EnComboBoxWerte> elemente = new List<EnComboBoxWerte>();
            vk.loadElemente(elemente);
            comboBoxElement.Items.AddRange(elemente.ToArray());
            comboBoxElement.setSelectedItem(Preis.Id, true, false);

            //Werke            
            comboBoxWerk.Items.Add(new EnComboBoxWerte("Oldesloe", 0));
            comboBoxWerk.Items.Add(new EnComboBoxWerte("Magdeburg", 1));
            if (Preis.Id > 0)
                comboBoxWerk.setSelectedItem(Preis.Werk, true, false);

            //ElementId
            elemente.Add(new EnComboBoxWerte("Keine Element", 0));
            comboBoxElId.Items.AddRange(elemente.ToArray());
            comboBoxElId.setSelectedItem(Preis.AnzahlElementeId, true, false);

            //Anlage            
            comboBoxAnlage.Items.Add(new EnComboBoxWerte("ja", true));            
            comboBoxAnlage.Items.Add(new EnComboBoxWerte("nein", false));
            comboBoxAnlage.setSelectedItem(Preis.IsAnlage, true, false);

            //Dn Werte
            loadDn(comboBoxDn, true, Preis.Dn);
            loadDn(comboBoxDnvl, false, Preis.Dnvl);

            textBoxElAnzVon.Text = Preis.AnzahlElVon.ToString();
            textBoxElAnzBis.Text = Preis.AnzahlElBis.ToString();

            textBoxElAnzDnVon.Text = Preis.AnzahlElementeDnVon.ToString();
            textBoxElAnzDnBis.Text = Preis.AnzahlElementeDnBis.ToString();

            textBoxRohrlaengeVon.Text = Preis.RohrLaengeVon.ToString();
            textBoxRohrlaengeBis.Text = Preis.RohrLaengeBis.ToString();

            if (!isAdd)
            {
                textBoxPreis.Text = Preis.Preis.ToString();
                textBoxPreisVerzinkt.Text = Preis.PreisVerzinkt.ToString();
            }

            textBoxBeschreibung.Text = Preis.Beschreibung;
        }

        private void loadDn(ComboBoxExt cb, bool withZero, int dnVoreinstellung)
        {            
            int[] dnWerte = { 10, 15, 20, 25, 32, 40, 50, 65, 80, 100, 125, 150, 200, 250, 300, 350, 400 };
 
            if (withZero)
                cb.Items.Add(new EnComboBoxWerte("0", 0));
           
            foreach (int dnWert in dnWerte)
            {                
                cb.Items.Add(new EnComboBoxWerte(dnWert.ToString(), dnWert));
            }

            if (dnVoreinstellung >= 0)
                cb.setSelectedItem(dnVoreinstellung, true, false);
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            errorProvider1.Clear();

            //Pflichtfelder prüfen           

            //Element
            EnComboBoxWerte element = comboBoxElement.getSelectedComboboxWerte();
            if (element == null || element.Value == null)
            {
                errorProvider1.SetError(comboBoxElement, "Keine Auswahl");
                return;
            }
            Preis.Id = (int)element.Value;

            //Werk
            EnComboBoxWerte werk = comboBoxWerk.getSelectedComboboxWerte();
            if (werk == null || werk.Value == null)
            {
                errorProvider1.SetError(comboBoxWerk, "Keine Auswahl");
                return;
            }
            Preis.Werk = (int)werk.Value;


            //DN Wert
            EnComboBoxWerte dn = comboBoxDn.getSelectedComboboxWerte();
            if (dn == null || dn.Value == null)
            {
                errorProvider1.SetError(comboBoxDn, "Keine Auswahl");
                return;
            }
            Preis.Dn = (int)dn.Value;

            //DNvl Wert
            dn = comboBoxDnvl.getSelectedComboboxWerte();
            if (dn == null || dn.Value == null)
            {
                errorProvider1.SetError(comboBoxDnvl, "Keine Auswahl");
                return;
            }
            Preis.Dnvl = (int)dn.Value;

            EnComboBoxWerte anlage = comboBoxAnlage.getSelectedComboboxWerte();
            if (anlage == null || anlage.Value == null)
            {
                errorProvider1.SetError(comboBoxAnlage, "Keine Auswahl");
                return;
            }
            Preis.IsAnlage = (bool)anlage.Value;

            bool result = true;
            if (!checkDouble(textBoxPreis, ref Preis.Preis))
                result = false;

            if (!checkDouble(textBoxPreisVerzinkt, ref Preis.PreisVerzinkt))
                result = false;

            if (!result)
                return;

            //Bedingungen einlesen
            if (!checkInt(textBoxElAnzVon, ref Preis.AnzahlElVon))
                result = false;
            if (!checkInt(textBoxElAnzBis, ref Preis.AnzahlElBis))
                result = false;

            if (!checkInt(textBoxElAnzDnVon, ref Preis.AnzahlElementeDnVon))
                result = false;
            if (!checkInt(textBoxElAnzDnBis, ref Preis.AnzahlElementeDnBis))
                result = false;

            if (!checkInt(textBoxRohrlaengeVon, ref Preis.RohrLaengeVon))
                result = false;
            if (!checkInt(textBoxRohrlaengeBis, ref Preis.RohrLaengeBis))
                result = false;

            element = comboBoxElId.getSelectedComboboxWerte();
            if (element == null || element.Value == null)
            {
                errorProvider1.SetError(comboBoxElId, "Keine Auswahl");
                return;
            }
            Preis.AnzahlElementeId = (int)element.Value;           

            if (!result)
                return;

            Preis.Beschreibung = textBoxBeschreibung.Text;

            if (isAdd)
            {
                if (!vk.insertPreis(Preis))
                {
                    MessageBox.Show(vk.Errorliste.ToString(), "Fehler");
                    return;
                }
                MessageBox.Show("Der Preis wurde in der Datenbank hinzugefügt!");
            }
            else
            {
                if (!vk.editPreis(Preis))
                {
                    MessageBox.Show(vk.Errorliste.ToString(), "Fehler");
                    return;
                }
                MessageBox.Show("Der Preis Werte wurden in der Datenbank geändert!");            
            }


            //Dialog schließen
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private bool checkInt(TextBox textBox, ref int val)
        {
            if (String.IsNullOrEmpty(textBox.Text))
            {
                errorProvider1.SetError(textBox, "Wert muss gesetzt sein!");
                return false;
            }

            if (!int.TryParse(textBox.Text, out val))
            {
                errorProvider1.SetError(textBox, "Falscher Wert!");
                return false;
            }

            return true;
        }

        private bool checkDouble(TextBox textBox, ref double val)
        {           
            if (String.IsNullOrEmpty(textBox.Text))
            {
                errorProvider1.SetError(textBox, "Wert muss gesetzt sein!");
                return false;
            }
 
            string stval = textBox.Text.Replace('.', ',');
            if (!double.TryParse(stval, out val))
            {
                errorProvider1.SetError(textBox, "Falscher Wert!");
                return false;
            }
 
            return true;
        }

       

      

        
    }
}
