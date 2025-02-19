#include "inc.h"

#pragma region memory_allocation

void* __cdecl operator new(const std::size_t nSize)
{
	return MEM::HeapAlloc(nSize);
}

void* __cdecl operator new[](const std::size_t nSize)
{
	return MEM::HeapAlloc(nSize);
}

void __cdecl operator delete(void* pMemory) noexcept
{
	MEM::HeapFree(pMemory);
}

void __cdecl operator delete[](void* pMemory) noexcept
{
	MEM::HeapFree(pMemory);
}

void* MEM::HeapAlloc(const std::size_t nSize)
{
	const HANDLE hHeap = ::GetProcessHeap();
	return ::HeapAlloc(hHeap, 0UL, nSize);
}

void MEM::HeapFree(void* pMemory)
{
	if (pMemory != nullptr)
	{
		const HANDLE hHeap = ::GetProcessHeap();
		::HeapFree(hHeap, 0UL, pMemory);
	}
}

void* MEM::HeapRealloc(void* pMemory, const std::size_t nNewSize)
{
	if (pMemory == nullptr)
		return HeapAlloc(nNewSize);

	if (nNewSize == 0UL)
	{
		HeapFree(pMemory);
		return nullptr;
	}

	const HANDLE hHeap = ::GetProcessHeap();
	return ::HeapReAlloc(hHeap, 0UL, pMemory, nNewSize);
}

#pragma endregion

// @todo: move to win.cpp (or platform.cpp?) except getsectioninfo
#pragma region memory_get

void* MEM::GetModuleBaseHandle(const char* wszModuleName)
{
	const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

	if (wszModuleName == nullptr)
		return pPEB->ImageBaseAddress;

	void* pModuleBase = nullptr;
	for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
	{
		const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		char* tmp = new char[wcslen(pEntry->BaseDllName.Buffer) + 1] {0};
		wcstombs(tmp, pEntry->BaseDllName.Buffer, wcslen(pEntry->BaseDllName.Buffer));
		std::string modName = std::string(tmp);
		std::transform(modName.begin(), modName.end(), modName.begin(),
			[](unsigned char c) { return std::tolower(c); });

		if (FNV1A::Hash(wszModuleName) == FNV1A::Hash(modName.c_str()))
		{
			pModuleBase = pEntry->DllBase;
			delete[] tmp;
			break;
		}
		delete[] tmp;
	}

	return pModuleBase;
}

const char* MEM::GetModuleBaseFileName(const void* hModuleBase)
{
	const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

	if (hModuleBase == nullptr)
		hModuleBase = pPEB->ImageBaseAddress;

	::EnterCriticalSection(pPEB->LoaderLock);

	const char* wszModuleName = nullptr;
	for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
	{
		const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (pEntry->DllBase == hModuleBase)
		{
			char* q = new char[wcslen(pEntry->BaseDllName.Buffer) + 1] {0};
			wcstombs(q, pEntry->BaseDllName.Buffer, wcslen(pEntry->BaseDllName.Buffer));
			wszModuleName = q;
			break;
		}
	}

	::LeaveCriticalSection(pPEB->LoaderLock);

	return wszModuleName;
}

void* MEM::GetExportAddress(const void* hModuleBase, const char* szProcedureName)
{
	const auto pBaseAddress = static_cast<const std::uint8_t*>(hModuleBase);

	const auto pIDH = static_cast<const IMAGE_DOS_HEADER*>(hModuleBase);
	if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;

	const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS64*>(pBaseAddress + pIDH->e_lfanew);
	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		return nullptr;

	const IMAGE_OPTIONAL_HEADER64* pIOH = &pINH->OptionalHeader;
	const std::uintptr_t nExportDirectorySize = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	const std::uintptr_t uExportDirectoryAddress = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	if (nExportDirectorySize == 0U || uExportDirectoryAddress == 0U)
	{
		return nullptr;
	}

	const auto pIED = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(pBaseAddress + uExportDirectoryAddress);
	const auto pNamesRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfNames);
	const auto pNameOrdinalsRVA = reinterpret_cast<const std::uint16_t*>(pBaseAddress + pIED->AddressOfNameOrdinals);
	const auto pFunctionsRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfFunctions);

	// Perform binary search to find the export by name
	std::size_t nRight = pIED->NumberOfNames, nLeft = 0U;
	while (nRight != nLeft)
	{
		// Avoid INT_MAX/2 overflow
		const std::size_t uMiddle = nLeft + ((nRight - nLeft) >> 1U);
		const int iResult = strcmp(szProcedureName, reinterpret_cast<const char*>(pBaseAddress + pNamesRVA[uMiddle]));

		if (iResult == 0)
		{
			const std::uint32_t uFunctionRVA = pFunctionsRVA[pNameOrdinalsRVA[uMiddle]];

			// Check if it's a forwarded export
			if (uFunctionRVA >= uExportDirectoryAddress && uFunctionRVA - uExportDirectoryAddress < nExportDirectorySize)
			{
				// Forwarded exports are not supported
				break;
			}

			return const_cast<std::uint8_t*>(pBaseAddress) + uFunctionRVA;
		}

		if (iResult > 0)
			nLeft = uMiddle + 1;
		else
			nRight = uMiddle;
	}

	// Export not found
	return nullptr;
}

bool MEM::GetSectionInfo(const void* hModuleBase, const char* szSectionName, std::uint8_t** ppSectionStart, std::size_t* pnSectionSize)
{
	const auto pBaseAddress = static_cast<const std::uint8_t*>(hModuleBase);

	const auto pIDH = static_cast<const IMAGE_DOS_HEADER*>(hModuleBase);
	if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS*>(pBaseAddress + pIDH->e_lfanew);
	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		return false;

	const IMAGE_SECTION_HEADER* pISH = IMAGE_FIRST_SECTION(pINH);

	// go through all code sections
	for (WORD i = 0U; i < pINH->FileHeader.NumberOfSections; i++, pISH++)
	{
		// @test: use case insensitive comparison instead?
		if (strncmp(szSectionName, reinterpret_cast<const char*>(pISH->Name), IMAGE_SIZEOF_SHORT_NAME) == 0)
		{
			if (ppSectionStart != nullptr)
				*ppSectionStart = const_cast<std::uint8_t*>(pBaseAddress) + pISH->VirtualAddress;

			if (pnSectionSize != nullptr)
				*pnSectionSize = pISH->SizeOfRawData;

			return true;
		}
	}

	return false;
}

#pragma endregion

#pragma region memory_extra

[[nodiscard]] constexpr bool IsDigit(const std::uint8_t uChar)
{
	return (uChar >= '0' && uChar <= '9');
}

[[nodiscard]] constexpr std::uint32_t CharToHexInt(const std::uint8_t uChar)
{
	const std::uint8_t uCharLower = (uChar | ('a' ^ 'A'));
	return ((uCharLower >= 'a' && uCharLower <= 'f') ? (uCharLower - 'a' + 0xA) : (IsDigit(uChar) ? (uChar - '0') : 0x0));
}

std::size_t MEM::PatternToBytes(const char* szPattern, std::uint8_t* pOutByteBuffer, char* szOutMaskBuffer)
{
	std::uint8_t* pCurrentByte = pOutByteBuffer;

	while (*szPattern != '\0')
	{
		// check is a wildcard
		if (*szPattern == '?')
		{
			++szPattern;
#ifdef CS_PARANOID
			assert(*szPattern == '\0' || *szPattern == ' ' || *szPattern == '?'); // we're expect that next character either terminating null, whitespace or part of double wildcard (note that it's required if your pattern written without whitespaces)
#endif

			// ignore that
			* pCurrentByte++ = 0U;
			*szOutMaskBuffer++ = '?';
		}
		// check is not space
		else if (*szPattern != ' ')
		{
			// convert two consistent numbers in a row to byte value
			std::uint8_t uByte = static_cast<std::uint8_t>(CharToHexInt(*szPattern) << 4);

			++szPattern;
#ifdef CS_PARANOID
			assert(*szPattern != '\0' && *szPattern != '?' && *szPattern != ' '); // we're expect that byte always represented by two numbers in a row
#endif

			uByte |= static_cast<std::uint8_t>(CharToHexInt(*szPattern));

			*pCurrentByte++ = uByte;
			*szOutMaskBuffer++ = 'x';
		}

		++szPattern;
	}

	// zero terminate both buffers
	*pCurrentByte = 0U;
	*szOutMaskBuffer = '\0';

	return pCurrentByte - pOutByteBuffer;
}

std::size_t MEM::BytesToPattern(const std::uint8_t* pByteBuffer, const std::size_t nByteCount, char* szOutBuffer)
{
	constexpr char _TWO_DIGITS_HEX_LUT[] =
		"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	char* szCurrentPattern = szOutBuffer;

	for (std::size_t i = 0U; i < nByteCount; i++)
	{
		// manually convert byte to chars
		const char* szHexByte = &_TWO_DIGITS_HEX_LUT[pByteBuffer[i] * 2U];
		*szCurrentPattern++ = szHexByte[0];
		*szCurrentPattern++ = szHexByte[1];
		*szCurrentPattern++ = ' ';
	}
	*--szCurrentPattern = '\0';

	return szCurrentPattern - szOutBuffer;
}

#pragma endregion

void* MEM::RegisterChannel(const char* szName) {
	using fn_t = void*(*)(const char*, uintptr_t, int, int, uint32_t);
	static fn_t fn = (fn_t)GetExportAddress(GetModuleBaseHandle("tier0.dll"), "LoggingSystem_RegisterLoggingChannel");
	return fn(szName, 0, 0, 2, 0xFFFFFFFF);
}

void* MEM::AcquireChannel(const char* szName) {
	static std::unordered_map<FNV1A_t, void*> gChannels{};

	if (!gChannels.contains(FNV1A::Hash(szName)))
		gChannels.insert({ FNV1A::Hash(szName), MEM::RegisterChannel(szName) });

	return gChannels[FNV1A::Hash(szName)];
}

void MEM::Msg(void* pChannel, const char* szMsg, Color nColor) {
	using fn_t = void* (*)(void* pChannel, int nSeverity, Color Color, const char* szMsg);
	static fn_t fn = (fn_t)GetExportAddress(GetModuleBaseHandle("tier0.dll"), "?LoggingSystem_Log@@YA?AW4LoggingResponse_t@@HW4LoggingSeverity_t@@VColor@@PEBDZZ");
	fn(pChannel, 2, nColor, (std::string(szMsg) + "\n").c_str());
}