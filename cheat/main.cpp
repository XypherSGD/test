#include <windows.h>
#include <cmath>
#include <cstdio>
#include "ioctl_comm.h"
#include "offsets.h"

struct Vec3 { float x, y, z; };

// ---- config ---- (tune these)
static float cfg_fov       = 4.0f;   // degrees — small on purpose
static float cfg_smoothing = 10.0f;  // higher = gentler pull
static float cfg_sens      = 0.06f;  // your in-game sens as rad-per-pixel; we'll calibrate

bool WorldToScreen(Vec3 pos, POINT& out)
{
    // TODO: camera/view matrix read here (matrix offset goes in offsets.h)
    return false;
}

float D2R(float d) { return d * 3.14159265f / 180.0f; }

void SoftAimStep(Vec3 myHead, Vec3 targetHead, float viewYaw, float viewPitch)
{
    Vec3 d = { targetHead.x - myHead.x, targetHead.y - myHead.y, targetHead.z - myHead.z };
    float yaw   = atan2f(d.y, d.x);
    float pitch  = atan2f(d.z, sqrtf(d.x * d.x + d.y * d.y));
    float dy = yaw - viewYaw, dp = pitch - viewPitch;
    if (dy >  3.14159f) dy -= 2 * 3.14159f;
    if (dy < -3.14159f) dy += 2 * 3.14159f;
    float dist = sqrtf(dy * dy + dp * dp);
    if (dist > D2R(cfg_fov)) return;

    long px = (long)(-dy / cfg_smoothing / cfg_sens);
    long py = (long)( dp / cfg_smoothing / cfg_sens);
    INPUT in = {};
    in.type = INPUT_MOUSE;
    in.mi.dx = px; in.mi.dy = py;
    SendInput(1, &in, sizeof(INPUT));
}

int main()
{
    if (!drv::Open())
    {
        MessageBoxW(nullptr, L"Driver not loaded — run mapper first",
                    L"SoftAssist", MB_ICONWARNING);
        return 1;
    }

    // TODO: offset validation hook — if offsets are wrong, exit instead of acting

    while (!GetAsyncKeyState(VK_END))
    {
        // Loop: read players, find nearest target inside FOV, SoftAimStep(...)
        Sleep(4); // ~250 Hz, gentle on CPU
    }
    drv::Close();
    return 0;
}
