#pragma once

NS_JMW_BEGIN

class TextureManager
{

	map<string, shared_ptr<Image>> m_TexMng;
private:
	TextureManager();
	~TextureManager();

	static TextureManager* pInstance;


public:
	shared_ptr<Image> GetImage(const string);
	shared_ptr<Image> GetImageFromRes(const string);

	shared_ptr<Image> LoadPng(const string);
	

	static TextureManager& GetInstance(void);
	static void ReleaseInstance(void);

};

#define GetTexManager() TextureManager::GetInstance()

NS_JMW_END