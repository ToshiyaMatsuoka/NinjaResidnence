/**
* @file SceneManager.cpp
* @brief SceneManagerクラス
* @author Kojiro Kawahara
*/
#include "SceneManager.h"
#include "TITLESCENE.h"
#include "STAGESELECTSCENE.h"
#include "GAMESCENE.h"
#include "VOLUMESELECTSCENE.h"

Scene*	SceneManager::m_pScene = NULL;

SceneManager::SceneManager(DirectX* pDirectX, SoundOperater* pSoundOperater)
	:m_CurrentScene(SCENE_NONE),m_NextScene(TITLE_SCENE)
{
	m_pDirectX = pDirectX;
	m_pSoundOperater = pSoundOperater;
	m_pVolumeSettingScene = new VOLUMESELECTSCENE(m_pDirectX, m_pSoundOperater);

	//ゲームシーンへショートカットする
	//m_NextScene = GAME_SCENE;
}

SceneManager::~SceneManager()
{
	m_pSoundOperater->AllStop();
	delete m_pScene;
	m_pScene = NULL;
	delete m_pVolumeSettingScene;
	m_pVolumeSettingScene = NULL;
}

int SceneManager::Update()
{

	KeyOperation();
	if (m_pScene->GetSoundSetting()) {
		
		TestTime++;
		m_pVolumeSettingScene->Update();
		
		if (m_pVolumeSettingScene->GetExitScene()) {
			m_pScene->InactiveSoundSetting();
			m_pVolumeSettingScene->InitExitScene();
		}
		return m_pScene->GetGameState();
	}
	if (m_CurrentScene != m_NextScene&& m_NextScene !=SCENE_NONE)
	{
		if (!isRunOnce) {
			delete m_pScene;
		}
		isThreadActive = true;
		switch (m_NextScene)
		{
		case TITLE_SCENE:
			m_pScene = new  TitleScene(m_pDirectX, m_pSoundOperater);
			break;
		case STAGESELECT_SCENE:
			m_pScene = new  StageSelectScene(m_pDirectX, m_pSoundOperater);
			break;
		case GAME_SCENE:
			m_pScene = new  GameScene(m_pDirectX, m_pSoundOperater);
			break;
		}
		m_NextScene = m_pScene->GetNextScene();
	}
	if (!isThreadActive) {
		m_NextScene = m_pScene->Update();
	}
	else LoadAction();

	return m_pScene->GetGameState();
}

void SceneManager::Render()
{
	if (m_pScene->GetSoundSetting()) {
		m_pVolumeSettingScene->Render();
		return;
	}

	if (!isThreadActive) {
		m_pScene->Render();
	}
	else {
		LoadAnimation();

	}
}
void SceneManager::LoadResouce()
{
	m_pScene->LoadResouce();
}
DWORD WINAPI SceneManager::Thread(LPVOID *data)
{
	if (!m_pScene) return 0;
	m_pScene->LoadResouce();
	Sleep(1);
	ExitThread(0);
}

void SceneManager::LoadAnimation() {
	m_pScene->LoadAnimation();
}

void SceneManager::KeyOperation()
{
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_0))
	{
		m_pSoundOperater->AllSetVolume(0);
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_9))
	{
	}

}

void SceneManager::LoadAction() {
	++LoadTime;
	
	if (!isRunOnce) {
	m_threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, m_pScene, 0, NULL);
	ResumeThread(m_threadHandle);
		isRunOnce = true;
		isThreadActive = true;
	}
	GetExitCodeThread(m_threadHandle, &m_threadResult);
	if (m_threadResult != STILL_ACTIVE && LoadTime >= 20) {
		CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
		isRunOnce = false;
		LoadTime = 0;
		isThreadActive = false;
		m_CurrentScene = m_NextScene;
		Sleep(100);
	}
}

