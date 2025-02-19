#pragma once
#define MEMORY_H_
#include <memory>
#include <vector>
#include <cstdint>
#include "Color.h"

#pragma region memory_definitions
#pragma warning(push)
#pragma warning(disable: 6255) // '_alloca' indicates failure by raising a stack overflow exception. consider using '_malloca' instead
#define MEM_STACKALLOC(SIZE) _malloca(SIZE)
#pragma warning(pop)
#define MEM_STACKFREE(MEMORY) static_cast<void>(0)

#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)

#define MEM_PAD(SIZE)										\
private:												\
	char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE]; \
public:
#pragma endregion

struct Vector;
struct KV3ID_t;
struct SOID_t;
class CUtlBuffer;
class CGCClientSystem;
class CGCClientSharedObjectCache;
class CGCClientSharedObjectTypeCache;
class CCSInventoryManager;
class CKeyValues3;
class C_BaseEntity;

#define CHAT_COLOR_WHITE "\x01"
#define CHAT_COLOR_DARK_RED "\x02"
#define CHAT_COLOR_PURPLE "\x03"
#define CHAT_COLOR_DARK_GREEN "\x04"
#define CHAT_COLOR_LIGHT_GREEN "\x05"
#define CHAT_COLOR_GREEN "\x06"
#define CHAT_COLOR_LIGHT_RED "\x07"
#define CHAT_COLOR_GRAY "\x08"
#define CHAT_COLOR_YELLOW "\x09"
#define CHAT_COLOR_DARK_YELLOW "\x10"
#define CHAT_COLOR_LIGHT_BLUE "\x0A"
#define CHAT_COLOR_BLUE "\x0B"
#define CHAT_COLOR_DARK_BLUE "\x0C"
#define CHAT_COLOR_DARK_PURPLE "\x0D"
#define CHAT_COLOR_PINK "\x0E"
#define CHAT_COLOR_RED "\x0F"

namespace MEM
{
	void* HeapAlloc(const std::size_t nSize);
	void HeapFree(void* pMemory);
	void* HeapRealloc(void* pMemory, const std::size_t nNewSize);

	void* GetModuleBaseHandle(const char* wszModuleName);
	const char* GetModuleBaseFileName(const void* hModuleBase);
	void* GetExportAddress(const void* hModuleBase, const char* szProcedureName);
	bool GetSectionInfo(const void* hModuleBase, const char* szSectionName, std::uint8_t** ppSectionStart, std::size_t* pnSectionSize);

	void* RegisterChannel(const char* szName);
	void* AcquireChannel(const char* szName);
	void Msg(void* pChannel, const char* szMsg, Color nColor);

	inline void* GetAbsoluteAddress(void* pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
	{
		if (!pRelativeAddress)
			return nullptr;

        uint8_t* k = (uint8_t*)pRelativeAddress;
		k += nPreOffset;
		k += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(k);
		k += nPostOffset;
		return k;
	}

	inline void* PatternScan(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
			};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scanBytes[i];
			}
		}
		return nullptr;
	}

	inline std::uint8_t* ResolveRelativeAddress(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
	{
		std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(nAddressBytes + nRVAOffset);
		std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

		return reinterpret_cast<std::uint8_t*>(nRVA + nRIP);
	}

	template <typename T = void*>
	inline T GetVFunc(const void* thisptr, std::size_t nIndex)
	{
		T* p = (*static_cast<T* const*>(thisptr));
		return p[nIndex];
	}

	template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
	static inline T CallVFunc(CBaseClass* thisptr, Args_t... argList)
	{
		using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
		auto q = (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)));
		if (q && q[nIndex])
			return q[nIndex](thisptr, argList...);
	}

	std::size_t PatternToBytes(const char* szPattern, std::uint8_t* pOutByteBuffer, char* szOutMaskBuffer);
	std::size_t BytesToPattern(const std::uint8_t* pByteBuffer, const std::size_t nByteCount, char* szOutBuffer);
}