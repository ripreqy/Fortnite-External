#include <include.h>
#include <src/cache/index.h>
#include <src/rendering/menu/index.h>
#include "memory.h"
#include "driver_data.h"
#include "mapper_data.h"
#include <fstream>
#include <cstdio>
#include <windows.h>
#include <thread>

// Console UI
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND FortniteWindow = NULL;

// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT 
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT 

// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT
// YOU ARE NOT ALLOWED TO SELL THIS SOURCE & CHEAT


#define LOGO_TEXT R"(
          $$\       $$$$$$\ $$$$$$$$\ $$\   $$\ $$$$$$\ $$\   $$\ $$\      $$\ 
          $$ |      \_$$  _|\__$$  __|$$ |  $$ |\_$$  _|$$ |  $$ |$$$\    $$$ | 
          $$ |        $$ |     $$ |   $$ |  $$ |  $$ |  $$ |  $$ |$$$$\  $$$$ |
          $$ |        $$ |     $$ |   $$$$$$$$ |  $$ |  $$ |  $$ |$$\$$\$$ $$ | 
          $$ |        $$ |     $$ |   $$  __$$ |  $$ |  $$ |  $$ |$$ \$$$  $$ | 
          $$ |        $$ |     $$ |   $$ |  $$ |  $$ |  $$ |  $$ |$$ |\$  /$$ | 
          $$$$$$$$\ $$$$$$\    $$ |   $$ |  $$ |$$$$$$\ \$$$$$$  |$$ | \_/ $$ |   
          \________|\______|   \__|   \__|  \__|\______| \______/ \__|     \__|)"

    void NewPage()
          {
              system("cls");
              SetConsoleOutputCP(CP_UTF8);
              SetConsoleCP(CP_UTF8);
              SetConsoleTextAttribute(hConsole, 5);
              std::cout << LOGO_TEXT;
          }

          void newLine(int count) {
              for (int i = 0; i < count; ++i) {
                  std::cout << "\n";
              }
          }

          bool CreateFileFromMemory(const std::string& path, const char* data, size_t size) {
              std::ofstream file(path, std::ios::binary);
              if (!file) {
                  return false;
              }

              file.write(data, size);
              file.close();

              return true;
          }

          bool isFortniteRunning() {
              return FindWindowA(NULL, "Fortnite") != NULL;
          }

          bool IsUserAdmin() {
              BOOL isAdmin = FALSE;
              PSID adminGroup = nullptr;
              SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

              if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                  DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
                  CheckTokenMembership(nullptr, adminGroup, &isAdmin);
                  FreeSid(adminGroup);
              }
              return isAdmin;
          }

          void ShowSuccessBeep() {
              Beep(1000, 200);
              Beep(1500, 300); 
          }

          int main() {
              NewPage();
              newLine(3);
              SetConsoleTextAttribute(hConsole, 5);
              std::cout << "  [+] ";
              SetConsoleTextAttribute(hConsole, 7);
              std::cout << "Loading Library...";
              Sleep(1500);

              if (!IsUserAdmin()) {
                  NewPage();
                  newLine(3);
                  SetConsoleTextAttribute(hConsole, 12);
                  std::cout << "  [!] ";
                  SetConsoleTextAttribute(hConsole, 7);
                  std::cout << "Please run as Administrator";
                  Sleep(3000);
                  return 1;
              }

              if (!Driver::Init()) {
                  if (isFortniteRunning()) {
                      MessageBoxA(0, "Please close Fortnite first", "Warning", MB_ICONWARNING);
                      return 1;
                  }

                  NewPage();
                  newLine(3);
                  SetConsoleTextAttribute(hConsole, 5);
                  std::cout << "  [+] ";
                  SetConsoleTextAttribute(hConsole, 7);
                  std::cout << "Installing driver components...";
                  Sleep(1000);

                  bool mapperSuccess = CreateFileFromMemory("MAPPER NAME",
                      reinterpret_cast<const char*>(mapper_data),
                      sizeof(mapper_data));

                  bool driverSuccess = CreateFileFromMemory("DRIVER NAME",
                      reinterpret_cast<const char*>(driver_data),
                      sizeof(driver_data));

                  if (!mapperSuccess || !driverSuccess) {
                      MessageBoxA(0, "Failed to create driver files", "Error", MB_ICONERROR);
                      return 1;
                  }

                  if (system("") != 0) {
                      MessageBoxA(0, "Driver mapping failed", "Error", MB_ICONERROR);
                      return 1;
                  }

                  std::remove("LIL NIGGA");
                  std::remove("FUCKER");

                  NewPage();
                  newLine(3);
                  SetConsoleTextAttribute(hConsole, 10);
                  std::cout << "  [+] ";
                  SetConsoleTextAttribute(hConsole, 7);
                  std::cout << "Driver and mapper loaded successfully!";
                  ShowSuccessBeep();
                  Sleep(1500);

                  if (!Driver::Init()) {
                      MessageBoxA(0, "Driver initialization failed after install",
                      "Error", MB_ICONERROR);
                      return 1;
                  }
              }
              else {
                  NewPage();
                  newLine(3);
                  SetConsoleTextAttribute(hConsole, 10);
                  std::cout << "  [+] ";
                  SetConsoleTextAttribute(hConsole, 7);
                  std::cout << "Driver already loaded!";
                  ShowSuccessBeep();
                  Sleep(1500);
              }
              NewPage();
              newLine(3);
              SetConsoleTextAttribute(hConsole, 5);
              std::cout << "  [+] ";
              SetConsoleTextAttribute(hConsole, 7);
              std::cout << "Start Fortnite now...";

              int attempts = 0;
              while (attempts++ < 120) {
                  FortniteWindow = FindWindowA(NULL, "Fortnite");
                  if (FortniteWindow) break;
                  std::cout << ".";
                  Sleep(1000);
              }

              if (!FortniteWindow) {
                  MessageBoxA(0, "Fortnite start timed out", "Error", MB_ICONERROR);
                  return 1;
              }

              DWORD pid = Driver::FindProcess(L"FortniteClient-Win64-Shipping.exe");
              uintptr_t base_address = Driver::GetBase();

              if (!base_address) {
                  MessageBoxA(0, "Failed to initialize cheat", "Error", MB_ICONERROR);
                  return 1;
              }

              std::thread(&updater::run).detach();
              std::thread(&updater::CacheLevels).detach();

              if (!overlay::run()) {
                  MessageBoxA(0, "Failed to start Overlay", "Error", MB_ICONERROR);
                  return 1;
              }

              return 0;
          }