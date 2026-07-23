using MxLib.Dialog;
namespace WaxEdit
{
    partial class FormAddPreis
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.comboBoxElement = new MxLib.Dialog.ComboBoxExt();
            this.textBoxRohrlaengeVon = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.comboBoxWerk = new MxLib.Dialog.ComboBoxExt();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxRohrlaengeBis = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBoxElAnzBis = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBoxElAnzVon = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.comboBoxElId = new MxLib.Dialog.ComboBoxExt();
            this.label9 = new System.Windows.Forms.Label();
            this.textBoxElAnzDnBis = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxElAnzDnVon = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textBoxPreis = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.textBoxPreisVerzinkt = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.comboBoxAnlage = new MxLib.Dialog.ComboBoxExt();
            this.label14 = new System.Windows.Forms.Label();
            this.comboBoxDn = new MxLib.Dialog.ComboBoxExt();
            this.comboBoxDnvl = new MxLib.Dialog.ComboBoxExt();
            this.textBoxBeschreibung = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(342, 504);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(75, 23);
            this.buttonAdd.TabIndex = 0;
            this.buttonAdd.Text = "Hinzufügen";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 119);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "DN Rohr";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 146);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(117, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "DN Element (abhängig)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 55);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Element";
            // 
            // comboBoxElement
            // 
            this.comboBoxElement.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxElement.DropDownWidth = 450;
            this.comboBoxElement.FormattingEnabled = true;
            this.comboBoxElement.Location = new System.Drawing.Point(95, 52);
            this.comboBoxElement.Name = "comboBoxElement";
            this.comboBoxElement.Size = new System.Drawing.Size(322, 21);
            this.comboBoxElement.TabIndex = 4;
            this.comboBoxElement.Values = null;
            // 
            // textBoxRohrlaengeVon
            // 
            this.textBoxRohrlaengeVon.Location = new System.Drawing.Point(134, 185);
            this.textBoxRohrlaengeVon.Name = "textBoxRohrlaengeVon";
            this.textBoxRohrlaengeVon.Size = new System.Drawing.Size(80, 20);
            this.textBoxRohrlaengeVon.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 188);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Rohrlänge von";
            // 
            // comboBoxWerk
            // 
            this.comboBoxWerk.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxWerk.FormattingEnabled = true;
            this.comboBoxWerk.Location = new System.Drawing.Point(95, 79);
            this.comboBoxWerk.Name = "comboBoxWerk";
            this.comboBoxWerk.Size = new System.Drawing.Size(119, 21);
            this.comboBoxWerk.TabIndex = 8;
            this.comboBoxWerk.Values = null;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 82);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Werk";
            // 
            // textBoxRohrlaengeBis
            // 
            this.textBoxRohrlaengeBis.Location = new System.Drawing.Point(134, 211);
            this.textBoxRohrlaengeBis.Name = "textBoxRohrlaengeBis";
            this.textBoxRohrlaengeBis.Size = new System.Drawing.Size(80, 20);
            this.textBoxRohrlaengeBis.TabIndex = 10;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 214);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(72, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Rohrlänge bis";
            // 
            // textBoxElAnzBis
            // 
            this.textBoxElAnzBis.Location = new System.Drawing.Point(134, 275);
            this.textBoxElAnzBis.Name = "textBoxElAnzBis";
            this.textBoxElAnzBis.Size = new System.Drawing.Size(80, 20);
            this.textBoxElAnzBis.TabIndex = 14;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 278);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(92, 13);
            this.label7.TabIndex = 13;
            this.label7.Text = "Elementanzahl bis";
            // 
            // textBoxElAnzVon
            // 
            this.textBoxElAnzVon.Location = new System.Drawing.Point(134, 249);
            this.textBoxElAnzVon.Name = "textBoxElAnzVon";
            this.textBoxElAnzVon.Size = new System.Drawing.Size(80, 20);
            this.textBoxElAnzVon.TabIndex = 12;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 252);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(97, 13);
            this.label8.TabIndex = 11;
            this.label8.Text = "Elementanzahl von";
            // 
            // comboBoxElId
            // 
            this.comboBoxElId.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxElId.DropDownWidth = 450;
            this.comboBoxElId.FormattingEnabled = true;
            this.comboBoxElId.Location = new System.Drawing.Point(95, 356);
            this.comboBoxElId.Name = "comboBoxElId";
            this.comboBoxElId.Size = new System.Drawing.Size(322, 21);
            this.comboBoxElId.TabIndex = 16;
            this.comboBoxElId.Values = null;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 359);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(59, 13);
            this.label9.TabIndex = 15;
            this.label9.Text = "Element ID";
            // 
            // textBoxElAnzDnBis
            // 
            this.textBoxElAnzDnBis.Location = new System.Drawing.Point(134, 327);
            this.textBoxElAnzDnBis.Name = "textBoxElAnzDnBis";
            this.textBoxElAnzDnBis.Size = new System.Drawing.Size(80, 20);
            this.textBoxElAnzDnBis.TabIndex = 20;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 330);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(111, 13);
            this.label10.TabIndex = 19;
            this.label10.Text = "Elementanzahl DN bis";
            // 
            // textBoxElAnzDnVon
            // 
            this.textBoxElAnzDnVon.Location = new System.Drawing.Point(134, 301);
            this.textBoxElAnzDnVon.Name = "textBoxElAnzDnVon";
            this.textBoxElAnzDnVon.Size = new System.Drawing.Size(80, 20);
            this.textBoxElAnzDnVon.TabIndex = 18;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 304);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(116, 13);
            this.label11.TabIndex = 17;
            this.label11.Text = "Elementanzahl DN von";
            // 
            // textBoxPreis
            // 
            this.textBoxPreis.Location = new System.Drawing.Point(109, 436);
            this.textBoxPreis.Name = "textBoxPreis";
            this.textBoxPreis.Size = new System.Drawing.Size(105, 20);
            this.textBoxPreis.TabIndex = 22;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(12, 439);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(30, 13);
            this.label12.TabIndex = 21;
            this.label12.Text = "Preis";
            // 
            // textBoxPreisVerzinkt
            // 
            this.textBoxPreisVerzinkt.Location = new System.Drawing.Point(109, 462);
            this.textBoxPreisVerzinkt.Name = "textBoxPreisVerzinkt";
            this.textBoxPreisVerzinkt.Size = new System.Drawing.Size(105, 20);
            this.textBoxPreisVerzinkt.TabIndex = 24;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(12, 465);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(70, 13);
            this.label13.TabIndex = 23;
            this.label13.Text = "Preis verzinkt";
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // comboBoxAnlage
            // 
            this.comboBoxAnlage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAnlage.FormattingEnabled = true;
            this.comboBoxAnlage.Location = new System.Drawing.Point(95, 398);
            this.comboBoxAnlage.Name = "comboBoxAnlage";
            this.comboBoxAnlage.Size = new System.Drawing.Size(119, 21);
            this.comboBoxAnlage.TabIndex = 26;
            this.comboBoxAnlage.Values = null;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(12, 401);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(40, 13);
            this.label14.TabIndex = 25;
            this.label14.Text = "Anlage";
            // 
            // comboBoxDn
            // 
            this.comboBoxDn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDn.FormattingEnabled = true;
            this.comboBoxDn.Location = new System.Drawing.Point(134, 143);
            this.comboBoxDn.Name = "comboBoxDn";
            this.comboBoxDn.Size = new System.Drawing.Size(80, 21);
            this.comboBoxDn.TabIndex = 27;
            this.comboBoxDn.Values = null;
            // 
            // comboBoxDnvl
            // 
            this.comboBoxDnvl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDnvl.FormattingEnabled = true;
            this.comboBoxDnvl.Location = new System.Drawing.Point(134, 116);
            this.comboBoxDnvl.Name = "comboBoxDnvl";
            this.comboBoxDnvl.Size = new System.Drawing.Size(80, 21);
            this.comboBoxDnvl.TabIndex = 28;
            this.comboBoxDnvl.Values = null;
            // 
            // textBoxBeschreibung
            // 
            this.textBoxBeschreibung.Location = new System.Drawing.Point(95, 13);
            this.textBoxBeschreibung.MaxLength = 60;
            this.textBoxBeschreibung.Name = "textBoxBeschreibung";
            this.textBoxBeschreibung.Size = new System.Drawing.Size(322, 20);
            this.textBoxBeschreibung.TabIndex = 30;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(12, 16);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(75, 13);
            this.label15.TabIndex = 29;
            this.label15.Text = "Beschreibung:";
            // 
            // FormAddPreis
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(429, 543);
            this.Controls.Add(this.textBoxBeschreibung);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.comboBoxDnvl);
            this.Controls.Add(this.comboBoxDn);
            this.Controls.Add(this.comboBoxAnlage);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textBoxPreisVerzinkt);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.textBoxPreis);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.textBoxElAnzDnBis);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBoxElAnzDnVon);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.comboBoxElId);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.textBoxElAnzBis);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBoxElAnzVon);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBoxRohrlaengeBis);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.comboBoxWerk);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBoxRohrlaengeVon);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.comboBoxElement);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonAdd);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormAddPreis";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Preis einfügen";
            this.Load += new System.EventHandler(this.FormAddPreis_Load);
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private ComboBoxExt comboBoxElement;
        private System.Windows.Forms.TextBox textBoxRohrlaengeVon;
        private System.Windows.Forms.Label label4;
        private ComboBoxExt comboBoxWerk;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxRohrlaengeBis;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxElAnzBis;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBoxElAnzVon;
        private System.Windows.Forms.Label label8;
        private ComboBoxExt comboBoxElId;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBoxElAnzDnBis;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxElAnzDnVon;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textBoxPreis;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBoxPreisVerzinkt;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private ComboBoxExt comboBoxAnlage;
        private System.Windows.Forms.Label label14;
        private ComboBoxExt comboBoxDnvl;
        private ComboBoxExt comboBoxDn;
        private System.Windows.Forms.TextBox textBoxBeschreibung;
        private System.Windows.Forms.Label label15;
    }
}