# MagicVault

A simple **global key-value storage** for C++ projects.  
Supports any type via `std::any`, thread-safe, and organized by "gates" (namespaces).

## 📦 Build & Install

```bash
git clone https://github.com/YourName/MagicVault.git
cd MagicVault
mkdir build && cd build
cmake ..
make
sudo make install
```
## 🚀 Usage Example
```cpp
#include "MagicVault.hpp"
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
```
## ⚙ Features
- Thread-safe
- Supports any type
- Organized storage with "gates"
- Single instance (singleton)

#### 📜 License
MIT