#pragma once

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15
#define ENT_MAX_NETWORKED_ENTRY 16384

class CBaseHandle
{
public:
	CBaseHandle() noexcept :
		nIndex(INVALID_EHANDLE_INDEX) { }

	CBaseHandle(const int nEntry) noexcept
	{
		nIndex = nEntry;
	}

	CBaseHandle(const int nEntry, const int nSerial) noexcept
	{
		nIndex = nEntry | (nSerial << NUM_SERIAL_NUM_SHIFT_BITS);
	}

	bool operator!=(const CBaseHandle& other) const noexcept
	{
		return nIndex != other.nIndex;
	}

	bool operator==(const CBaseHandle& other) const noexcept
	{
		return nIndex == other.nIndex;
	}

	bool operator<(const CBaseHandle& other) const noexcept
	{
		return nIndex < other.nIndex;
	}

	[[nodiscard]] bool m_bValid() const noexcept
	{
		return nIndex != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int m_iEntryIndex() const noexcept
	{
		return static_cast<int>(nIndex & ENT_ENTRY_MASK);
	}

	[[nodiscard]] int m_iSerialNumber() const noexcept
	{
		return static_cast<int>(nIndex >> NUM_SERIAL_NUM_SHIFT_BITS);
	}

private:
	std::uint32_t nIndex;
};