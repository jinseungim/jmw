#pragma once


NS_JMW_BEGIN

class AppMain
{
private:
	AppMain(void);
	~AppMain(void);

	static AppMain *pInstance;


	Application		*m_App;
	bool			 m_firstRun;


public:
	static AppMain& GetInstance(void);
	static void ReleaseInstance(void);

	bl   Init(Application*);
	void Run(void);
	void Resize(s32, s32);

	void HandleTouch(s32, s32, f64, f64);
};

NS_JMW_END
