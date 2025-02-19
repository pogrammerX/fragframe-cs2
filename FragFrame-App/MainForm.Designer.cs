namespace FragFrame
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            SideBar = new Panel();
            StartButton = new Button();
            label1 = new Label();
            pictureBox1 = new PictureBox();
            indexPage = new Panel();
            label15 = new Label();
            label14 = new Label();
            label13 = new Label();
            label12 = new Label();
            label11 = new Label();
            label10 = new Label();
            label9 = new Label();
            label8 = new Label();
            label7 = new Label();
            label6 = new Label();
            label5 = new Label();
            label4 = new Label();
            label3 = new Label();
            label2 = new Label();
            SideBar.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            indexPage.SuspendLayout();
            SuspendLayout();
            // 
            // SideBar
            // 
            SideBar.BackColor = Color.FromArgb(26, 28, 35);
            SideBar.Controls.Add(StartButton);
            SideBar.Controls.Add(label1);
            SideBar.Controls.Add(pictureBox1);
            SideBar.Dock = DockStyle.Left;
            SideBar.Location = new Point(0, 0);
            SideBar.Name = "SideBar";
            SideBar.Size = new Size(332, 1028);
            SideBar.TabIndex = 0;
            // 
            // StartButton
            // 
            StartButton.FlatAppearance.BorderSize = 0;
            StartButton.FlatStyle = FlatStyle.Flat;
            StartButton.Font = new Font("Segoe UI", 12F, FontStyle.Bold, GraphicsUnit.Point, 0);
            StartButton.Location = new Point(18, 952);
            StartButton.Name = "StartButton";
            StartButton.Size = new Size(299, 64);
            StartButton.TabIndex = 2;
            StartButton.Text = "Start FragFrame";
            StartButton.UseVisualStyleBackColor = true;
            StartButton.Click += StartButton_Click;
            // 
            // label1
            // 
            label1.Font = new Font("Segoe UI", 16F, FontStyle.Bold | FontStyle.Italic, GraphicsUnit.Point, 0);
            label1.ImageAlign = ContentAlignment.TopCenter;
            label1.Location = new Point(3, 287);
            label1.Name = "label1";
            label1.Size = new Size(326, 53);
            label1.TabIndex = 1;
            label1.Text = "FragFrame for CS2";
            label1.TextAlign = ContentAlignment.TopCenter;
            // 
            // pictureBox1
            // 
            pictureBox1.BackgroundImage = (Image)resources.GetObject("pictureBox1.BackgroundImage");
            pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            pictureBox1.Location = new Point(30, 12);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(272, 272);
            pictureBox1.TabIndex = 0;
            pictureBox1.TabStop = false;
            // 
            // indexPage
            // 
            indexPage.Controls.Add(label15);
            indexPage.Controls.Add(label14);
            indexPage.Controls.Add(label13);
            indexPage.Controls.Add(label12);
            indexPage.Controls.Add(label11);
            indexPage.Controls.Add(label10);
            indexPage.Controls.Add(label9);
            indexPage.Controls.Add(label8);
            indexPage.Controls.Add(label7);
            indexPage.Controls.Add(label6);
            indexPage.Controls.Add(label5);
            indexPage.Controls.Add(label4);
            indexPage.Controls.Add(label3);
            indexPage.Controls.Add(label2);
            indexPage.Location = new Point(335, 0);
            indexPage.Name = "indexPage";
            indexPage.Size = new Size(1083, 1028);
            indexPage.TabIndex = 1;
            // 
            // label15
            // 
            label15.AutoSize = true;
            label15.Font = new Font("Segoe UI", 12F);
            label15.Location = new Point(27, 952);
            label15.Name = "label15";
            label15.Size = new Size(765, 64);
            label15.TabIndex = 13;
            label15.Text = "Neither I, pogrammerX, nor any contributors to the FragFrame project \r\nare affiliated with the Valve Corporation.";
            // 
            // label14
            // 
            label14.AutoSize = true;
            label14.Font = new Font("Segoe UI", 16F, FontStyle.Bold);
            label14.Location = new Point(27, 907);
            label14.Name = "label14";
            label14.Size = new Size(182, 45);
            label14.TabIndex = 12;
            label14.Text = "Legal Note";
            // 
            // label13
            // 
            label13.AutoSize = true;
            label13.Font = new Font("Segoe UI", 12F, FontStyle.Bold);
            label13.ForeColor = Color.FromArgb(255, 200, 200);
            label13.Location = new Point(27, 693);
            label13.Name = "label13";
            label13.Size = new Size(772, 32);
            label13.TabIndex = 11;
            label13.Text = "Important Note: Do NOT Join any Server until FragFrame is ready!";
            // 
            // label12
            // 
            label12.AutoSize = true;
            label12.Font = new Font("Segoe UI", 12F);
            label12.Location = new Point(27, 614);
            label12.Name = "label12";
            label12.Size = new Size(692, 64);
            label12.TabIndex = 10;
            label12.Text = "After the Game opened, wait for the \"FragFrame ready\" popup, \r\nthen Join the Server and start Spectating!";
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Font = new Font("Segoe UI", 16F, FontStyle.Bold);
            label11.Location = new Point(27, 569);
            label11.Name = "label11";
            label11.Size = new Size(271, 45);
            label11.TabIndex = 9;
            label11.Text = "3. Wait and Join!";
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Font = new Font("Segoe UI", 12F);
            label10.Location = new Point(27, 516);
            label10.Name = "label10";
            label10.Size = new Size(770, 32);
            label10.TabIndex = 8;
            label10.Text = "Yes, that's it! You don't even need to start your Game, we do it for you!";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Font = new Font("Segoe UI", 16F, FontStyle.Bold);
            label9.Location = new Point(27, 471);
            label9.Name = "label9";
            label9.Size = new Size(456, 45);
            label9.TabIndex = 7;
            label9.Text = "2. Click on \"Start FragFrame\"";
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Font = new Font("Segoe UI", 12F);
            label8.ForeColor = Color.Red;
            label8.Location = new Point(27, 393);
            label8.Name = "label8";
            label8.Size = new Size(898, 64);
            label8.TabIndex = 6;
            label8.Text = "FragFrame should not lower Trust Factor or cause VAC bans, but it very well might. \r\nUse at your own risk!";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Font = new Font("Segoe UI", 12F);
            label7.Location = new Point(27, 361);
            label7.Name = "label7";
            label7.Size = new Size(1005, 32);
            label7.TabIndex = 5;
            label7.Text = "Due to the nature of FragFrame, we recommend using a Steam Account, you don't play with.";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Font = new Font("Segoe UI", 16F, FontStyle.Bold);
            label6.Location = new Point(27, 316);
            label6.Name = "label6";
            label6.Size = new Size(282, 45);
            label6.TabIndex = 4;
            label6.Text = "1. Log into Steam";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Font = new Font("Segoe UI", 12F);
            label5.Location = new Point(27, 230);
            label5.Name = "label5";
            label5.Size = new Size(770, 64);
            label5.TabIndex = 3;
            label5.Text = "FragFrame makes spectating eSport events and Tournaments a breeze!\r\nJust follow these 3 simple steps to Get Started:";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Font = new Font("Segoe UI", 20F, FontStyle.Bold | FontStyle.Italic);
            label4.Location = new Point(17, 176);
            label4.Name = "label4";
            label4.Size = new Size(730, 54);
            label4.TabIndex = 2;
            label4.Text = "Easy Instructions for the easy Overlay";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Segoe UI", 12F, FontStyle.Italic);
            label3.Location = new Point(27, 98);
            label3.Name = "label3";
            label3.Size = new Size(776, 32);
            label3.TabIndex = 1;
            label3.Text = "The easiest Counter-Strike 2 Overlay for eSports Events and Tournaments";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 24F, FontStyle.Bold | FontStyle.Italic);
            label2.Location = new Point(17, 23);
            label2.Name = "label2";
            label2.Size = new Size(548, 65);
            label2.TabIndex = 0;
            label2.Text = "Welcome to FragFrame";
            // 
            // MainForm
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.FromArgb(18, 19, 23);
            ClientSize = new Size(1418, 1028);
            Controls.Add(indexPage);
            Controls.Add(SideBar);
            ForeColor = Color.White;
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            MaximizeBox = false;
            Name = "MainForm";
            Text = "FragFrame";
            Load += MainForm_Load;
            SideBar.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            indexPage.ResumeLayout(false);
            indexPage.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private Panel SideBar;
        private PictureBox pictureBox1;
        private Button StartButton;
        private Label label1;
        private Panel indexPage;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label8;
        private Label label7;
        private Label label6;
        private Label label5;
        private Label label13;
        private Label label12;
        private Label label11;
        private Label label10;
        private Label label9;
        private Label label15;
        private Label label14;
    }
}