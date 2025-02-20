#pragma once
#include <cstdint>

class IDXGISwapChain;
struct ISwapChainDx11
{
    char _pad[0x170];
    IDXGISwapChain* pDXGISwapChain;
};

class IEngineClient {
public:
	void ClientCmd_Unrestricted(const char* szCmd);
	bool IsInGame();
};

class CBaseFileSystem
{
public:
	// Reads a file
	int Read(void* pOutput, int size, void* file);
	// Opens a file
	void* Open(const char* pFileName, const char* pOptions, const char* pathID = nullptr);
	// Closes a file
	void Close(void* file);
	// Get's a file's size
	unsigned int Size(void* file);
	// not sure what this does
	const char* unk(const char* file, uintptr_t* unk);
};

class IInputSystem {
public:
	void SetRelativeMouseMode(bool bMouseMode);
};


class CUIPanel;

struct CPanel2D {
	void* vmt;
	CUIPanel* uiPanel;
};

class CUIEngine {
public:
	void RunScript(CUIPanel* panel, const char* script);
};

class CPanoramaUIEngine {
public:
	CUIEngine* AccessUIEngine();

	CPanel2D* GetMainMenuPanel();

	void RunScriptOnMainMenuPanel(const char* script);
};

template <class T, class I>
struct UtlLinkedListElement_t
{
	UtlLinkedListElement_t(const UtlLinkedListElement_t&) = delete;

	T element;
	I iPrevious;
	I iNext;
};

template <class T, class N = int>
class CUtlMemory
{
	enum
	{
		EXTERNAL_BUFFER_MARKER = -1,
		EXTERNAL_CONST_BUFFER_MARKER = -2,
	};

public:
	class Iterator_t
	{
	public:
		Iterator_t(const N nIndex) :
			nIndex(nIndex) {
		}

		bool operator==(const Iterator_t& it) const
		{
			return nIndex == it.nIndex;
		}

		bool operator!=(const Iterator_t& it) const
		{
			return nIndex != it.nIndex;
		}

		N nIndex;
	};

	CUtlMemory(const int nInitialGrowSize, const int nAllocationCount) :
		pMemory(nullptr), nAllocationCount(nAllocationCount), nGrowSize(nInitialGrowSize)
	{
		assert(nInitialGrowSize >= 0);
	}

	CUtlMemory(T* pMemory, const int nElements) :
		pMemory(pMemory), nAllocationCount(nElements), nGrowSize(EXTERNAL_BUFFER_MARKER) {
	}

	CUtlMemory(const T* pMemory, const int nElements) :
		pMemory(pMemory), nAllocationCount(nElements), nGrowSize(EXTERNAL_CONST_BUFFER_MARKER) {
	}

	~CUtlMemory()
	{
	}

	CUtlMemory(const CUtlMemory&) = delete;

	CUtlMemory(CUtlMemory&& moveFrom) noexcept :
		pMemory(moveFrom.pMemory), nAllocationCount(moveFrom.nAllocationCount), nGrowSize(moveFrom.nGrowSize)
	{
		moveFrom.pMemory = nullptr;
		moveFrom.nAllocationCount = 0;
		moveFrom.nGrowSize = 0;
	}

	CUtlMemory& operator=(const CUtlMemory&) = delete;

	CUtlMemory& operator=(CUtlMemory&& moveFrom) noexcept
	{
		// copy member variables to locals before purge to handle self-assignment
		T* pMemoryTemp = moveFrom.pMemory;
		const int nAllocationCountTemp = moveFrom.nAllocationCount;
		const int nGrowSizeTemp = moveFrom.nGrowSize;

		moveFrom.pMemory = nullptr;
		moveFrom.nAllocationCount = 0;
		moveFrom.nGrowSize = 0;

		pMemory = pMemoryTemp;
		nAllocationCount = nAllocationCountTemp;
		nGrowSize = nGrowSizeTemp;
		return *this;
	}

	[[nodiscard]] T& operator[](const N nIndex)
	{
		assert(IsValidIndex(nIndex));
		return pMemory[nIndex];
	}

	[[nodiscard]] const T& operator[](const N nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return pMemory[nIndex];
	}

	[[nodiscard]] T& Element(const N nIndex)
	{
		assert(IsValidIndex(nIndex));
		return pMemory[nIndex];
	}

	[[nodiscard]] const T& Element(const N nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return pMemory[nIndex];
	}

	[[nodiscard]] T* Base()
	{
		return pMemory;
	}

	[[nodiscard]] const T* Base() const
	{
		return pMemory;
	}

	[[nodiscard]] int AllocationCount() const
	{
		return nAllocationCount;
	}

	[[nodiscard]] bool IsExternallyAllocated() const
	{
		return nGrowSize <= EXTERNAL_BUFFER_MARKER;
	}

	[[nodiscard]] static N InvalidIndex()
	{
		return static_cast<N>(-1);
	}

	[[nodiscard]] bool IsValidIndex(N nIndex) const
	{
		return (nIndex >= 0) && (nIndex < nAllocationCount);
	}

	[[nodiscard]] Iterator_t First() const
	{
		return Iterator_t(IsValidIndex(0) ? 0 : InvalidIndex());
	}

	[[nodiscard]] Iterator_t Next(const Iterator_t& it) const
	{
		return Iterator_t(IsValidIndex(it.nIndex + 1) ? it.nIndex + 1 : InvalidIndex());
	}

	[[nodiscard]] N GetIndex(const Iterator_t& it) const
	{
		return it.nIndex;
	}

	[[nodiscard]] bool IsIndexAfter(N nIndex, const Iterator_t& it) const
	{
		return nIndex > it.nIndex;
	}

	[[nodiscard]] bool IsValidIterator(const Iterator_t& it) const
	{
		return IsValidIndex(it.index);
	}

	[[nodiscard]] Iterator_t InvalidIterator() const
	{
		return Iterator_t(InvalidIndex());
	}

	void EnsureCapacity(const int nCapacity)
	{
		if (nAllocationCount >= nCapacity)
			return;

		if (IsExternallyAllocated())
		{
			// can't grow a buffer whose memory was externally allocated
			assert(false);
			return;
		}

		nAllocationCount = nCapacity;
	}

protected:
	T* pMemory; // 0x00
	int nAllocationCount; // 0x04
	int nGrowSize; // 0x08
};

template <class T, class S = unsigned short, bool ML = false, class I = S, class M = CUtlMemory<UtlLinkedListElement_t<T, S>, I>>
class CUtlLinkedList
{
public:
	using ElemType_t = T;
	using IndexType_t = S;
	using IndexLocalType_t = I;
	using MemoryAllocator_t = M;

	template <typename List_t>
	class ConstIterator_t
	{
	public:
		typedef typename List_t::ElemType_t ElemType_t;
		typedef typename List_t::IndexType_t IndexType_t;

		ConstIterator_t() :
			pList(nullptr), nIndex(List_t::InvalidIndex()) {
		}

		ConstIterator_t(const List_t& list, IndexType_t nIndex) :
			pList(&list), nIndex(nIndex) {
		}

		ConstIterator_t& operator++()
		{
			nIndex = pList->Next(nIndex);
			return *this;
		}

		ConstIterator_t operator++(int)
		{
			ConstIterator_t pCopy = *this;
			++(*this);
			return pCopy;
		}

		ConstIterator_t& operator--()
		{
			assert(nIndex != pList->Head());
			nIndex = (nIndex == pList->InvalidIndex() ? pList->Tail() : pList->Previous(nIndex));
			return *this;
		}

		ConstIterator_t operator--(int)
		{
			ConstIterator_t pCopy = *this;
			--(*this);
			return pCopy;
		}

		bool operator==(const ConstIterator_t& other) const
		{
			assert(pList == other.pList);
			return nIndex == other.nIndex;
		}

		bool operator!=(const ConstIterator_t& other) const
		{
			assert(pList == other.pList);
			return nIndex != other.nIndex;
		}

		const ElemType_t& operator*() const
		{
			return pList->Element(nIndex);
		}

		const ElemType_t* operator->() const
		{
			return (&**this);
		}

	protected:
		const List_t* pList;
		IndexType_t nIndex;
	};

	template <typename List_t>
	class Iterator_t : public ConstIterator_t<List_t>
	{
	public:
		using ElemType_t = typename List_t::ElemType_t;
		using IndexType_t = typename List_t::IndexType_t;
		using Base_t = ConstIterator_t<List_t>;

		Iterator_t() {}

		Iterator_t(const List_t& list, IndexType_t nIndex) :
			ConstIterator_t<List_t>(list, nIndex) {}

		Iterator_t& operator++()
		{
			Base_t::nIndex = Base_t::pList->Next(Base_t::nIndex);
			return *this;
		}

		Iterator_t operator++(int)
		{
			Iterator_t pCopy = *this;
			++(*this);
			return pCopy;
		}

		Iterator_t& operator--()
		{
			Base_t::nIndex = (Base_t::nIndex == Base_t::pList->InvalidIndex() ? Base_t::pList->Tail() : Base_t::pList->Previous(Base_t::nIndex));
			return *this;
		}

		Iterator_t operator--(int)
		{
			Iterator_t pCopy = *this;
			--(*this);
			return pCopy;
		}

		ElemType_t& operator*() const
		{
			List_t* pMutableList = const_cast<List_t*>(Base_t::pList);
			return pMutableList->Element(Base_t::nIndex);
		}

		ElemType_t* operator->() const
		{
			return (&**this);
		}
	};

	CUtlLinkedList(int nGrowSize = 0, int nSize = 0) :
		memory(nGrowSize, nSize), iHead(InvalidIndex()), iTail(InvalidIndex()), iFirstFree(InvalidIndex()), nElementCount(0), nAllocated(0), itLastAlloc(memory.InvalidIterator()), pElements(memory.Base()) {
	}

	~CUtlLinkedList()
	{
		RemoveAll();
	}

	CUtlLinkedList(const CUtlLinkedList&) = delete;
	CUtlLinkedList& operator=(const CUtlLinkedList&) = delete;

	T& operator[](const I nIndex)
	{
		assert(IsValidIndex(nIndex));
		return memory[nIndex].element;
	}

	const T& operator[](const I nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return memory[nIndex].element;
	}

	[[nodiscard]] T& Element(const I nIndex)
	{
		assert(IsValidIndex(nIndex));
		return memory[nIndex].element;
	}

	[[nodiscard]] const T& Element(const I nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return memory[nIndex].element;
	}

	[[nodiscard]] I Head() const
	{
		return iHead;
	}

	[[nodiscard]] I Tail() const
	{
		return iTail;
	}

	[[nodiscard]] I Previous(const I nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return InternalElement(nIndex).iPrevious;
	}

	[[nodiscard]] I Next(const I nIndex) const
	{
		assert(IsValidIndex(nIndex));
		return InternalElement(nIndex).iNext;
	}

	[[nodiscard]] static S InvalidIndex()
	{
		return static_cast<S>(M::InvalidIndex());
	}

	[[nodiscard]] bool IsValidIndex(const I nIndex) const
	{
		if (!memory.IsValidIndex(nIndex))
			return false;

		if (memory.IsIndexAfter(nIndex, itLastAlloc))
			return false; // don't read values that have been allocated, but not constructed

		return (memory[nIndex].iPrevious != nIndex) || (memory[nIndex].iNext == nIndex);
	}

	[[nodiscard]] static bool IsIndexInRange(I nIndex)
	{
		static_assert(sizeof(I) >= sizeof(S));
		static_assert(sizeof(S) > 2 || static_cast<S>(-1) > 0);
		static_assert(M::InvalidIndex() == -1 || M::InvalidIndex() == static_cast<S>(M::InvalidIndex()));

		return (static_cast<S>(nIndex) == nIndex && static_cast<S>(nIndex) != InvalidIndex());
	}

	[[nodiscard]] I Find(const T& source) const
	{
		for (I i = iHead; i != InvalidIndex(); i = Next(i))
		{
			if (Element(i) == source)
				return i;
		}

		return InvalidIndex();
	}

	void RemoveAll()
	{
		if (itLastAlloc == memory.InvalidIterator())
		{
			assert(iHead == InvalidIndex() && iTail == InvalidIndex() && iFirstFree == InvalidIndex() && nElementCount == 0);
			return;
		}

		if constexpr (ML)
		{
			for (typename M::Iterator_t it = memory.First(); it != memory.InvalidIterator(); it = memory.Next(it))
			{
				I i = memory.GetIndex(it);

				// skip elements already in the free list
				if (IsValidIndex(i))
				{
					ListElement_t& internalElement = InternalElement(i);
					(&internalElement.element)->~T();
					internalElement.iPrevious = i;
					internalElement.iNext = iFirstFree;
					iFirstFree = i;
				}

				// don't destruct elements that haven't ever been constructed
				if (it == itLastAlloc)
					break;
			}
		}
		else
		{
			I i = iHead, iNext;
			while (i != InvalidIndex())
			{
				ListElement_t& internalElement = InternalElement(i);
				(&internalElement.element)->~T();

				internalElement.iPrevious = i;
				iNext = Next(i);
				internalElement.iNext = iNext == InvalidIndex() ? iFirstFree : iNext;
				i = iNext;
			}

			if (iHead != InvalidIndex())
				iFirstFree = iHead;
		}

		// clear everything else out
		iHead = InvalidIndex();
		iTail = InvalidIndex();
		nElementCount = 0;
	}

	[[nodiscard]] auto begin() const
	{
		return ConstIterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, Head());
	}

	[[nodiscard]] auto begin()
	{
		return Iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, Head());
	}

	[[nodiscard]] auto end() const
	{
		return ConstIterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, InvalidIndex());
	}

	[[nodiscard]] auto end()
	{
		return Iterator_t<CUtlLinkedList<T, S, ML, I, M>>(*this, InvalidIndex());
	}

protected:
	using ListElement_t = UtlLinkedListElement_t<T, S>;

	[[nodiscard]] ListElement_t& InternalElement(const I nIndex)
	{
		return memory[nIndex];
	}

	[[nodiscard]] const ListElement_t& InternalElement(const I nIndex) const
	{
		return memory[nIndex];
	}

	M memory;
	I iHead;
	I iTail;
	I iFirstFree;
	I nElementCount;
	I nAllocated;
	typename M::Iterator_t itLastAlloc;
	ListElement_t* pElements;
};

// command to convars and concommands
enum EConVarFlag : int
{
	// convar systems
	FCVAR_NONE = 0,
	FCVAR_UNREGISTERED = (1 << 0),	// if this is set, don't add to linked list, etc
	FCVAR_DEVELOPMENTONLY = (1 << 1),	// hidden in released products. flag is removed automatically if allow_development_cvars is defined
	FCVAR_GAMEDLL = (1 << 2),	// defined by the game dll
	FCVAR_CLIENTDLL = (1 << 3),	// defined by the client dll
	FCVAR_HIDDEN = (1 << 4),	// hidden. doesn't appear in find or autocomplete. like developmentonly, but can't be compiled out

	// convar only
	FCVAR_PROTECTED = (1 << 5),	// it's a server cvar, but we don't send the data since it's a password, etc. sends 1 if it's not bland/zero, 0 otherwise as value
	FCVAR_SPONLY = (1 << 6),	// this cvar cannot be changed by clients connected to a multiplayer server
	FCVAR_ARCHIVE = (1 << 7),	// set to cause it to be saved to vars.rc
	FCVAR_NOTIFY = (1 << 8),	// notifies players when changed
	FCVAR_USERINFO = (1 << 9),	// changes the client's info string
	FCVAR_CHEAT = (1 << 14),	// only useable in singleplayer/debug/multiplayer & sv_cheats
	FCVAR_PRINTABLEONLY = (1 << 10),	// this cvar's string cannot contain unprintable characters (e.g., used for player name etc)
	FCVAR_UNLOGGED = (1 << 11),	// if this is a fcvar_server, don't log changes to the log file / console if we are creating a log
	FCVAR_NEVER_AS_STRING = (1 << 12),	// never try to print that cvar

	// it's a convar that's shared between the client and the server.
	// at signon, the values of all such convars are sent from the server to the client (skipped for local client, ofc)
	// if a change is requested it must come from the console (i.e., no remote client changes)
	// if a value is changed while a server is active, it's replicated to all connected clients
	FCVAR_REPLICATED = (1 << 13),	// server setting enforced on clients, replicated
	// @todo: (1 << 14) used by the game, probably used as modification detection
	FCVAR_DEMO = (1 << 16),	// record this cvar when starting a demo file
	FCVAR_DONTRECORD = (1 << 17),	// don't record these command in demofiles
	FCVAR_RELOAD_MATERIALS = (1 << 20),	// if this cvar changes, it forces a material reload
	FCVAR_RELOAD_TEXTURES = (1 << 21),	// if this cvar changes, if forces a texture reload
	FCVAR_NOT_CONNECTED = (1 << 22),	// cvar cannot be changed by a client that is connected to a server
	FCVAR_MATERIAL_SYSTEM_THREAD = (1 << 23),	// indicates this cvar is read from the material system thread
	FCVAR_ARCHIVE_XBOX = (1 << 24),	// cvar written to config.cfg on the xbox
	FCVAR_ACCESSIBLE_FROM_THREADS = (1 << 25),	// used as a debugging tool necessary to check material system thread convars
	FCVAR_SERVER_CAN_EXECUTE = (1 << 28),	// the server is allowed to execute this command on clients via clientcommand/net_stringcmd/cbaseclientstate::processstringcmd
	FCVAR_SERVER_CANNOT_QUERY = (1 << 29),	// if this is set, then the server is not allowed to query this cvar's value (via iserverpluginhelpers::startquerycvarvalue)
	FCVAR_CLIENTCMD_CAN_EXECUTE = (1 << 30),	// ivengineclient::clientcmd is allowed to execute this command
	FCVAR_MATERIAL_THREAD_MASK = (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

enum EConVarType : short
{
	EConVarType_Invalid = -1,
	EConVarType_Bool,
	EConVarType_Int16,
	EConVarType_UInt16,
	EConVarType_Int32,
	EConVarType_UInt32,
	EConVarType_Int64,
	EConVarType_UInt64,
	EConVarType_Float32,
	EConVarType_Float64,
	EConVarType_String,
	EConVarType_Color,
	EConVarType_Vector2,
	EConVarType_Vector3,
	EConVarType_Vector4,
	EConVarType_Qangle,
	EConVarType_MAX
};

union CVValue_t
{
	bool i1;
	short i16;
	uint16_t u16;
	int i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	float fl;
	double db;
	const char* sz;
	Color clr;
	Vector2D vec2;
	Vector vec3;
	Vector4D vec4;
	QAngle ang;
};

class CConVar
{
public:
	const char* szName; // 0x0000
	CConVar* m_pNext; // 0x0008
	MEM_PAD(0x10); // 0x0010
	const char* szDescription; // 0x0020
	uint32_t nType; // 0x28
	uint32_t nRegistered; // 0x2C
	uint32_t nFlags; // 0x30
	MEM_PAD(0x8); // 0x34
	// @note: read-only, mofify with caution
	CVValue_t value; // 0x40
};

class IEngineCVar
{
public:
	MEM_PAD(0x40);
	CUtlLinkedList<CConVar*> listConvars;

	CConVar* Find(FNV1A_t uHashedName);
};