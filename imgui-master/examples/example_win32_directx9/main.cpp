#include <iostream>
#include <windows.h>
#include <string>
#include <fstream> 
#include <sstream>
#include <thread>
#include <tchar.h>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <time.h>

#include <d3d9.h> 
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#define BUFSIZE 1024

using namespace std;
using namespace chrono;
using namespace this_thread;
using namespace literals::chrono_literals;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec4 redColor = (ImVec4)ImColor::HSV(1.f, 1.f, 1.f, 0.8f);
ImVec4 blueColor = (ImVec4)ImColor::HSV(0.6, 1.f, 1.f, 0.8f);
ImVec4 greenColor = (ImVec4)ImColor::HSV(0.3f, 1.f, 1.f, 0.8f);

string tempFolder = getenv("TEMP");
string tempCrypto = tempFolder + "\\Crypto\\";

// Settings
static int  styleColor = 1; // dropdown / setting
static int  currency = 0; // dropdown / setting
static int  language = 0; // dropdown / setting
static bool edit = false; // checkbox / setting
static bool showFps = true; // checkbox / setting
static bool showLastRerfresh = true;
static bool showAutoRerfresh = true;
static int  autoSaveDelay = 30;
static int  autoRefreshText = 300;
static int  autoRefreshTime = 300; // in secounds
static int  nextRefresh = 300; // 
static int  lastRefresh = 0; // 
static bool autoRefresh = true; //  checkbox / setting

static int autoRefreshTimeTmp = autoRefreshTime; // ??
static int autoSaveDelayTmp = autoSaveDelay; // ??

static string cryptoBaseTxt = ""; // file text storage // auto-fill
static int cryptoBaseSize = 0; // Size of cryptoBase // auto-fill

static int  cryptoBlockchain; // combox   
static char cryptoDescription[128]; // InputText 
static char cryptoSymbol[128]; // InputText     
static char cryptoPrice[128]; // auto-fill

static char cryptoAlertLevel[128]; // auto-fill     

static int  cryptoBlockchainTmp[999]; // Blockchain temp table 
static char cryptoBaseDescription[999][128];
static char cryptoBaseSymbol[999][128];
static char cryptoBasePrice[999][128];

static char* blockchains[] = { "Add Blockchain", "empty" };
static int blockchain[1000];
static string blockchainTxtFile = "";
static int blockchainNum = 1;
static bool blockchainWin = false;
static char addNewBlockchain[128];

static bool programing_mode = true; //
static bool colorTool = true; // CollapsingHeader / progrming_mode

static bool alertLevel = true;

static int autoSaveTime = 0;

static int totalSaldo = 0;
static int bestScore = 0;

string data_scraper_loc = "";

static char addAmount[128];
static char sellAmount[128];

static float kursDolara = 4.11;

const char* errorReason = NULL; // error reason text
static bool errorVisible = false; // error win visible
void addError(const char* reason)
{
    errorReason = reason;
    errorVisible = true;
}
static void showErrorWindow()
{
    if (errorVisible)
    {
        ImGui::Begin(" ", &errorVisible);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        {
            ImGui::Text(errorReason);
        }
        ImGui::End();
    }
}

const char* commandText = NULL;
void system_command(const char* text) { system(commandText = text); }

static void print(string text) { std::cout << text << std::endl; }

template <typename type> type rev(type value)
{
    if constexpr (is_same_v<type, bool>)
    {
        return !value;
    }
    else if constexpr (is_same_v<type, int>)
    {
        if (value < 0) return value * -1;
        else return value;
    }
    else if constexpr (is_same_v<type, float> || is_same_v<type, double> || is_same_v<type, double long>)
    {
        if (value < 0) return value * -1;
        else return value;
    }
    else if constexpr (is_same_v<type, vec2>)
    {
        vec2 return_value = value;

        if (return_value.x < 0) return_value.x *= -1.f;
        if (return_value.y < 0) return_value.y *= -1.f;

        return return_value;
    }
    else if constexpr (is_same_v<type, vec3>)
    {
        vec3 return_value = value;

        if (return_value.x < 0) return_value.x *= -1.f;
        if (return_value.y < 0) return_value.y *= -1.f;
        if (return_value.z < 0) return_value.z *= -1.f;

        return return_value;
    }
    else return value;
}

struct CryptocurrencyStruct
{
    string description = "NULL"; // user input
    string symbol = "NULL";  // user input
    int Blockchain = 0; // user input
    float alertLevel = 100; // user input
    double price = NULL; // auto-fill
    string typee = "auto-fill"; // auto fill
    string prizeChange24h = "auto-fill"; //auto fill
    bool isIncrese = NULL; // auto fill
    string SearchResults = "auto-fill"; // auto fill
    
}grab, cryptoBase[999], cryptoExamples[2];

struct MyStrddawuct
{
    float saldo = 100000.f;
    float totalSaldo = 100000.f;

    int score = 0;
    int bestScore = 0;

    struct frefwefweqf
    {
        float owend[999];
        float total[999];
    }crypto;

}user;

void saveUsersCrypto()
{
    ofstream userCrypto(tempCrypto + "userCrypto.txt");
    {
        if (!userCrypto.fail()) {
            for (int i = 0; i < 999; i++)
            {
                userCrypto << user.crypto.total[i] << endl;
                userCrypto << user.crypto.owend[i] << endl;
            }
            userCrypto << user.saldo;
        }
    }
    userCrypto.close();
}
void updateUsersCrypto()
{
    ifstream userCrypto(tempCrypto + "userCrypto.txt");
    {
        if (!userCrypto.fail()) {
            for (int i = 0; i < 999; i++)
            {
                userCrypto >> user.crypto.total[i];
                userCrypto >> user.crypto.owend[i];
            }
            userCrypto >> user.saldo;
        }
        else
        {
            for (int i = 0; i < 999; i++)
            {
                user.crypto.total[i] = 0;
                user.crypto.owend[i] = 0;
            }
        }
    }
    userCrypto.close();
}

void deleteCrypto(int i)
{
    for (int j = i; j < cryptoBaseSize; j++)
    {
        cryptoBase[j - 1].symbol = cryptoBase[j].symbol;
        cryptoBase[j - 1].description = cryptoBase[j].description;
    }
}

void destroy(string fileName)
{
    int status = remove(string(fileName).c_str());

#ifdef _DEBUG

    if (status == 1)
        cout << "Deleting fail(\"" << fileName << "\") failed" << endl;
    else
        cout << "Deleting fail(\"" << fileName << "\") successful" << endl;
#endif
}

bool isKnownChar(char input)
{
    if ((int)input <= 172 && (int)input > 63)
        return true;
    else
        return false;
}

float matchCase(string word1, string word2)
{
    string correctedWord1 = "";
    string correctedWord2 = "";

    for (int i = 0; i < word1.length(); i++)
        if (isKnownChar(word1[i]) == true)
            correctedWord1 += word1[i];

    for (int i = 0; i < word2.length(); i++)
        if (isKnownChar(word2[i]) == true)
            correctedWord2 += word2[i];

    if (correctedWord1.length() != correctedWord2.length())
        return 0;
    else
    {
        float matchCorrectness = 0;

        for (int i = 0; i < correctedWord1.length(); i++)
        {
            if (correctedWord1[i] == correctedWord2[i])
                matchCorrectness++;
        }
        return matchCorrectness / correctedWord1.length() * 100.f;
    }
}

void saveSettings()
{
    ofstream settingsFile(tempCrypto + "settings.txt");
    {
        if (!settingsFile.fail()) // Success
        {
            settingsFile << styleColor << endl;
            settingsFile << currency << endl;
            settingsFile << language << endl;

            settingsFile << autoRefresh << endl;
            settingsFile << autoRefreshTime << endl;

            settingsFile << edit << endl;
            settingsFile << showFps << endl;

            settingsFile << showLastRerfresh << endl;
            settingsFile << showAutoRerfresh << endl;

            settingsFile << autoSaveDelay << endl;

            settingsFile << programing_mode << endl;

            settingsFile << bestScore << endl;
        }
    }
    settingsFile.close();
}

void updateSettings()
{
    ifstream settingsFile(tempCrypto + "settings.txt");
    {
        if (!settingsFile.fail()) // Success
        {
            settingsFile >> styleColor;
            settingsFile >> currency;
            settingsFile >> language;

            settingsFile >> autoRefresh;
            settingsFile >> autoRefreshTime;

            settingsFile >> edit;
            settingsFile >> showFps;

            settingsFile >> showLastRerfresh;
            settingsFile >> showAutoRerfresh;

            settingsFile >> autoSaveDelay;

            settingsFile >> programing_mode;

            settingsFile >> bestScore;

            if (styleColor == 0)
            {
                ImGui::StyleColorsDark();
            }
            else if (styleColor == 1)
            {
                ImGui::StyleColorsClassic();
            }
            else if (styleColor == 2)
            {
                ImGui::StyleColorsLight();
            }
        }
    } settingsFile.close();
}

void collectData()
{
    ifstream cryptoScrapedBase(tempCrypto + "cryptocurrencyScrapedBase.txt");
    {
        if (!cryptoScrapedBase.fail())
        {
            char ch = '0';
            string text = "";
            int part = 0;
            int id = 0;
            string price = "";
            string prizeChange24h = "";
            string typee = "";

            while (cryptoScrapedBase >> ch)
            {
                text += ch;

                if ((int)ch == -96) {
                    part = 4;
                }

                if (part == 0)
                {
                    for (int i = 0; i <= cryptoBaseSize; i++)
                    {
                        if (text == cryptoBase[i].symbol)
                        {
                            id = i;
                            part = 1;
                        }
                    }
                }
                else if (part == 1)
                {
                    if (ch == 'P')
                    {    
                        cryptoBase[id].typee = typee;
                        part = 2;
                    }
                    else typee += ch;
                }
                else if (part == 2 && ch == '$') part = 3;
                else if (part == 3)
                {
                    if (ch != (char)-62)
                    {
                        if (ch >= 48 && ch <= 57 || ch == 46)
                        {
                            price += ch;
                        }
                    }
                    else
                    {
                        cryptoBase[id].price = (double)atof(price.c_str());
                        part = 3;
                    }
                }
                else if (part == 4)
                {
                    if (ch == '%')
                    {
                        cryptoScrapedBase >> ch;
                        if (ch == '+') cryptoBase[id].isIncrese = true;
                        else cryptoBase[id].isIncrese = false;
                    
                        cryptoBase[id].prizeChange24h = prizeChange24h;
                        part = 0;
                        text = "";
                        price = "";
                        typee = "";
                        prizeChange24h = "";
                    }
                    else if ((int)ch != -96 && (int)ch != -62)
                    {
                        prizeChange24h += (char)ch;
                    }
                }
            }
        }
        else addError("!cryptoScrapedBase.fail()");
    }
    cryptoScrapedBase.close();

    return;
}

struct localDateStruct
{
    string year = "";
    string month = "";
    string day = "";

}localDate;

const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

void getData()
{
    const string dataTime = currentDateTime();
    static int tf = 0;

    for (int i = 0; i < dataTime.length(); i++)
    {
        if (dataTime[i] == '-' || dataTime[i] == '.') tf++;
        else
        {
            if (tf == 0) localDate.year += dataTime[i];
            if (tf == 1) localDate.month += dataTime[i];
            if (tf == 2) localDate.day += dataTime[i];
        }
    }
}

void refresh()
{
    system_command(("start " + data_scraper_loc).c_str());

    collectData();
#ifdef _DEBUG
    cout << "Data collected sucesfull." << endl;
#endif
    nextRefresh = clock() + autoRefreshTime * 1000;
    lastRefresh = clock();
}

void resetScore()
{
    destroy(tempCrypto + "userCrypto.txt"); // Deleting file reset score.
    system("start C:\\Users\\mati\\Desktop\\CryptoApp\\imgui-master\\examples\\example_win32_directx9\\Debug\\example_win32_directx9.exe");
    exit(0);
}

void takeProfit()
{
    if (user.saldo > 100000)
    {
        static float take = user.score;

        for (int i = 0; i < 999; i++)
        {
            if (user.crypto.total[i] > 0)
            {
                cout << cryptoBase[i].description << " - " << (int)user.crypto.total[i] / user.totalSaldo * (int)(user.crypto.total[i] / totalSaldo * 100.f) << endl;
            }
        }
    }
    return;
}

int main(int, char**)
{
#ifdef _DEBUG
    ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _DEBUG

    bool appRunning = true;
    
    getData();
    if (atof(localDate.year.c_str()) >= 2021 && atof(localDate.month.c_str()) >= 6 && atof(localDate.day.c_str()) >= 0) appRunning == false;

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T(" "), WS_OVERLAPPEDWINDOW, 100, 100, 740, 680, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    for (int i = 0; i < 999; i++)
    {
        blockchain[i] = 1;
    }
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ifstream placeTxt(tempCrypto + "place.txt");
    {
        if (!placeTxt.fail())
        {
            placeTxt >> data_scraper_loc;
        }
        else
        {
            
        }
    } placeTxt.close();
    print("data_scraper_loc" + data_scraper_loc);

    ifstream if_cryptoBaseTxtFile(tempCrypto + "cryptocurrencyBase.txt");
    {
        if (!if_cryptoBaseTxtFile.fail())
        {
            while (if_cryptoBaseTxtFile >> grab.symbol)
            {
                if_cryptoBaseTxtFile >> grab.description;
      
                cryptoBase[cryptoBaseSize].symbol = grab.symbol;
                cryptoBase[cryptoBaseSize].description = grab.description;

                cryptoBaseSize++;

                cryptoBaseTxt += grab.symbol + " " + grab.description + " ";
            }
        }
        else addError("cryptoBaseTxtFile.fail()");
    }
    if_cryptoBaseTxtFile.close();

    refresh();
    collectData();
    updateSettings();
    updateUsersCrypto();

    string dataAnalysis = "NULL";
    bool dataAnalysisWin = false;
    int dataAnalysisI = NULL;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    static ImGuiTextFilter filter;

    while (appRunning == true)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                appRunning = false;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {    
            ImGui::ShowDemoWindow();

            ImGui::Begin("Cryptocurrency analysis", NULL, window_flags);
            {
                if (ImGui::CollapsingHeader("Wallet", ImGuiTreeNodeFlags_None))
                {
                    user.totalSaldo = user.saldo;

                    for (int i = 0; i < 999; i++)
                    {
                        user.totalSaldo += user.crypto.owend[i] * cryptoBase[i].price;
                    }

                    ImGui::Text("User saldo:");
                    ImGui::SameLine();
                    if (user.saldo > 999) {

                        ImGui::Text(("$" + to_string((int)(user.saldo / 1000)) + "k").c_str());
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                            ImGui::TextUnformatted(to_string(user.saldo).c_str());
                            ImGui::PopTextWrapPos();
                            ImGui::EndTooltip();
                        }
                    }
                    else
                    {
                        ImGui::Text((to_string((int)user.saldo) + "$").c_str());
                    }

                    ImGui::SameLine();
                    user.score = (int)(user.totalSaldo - 100000);
                    ImGui::Text(("| Score: " + to_string(user.score)).c_str());

                    ImGui::SameLine();
                    user.bestScore = user.bestScore = max(user.bestScore, (int)(user.totalSaldo - 100000));
                    ImGui::Text(("| Best score: " + to_string(user.score)).c_str());

                    ImGui::SameLine();
                    ImGui::Text(("| Total saldo: " + to_string((int)(user.totalSaldo))).c_str());

                    ImGui::SameLine();
                    if (ImGui::Button("Reset score")) resetScore();

                    ImGui::SameLine();
                    if (ImGui::Button("take profit"))  takeProfit();

                    if (ImGui::BeginTable("tablee", 6, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoHostExtendX, ImVec2(700, 100)))
                    {
                        ImGui::TableSetupColumn("Description");
                        ImGui::TableSetupColumn("Ammount");
                        ImGui::TableSetupColumn("Market value");
                        ImGui::TableSetupColumn("Curr. price");
                        ImGui::TableSetupColumn("24h %");
                        ImGui::TableSetupColumn("dasd");

                        ImGui::TableHeadersRow();
                    }

                    for (int i = 0; i < 999; i++)
                    {
                        if (user.crypto.total[i] > 0)
                        {
                            ImGui::TableNextRow();
                            {
                                ImGui::TableNextColumn(); // Description
                                {
                                    ImGui::Text((cryptoBase[i].description).c_str());
                                }
                                ImGui::TableNextColumn(); // Ammount
                                {
                                    if(user.crypto.owend[i] > 0)
                                        ImGui::Text(to_string((int)user.crypto.owend[i]).c_str());
                                    else
                                        ImGui::Text(to_string((float)user.crypto.owend[i]).c_str());
                                }
                                ImGui::TableNextColumn(); // Value
                                {
                                    ImGui::Text(to_string((int)user.crypto.total[i]).c_str());
                                }
                                ImGui::TableNextColumn(); // Curr. price
                                {
                                    ImGui::Text(to_string((int)cryptoBase[i].price).c_str());
                                }
                                ImGui::TableNextColumn(); // prizeChange24h
                                {
                                    ImGui::Text((cryptoBase[i].prizeChange24h).c_str());
                                }
                                ImGui::TableNextColumn(); // imp
                                {
                                    ImGui::Text((to_string((int)(user.crypto.total[i] / totalSaldo * 100.f)) + "%").c_str());
                                }
                                /*
                                { // amount
                                    ImGui::SameLine();
                                    ImGui::Text(("amount: " + to_string((int)user.crypto.owend[i]) + ",").c_str());
                                }
                                { // total
                                    ImGui::SameLine();
                                    ImGui::Text(("value: " + to_string((int)user.crypto.total[i]) + "$,").c_str());
                                }
                                { // total
                                    ImGui::SameLine();
                                    ImGui::Text(("imp: " + to_string((int)(user.crypto.total[i] * 100.f / totalSaldo)) + "").c_str());
                                }
                                */
                            }
                        }
                    }
                    ImGui::EndTable();
                }

                if (ImGui::CollapsingHeader("Cryptocurrency base", ImGuiTreeNodeFlags_None))
                {
                    ImGui::Checkbox("Edit", &edit);

                    ImGui::SameLine();
                    filter.Draw("<- Search", 200);

                    ImGui::SameLine();
                    if (ImGui::Button("Refresh") || autoRefresh == true && nextRefresh < clock())
                    {
                        refresh();
                    }                    
                    if (showLastRerfresh == true)
                    {
                        ImGui::SameLine();
                        ImGui::Text(("(Last refresh " + to_string((clock() - lastRefresh) / 1000) + "s)").c_str());
                    }
                    if (showAutoRerfresh == true)
                    {
                        autoRefreshText = (nextRefresh - clock()) / 1000 + 1;
                        ImGui::SameLine();
                        ImGui::Text(("(Auto refresh " + to_string(autoRefreshText) + "s)").c_str());
                    }
                    else nextRefresh = clock() + autoRefreshTime * 1000;
                    
                    if (ImGui::BeginTable("table", 9, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoHostExtendX, ImVec2(700, 100)))
                    {
                        ImGui::TableSetupColumn("Symbol");
                        ImGui::TableSetupColumn("Description");
                        ImGui::TableSetupColumn("Blockchain");

                        ImGui::TableSetupColumn("$ Price $");
                        ImGui::TableSetupColumn("24h % Change");
                        ImGui::TableSetupColumn("Search results");
                        ImGui::TableSetupColumn("Type");
                        ImGui::TableSetupColumn("Add / Delete");
                        ImGui::TableHeadersRow();
                    }
                    ImGui::TableNextRow();
                    {
                        ImGui::TableNextColumn(); // symbol
                        {
                            ImGui::PushItemWidth(80);
                            ImGui::InputText(" ", cryptoSymbol, IM_ARRAYSIZE(cryptoSymbol));
                        }
                        ImGui::TableNextColumn(); // description
                        {
                            ImGui::PushItemWidth(80);
                            ImGui::InputText("", cryptoDescription, IM_ARRAYSIZE(cryptoDescription));
                        }
                        ImGui::TableNextColumn(); // blockchain
                        {
                            ImGui::PushItemWidth(125);
                            ImGui::Combo("b", &blockchain[999], blockchains, IM_ARRAYSIZE(blockchains));
                            if (blockchain[999] == 0) blockchainWin = true;
                        }
                        ImGui::TableNextColumn(); // price
                        {
                            ImGui::TextDisabled("Auto-fill");
                        }
                        ImGui::TableNextColumn(); // 24h prize 
                        {
                            ImGui::TextDisabled("Auto-fill");
                        }
                        ImGui::TableNextColumn(); // search results
                        {
                            ImGui::TextDisabled("Auto-fill");
                        }
                        ImGui::TableNextColumn(); // type
                        {
                            ImGui::TextDisabled("Auto-fill");
                        }
                        ImGui::TableNextColumn(); // add / delete
                        {
                            ImGui::PushID("add");
                            ImGui::PushStyleColor(ImGuiCol_Button, greenColor);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, greenColor);
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, greenColor);

                            if (ImGui::Button("Add"))
                            {
                                if ((int)cryptoDescription[0] == 0 || (int)cryptoSymbol[0] == 0)
                                {
                                    addError("You need to fill: description, symbol and index.");
                                }
                                else
                                {
                                    ofstream of_cryptoBaseTxtFile(tempCrypto + "cryptocurrencyBase.txt");
                                    {
                                        if (!of_cryptoBaseTxtFile.fail())
                                        {
                                            for (int i = 0; i <= cryptoBaseSize; i++) // cheking for adding same currency
                                            {
                                                if (i == cryptoBaseSize)
                                                {
                                                    cryptoBase[cryptoBaseSize++] = { cryptoDescription, cryptoSymbol };

                                                    cryptoBaseTxt += cryptoSymbol;
                                                    cryptoBaseTxt += (" ");
                                                    cryptoBaseTxt += cryptoDescription;
                                                    cryptoBaseTxt += (" ");

                                                    of_cryptoBaseTxtFile << cryptoBaseTxt;

                                                    for (int i = 0; i < 127; i++)
                                                    {
                                                        cryptoDescription[i] = (int)0;
                                                        cryptoSymbol[i] = (int)0;
                                                    }

                                                    addError("New cryptocurrency has been added successfully.");

                                                    break;
                                                }

                                                if (cryptoBase[i].description == cryptoDescription || cryptoBase[i].symbol == cryptoSymbol)
                                                {
                                                    if (cryptoBase[i].description == cryptoDescription && cryptoBase[i].symbol == cryptoSymbol)
                                                        addError("Adding new crypto has been failed.\nThis cryptocurrency is already added.");
                                                    else
                                                    {
                                                        if (cryptoBase[i].description == cryptoDescription)
                                                            addError("Ther is another cryptocurrency with same description");

                                                        else if (cryptoBase[i].symbol == cryptoSymbol)
                                                            addError("Ther is another cryptocurrency with same symbol");
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        else addError("Adding new cryptocurrency has been failed.");

                                    } of_cryptoBaseTxtFile.close();
                                }
                            }
                            ImGui::PopStyleColor(3);
                            ImGui::PopID();
                        }
                    }

                    for (int i = 0; i < cryptoBaseSize; i++)
                    {
                        if (filter.PassFilter(cryptoBase[i].symbol.c_str()) || filter.PassFilter(cryptoBase[i].description.c_str()))
                        {
                            for (int j = 0; j < cryptoBase[i].description.length(); j++) cryptoBaseDescription[i][j] = cryptoBase[i].description[j];
                            for (int j = 0; j < (cryptoBase[i].symbol).length(); j++) cryptoBaseSymbol[i][j] = (cryptoBase[i].symbol)[j];

                            ImGui::TableNextRow();
                            {
                                if (edit == 0)
                                {
                                    ImGui::TableNextColumn(); // Symbol
                                    {
                                        if (ImGui::Button(cryptoBase[i].symbol.c_str()))
                                        {
                                            if (dataAnalysisWin == false) dataAnalysisWin = !dataAnalysisWin;

                                            dataAnalysis = cryptoBase[i].description + " (" + cryptoBase[i].symbol + ") - Data analysis";
                                            dataAnalysisI = i;
                                        }
                                    }
                                    ImGui::TableNextColumn(); // Description
                                    {
                                        ImGui::Text(cryptoBaseDescription[i]);
                                    }
                                    ImGui::TableNextColumn(); // Blockchain
                                    {
                                        /*
                                        ImGui::Combo(("b" + to_string(i)).c_str(), &blockchain[i], blockchains, IM_ARRAYSIZE(blockchains));
                                        if (blockchain[i] == 0) blockchainWin = true;
                                        */
                                    }
                                    ImGui::TableNextColumn(); // $ Price $
                                    {
                                        if (cryptoBase[i].price > 999)
                                        {
                                            ImGui::Text(("$" + to_string((int)(cryptoBase[i].price / 1000)) + "k").c_str());

                                            if (ImGui::IsItemHovered())
                                            {
                                                ImGui::BeginTooltip();
                                                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                                                ImGui::TextUnformatted(to_string(cryptoBase[i].price).c_str());
                                                ImGui::PopTextWrapPos();
                                                ImGui::EndTooltip();
                                            }
                                        }
                                        else ImGui::Text(to_string((float)cryptoBase[i].price).c_str());
                                    }
                                    ImGui::TableNextColumn(); // 24h prize change
                                    {
                                        if (cryptoBase[i].isIncrese)
                                            ImGui::TextColored(((ImVec4)ImColor::HSV((float)(cryptoBase[i].alertLevel / 100.f * 0.3), 1.00f, 1.00f, 1)), ("+" + (cryptoBase[i].prizeChange24h)).c_str());
                                        else
                                            ImGui::TextColored(((ImVec4)ImColor::HSV((float)(cryptoBase[i].alertLevel / 100.f * 0.3), 1.00f, 1.00f, 1)), ("-" + (cryptoBase[i].prizeChange24h)).c_str());
                                    }
                                    ImGui::TableNextColumn(); // Search results
                                    {
                                        ImGui::Text(cryptoBase[i].SearchResults.c_str());
                                    }
                                    ImGui::TableNextColumn(); // type
                                    {
                                        ImGui::Text(cryptoBase[i].typee.c_str());
                                    }
                                    /*
                                    ImGui::TableNextColumn(); // add / delete
                                    {

                                    }*/
                                }
                                else if (edit == 1)
                                {
                                    ImGui::TableNextColumn(); // Symbol
                                    {
                                        ImGui::PushItemWidth(80);
                                        ImGui::InputText((" " + to_string(i)).c_str(), cryptoBaseSymbol[i], IM_ARRAYSIZE(cryptoBaseSymbol[i]));
                                    }
                                    ImGui::TableNextColumn(); // Description
                                    {
                                        ImGui::PushItemWidth(80);
                                        ImGui::InputText(to_string(i).c_str(), cryptoBaseDescription[i], IM_ARRAYSIZE(cryptoBaseDescription[i]));
                                    }
                                    ImGui::TableNextColumn(); // Blockchain
                                    {
                                        ImGui::PushItemWidth(125);
                                        ImGui::Combo("Blockchain", &blockchain[i], blockchains, IM_ARRAYSIZE(blockchains));
                                        if (blockchain[i] == 0) blockchainWin = true;
                                    }
                                    ImGui::TableNextColumn(); // $ Price $
                                    {
                                        ImGui::TextDisabled(to_string((double)cryptoBase[i].price).c_str());
                                    }
                                    ImGui::TableNextColumn(); // 24h prize change
                                    {
                                        ImGui::TextDisabled(cryptoBase[i].prizeChange24h.c_str());
                                    }
                                    ImGui::TableNextColumn(); // Search results
                                    {
                                        ImGui::TextDisabled(cryptoBase[i].SearchResults.c_str());
                                    }
                                    ImGui::TableNextColumn(); // Type
                                    {
                                        ImGui::TextDisabled(cryptoBase[i].typee.c_str());
                                    }
                                    ImGui::TableNextColumn(); // Add / Delete
                                    {
                                        ImGui::PushID(i);
                                        ImGui::PushStyleColor(ImGuiCol_Button, redColor);
                                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redColor);
                                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, redColor);

                                        if (ImGui::Button(("Delete no." + to_string(i)).c_str()))
                                        {
                                            deleteCrypto(i);
                                            ofstream of_cryptoBaseTxtFile(tempCrypto + "cryptocurrencyBase.txt");
                                            {
                                                if (!of_cryptoBaseTxtFile.fail())
                                                {
                                                    cryptoBaseTxt = "";
                                                    for (int i = 0; i < cryptoBaseSize; i++)
                                                    {
                                                        cryptoBaseTxt += cryptoSymbol;
                                                        cryptoBaseTxt += " ";
                                                        cryptoBaseTxt += cryptoDescription;
                                                        cryptoBaseTxt += " ";
                                                    }
                                                    of_cryptoBaseTxtFile << cryptoBaseTxt;
                                                }
                                            } of_cryptoBaseTxtFile.close();
                                        }
                                        ImGui::PopStyleColor(3);
                                        ImGui::PopID();
                                    }
                                }
                            }
                            for (int j = 0; j < 120; j++) cryptoBase[i].description = cryptoBaseDescription[i];
                            for (int j = 0; j < 120; j++) cryptoBase[i].symbol = cryptoBaseSymbol[i];
                        }
                    }
                    ImGui::EndTable();
                }

                if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_None))
                {
                    ImGuiStyle& style = ImGui::GetStyle();
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
                    ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
                    ImGui::BeginChild("scrolling", ImVec2(700, 100), true, ImGuiWindowFlags_HorizontalScrollbar);
                    {
                        if (ImGui::BeginTable("table/settings", 6, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoHostExtendX, ImVec2(1500, 100)))
                        {
                            ImGui::TableSetupColumn("       Visual");
                            ImGui::TableSetupColumn("       Shortcuts");
                            ImGui::TableSetupColumn("       Misc");
                            ImGui::TableSetupColumn("       Preferences");
                            ImGui::TableSetupColumn("       Automation");
                            ImGui::TableSetupColumn("       Fix");
                            ImGui::TableHeadersRow();
                        }

                        ImGui::TableNextRow(); // row
                        {
                            ImGui::TableNextColumn(); // Visual
                            {
                                ImGui::Checkbox("show last rerfresh", &showLastRerfresh);

                                ImGui::Checkbox("show auto-rerfresh", &showAutoRerfresh);

                                ImGui::Checkbox("Show fps", &showFps);
                            }
                            ImGui::TableNextColumn(); // Shortcuts
                            {
                                if (ImGui::Button("Open Crpyto temp folder"))
                                {
                                    system_command(("start " + tempCrypto).c_str());
                                }
                                if (ImGui::Button("Open coinmarketcap.com"))
                                {
                                    system("start https://coinmarketcap.com/currencies/");
                                }
                                if (ImGui::Button("Open Google finance"))
                                {
                                    system("start https://www.google.com/finance/");
                                }
                            }
                            ImGui::TableNextColumn(); // Misc
                            {
                                ImGui::Checkbox("Programing mode", &programing_mode);
                            }
                            ImGui::TableNextColumn(); // Preferences
                            {
                                ImGui::Combo("Style color", &styleColor, "Dark\0Light\0Classic\0");

                                ImGui::Combo("Language", &language, "English\0...\0");

                                ImGui::Combo("Currency", &currency, "Dollar$\0...\0");
                            }
                            ImGui::TableNextColumn(); // Misc
                            {
                                ImGui::Checkbox("Auto refresh", &autoRefresh);

                                ImGui::SameLine();
                                ImGui::PushItemWidth(80);

                                if (autoRefresh == false)
                                    ImGui::BeginDisabled();

                                ImGui::SliderInt("sec", &autoRefreshTimeTmp, 5, 300);

                                if (autoRefresh == false)
                                    ImGui::EndDisabled();


                                if (autoRefresh == true)
                                {
                                    if (autoRefreshTimeTmp != autoRefreshTime)
                                    {
                                        nextRefresh = clock() + (autoRefreshTimeTmp * 1000);
                                    }
                                    autoRefreshTime = autoRefreshTimeTmp;
                                }
                                else
                                {
                                    nextRefresh = clock() + autoRefreshTime * 1000;
                                }

                                if (styleColor == 0)
                                    ImGui::StyleColorsDark();

                                else if (styleColor == 1)
                                    ImGui::StyleColorsClassic();

                                else if (styleColor == 2)
                                    ImGui::StyleColorsLight();

                                ImGui::Text("Auto-save after delay");

                                ImGui::SameLine();

                                autoSaveDelayTmp = autoSaveDelay;

                                ImGui::SliderInt("sec ", &autoSaveDelayTmp, 1, 100);

                                if (autoSaveDelay != autoSaveDelayTmp)
                                {
                                    autoSaveTime = clock() + (autoSaveDelay * 1000);
                                }
                                autoSaveDelay = autoSaveDelayTmp;
                            }
                            ImGui::TableNextColumn();
                            {
                                if (ImGui::Button("fix refresh btn"))
                                {
                                    destroy("place.txt");
                                    system_command(("start " + data_scraper_loc).c_str());
                                }
                            }
                        }
                        ImGui::EndTable();
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleVar(2);
                    ImGui::Spacing();
                }

                if(colorTool == true && programing_mode == true)
                    if (ImGui::CollapsingHeader("color", ImGuiTreeNodeFlags_None))
                    {
                        static ImVec4 color_hsv(0.23f, 1.0f, 1.0f, 1.0f); // Stored as HSV!

                        ImGui::ColorEdit4("HSV shown as HSV##1", (float*)&color_hsv, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
                    }

                if (showFps == true)
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            if (autoSaveTime < clock())
            {
                saveSettings();

                autoSaveTime = clock() + (autoSaveDelay * 1000);
            }
            ImGui::End();

            if (blockchainWin == true)
            {
                ImGui::Begin("Add new Blockchain", NULL, NULL);
                {
                    ImGui::InputText("", addNewBlockchain, IM_ARRAYSIZE(addNewBlockchain));
                    ImGui::SameLine();
                    if (ImGui::Button("Add"))
                    {
                        blockchains[blockchainNum] = addNewBlockchain;
                        cout << "blockchainNum " << blockchainNum << endl;

                        blockchainNum += 1;
                        blockchainWin = false;

                        for (int i = 0; i < 99; i++) { addNewBlockchain[i] = (char)0; }
                    }
                }
                ImGui::End();
            }
            
            if (dataAnalysisWin == true)
            {
                ImGui::Begin(dataAnalysis.c_str(), NULL);
                {
                    ImGui::Text((cryptoBase[dataAnalysisI].description + " price ").c_str());
                    ImGui::SameLine();
                    ImGui::Text((to_string((double)cryptoBase[dataAnalysisI].price) + "$").c_str());

                    if (ImGui::TreeNode(("Alert level " + to_string((int)cryptoBase[dataAnalysisI].alertLevel) + "%").c_str()))
                    {
                        ImGui::SameLine();
                        if (ImGui::Button("Change value"))
                        {
                            string str(cryptoAlertLevel);

                            cryptoBase[dataAnalysisI].alertLevel = std::stof(str);

                            for (int i = 0; i < 127; i++) cryptoAlertLevel[i] = (int)0;
                        }
                        ImGui::SameLine();
                        ImGui::PushItemWidth(40);
                        ImGui::InputText("%", cryptoAlertLevel, 64, ImGuiInputTextFlags_CharsDecimal);
                        ImGui::SameLine();
                        ImGui::TextDisabled("(Text color depend on it)");
                        ImGui::TreePop();
                    }
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
                                addAmount[i] = (char)to_string(user.saldo / cryptoBase[dataAnalysisI].price)[i];
                            }
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Sell"))
                        {
                            if (atof(sellAmount) <= 0.01 + user.crypto.owend[dataAnalysisI])
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
                        ImGui::InputText("          ", sellAmount, 64, ImGuiInputTextFlags_CharsDecimal);

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

                }//
                ImGui::End();
            }
            showErrorWindow();
        }

        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
/*  
        if (unsaved_document) window_flags |= ImGuiWindowFlags_UnsavedDocument;

*/
