using System;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace CryptoApp_Launcher
{
    static class console
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
            string text = "";
            string partOfPath = "";
            string[] floderPaths = { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", }; // [128]
            int currArray = 0;

            for (int i = 0; i < str.Length; i++)
            {
                if (strChared[i] == (char)92) // mean next folder 
                {
                    if ("CryptoApp" == partOfPath) cryptoAppPath = text;

                    floderPaths[currArray++] = partOfPath;

                    partOfPath = "";
                }
                else partOfPath += strChared[i];
                
                text += strChared[i];
            }
            Console.WriteLine(cryptoAppPath);      
        }
        static void Main(string[] args)
        {
#if DEBUG
            console.Show();
#else
            console.Hide();
#endif        
            checkPath(Path.GetFullPath("CryptoApp-Launcher.exe"));

            if ((File.Exists((cryptoAppPath + @"\cryptoApp.exe")) ? "File exists." : "File does not exist.") == "File does not exist.")
            {
                if ((File.Exists((cryptoAppPath + cppPath)) ? "File exists." : "File does not exist.") == "File exists.")
                {
                    system("start " + cryptoAppPath + cppPath); // Run cryoto app
                    System.IO.File.Copy((cryptoAppPath + cppPath), (cryptoAppPath + @"\cryptoApp.exe"), true); // Copy cpp imgui app to cryptoApp folder
                }
                console.Hide(); // Hide console
                System.Environment.Exit(0); // Exit program
            }
            else system("start " + cryptoAppPath + cppPath);
            // (cryptoAppPath + @"\cryptoApp.exe") -> "File exists."
        }
    }
}
