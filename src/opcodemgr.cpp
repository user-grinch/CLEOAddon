#include "pch.h"
#include "opcodemgr.h"
#include "browser.h"
#include "sdk/CVector.h"
#include "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\d3dx9math.h"

static HandlerResult BrowserOpenLink(Context ctx)
{
	char buf[STR_MAX_LEN];
	GetStringParam(ctx, buf, STR_MAX_LEN);
	Browser::OpenLink(buf);
	return HandlerResult::CONTINUE;
}

static HandlerResult BrowserDownloadLink(Context ctx)
{
	char buf[STR_MAX_LEN];
	GetStringParam(ctx, buf, STR_MAX_LEN);
	cHandle handle = Browser::AddDownloadRequest(buf);
	SetIntParam(ctx, handle);
	return HandlerResult::CONTINUE;
}

static HandlerResult BrowserGetDownloadStatus(Context ctx)
{
	cHandle handle = GetIntParam(ctx);
	int state = static_cast<int>(Browser::GetDownloadRequestState(handle));
	SetIntParam(ctx, state);
	return HandlerResult::CONTINUE;
}

// Taken from MTA
CVector ScreenToWorld(CVector vecScreen)
{
    CVector vecWorld;
    // Get the static view matrix as D3DXMATRIX
    D3DXMATRIXA16 m((float*)(0xB6FA2C));

    // Invert the view matrix
    D3DXMATRIXA16 minv;
    memset(&minv, 0, sizeof(D3DXMATRIXA16));
    m._44 = 1.0f;
    D3DXMatrixInverse(&minv, NULL, &m);

    DWORD* dwLenX = (DWORD*)(0xC17044);
    DWORD* dwLenY = (DWORD*)(0xC17048);

    // Reverse screen coordinates
    float fRecip = 1.0f / vecScreen.z;
    vecScreen.x /= fRecip * (*dwLenX);
    vecScreen.y /= fRecip * (*dwLenY);

    // Do an (inverse) transformation
    vecWorld.x = vecScreen.z * minv._31 + vecScreen.y * minv._21 + vecScreen.x * minv._11 + minv._41;
    vecWorld.y = vecScreen.z * minv._32 + vecScreen.y * minv._22 + vecScreen.x * minv._12 + minv._42;
    vecWorld.z = vecScreen.z * minv._33 + vecScreen.y * minv._23 + vecScreen.x * minv._13 + minv._43;

    return vecWorld;
}

static HandlerResult ConvertScreenToWorld(Context ctx)
{
    CVector screen;
	screen.x = GetFloatParam(ctx);
    screen.y = GetFloatParam(ctx);
    screen.z = GetFloatParam(ctx);

    CVector world = ScreenToWorld(screen);

	SetFloatParam(ctx, world.x);
    SetFloatParam(ctx, world.y);
    SetFloatParam(ctx, world.z);
	return HandlerResult::CONTINUE;
}

// Taken from MTA
CVector WorldToScreen(CVector vecWorld)
{
    CVector vecScreen;

    D3DXMATRIX m((float*)(0xB6FA2C));

    DWORD* dwLenX = (DWORD*)(0xC17044);
    DWORD* dwLenY = (DWORD*)(0xC17048);

    vecScreen.x = (vecWorld.z * m._31) + (vecWorld.y * m._21) + (vecWorld.x * m._11) + m._41;
    vecScreen.y = (vecWorld.z * m._32) + (vecWorld.y * m._22) + (vecWorld.x * m._12) + m._42;
    vecScreen.z = (vecWorld.z * m._33) + (vecWorld.y * m._23) + (vecWorld.x * m._13) + m._43;

    double fRecip = (double)1.0 / vecScreen.z;
    vecScreen.x *= (float)(fRecip * (*dwLenX));
    vecScreen.y *= (float)(fRecip * (*dwLenY));

    return vecScreen;
}

static HandlerResult ConvertWorldToScreen(Context ctx)
{
    CVector world;
	world.x = GetFloatParam(ctx);
    world.y = GetFloatParam(ctx);
    world.z = GetFloatParam(ctx);

    CVector screen = WorldToScreen(world);

	SetFloatParam(ctx, screen.x);
    SetFloatParam(ctx, screen.y);
    SetFloatParam(ctx, screen.z);
	return HandlerResult::CONTINUE;
}

void OpcodeMgr::RegisterCommands()
{
	// Browser opcodes
	RegisterCommand("BROWSER_OPEN_LINK", BrowserOpenLink);
	RegisterCommand("BROWSER_DOWNLOAD_FILE", BrowserDownloadLink);
	RegisterCommand("BROWSER_GET_DOWNLOAD_STATUS", BrowserGetDownloadStatus);
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&Browser::HandleDownloadRequests, nullptr, NULL, nullptr);
    RegisterCommand("CONVERT_SCREEN_TO_WORLD", ConvertScreenToWorld);
    RegisterCommand("CONVERT_WORLD_TO_SCREEN", ConvertWorldToScreen);
}
