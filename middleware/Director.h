#pragma once

NS_JMW_BEGIN

class Scene;

class Director
{
private:
	Director(void);
	~Director(void);

	static Director *pInstance;

	Stack<Scene*>    m_SceneVec;
	
	GETSET(s32, DeviceWidth);
	GETSET(s32, DeviceHeight);
	GETSET(s32, GameWidth);
	GETSET(s32, GameHeight);
	GETSET(f32, FactorX);
	GETSET(f32, FactorY);

public:
	static Director& GetInstance(void);
	static void ReleaseInstance(void);

	void Init(void);
	void Resize(s32 width, s32 height);
	void Run(sl32);

	void PushScene(Scene*);
	void PopScene(void);

	f64  GetAspectRatio(void) const { return (f64)GetDeviceHeight() / GetDeviceWidth(); }

	void SetScreenSize(s32 w, s32 h) {
		SetDeviceWidth(w);
		SetDeviceHeight(h);  
	}

	void HandleTouch(s32 type, s32 touchId, f64 x, f64 y);
};



#define GetDirector() Director::GetInstance()

NS_JMW_END
