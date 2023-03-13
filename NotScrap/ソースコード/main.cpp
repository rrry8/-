#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include"class/Scene/SceneManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	lpSceneMng.Run();

	Effkseer_End();
	DxLib_End();

	return 0;
}
