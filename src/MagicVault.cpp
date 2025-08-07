#include "../include/MagicVault.hpp"
#include <iostream>

int main() {
    // Store data
    MagicVault::Instance().set("LoginGate", "HWND_Main", 12345);
    MagicVault::Instance().set("Settings", "Volume", 0.8f);

    // Retrieve data
    auto hwnd = MagicVault::Instance().getOpt<int>("LoginGate", "HWND_Main");
    auto volume = MagicVault::Instance().getOpt<float>("Settings", "Volume");

    if (hwnd) std::cout << "HWND: " << *hwnd << "\n";
    if (volume) std::cout << "Volume: " << *volume << "\n";

    return 0;
}
