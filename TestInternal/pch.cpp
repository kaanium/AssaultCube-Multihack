#include "pch.h"
#include "mem.h"
#include <iostream>

constexpr float degreeToRadian = (3.141592653589793238463f / 180.0f);
constexpr auto side = 905;
constexpr float radianToDegree = (180.0f / 3.141592653589793238463f);

Vector3 CalculateDegrees(Vector3 myPosition, Vector3 targetPosition)
{
    Vector3 distance{ abs(targetPosition.x - myPosition.x),
                      abs(targetPosition.y - myPosition.y),
                      abs(targetPosition.z - myPosition.z) };

    float pisagor = sqrtf(distance.x * distance.x + distance.y * distance.y),
        pisagor2 = sqrtf(distance.z * distance.z + pisagor * pisagor),
        sonuc = asinf(distance.y / pisagor) * radianToDegree,
        sonuc2 = asinf(distance.z / pisagor2) * radianToDegree, relative;

    if (targetPosition.x > myPosition.x)               //tx > mx, ty > my = > 90  + sonuc
    {												   //tx > mx, ty < my = > 90  - sonuc
        relative = 90.0f;                              //tx < mx, ty > my = > 270 - sonuc
        if (targetPosition.y < myPosition.y)           //tx < mx, ty < my = > 270 + sonuc
            sonuc *= -1.0f;
    }
    else
    {
        relative = 270.0f;
        if (targetPosition.y > myPosition.y)
            sonuc *= -1.0f;
    }

    if (targetPosition.z < myPosition.z)
        sonuc2 *= -1.0f;

    return { relative + sonuc, sonuc2, 0.0f };
}

bool Visible(Vector3 from, Vector3 to)
{
    /*static*/ DWORD p = 0x0048ABD0;
    __asm
    {
        push to.z
        push to.y
        push to.x
        push from.z
        push from.y
        push from.x
        xor eax, eax
        call p
        add esp, 24   // stack 6 arguments * 4
    }
}

int GetClosestAliveEnemy(Vector3 myPosition, int entityCount, entityList* enemy)
{
    float minDistance = 500;
    int index = -1;
    for (int i = 0; i < entityCount; ++i)
    {
        if ((minDistance > (abs(myPosition.x - enemy->botPtr[i]->headPosition.x)
                            + abs(myPosition.y - enemy->botPtr[i]->headPosition.y)
                            + abs(myPosition.z - enemy->botPtr[i]->headPosition.z)))
                          && !(enemy->botPtr[i]->dead)
                          && Visible(myPosition, enemy->botPtr[i]->headPosition))
        {
            minDistance = abs(myPosition.x - enemy->botPtr[i]->headPosition.x)
                + abs(myPosition.y - enemy->botPtr[i]->headPosition.y)
                + abs(myPosition.z - enemy->botPtr[i]->headPosition.z);
            index = i;
        }
    }
    return index;
}

bool CheckCrosshair(int entityCount, entityList* enemy,
                    int crosshair, int ptr)
{
    for (unsigned int i = 1; i <= entityCount; ++i)
    {
        uintptr_t a = mem::FindDMAAddy(ptr, { i * 0x4, 0x0 });
        if (a == crosshair
         && *(bool*)(a + 0x32c)
         && (!*(bool*)(a + 0x338))) return true;
    }
    return false;
}

void drawRect(std::vector<Vector3> mouse,
              std::vector<Vector3> foot,
              std::vector<Vector3> head)
{
    static HWND hWnd = FindWindowA(NULL, "AssaultCube");

    // We calculate the difference of degrees between our screen's position
    // and the required position for aiming at a target. Then we imagine a
    // 90-45-45 triangle and draw a line to Pythagorean, which is the
    // width of our screen, 1280 in this case, from the corner with
    // 90 degrees using the degree we found before. After that, we use this formula to
    // calculate the side our degree is pointing at -- c * sin(a) * sin(b) --
    // (c) is the side that the corner with 45 degrees pointing. (a)
    // is the degree we found before. Since we know (c)'s degree is
    // always going to be 45 degrees, we can swap (b) with (135 - a). Which,
    // results in -- 905 * sin(a) * sin(135 - a) --

    float relx, rely, length;
    static HDC screenDC = ::GetDC(hWnd);
    static HDC bufferDC = screenDC;

    static HBRUSH brush = ::CreateSolidBrush(RGB(255, 0, 0));
    HBITMAP bmp = CreateCompatibleBitmap(screenDC,
                                         1280,
                                         720);

    HBITMAP oldBmp = (HBITMAP)SelectObject(bufferDC, bmp);
    for (int i = 0, a = mouse.size(); i < a; ++i)
    {
        relx = side * sin((head[i].x - mouse[i].x + 45) * degreeToRadian)
            / sin((mouse[i].x - head[i].x + 90) * degreeToRadian),
            rely = 720 - 720 * sin(((foot[i].y + head[i].y)
                                    / 2 - mouse[i].y + 30)
                                     * degreeToRadian)
            / sin((90 + mouse[i].y - (foot[i].y + head[i].y) / 2) * degreeToRadian),
            length = (head[i].y - foot[i].y) * 14.0f;

        const RECT rect = { relx - length * 0.1f, rely + length * 0.3f,
                      relx + length * 0.25f, rely + length * 1.3f };

        ::FrameRect(screenDC, &rect, brush);
    }
    BitBlt(screenDC, 0, 0, 1280,
           720, bufferDC, 0, 0, SRCCOPY);
    SelectObject(bufferDC, oldBmp);
    DeleteObject(bmp);
}