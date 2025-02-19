namespace FragFrame.Installer
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            label1 = new Label();
            label2 = new Label();
            checkBox1 = new CheckBox();
            linkLabel1 = new LinkLabel();
            button1 = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 24F, FontStyle.Bold | FontStyle.Italic);
            label1.Location = new Point(13, 10);
            label1.Name = "label1";
            label1.Size = new Size(848, 65);
            label1.TabIndex = 0;
            label1.Text = "Welcome to the FragFrame Installer!";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 16F);
            label2.Location = new Point(13, 75);
            label2.Name = "label2";
            label2.Size = new Size(885, 45);
            label2.TabIndex = 1;
            label2.Text = "It only takes 2 clicks to install! (it's supposed to be simple :D)";
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Font = new Font("Segoe UI", 12F);
            checkBox1.Location = new Point(13, 143);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(415, 36);
            checkBox1.TabIndex = 2;
            checkBox1.Text = "I've read and agreed to the License";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
            // 
            // linkLabel1
            // 
            linkLabel1.AutoSize = true;
            linkLabel1.Font = new Font("Segoe UI", 12F);
            linkLabel1.Location = new Point(13, 182);
            linkLabel1.Name = "linkLabel1";
            linkLabel1.Size = new Size(663, 32);
            linkLabel1.TabIndex = 3;
            linkLabel1.TabStop = true;
            linkLabel1.Text = "Don't wanna read it? Don't worry, we've simplified it for you!";
            linkLabel1.LinkClicked += linkLabel1_LinkClicked;
            // 
            // button1
            // 
            button1.Cursor = Cursors.No;
            button1.FlatAppearance.BorderSize = 0;
            button1.FlatStyle = FlatStyle.Flat;
            button1.Font = new Font("Segoe UI", 18F);
            button1.Location = new Point(12, 233);
            button1.Name = "button1";
            button1.Size = new Size(1031, 75);
            button1.TabIndex = 4;
            button1.Text = "Install!";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.FromArgb(18, 19, 23);
            ClientSize = new Size(1055, 320);
            Controls.Add(button1);
            Controls.Add(linkLabel1);
            Controls.Add(checkBox1);
            Controls.Add(label2);
            Controls.Add(label1);
            ForeColor = Color.White;
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "Form1";
            Text = "FragFrame Installer";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private Label label2;
        private CheckBox checkBox1;
        private LinkLabel linkLabel1;
        private Button button1;
    }
}
