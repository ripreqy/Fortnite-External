#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

// --- Globale Variablen ---
inline uintptr_t Nunflaggedbase = 0;

// --- IOCTL-Definitionen ---
#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

#define CODE_RW                CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1336, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_BA                CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1921, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_GUARDED_REGION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1427, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_GET_DIR_BASE       CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1288, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CODE_SECURITY          0x5353735

// --- Maus-Flags ---
enum MouseFlags {
    Mouse_NONE = 0,
    Mouse_LeftButtonDown = 1,
    Mouse_LeftButtonUp = 2,
    Mouse_RightButtonDown = 4,
    Mouse_RightButtonUp = 8,
    Mouse_MiddleButtonDown = 16,
    Mouse_MiddleButtonUp = 32,
    Mouse_XButton1Down = 64,
    Mouse_XButton1Up = 128,
    Mouse_XButton2Down = 256,
    Mouse_XButton2Up = 512,
    Mouse_Wheel = 1024,
    Mouse_HorizontalWheel = 2048
};

// --- Strukturen ---
typedef struct _RW {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
    BOOLEAN write;
} RW, * PRW;

typedef struct _BA {
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} BA, * PBA;

typedef struct _SMoveMouse {
    long X;
    long Y;
    unsigned short ButtonFlags;
} SMoveMouse, * PSMoveMouse;

// --- Treiber-Kommunikation ---
namespace Driver {
    inline HANDLE driver_handle;
    inline INT32 process_id;

    inline bool Init() {
        driver_handle = CreateFileW(
            L"\\\\.\\{38472659102837465091827364501928374650918273645091827364501928374650918273645}",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        return (driver_handle != INVALID_HANDLE_VALUE);
    }

    inline void ReadPhysical(PVOID address, PVOID buffer, DWORD size) {
        RW arguments;
        arguments.security = CODE_SECURITY;
        arguments.process_id = process_id;
        arguments.address = (ULONGLONG)address;
        arguments.buffer = (ULONGLONG)buffer;
        arguments.size = (ULONGLONG)size;
        arguments.write = FALSE;

        DeviceIoControl(driver_handle, CODE_RW, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    inline void MoveMouse(int X, int Y, MouseFlags ButtonFlags) {
        SMoveMouse arguments;
        arguments.X = X;
        arguments.Y = Y;
        arguments.ButtonFlags = (unsigned short)ButtonFlags;

        DeviceIoControl(driver_handle, CODE_GET_GUARDED_REGION, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    inline uintptr_t GetBase() {
        uintptr_t base = 0;
        BA arguments;
        arguments.security = CODE_SECURITY;
        arguments.process_id = process_id;
        arguments.address = (ULONGLONG*)&base;

        DeviceIoControl(driver_handle, CODE_BA, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
        Nunflaggedbase = base;
        return base;
    }

    inline DWORD FindProcess(const wchar_t* name) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (Process32FirstW(snap, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, name) == 0) {
                    process_id = pe.th32ProcessID;
                    CloseHandle(snap);
                    return process_id;
                }
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
        return 0;
    }
}

// --- Hilfsfunktionen ---
inline bool is_valid(uint64_t address) {
    return (address > 0x400000 && address != 0xCCCCCCCCCCCCCCCC && address <= 0x7FFFFFFFFFFFFFFF);
}

template <typename T>
T read(uint64_t address) {
    T buffer{};
    if (is_valid(address)) {
        Driver::ReadPhysical((PVOID)address, &buffer, sizeof(T));
    }
    return buffer;
}

inline std::string read_wstr(uintptr_t address) {
    if (!read<uintptr_t>(address)) return "";

    const size_t buffer_size = 1024;
    wchar_t buffer[buffer_size] = { 0 };
    Driver::ReadPhysical((PVOID)address, buffer, buffer_size * sizeof(wchar_t));

    // Konvertierung mit Windows-API
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], size_needed, NULL, NULL);
    return result;
}