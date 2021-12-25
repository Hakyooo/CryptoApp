using System;
using System.IO;
using System.Linq;
using System.Threading;
using System.Runtime.InteropServices;

using System.Diagnostics;


namespace CryptoApp_Launcher
{
    static class ConsoleExtension
    {
        readonly static IntPtr handle = GetConsoleWindow();
        [DllImport("kernel32.dll")] static extern IntPtr GetConsoleWindow();
        [DllImport("user32.dll")] static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
        public static void Hide() { ShowWindow(handle, 0); }
        public static void Show() { ShowWindow(handle, 5); }
    }
    class Program
    {
        static void Main(string[] args)
        {
#if DEBUG
            ConsoleExtension.Show();
#else
            ConsoleExtension.Hide();
#endif
            string path2 = @"C:\Users\moolm\OneDrive\Documents\GitHub\CryptoApp";
            string path3 = @"\data-scraper\data_scraper\data_scraper\bin\Release\netcoreapp3.1\data_scraper.exe";
            string path4 = @"\imgui-master\examples\example_win32_directx9\Release\example_win32_directx9.exe";
       //     string fullPath = Path.GetFullPath(path2 + path3);
       //     Console.WriteLine(fullPath);

            Process cmd = new Process();
            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.StandardInput.WriteLine("start " + path2 + path4);
            cmd.StandardInput.Flush();
            cmd.StandardInput.Close();
            cmd.WaitForExit();
            Console.WriteLine(cmd.StandardOutput.ReadToEnd());

            Console.WriteLine("Hello World!");
        }
    }
}
