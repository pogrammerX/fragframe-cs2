using Newtonsoft.Json.Linq;
using System.Diagnostics;
using System.IO.Compression;
using System.Net;

namespace FragFrame.Installer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            // Wasn't there some better way for this?
            Process.Start(new ProcessStartInfo()
            {
                CreateNoWindow = true,
                WindowStyle = ProcessWindowStyle.Hidden,
                FileName = "cmd.exe",
                Arguments = "/C start https://github.com/pogrammerx/fragframe-cs2/blob/master/license-simpified.md"
            });
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            button1.BackColor = Color.FromArgb(106, 38, 222);
            button1.FlatAppearance.MouseDownBackColor = Color.FromArgb(106, 38, 222);
            button1.FlatAppearance.MouseOverBackColor = Color.FromArgb(106, 38, 222);
            button1.ForeColor = Color.FromArgb(170, 170, 170);
            button1.Region = Region.FromHrgn(GDI32.CreateRoundRectRgn(0, 0, button1.Width, button1.Height, 8, 8));
            button1.Cursor = Cursors.No;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                button1.BackColor = Color.FromArgb(126, 58, 242);
                button1.FlatAppearance.MouseDownBackColor = Color.FromArgb(126, 58, 242);
                button1.FlatAppearance.MouseOverBackColor = Color.FromArgb(108, 43, 217);
                button1.ForeColor = Color.White;
                button1.Cursor = Cursors.Default;
            }
            else
            {
                button1.Cursor = Cursors.No;
                button1.BackColor = Color.FromArgb(106, 38, 222);
                button1.FlatAppearance.MouseDownBackColor = Color.FromArgb(106, 38, 222);
                button1.FlatAppearance.MouseOverBackColor = Color.FromArgb(106, 38, 222);
                button1.ForeColor = Color.FromArgb(170, 170, 170);
            }
        }

        // https://stackoverflow.com/a/4897700/27301513
        private void appShortcutToDesktop(string linkName, string app)
        {
            string deskDir = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

            using (StreamWriter writer = new StreamWriter(deskDir + "\\" + linkName + ".url"))
            {
                writer.WriteLine("[InternetShortcut]");
                writer.WriteLine("URL=file:///" + app);
                writer.WriteLine("IconIndex=0");
                string icon = app.Replace('\\', '/');
                writer.WriteLine("IconFile=" + icon);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!checkBox1.Checked)
                return;

            new Thread(() =>
            {
                LoadingForm lf = null;
                this.Invoke(() =>
                {
                    Hide();
                    lf = new();
                    lf.Show();
                });

                using(HttpClient http = new())
                {
                    JObject laiInfo = JObject.Parse(http.GetStringAsync("https://raw.githubusercontent.com/pogrammerX/fragframe-cs2/refs/heads/master/lai-project-inf.json").Result);

                    string tempDir = Path.GetTempPath() + "\\" + Guid.NewGuid().ToString();
                    Directory.CreateDirectory(tempDir);

                    File.WriteAllBytes(tempDir + "\\assets.zip", http.GetByteArrayAsync(laiInfo["assets"].Value<string>()).Result);
                    File.WriteAllBytes(tempDir + "\\binaries.zip", http.GetByteArrayAsync(laiInfo["binaries"].Value<string>()).Result);

                    string endDir = "C:\\Program Files\\FragFrame";
                    if (Directory.Exists(endDir))
                        Directory.Delete(endDir, true);
                    Directory.CreateDirectory(endDir);

                    ZipFile.ExtractToDirectory(tempDir + "\\assets.zip", endDir);
                    ZipFile.ExtractToDirectory(tempDir + "\\binaries.zip", endDir + "\\bin-x64");
                    File.WriteAllText(endDir + "\\ver", laiInfo["version"].Value<string>());

                    Directory.Delete(tempDir, true);

                    appShortcutToDesktop("FragFrame", endDir + "\\bin-x64\\ffapp.exe");
                }
            }).Start();
        }
    }
}
