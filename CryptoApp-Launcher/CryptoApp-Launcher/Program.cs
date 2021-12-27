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
        static void system(string str)
        {
            Process cmd = new Process();

            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.StandardInput.WriteLine(str);
            cmd.StandardInput.Flush();
            cmd.StandardInput.Close();
            cmd.WaitForExit();
        
            //    Console.WriteLine(cmd.StandardOutput.ReadToEnd());
        }

        static string   cryptoAppPath = "";
        const string    cHaszPath = @"\data-scraper\data_scraper\data_scraper\bin\Release\netcoreapp3.1\data_scraper.exe";
        const string    cppPath = @"\imgui-master\examples\example_win32_directx9\Release\example_win32_directx9.exe";
        static void checkPath(string str)
        {
            char[] strChared = str.ToCharArray(0, str.Length);
            string fullPath = "";
            string partOfPath = "";
            string[] floderPaths = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", }; // [128]
            int currArray = 0;

            for (int i = 0; i < str.Length; i++)
            {
            //    Console.Write(strChared[i]);

                if (strChared[i] == (char)92) // mean next folder 
                {
                    Console.WriteLine("");

                    floderPaths[currArray++] = partOfPath;
                    Console.WriteLine("partOfPath : " + partOfPath);

                    if ("CryptoApp" == partOfPath) cryptoAppPath = fullPath;
                    
                    partOfPath = "";
                }
                else
                {
                    partOfPath += strChared[i];
                }
                fullPath += strChared[i];
            }
            Console.WriteLine(cryptoAppPath);      
        }

static void Main(string[] args)
        {
#if DEBUG
            ConsoleExtension.Show();
#else
            ConsoleExtension.Hide();
#endif        
            checkPath(Path.GetFullPath("CryptoApp-Launcher.exe"));

            if ((File.Exists((cryptoAppPath + @"\cryptoApp.exe")) ? "File exists." : "File does not exist.") == "File does not exist.")
            {
                System.IO.File.Copy((cryptoAppPath + cppPath), (cryptoAppPath + @"\cryptoApp.exe"), true);

                system("start" + cryptoAppPath + @"\cryptoApp.exe");

                System.Environment.Exit(0);
            }
            else // "File exists."
            {
             //   system("start " + cryptoAppPath + @"\cryptoApp.exe");
                system("start " + cryptoAppPath + cppPath);
            }
        }
    }
}
