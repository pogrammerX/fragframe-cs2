using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FragFrame
{
    public static class Loader
    {
        // Import necessary Windows API functions
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr OpenProcess(int dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out UIntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out uint lpThreadId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

        // Constants for process access rights
        private const int PROCESS_CREATE_THREAD = 0x0002;
        private const int PROCESS_QUERY_INFORMATION = 0x0400;
        private const int PROCESS_VM_OPERATION = 0x0008;
        private const int PROCESS_VM_WRITE = 0x0020;
        private const int PROCESS_VM_READ = 0x0010;

        // Constants for memory allocation
        private const uint MEM_RELEASE = 0x00008000;
        private const uint MEM_COMMIT = 0x1000;
        private const uint MEM_RESERVE = 0x2000;
        private const uint PAGE_READWRITE = 0x04;

        public static int StartGame()
        {
            // Well, it's already running, just go inject
            if (Process.GetProcessesByName("cs2").Length > 0)
                return Process.GetProcessesByName("cs2")[0].Id;

            // Attempt to Start the game
            string cs2Path = PathUtility.FindCS2();

            // -allow_third_party_software is required to allow us to inject using LoadLibrary
            Process? cs2 = Process.Start(new ProcessStartInfo()
            {
                FileName = cs2Path,
                Arguments = "-steam -allow_third_party_software",
                WorkingDirectory = "C:\\Program Files (x86)\\Steam"
            });

            // The game has this time where only the Valve logo is shown, it's important to note that most of the Interfaces are nullptr
            // and d3d11 hasn't been initialized by then, this is here to make sure that we don't inject during that time
            Thread.Sleep(30000); // 30 second delay because 5 wasnt enough

            return cs2.Id;
        }

        public static void InjectDll(int pid, string dllPath)
        {
            // Open the target process
            IntPtr hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, pid);
            if (hProcess == IntPtr.Zero)
            {
                throw new Exception("Failed to open target process.");
            }

            // Allocate memory in the target process for the DLL path
            IntPtr pRemoteMemory = VirtualAllocEx(hProcess, IntPtr.Zero, (uint)((dllPath.Length + 1) * Marshal.SizeOf(typeof(char))), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (pRemoteMemory == IntPtr.Zero)
            {
                CloseHandle(hProcess);
                throw new Exception("Failed to allocate memory in target process.");
            }

            // Write the DLL path to the allocated memory
            byte[] dllPathBytes = System.Text.Encoding.Unicode.GetBytes(dllPath);
            if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPathBytes, (uint)dllPathBytes.Length, out UIntPtr bytesWritten))
            {
                VirtualAllocEx(hProcess, IntPtr.Zero, 0, MEM_RELEASE, PAGE_READWRITE); // Free allocated memory
                CloseHandle(hProcess);
                throw new Exception("Failed to write DLL path to target process.");
            }

            // Get the address of LoadLibraryW in kernel32.dll
            IntPtr pLoadLibrary = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryW");
            if (pLoadLibrary == IntPtr.Zero)
            {
                VirtualAllocEx(hProcess, IntPtr.Zero, 0, MEM_RELEASE, PAGE_READWRITE); // Free allocated memory
                CloseHandle(hProcess);
                throw new Exception("Failed to get address of LoadLibraryW.");
            }

            // Create a remote thread in the target process to load the DLL
            IntPtr hRemoteThread = CreateRemoteThread(hProcess, IntPtr.Zero, 0, pLoadLibrary, pRemoteMemory, 0, out uint threadId);
            if (hRemoteThread == IntPtr.Zero)
            {
                VirtualAllocEx(hProcess, IntPtr.Zero, 0, MEM_RELEASE, PAGE_READWRITE); // Free allocated memory
                CloseHandle(hProcess);
                throw new Exception("Failed to create remote thread in target process.");
            }

            // Wait for the remote thread to finish
            WaitForSingleObject(hRemoteThread, 0xFFFFFFFF);

            // Clean up
            VirtualAllocEx(hProcess, IntPtr.Zero, 0, MEM_RELEASE, PAGE_READWRITE); // Free allocated memory
            CloseHandle(hRemoteThread);
            CloseHandle(hProcess);
        }
    }
}
