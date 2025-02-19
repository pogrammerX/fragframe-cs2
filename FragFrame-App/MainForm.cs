using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

/*
 * 
 * BIG TODO!
 * -> Make the app look less bad, we can use CefSharp & HTML/CSS for the UI.
 * 
 */

namespace FragFrame
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            StartButton.BackColor = Color.FromArgb(126, 58, 242);
            StartButton.FlatAppearance.MouseDownBackColor = Color.FromArgb(126, 58, 242);
            StartButton.FlatAppearance.MouseOverBackColor = Color.FromArgb(108, 43, 217);
            StartButton.Location = new Point((SideBar.Width / 2) - (StartButton.Width / 2), StartButton.Location.Y); // Center the Button, cuz im too lazy to do the math
            StartButton.Region = Region.FromHrgn(GDI32.CreateRoundRectRgn(0, 0, StartButton.Width, StartButton.Height, 8, 8));


        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            new Thread(() =>
            {
                this.Invoke(() =>
                {
                    Hide();
                    LoadingForm loading = new();
                    loading.Show();
                });


                int pid = Loader.StartGame();
                Loader.InjectDll(pid, "C:\\Users\\pogrammerX\\source\\repos\\FragFrame\\x64\\Debug\\fragframe-cs2.dll");
                Thread.Sleep(3200);
                Environment.Exit(0);
            }).Start();
        }
    }
}
