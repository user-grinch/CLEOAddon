#include "pch.h"
#include "opcodemgr.h"

BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
    if (nReason == DLL_PROCESS_ATTACH)
    {
		HostId id = GetHostId();
		if (id == HostId::SA)
		{
			OpcodeMgr::RegisterCommands();
		}
    }

    return TRUE;
}
