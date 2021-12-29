using System;
using System.IO;
using System.Linq;
using System.Threading;
using HtmlAgilityPack;
using System.Runtime.InteropServices;

using System.Diagnostics;

namespace scraper
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
        public static void print(string text) { Console.WriteLine(text); }
        public static String getCrypto(string cryptocurrency)
        {
            // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";
            string link = "https://coinmarketcap.com/currencies/" + "cryptocurrency";
            string text = "";

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            foreach (var item in doc.DocumentNode.SelectNodes("//*[@class='sc-16r8icm-0 nds9rn-0 dAxhCK']")) // .First().InnerText);
            {
                text += item.InnerText;
            }
            return text;
        }
        public static String getCryptoPriceChange(string cryptocurrency)
        {
            // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";
            string link = "https://coinmarketcap.com/currencies/" + cryptocurrency; //print("start " + link);

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            return doc.DocumentNode.SelectNodes("//*[@class=\"sc-16r8icm-0 kjciSH priceSection\"]").First().InnerText;
        }

        //    static string currency; // = "Dollar" / "Zloty"

        // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";

         static string coinmarketcapLink_pl = "https://coinmarketcap.com/pl/currencies/";
         static string coinmarketcapLink_us = "https://coinmarketcap.com/plcurrencies/";

         static string coinmarketcapLink = coinmarketcapLink_us;

        public static String getCryptoPrice(string cryptocurrency)
        {
            string link = "https://coinmarketcap.com/currencies/" + cryptocurrency;
            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            return doc.DocumentNode.SelectNodes("//*[@class=\"imn55z-0 hCqbVS price\"]").First().InnerText;
        }
        public static String getCryptoType(string cryptocurrency)
        {
            string link = "https://coinmarketcap.com/currencies/" + cryptocurrency; //print("start " + link);

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            return doc.DocumentNode.SelectNodes("//*[@class=\"namePill\"]").First().InnerText;
        }
        static void Main(string[] args)
        {
#if DEBUG
            ConsoleExtension.Show();    
#else
            ConsoleExtension.Hide();
#endif
            string tmpFile = System.IO.Path.GetTempPath();
            string cryptoFile = tmpFile + "Crypto\\";

            if ((File.Exists(cryptoFile + "cHaszScraper") ? "File exists." : "File does not exist.") == "File does not exist.")
            {
                System.IO.File.Copy((Path.GetFullPath("data_scraper.exe")), (cryptoFile + "data_scraper.exe"), true);
            }
            else
            {

            }


            if ((File.Exists(cryptoFile + "place.txt") ? "File exists." : "File does not exist.") == "File exists.")
            {

            }
            else
            { 
                StreamWriter placeTxtLoc = new StreamWriter(cryptoFile + "place.txt");
                string fullPath = Path.GetFullPath("data_scraper.exe");
                placeTxtLoc.WriteLine(fullPath);
                placeTxtLoc.Close();
            }

            print("--------------------------");
            print("Test:  Bitcoin");
            print("Type:  " + getCryptoType("bitcoin"));
            print(getCryptoPrice("bitcoin"));
            print("--------------------------");

            if ((File.Exists(cryptoFile + "cryptocurrencyBase.txt") ? "File exists." : "File does not exist.") == "File exists.")
            {
                StreamReader readData = new StreamReader(cryptoFile + "cryptocurrencyBase.txt");
                StreamWriter scapedData = new StreamWriter(cryptoFile + "cryptocurrencyScrapedBase.txt");
                {
                    string symb = "", desc = "";
                    int turnFor = 0;
                    char ch;

                    do {
                        ch = (char)readData.Read();// print(ch + " | ASCI " + Convert.ToInt32(ch));

                        if (Convert.ToInt32(ch) >= 97 && Convert.ToInt32(ch) <= 122 || // small letter
                            Convert.ToInt32(ch) >= 65 && Convert.ToInt32(ch) <= 90) // big letters
                        {
                            if (turnFor == 0) symb += ch;
                            else if (turnFor == 1) desc += ch;
                        }
                        else if (Convert.ToInt32(ch) == 32)
                        {
                            if (turnFor == 0) turnFor = 1;
                            else if (turnFor == 1)
                            {

                                string cryptoType = getCryptoType(desc);
                                string cryptoPrice = getCryptoPrice(desc);

                                print(cryptoType + " " + desc + " " + cryptoPrice);

                                scapedData.WriteLine(symb + " " + cryptoType + " " + cryptoPrice + "+");
                                
                                turnFor = 0; desc = ""; symb = "";
                            }
                        }
                    } while (!readData.EndOfStream);

                    scapedData.Close();
                    readData.Close();
                    Thread.Sleep(1000);
                }
            }
        }
    }
}