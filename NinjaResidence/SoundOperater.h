/**
* @file SoundOperater.h
* @brief サウンドキーの管理
* @author Toshiya Matsuoka
*/
#pragma once

#include <vector>
#include "SoundsManager.h"

using SoundLib::SoundsManager;

enum SoundType {
	TYPE_NONE,
	BGM,
	SE,
};

struct SoundKey {
	const char* Key;
	SoundType Type;
};

/**
* @brief 音声キーの管理クラス
* @sa SoundLib::SoundsManager
*/
class SoundOperater
{
public:
	SoundOperater();
	~SoundOperater();

	SoundLib::PlayingStatus GetStatus(const char* pKey) const;

	uint8_t GetVolume(const char* pKey) const;

	bool SetVolume(const char* pKey, uint8_t volume);

	float GetFrequencyRatio(const char* pKey) const;

	bool SetFrequencyRatio(const char* pKey, float ratio);

	bool Initialize();

	/**
	* @brief 音声ファイルの登録
	* @param pFilePath 
	* @param pKey 
	* @param type 
	*/
	bool AddFile(const char* pFilePath, const char* pKey, SoundType type);

	/**
	* @brief 音声の再生
	*/
	bool Start(const char* pKey, bool isLoopPlayback = false);

	/**
	* @brief 音声の停止
	* @sa SoundLib::Stop
	*/
	bool Stop(const char* pKey);

	/**
	* @brief 音声の一時停止
	*/
	bool Pause(const char* pKey);

	/**
	* @brief 一時停止中の音声を続きから再生
	*/
	bool Resume(const char* pKey);

	/**
	* @brief 登録されている音声の全停止
	*/
	bool AllStop();

	/**
	* @brief 登録されている音声すべての音量変更
	*/
	bool AllSetVolume(uint8_t volume);

	/**
	* @brief SoundTypeがBGMの音声の音量変更
	*/
	bool BGMSetVolume(uint8_t volume);

	/**
	* @brief SoundTypeがSEの音声の音量変更
	*/
	bool SESetVolume(uint8_t volume);
private:
	SoundsManager* m_pSoundManager = NULL;
	std::vector<SoundKey> m_SoundKey;

	SoundKey m_SoundKeyBuff;

	SoundType GetSoundType(int arrayNum) {
		return m_SoundKey[arrayNum].Type;
	}
};

