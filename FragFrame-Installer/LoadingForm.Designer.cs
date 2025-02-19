namespace FragFrame
{
    partial class LoadingForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoadingForm));
            pictureBox1 = new PictureBox();
            ProgressBarContainer = new Panel();
            label1 = new Label();
            label2 = new Label();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            SuspendLayout();
            // 
            // pictureBox1
            // 
            pictureBox1.BackgroundImage = (Image)resources.GetObject("pictureBox1.BackgroundImage");
            pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            pictureBox1.Location = new Point(12, 12);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(355, 355);
            pictureBox1.TabIndex = 0;
            pictureBox1.TabStop = false;
            // 
            // ProgressBarContainer
            // 
            ProgressBarContainer.Location = new Point(373, 311);
            ProgressBarContainer.Name = "ProgressBarContainer";
            ProgressBarContainer.Size = new Size(703, 56);
            ProgressBarContainer.TabIndex = 1;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 26F, FontStyle.Bold | FontStyle.Italic);
            label1.Location = new Point(373, 12);
            label1.Name = "label1";
            label1.Size = new Size(583, 70);
            label1.TabIndex = 2;
            label1.Text = "Installing FragFrame...";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 14F, FontStyle.Italic);
            label2.Location = new Point(373, 98);
            label2.Name = "label2";
            label2.Size = new Size(572, 38);
            label2.TabIndex = 3;
            label2.Text = "Please be patient while we install everything...";
            // 
            // LoadingForm
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.FromArgb(18, 19, 23);
            ClientSize = new Size(1088, 379);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(ProgressBarContainer);
            Controls.Add(pictureBox1);
            ForeColor = Color.White;
            FormBorderStyle = FormBorderStyle.None;
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "LoadingForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Loading...";
            Load += LoadingForm_Load;
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private PictureBox pictureBox1;
        private Panel ProgressBarContainer;
        private Label label1;
        private Label label2;
    }
}