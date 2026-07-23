
using MxLib.Dialog;
namespace MxStat.Dialog
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.buttonStart = new System.Windows.Forms.Button();
            this.dateTimePickerBis = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerVon = new System.Windows.Forms.DateTimePicker();
            this.labelBis = new System.Windows.Forms.Label();
            this.labelVon = new System.Windows.Forms.Label();
            this.buttonExcel = new System.Windows.Forms.Button();
            this.textBoxExcelDat = new System.Windows.Forms.TextBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.labelStartExport = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.labelWanr = new System.Windows.Forms.Label();
            this.textBoxWanr = new System.Windows.Forms.TextBox();
            this.textBoxSdAuftrag = new System.Windows.Forms.TextBox();
            this.labelSdAuftrag = new System.Windows.Forms.Label();
            this.textBoxSdPosVon = new System.Windows.Forms.TextBox();
            this.labelSdPosVon = new System.Windows.Forms.Label();
            this.textBoxSdPosBis = new System.Windows.Forms.TextBox();
            this.labelSdPosBis = new System.Windows.Forms.Label();
            this.listView1 = new MxLib.Dialog.ListViewExtended();
            this.SuspendLayout();
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(799, 45);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(181, 23);
            this.buttonStart.TabIndex = 0;
            this.buttonStart.Text = "Auswertung starten";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.button1_Click);
            // 
            // dateTimePickerBis
            // 
            this.dateTimePickerBis.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dateTimePickerBis.Location = new System.Drawing.Point(463, 84);
            this.dateTimePickerBis.Name = "dateTimePickerBis";
            this.dateTimePickerBis.Size = new System.Drawing.Size(100, 20);
            this.dateTimePickerBis.TabIndex = 4;
            // 
            // dateTimePickerVon
            // 
            this.dateTimePickerVon.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dateTimePickerVon.Location = new System.Drawing.Point(463, 49);
            this.dateTimePickerVon.Name = "dateTimePickerVon";
            this.dateTimePickerVon.Size = new System.Drawing.Size(100, 20);
            this.dateTimePickerVon.TabIndex = 3;
            // 
            // labelBis
            // 
            this.labelBis.AutoSize = true;
            this.labelBis.Location = new System.Drawing.Point(410, 84);
            this.labelBis.Name = "labelBis";
            this.labelBis.Size = new System.Drawing.Size(24, 13);
            this.labelBis.TabIndex = 2;
            this.labelBis.Text = "Bis:";
            // 
            // labelVon
            // 
            this.labelVon.AutoSize = true;
            this.labelVon.Location = new System.Drawing.Point(410, 49);
            this.labelVon.Name = "labelVon";
            this.labelVon.Size = new System.Drawing.Size(29, 13);
            this.labelVon.TabIndex = 0;
            this.labelVon.Text = "Von:";
            // 
            // buttonExcel
            // 
            this.buttonExcel.Location = new System.Drawing.Point(799, 81);
            this.buttonExcel.Name = "buttonExcel";
            this.buttonExcel.Size = new System.Drawing.Size(181, 23);
            this.buttonExcel.TabIndex = 3;
            this.buttonExcel.Text = "Export nach Excel";
            this.buttonExcel.UseVisualStyleBackColor = true;
            this.buttonExcel.Click += new System.EventHandler(this.buttonExcel_Click);
            // 
            // textBoxExcelDat
            // 
            this.textBoxExcelDat.Location = new System.Drawing.Point(799, 117);
            this.textBoxExcelDat.Name = "textBoxExcelDat";
            this.textBoxExcelDat.Size = new System.Drawing.Size(181, 20);
            this.textBoxExcelDat.TabIndex = 4;
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownHeight = 500;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.IntegralHeight = false;
            this.comboBox1.Location = new System.Drawing.Point(12, 42);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(373, 21);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.onSelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 12);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(162, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "SCHRITT 1: Auswertung wählen";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(410, 18);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(125, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "SCHRITT 2: Filter setzen";
            // 
            // labelStartExport
            // 
            this.labelStartExport.AutoSize = true;
            this.labelStartExport.Location = new System.Drawing.Point(796, 15);
            this.labelStartExport.Name = "labelStartExport";
            this.labelStartExport.Size = new System.Drawing.Size(189, 13);
            this.labelStartExport.TabIndex = 7;
            this.labelStartExport.Text = "SCHRITT 3+4: starten und exportieren";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 651);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(992, 22);
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // labelWanr
            // 
            this.labelWanr.AutoSize = true;
            this.labelWanr.Location = new System.Drawing.Point(410, 120);
            this.labelWanr.Name = "labelWanr";
            this.labelWanr.Size = new System.Drawing.Size(42, 13);
            this.labelWanr.TabIndex = 10;
            this.labelWanr.Text = "WA Nr:";
            // 
            // textBoxWanr
            // 
            this.textBoxWanr.Location = new System.Drawing.Point(463, 117);
            this.textBoxWanr.Name = "textBoxWanr";
            this.textBoxWanr.Size = new System.Drawing.Size(100, 20);
            this.textBoxWanr.TabIndex = 11;
            // 
            // textBoxSdAuftrag
            // 
            this.textBoxSdAuftrag.Location = new System.Drawing.Point(666, 47);
            this.textBoxSdAuftrag.Name = "textBoxSdAuftrag";
            this.textBoxSdAuftrag.Size = new System.Drawing.Size(100, 20);
            this.textBoxSdAuftrag.TabIndex = 13;
            // 
            // labelSdAuftrag
            // 
            this.labelSdAuftrag.AutoSize = true;
            this.labelSdAuftrag.Location = new System.Drawing.Point(591, 50);
            this.labelSdAuftrag.Name = "labelSdAuftrag";
            this.labelSdAuftrag.Size = new System.Drawing.Size(62, 13);
            this.labelSdAuftrag.TabIndex = 12;
            this.labelSdAuftrag.Text = "SD-Auftrag:";
            // 
            // textBoxSdPosVon
            // 
            this.textBoxSdPosVon.Location = new System.Drawing.Point(666, 83);
            this.textBoxSdPosVon.Name = "textBoxSdPosVon";
            this.textBoxSdPosVon.Size = new System.Drawing.Size(100, 20);
            this.textBoxSdPosVon.TabIndex = 15;
            // 
            // labelSdPosVon
            // 
            this.labelSdPosVon.AutoSize = true;
            this.labelSdPosVon.Location = new System.Drawing.Point(591, 86);
            this.labelSdPosVon.Name = "labelSdPosVon";
            this.labelSdPosVon.Size = new System.Drawing.Size(67, 13);
            this.labelSdPosVon.TabIndex = 14;
            this.labelSdPosVon.Text = "SD-Pos von:";
            // 
            // textBoxSdPosBis
            // 
            this.textBoxSdPosBis.Location = new System.Drawing.Point(667, 117);
            this.textBoxSdPosBis.Name = "textBoxSdPosBis";
            this.textBoxSdPosBis.Size = new System.Drawing.Size(100, 20);
            this.textBoxSdPosBis.TabIndex = 17;
            // 
            // labelSdPosBis
            // 
            this.labelSdPosBis.AutoSize = true;
            this.labelSdPosBis.Location = new System.Drawing.Point(591, 120);
            this.labelSdPosBis.Name = "labelSdPosBis";
            this.labelSdPosBis.Size = new System.Drawing.Size(62, 13);
            this.labelSdPosBis.TabIndex = 16;
            this.labelSdPosBis.Text = "SD Pos bis:";
            // 
            // listView1
            // 
            this.listView1.FullRowSelect = true;
            this.listView1.GotoNextFieldAfterEnter = MxLib.Dialog.ListViewExtended.gotoNextFieldAfterEnter.senkrecht;
            this.listView1.GridLines = true;
            this.listView1.Location = new System.Drawing.Point(12, 148);
            this.listView1.Name = "listView1";
            this.listView1.RowStyleWhiteGrey = false;
            this.listView1.Size = new System.Drawing.Size(968, 499);
            this.listView1.StandardNachkomma = 2;
            this.listView1.TabIndex = 1;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(992, 673);
            this.Controls.Add(this.textBoxSdPosBis);
            this.Controls.Add(this.labelSdPosBis);
            this.Controls.Add(this.textBoxSdPosVon);
            this.Controls.Add(this.labelSdPosVon);
            this.Controls.Add(this.textBoxSdAuftrag);
            this.Controls.Add(this.labelSdAuftrag);
            this.Controls.Add(this.textBoxWanr);
            this.Controls.Add(this.labelWanr);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.labelStartExport);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.dateTimePickerBis);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.dateTimePickerVon);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.labelBis);
            this.Controls.Add(this.textBoxExcelDat);
            this.Controls.Add(this.labelVon);
            this.Controls.Add(this.buttonExcel);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.buttonStart);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(1000, 700);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Auswertungen";
            this.SizeChanged += new System.EventHandler(this.onSizeChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStart;
        private ListViewExtended listView1;
        private System.Windows.Forms.DateTimePicker dateTimePickerBis;
        private System.Windows.Forms.DateTimePicker dateTimePickerVon;
        private System.Windows.Forms.Label labelBis;
        private System.Windows.Forms.Label labelVon;
        private System.Windows.Forms.Button buttonExcel;
        private System.Windows.Forms.TextBox textBoxExcelDat;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label labelStartExport;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.Label labelWanr;
		private System.Windows.Forms.TextBox textBoxWanr;
		private System.Windows.Forms.TextBox textBoxSdAuftrag;
		private System.Windows.Forms.Label labelSdAuftrag;
		private System.Windows.Forms.TextBox textBoxSdPosVon;
		private System.Windows.Forms.Label labelSdPosVon;
		private System.Windows.Forms.TextBox textBoxSdPosBis;
        private System.Windows.Forms.Label labelSdPosBis;
    }
}

