#pragma once
#include <windows.h>
#include "offsets.h"
#pragma comment(lib, "user32.lib")

#define IOCTL_READ  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

struct Rq { void* address; void* buffer; size_t size; };

namespace drv
{
    HANDLE hDev = INVALID_HANDLE_VALUE;

    bool Open()
    {
        hDev = CreateFileW(L"\\\\.\\SoftAssistDev", GENERIC_READ | GENERIC_WRITE,
                           0, nullptr, OPEN_EXISTING, 0, nullptr);
        return hDev != INVALID_HANDLE_VALUE;
    }

    bool Read(void* addr, void* out, size_t len)
    {
        if (hDev == INVALID_HANDLE_VALUE) return false;
        Rq rq = { addr, out, len };
        DWORD ret = 0;
        return DeviceIoControl(hDev, IOCTL_READ, &rq, sizeof(rq), &rq, sizeof(rq), &ret, nullptr);
    }

    template <typename T>
    bool Read(uint64_t addr, T& out)
    {
        return Read((void*)addr, &out, sizeof(T));
    }

    bool Write(void* addr, const void* in, size_t len)
    {
        if (hDev == INVALID_HANDLE_VALUE) return false;
        Rq rq = { addr, (void*)in, len };
        DWORD ret = 0;
        return DeviceIoControl(hDev, IOCTL_WRITE, &rq, sizeof(rq), &rq, sizeof(rq), &ret, nullptr);
    }

    void Close() { if (hDev != INVALID_HANDLE_VALUE) CloseHandle(hDev); }
}
