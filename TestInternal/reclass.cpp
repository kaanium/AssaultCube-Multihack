#include "pch.h"
#include "reclass.h"

// Created with ReClass.NET 1.2 by KN4CK3R

//struct Vector3 { float x, y, z; };
//
//class playerEnt
//{
//public:
//	Vector3 headPosition; //0x0004
//	Vector3 velocity; //0x0010
//	int32_t movingDirectionX; //0x001C
//	int32_t movingDirectionY; //0x0020
//	char pad_0024[4]; //0x0024
//	Vector3 accelaration; //0x0028
//	Vector3 bodyPosition; //0x0034
//	Vector3 mousePosition; //0x0040
//	char pad_004C[29]; //0x004C
//	bool onAir; //0x0069
//	char pad_006A[6]; //0x006A
//	bool notAfk; //0x0070
//	char pad_0071[15]; //0x0071
//	int8_t directionKeyY; //0x0080
//	int8_t directionKeyX; //0x0081
//	char pad_0082[10]; //0x0082
//	bool aPressed; //0x008C
//	bool dPressed; //0x008D
//	bool wPressed; //0x008E
//	bool sPressed; //0x008F
//	char pad_0090[104]; //0x0090
//	int32_t health; //0x00F8
//	int32_t armor; //0x00FC
//	char pad_0100[4]; //0x0100
//	int32_t mainWeaponId; //0x0104
//	char pad_0108[24]; //0x0108
//	int32_t remainingAmmo; //0x0120
//	char pad_0124[36]; //0x0124
//	int32_t subClip; //0x0148
//	char pad_014C[36]; //0x014C
//	int32_t subROF; //0x0170
//	char pad_0174[177]; //0x0174
//	char N000002B5[16]; //0x0225
//	char pad_0235[317]; //0x0235
//	int16_t N00000112; //0x0372
//	class weapon* weaponPtr; //0x0374
//	char pad_0378[1328]; //0x0378
//}; //Size: 0x08A8
//
//class weaponEnt
//{
//public:
//	char pad_0000[4]; //0x0000
//	int32_t weaponID; //0x0004
//	void* playerPtr; //0x0008
//	char(*gunNamePointer)[16]; //0x000C
//}; //Size: 0x0010