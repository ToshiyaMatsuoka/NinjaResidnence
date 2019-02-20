/**
* @file SoundOperater.cpp
* @brief サウンドキーの管理
* @author Toshiya Matsuoka
*/
#include "SoundOperater.h"

using namespace SoundLib;

SoundOperater::SoundOperater()
{
	m_pSoundManager = new SoundsManager;
}

SoundOperater::~SoundOperater()
{
	m_SoundKey.clear();
	std::vector<SoundKey>().swap(m_SoundKey);
	delete m_pSoundManager;
	m_pSoundManager = NULL;
}

PlayingStatus SoundOperater::GetStatus(const char* pKey) const
{
	return m_pSoundManager->GetStatus(pKey);
}

uint8_t SoundOperater::GetVolume(const char* pKey) const
{
	return m_pSoundManager->GetVolume(pKey);
}

bool SoundOperater::SetVolume(const char* pKey, uint8_t volume)
{
	return m_pSoundManager->SetVolume(pKey, volume);
}

float SoundOperater::GetFrequencyRatio(const char* pKey) const
{
	return m_pSoundManager->GetFrequencyRatio(pKey);
}

bool SoundOperater::SetFrequencyRatio(const char* pKey, float ratio)
{
	return m_pSoundManager->SetFrequencyRatio(pKey, ratio);
}

bool SoundOperater::Initialize()
{
	return m_pSoundManager->Initialize();
}

bool SoundOperater::AddFile(const char* pFilePath, const char* pKey, SoundType type)
{
	bool SuccessAddFile = false;
	SuccessAddFile = m_pSoundManager->AddFile(pFilePath, pKey);
	if (SuccessAddFile) {
		m_SoundKeyBuff.Key = pKey;
		m_SoundKeyBuff.Type = type;
		m_SoundKey.push_back(m_SoundKeyBuff);
	}
	return SuccessAddFile;
}

bool SoundOperater::Start(const char* pKey, bool isLoopPlayback)
{
	return m_pSoundManager->Start(pKey,isLoopPlayback);
}

bool SoundOperater::Stop(const char* pKey)
{
	return m_pSoundManager->Stop(pKey);
}

bool SoundOperater::Pause(const char* pKey)
{
	return m_pSoundManager->Pause(pKey);
}

bool SoundOperater::Resume(const char* pKey)
{
	return m_pSoundManager->Resume(pKey);
}

bool SoundOperater::AllStop()
{
	bool SuccessAddFile = true;
	for (unsigned int i = 0; i < m_SoundKey.size(); ++i) {
		SuccessAddFile = m_pSoundManager->Stop(m_SoundKey[i].Key);
		Sleep(100);
	}	
	return SuccessAddFile;
}

bool SoundOperater::AllSetVolume(uint8_t volume)
{
	bool SuccessAddFile = true;
	for (unsigned int i = 0; i < m_SoundKey.size(); ++i) {

		SuccessAddFile = m_pSoundManager->SetVolume(m_SoundKey[i].Key, volume);
	}
	return SuccessAddFile;
}

bool SoundOperater::BGMSetVolume(uint8_t volume)
{
	bool SetSuccess = true;
	for (unsigned int i = 0; i < m_SoundKey.size(); ++i) {
		if (BGM == GetSoundType(i)) {
			SetSuccess = m_pSoundManager->SetVolume(m_SoundKey[i].Key, volume);
		}
	}
	return SetSuccess;
}

bool SoundOperater::SESetVolume(uint8_t volume)
{
	bool SetSuccess = true;
	for (unsigned int i = 0; i < m_SoundKey.size(); ++i) {
		if (SE == GetSoundType(i)) {
			SetSuccess = m_pSoundManager->SetVolume(m_SoundKey[i].Key, volume);
		}
	}
	return SetSuccess;
}
