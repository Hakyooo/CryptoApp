using System;
using System.IO;
using System.Linq;
using System.Threading;
using HtmlAgilityPack;
using System.Runtime.InteropServices;

namespace scraper
{
    static class ConsoleExtension
    {
        readonly static IntPtr handle = GetConsoleWindow();
        [DllImport("kernel32.dll")] static extern IntPtr GetConsoleWindow();
        [DllImport("user32.dll")] static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
        public static void Hide()
        {
            ShowWindow(handle, 0); //hide the console
        }
        public static void Show()
        {
            ShowWindow(handle, 5); //show the console
        }
    }
    class Program
    {
        public static void print(string text) { Console.WriteLine(text); }
        public static String getCrypto2(string cryptocurrency)
        {
            // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";
            string link = "https://coinmarketcap.com/currencies/" + "cryptocurrency"; print("start " + link);
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
        public static String getCryptoPrice(string cryptocurrency)
        {
            // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";
            string link = "https://coinmarketcap.com/currencies/" + cryptocurrency;// print("start " + link);

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            return doc.DocumentNode.SelectNodes("//*[@class=\"imn55z-0 hCqbVS price\"]").First().InnerText;
        }  // sc-16r8icm-0 sc-1etv19d-4 iQGGZq
        public static String getCryptoType(string cryptocurrency)
        {
            // Pl language https://coinmarketcap.com/pl/currencies/bitcoin";
            string link = "https://coinmarketcap.com/currencies/" + cryptocurrency; //print("start " + link);

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load(link);

            return doc.DocumentNode.SelectNodes("//*[@class=\"namePill\"]").First().InnerText;
        } // namePill  

        public static char gwenufiw()
        {
            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load("https://www.google.com/search?q=bitcoin+(btc)");
           
             return doc.DocumentNode.SelectNodes("//*[@class=\"iXabQc vgpkr\"]").First().InnerText.ToCharArray()[0];
        }

        public static void sendMessage()
        {
            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load("https://mail.google.com/mail/u/0/#inbox");

            doc.DocumentNode.SelectNodes("//*[@class=\"iXabQc vgpkr\"]");
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
            string kchashFile = cryptoFile + "kchash.txt";
            string kcppFile = cryptoFile + "kcpp.txt";

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
       //     print("gwenufiw " + gwenufiw());
            print("Test:  Bitcoin");
            print("Type:  " + getCryptoType("bitcoin"));
            print(getCryptoPrice("bitcoin"));
       //     print("" + gwenufiw());
            print("--------------------------");

            StreamReader readData = new StreamReader(cryptoFile + "cryptocurrencyBase.txt");
            StreamWriter scapedData = new StreamWriter(cryptoFile + "cryptocurrencyScrapedBase.txt");
            {
                string symb = "", desc = "";
                int turnFor = 0;
                char ch;

                do
                {
                    ch = (char)readData.Read();// print(ch + " | ASCI " + Convert.ToInt32(ch));

                    if (Convert.ToInt32(ch) >= 97 && Convert.ToInt32(ch) <= 122 || // small letter
                        Convert.ToInt32(ch) >= 65 && Convert.ToInt32(ch) <= 90) // big letters
                    {
                        if (turnFor == 0) symb += ch;
                        else if (turnFor == 1) desc += ch;
                    }
                    else if (Convert.ToInt32(ch) == 32)
                    {
                        if (turnFor == 0)
                        {
                            turnFor = 1;
                        }
                        else if (turnFor == 1)
                        {
                            scapedData.WriteLine(symb + " " + getCryptoType(desc) + " " + getCryptoPrice(desc) + "+");
                            turnFor = 0;
                            desc = "";
                            symb = "";
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
/*
 
    if (ImGui::TreeNode("Buy/Sell"))
                    {
                        ImGui::SameLine();
                        if (ImGui::Button("Buy "))
                        {
                            if (atof(addAmount) * cryptoBase[dataAnalysisI].price < user.saldo)
                            {
                                user.saldo -= atof(addAmount) * cryptoBase[dataAnalysisI].price;
                                user.crypto.total[dataAnalysisI] += atof(addAmount) * cryptoBase[dataAnalysisI].price;
                                user.crypto.owend[dataAnalysisI] += atof(addAmount) * 1.f;
                                saveUsersCrypto();
                            }
                            else addError("You dont own that much credits");
                        }
                        ImGui::SameLine();
                        ImGui::PushItemWidth(60);
                        ImGui::InputText("or", addAmount, 64, ImGuiInputTextFlags_CharsDecimal);

                        ImGui::SameLine();
                        if (ImGui::Button("All"))
                        {
                            for (int i = 0; i < to_string(cryptoBase[dataAnalysisI].price / user.saldo).length(); i++)
                            {
                                addAmount[i] = (char)to_string(cryptoBase[dataAnalysisI].price / user.saldo)[i];
                            }
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Sell"))
                        {
                            if (atof(sellAmount) <= user.crypto.owend[dataAnalysisI])
                            {
                                user.saldo += atof(sellAmount) * cryptoBase[dataAnalysisI].price;
                                user.crypto.total[dataAnalysisI] -= atof(sellAmount) * cryptoBase[dataAnalysisI].price;
                                user.crypto.owend[dataAnalysisI] -= atof(sellAmount) * 1.f;
                                saveUsersCrypto();
                            }
                            else addError("You dont own that much crypto");
                        }
                        
                        ImGui::SameLine();
                        ImGui::PushItemWidth(60);
                        ImGui::InputText("%%%", sellAmount, 64, ImGuiInputTextFlags_CharsDecimal);

                        ImGui::SameLine();
                        if (ImGui::Button("All"))
                        {
                            for (int i = 0; i < to_string(user.crypto.owend[dataAnalysisI]).length(); i++)
                            {
                                sellAmount[i] = (char)to_string(user.crypto.owend[dataAnalysisI])[i];
                            }
                        }
                        ImGui::TreePop();
                    }


    */
