#include "inc.h"

void IEngineClient::ClientCmd_Unrestricted(const char* szCmd) {
    using fn_t = void(__fastcall*)(IEngineClient*, int, const char*, uint8_t);
    static fn_t fn = (fn_t)MEM::PatternScan(MEM::GetModuleBaseHandle("engine2.dll"), "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 81 EC ? ? ? ? 8D 42 FC");
    fn(this, 0, szCmd, 0x7FFEF001);
}

bool IEngineClient::IsInGame() {
    return MEM::CallVFunc<bool, 35U>(this);
}

void IInputSystem::SetRelativeMouseMode(bool bMouseMode) {
    MEM::CallVFunc<void, 76U>(this, bMouseMode);
}

int CBaseFileSystem::Read(void* pOutput, int size, void* file)
{
    using Fn = int(__thiscall*)(void*, void*, int, void*);
    return MEM::CallVFunc<int, 11U>(this, pOutput, size, file);
}

void* CBaseFileSystem::Open(const char* pFileName, const char* pOptions, const char* pathID)
{
    using Fn = void* (__thiscall*)(void*, const char*, const char*, const char*);
    return MEM::CallVFunc<void*, 13U>(this, pFileName, pOptions, pathID);
}

void CBaseFileSystem::Close(void* file)
{
    using Fn = void(__thiscall*)(void*, void*);
    return MEM::CallVFunc<void, 14U>(this, file);
}

unsigned int CBaseFileSystem::Size(void* file)
{
    using Fn = unsigned int(__thiscall*)(void*, void*);
    return MEM::CallVFunc<unsigned int, 18U>(this, file);
}

const char* CBaseFileSystem::unk(const char* file, std::uintptr_t* unk)
{
    return MEM::CallVFunc<const char*, 60U>(this, file, unk);
}

void CUIEngine::RunScript(CUIPanel* panel, const char* script) {
	if (!panel || !script)
		return;

	MEM::CallVFunc<void, 80>(this, panel, script, (const char*)nullptr, 0);
}

CUIEngine* CPanoramaUIEngine::AccessUIEngine() {
	return MEM::CallVFunc<CUIEngine*, 13>(this);
}

CPanel2D* CPanoramaUIEngine::GetMainMenuPanel() {
	return *reinterpret_cast<CPanel2D**>(MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 05 ? ? ? ? EB ? 48 8B 05 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 4C 8B 78 38"), 0x3));
}

void CPanoramaUIEngine::RunScriptOnMainMenuPanel(const char* script) {
	CUIEngine* engine = AccessUIEngine();
	CPanel2D* panel = GetMainMenuPanel();
	if (!engine || !panel || !script)
		return;

	engine->RunScript(panel->uiPanel, script);
}