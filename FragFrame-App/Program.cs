using System.Diagnostics;
using System.Text.Json;

namespace FragFrame
{
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            ApplicationConfiguration.Initialize();

            Directory.SetCurrentDirectory(PathUtility.GetBasePath());

            AppDomain.CurrentDomain.UnhandledException += ExceptionHandler;

            using (HttpClient http = new())
            {
                if(File.Exists("../ver"))
                {
                    string installedVersion = File.ReadAllText("../ver");

                    var laiInfo = JsonDocument.Parse(http.GetStringAsync("https://raw.githubusercontent.com/pogrammerX/fragframe-cs2/refs/heads/master/lai-project-inf.json").Result);
                    string lastestVersion = laiInfo.RootElement.GetProperty("version").GetString();

                    if (installedVersion != lastestVersion)
                    {
                        DialogResult res = MessageBox.Show("There's an update available, it is always recommended to stay on the latest version because of Game Updates, Crash Fixes and Bug Fixes, do you want to update now?", "FragFrame Updater", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                        if (res == DialogResult.Yes)
                        {
                            Process.Start(new ProcessStartInfo()
                            {
                                CreateNoWindow = true,
                                WindowStyle = ProcessWindowStyle.Hidden,
                                FileName = "cmd.exe",
                                Arguments = "/C start https://github.com/pogrammerx/fragframe-cs2/releases"
                            });
                            Environment.Exit(0);
                            return;
                        }
                    }
                }
            }

            Application.Run(new MainForm());
        }

        private static void ExceptionHandler(object sender, UnhandledExceptionEventArgs e)
        {
            MessageBox.Show("Error! " + e.ExceptionObject.GetType().Name + ": " + ((Exception)e.ExceptionObject).Message, "FragFrame Error Handler", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}