using System;
using System.Collections.Generic;
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
            Assembly Assembly = Assembly.GetEntryAssembly();
            string Path = Assembly.Location;
            Path = Path.Substring(0, Path.LastIndexOf('\\')); // We only work on Windows, so we're assuming "\" as the escape character
            return Path;
        }

        public static string FindCS2()
        {
            return "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\game\\bin\\win64\\cs2.exe";
        }
    }
}
