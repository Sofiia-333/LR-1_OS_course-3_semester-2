#include <stdio.h>
#include <windows.h>
#include <Winerror.h>

#define MAX_KEY_LENGTH 255
#define MAX_STR_LENGTH 256
#define HARDWARE_LENGTH 5

// Function to open a registry key
void GetKeyHandle(HKEY hKeyValue, const char keyPath[], HKEY& hKey, LONG& lResult) {
    lResult = RegOpenKeyExA(
        hKeyValue,
        keyPath,
        0,
        KEY_READ,
        &hKey);
}

// Function to retrieve a registry value
void GetKeyValue(HKEY& hKey, LONG& lResult, const char keyStr[], char& buffer, DWORD& buffSize) {
    lResult = RegQueryValueExA(
        hKey,
        keyStr,
        NULL,
        NULL,
        (LPBYTE)&buffer,
        &buffSize);
}

int CheckError(LONG& lResult, int path) {
    if (lResult != ERROR_SUCCESS) {
        if (path == 1) {
            return 0x0BAD;
        }
        else {
            return 0x0BAD;
        }
    }
}

void printError(const char path[], const char key[], int check) {
    if (check == 1) {
        printf("[-] An error occurred while trying to get a key handle for:\n\t%s\n\n", path);
    }
    else {
        printf("[-] An error occurred while trying to get a value:\n\t%s%s\n\n", path, key);
    }
}

void noWarn(char* array) {
    array[MAX_STR_LENGTH - 1] = '\0';
}

int main() {

    HKEY hKey;
    LONG lResult;
    char procBuffer[MAX_STR_LENGTH],
        diskBuffer[MAX_STR_LENGTH],
        networkDriverBuffer[MAX_STR_LENGTH],
        videoDriverBuffer[MAX_STR_LENGTH],
        mouseDriverBuff[MAX_STR_LENGTH];
    DWORD buffSize;

    // Paths to hardware information
    const char* path[HARDWARE_LENGTH] = {
      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
      "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0000",
      "SYSTEM\\CurrentControlSet\\Control\\Video\\{6066D8FC-E209-11EE-BC54-806E6F6E6963}\\0000",
      "SYSTEM\\CurrentControlSet\\Services\\mouclass\\Enum",
      "SYSTEM\\CurrentControlSet\\Services\\disk\\Enum"
    };
    // Parameter names
    const char* filenames[HARDWARE_LENGTH] = {
      "ProcessorNameString",
      "DriverDesc",
      "DriverDesc",
      "0",
      "0"
    };
    const char* names[HARDWARE_LENGTH] = {
      "Processor",
      "Network card",
      "Video card",
      "Mouse",
      "Disk"
    };
    char* pointers[HARDWARE_LENGTH] = { procBuffer, diskBuffer, networkDriverBuffer, videoDriverBuffer, mouseDriverBuff };

    for (int i = 0; i < HARDWARE_LENGTH; i++) {
        GetKeyHandle(HKEY_LOCAL_MACHINE, path[i], hKey, lResult);
        if (CheckError(lResult, 1) == 0x0BAD) { printError(path[i], filenames[i], 1); continue; }
        buffSize = strlen(pointers[i]);
        GetKeyValue(hKey, lResult, filenames[i], *pointers[i], buffSize);
        if (CheckError(lResult, 0) == 0x0BAD) { printError(path[i], filenames[i], 0); continue; }
        noWarn(pointers[i]);
        printf("[*] %s: %s\n\n", names[i], pointers[i]);
    }
    // Close registry key
    RegCloseKey(hKey);
    return EXIT_SUCCESS;
}



