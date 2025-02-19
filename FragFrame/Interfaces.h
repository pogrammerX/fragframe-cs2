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