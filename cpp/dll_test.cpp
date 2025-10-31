#include <iostream>
#include <windows.h>

int main() {
    // Try to load the DLL
    HMODULE hLib = LoadLibrary("cton-sdk.dll");
    
    if (hLib == NULL) {
        DWORD error = GetLastError();
        std::cout << "Failed to load cton-sdk.dll. Error code: " << error << std::endl;
        
        // Try to load with full path
        hLib = LoadLibrary("C:\\Users\\Богдан\\Desktop\\cton-sdk\\cpp\\cton-sdk.dll");
        if (hLib == NULL) {
            error = GetLastError();
            std::cout << "Failed to load cton-sdk.dll with full path. Error code: " << error << std::endl;
            return 1;
        }
    }
    
    std::cout << "Successfully loaded cton-sdk.dll" << std::endl;
    
    // Try to get a function address
    FARPROC proc = GetProcAddress(hLib, "address_create");
    if (proc == NULL) {
        DWORD error = GetLastError();
        std::cout << "Failed to get address_create function. Error code: " << error << std::endl;
        FreeLibrary(hLib);
        return 1;
    }
    
    std::cout << "Successfully found address_create function" << std::endl;
    
    // Clean up
    FreeLibrary(hLib);
    
    std::cout << "DLL test completed successfully!" << std::endl;
    return 0;
}