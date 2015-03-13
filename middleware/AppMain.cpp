#include "jmw.h"
#include "AppMain.h"


NS_JMW_BEGIN

AppMain *AppMain::pInstance = nullptr;

AppMain::AppMain(void) : m_App(nullptr), m_firstRun(true)
{

}


AppMain::~AppMain(void)
{
    RunTime::ReleaseInstance();
	Director::ReleaseInstance();

	m_App = nullptr;
	LOG("AppMain Exit!!!");
}



AppMain& AppMain::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new AppMain();

	return *pInstance;
}


void AppMain::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


bl AppMain::Init(Application* app)
{
	// 이미 초기화가 되어있는데 또 초기화를 시도한다.
	if (m_App != nullptr) return true;

	ASSERT(app != NULL);

    LOG("AppMain Start!!!");

	RunTime::GetInstance().Init();
	GetDirector().Init();

	m_App = app;
	return true;

}

void AppMain::Run(void)
{

	if (m_firstRun && m_App)
	{
		m_App->AppDidFinishLaunching();
		m_firstRun = false;
	}

	sl32 delta = RunTime::GetInstance().GetDelta();

    // 게임루프
	GetDirector().Run(delta);

}


void AppMain::Resize(s32 width, s32 height)
{
	GetDirector().Resize(width, height);
}


void AppMain::HandleTouch(s32 type, s32 touchId, f64 x, f64 y)
{
	GetDirector().HandleTouch(type, touchId, x, y);
}


NS_JMW_END
