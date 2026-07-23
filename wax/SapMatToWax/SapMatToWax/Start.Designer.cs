namespace SapMatToWax
{
    partial class Start
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
            this.buttonStart = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.buttonOk = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxFile = new System.Windows.Forms.TextBox();
            this.buttonWahlFile = new System.Windows.Forms.Button();
            this.checkBoxPreise = new System.Windows.Forms.CheckBox();
            this.labelDb = new System.Windows.Forms.Label();
            this.textBoxDb = new System.Windows.Forms.TextBox();
            this.buttonWahlDb = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(12, 85);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(121, 23);
            this.buttonStart.TabIndex = 0;
            this.buttonStart.Text = "einlesen starten ...";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 133);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Status:";
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Location = new System.Drawing.Point(12, 162);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(0, 13);
            this.labelStatus.TabIndex = 2;
            // 
            // buttonOk
            // 
            this.buttonOk.Location = new System.Drawing.Point(378, 225);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 3;
            this.buttonOk.Text = "Ok";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 17);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Datei:";
            // 
            // textBoxFile
            // 
            this.textBoxFile.Location = new System.Drawing.Point(87, 14);
            this.textBoxFile.Name = "textBoxFile";
            this.textBoxFile.Size = new System.Drawing.Size(269, 20);
            this.textBoxFile.TabIndex = 5;
            // 
            // buttonWahlFile
            // 
            this.buttonWahlFile.Location = new System.Drawing.Point(378, 12);
            this.buttonWahlFile.Name = "buttonWahlFile";
            this.buttonWahlFile.Size = new System.Drawing.Size(75, 23);
            this.buttonWahlFile.TabIndex = 6;
            this.buttonWahlFile.Text = "wählen";
            this.buttonWahlFile.UseVisualStyleBackColor = true;
            this.buttonWahlFile.Click += new System.EventHandler(this.buttonWahl_Click);
            // 
            // checkBoxPreise
            // 
            this.checkBoxPreise.AutoSize = true;
            this.checkBoxPreise.Location = new System.Drawing.Point(190, 89);
            this.checkBoxPreise.Name = "checkBoxPreise";
            this.checkBoxPreise.Size = new System.Drawing.Size(97, 17);
            this.checkBoxPreise.TabIndex = 7;
            this.checkBoxPreise.Text = "Preise einlesen";
            this.checkBoxPreise.UseVisualStyleBackColor = true;
            // 
            // labelDb
            // 
            this.labelDb.AutoSize = true;
            this.labelDb.Location = new System.Drawing.Point(12, 49);
            this.labelDb.Name = "labelDb";
            this.labelDb.Size = new System.Drawing.Size(63, 13);
            this.labelDb.TabIndex = 8;
            this.labelDb.Text = "Datenbank:";
            // 
            // textBoxDb
            // 
            this.textBoxDb.Location = new System.Drawing.Point(87, 46);
            this.textBoxDb.Name = "textBoxDb";
            this.textBoxDb.Size = new System.Drawing.Size(269, 20);
            this.textBoxDb.TabIndex = 9;
            // 
            // buttonWahlDb
            // 
            this.buttonWahlDb.Location = new System.Drawing.Point(378, 44);
            this.buttonWahlDb.Name = "buttonWahlDb";
            this.buttonWahlDb.Size = new System.Drawing.Size(75, 23);
            this.buttonWahlDb.TabIndex = 10;
            this.buttonWahlDb.Text = "wählen";
            this.buttonWahlDb.UseVisualStyleBackColor = true;
            this.buttonWahlDb.Click += new System.EventHandler(this.buttonWahlDb_Click);
            // 
            // Start
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(465, 260);
            this.Controls.Add(this.buttonWahlDb);
            this.Controls.Add(this.textBoxDb);
            this.Controls.Add(this.labelDb);
            this.Controls.Add(this.checkBoxPreise);
            this.Controls.Add(this.buttonWahlFile);
            this.Controls.Add(this.textBoxFile);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonOk);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonStart);
            this.MinimumSize = new System.Drawing.Size(473, 287);
            this.Name = "Start";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "SAP Materialstamm einlesen";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.Start_Load);
            this.SizeChanged += new System.EventHandler(this.onSizeChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxFile;
        private System.Windows.Forms.Button buttonWahlFile;
        private System.Windows.Forms.CheckBox checkBoxPreise;
        private System.Windows.Forms.Label labelDb;
        private System.Windows.Forms.TextBox textBoxDb;
        private System.Windows.Forms.Button buttonWahlDb;
    }
}

