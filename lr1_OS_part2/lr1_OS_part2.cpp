#include <windows.h>
#include <iostream>

int main() {
    HKEY hKey;
    DWORD dwData;

    // Відкриття ключа реєстру
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\ControlSet001\\Control\\Network\\SharedAccessConnection"), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
        std::cout << "Unable to open registry key.\n";
        return 1;
    }

    // Отримання значення
    DWORD dwSize = sizeof(DWORD);
    if (RegQueryValueEx(hKey, TEXT("EnableControl"), NULL, NULL, reinterpret_cast<LPBYTE>(&dwData), &dwSize) != ERROR_SUCCESS) {
        std::cout << "Unable to retrieve value.\n";
        return 1;
    }

    // Зміна значення
    dwData = dwData == 0 ? 1 : 0;

    // Запис зміненого значення
    if (RegSetValueEx(hKey, TEXT("EnableControl"), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), sizeof(dwData)) != ERROR_SUCCESS) {
        std::cout << "Unable to write a value.\n";
        return 1;
    }

    // Закриття ключа реєстру
    RegCloseKey(hKey);

    std::cout << "Value successfully changed!!!\n";
    return 0;
}