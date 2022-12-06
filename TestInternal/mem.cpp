#include "pch.h"
#include "mem.h"

// 0x90 NOP, 0xE9 jump

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr,
					  std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0, offSize = offsets.size();
		i < offSize; ++i)
	{
		address = *(uintptr_t*)address;
		address += offsets[i];
	}
	return address;
}

bool mem::Hook(void* dst, void* newFunc, int size)
{
	if (size < 5)
		return false;

	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	DWORD relativeAddress = ((DWORD)newFunc - (DWORD)dst) - 5;

	*(BYTE*)dst = 0xE9;
	*(DWORD*)((DWORD)dst + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(dst, size, oldprotect, &temp);

	return true;
}

bool mem::Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAdress = dst - src - 5;
	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAdress;
	VirtualProtect(src, len, curProtection, &curProtection);
	return true;
}

BYTE* mem::TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return 0;

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len,
										MEM_COMMIT |
										MEM_RESERVE,
										PAGE_EXECUTE_READWRITE);

	memcpy_s(gateway, len, src, len);
	uintptr_t gatewayRelativeAddress = src - gateway - 5;
	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddress;
	Detour32(src, dst, len);
	return gateway;
}