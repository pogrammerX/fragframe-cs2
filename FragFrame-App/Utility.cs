using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace FragFrame
{
    /// <summary>
    /// Utility for everything path related
    /// </summary>
    public static class PathUtility
    {
        /// <summary>
        /// Get's path of the base path (on installed versions: C:\Program Files\FragFrame)
        /// </summary>
        public static string GetBasePath()
        {
            string res = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName);
            MessageBox.Show(res);
            return res;
        }

        public static string FindCS2()
        {
            // TODO: allow CS2 to be installed in other locations ^^'
            return "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\game\\bin\\win64\\cs2.exe";
        }
    }
}
