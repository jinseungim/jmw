#include "jmw.h"
#include "Director.h"
#include "scene\Scene.h"

NS_JMW_BEGIN


Director *Director::pInstance = nullptr;

Director& Director::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new Director();

	return *pInstance;
}


void Director::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


Director::Director(void) : INIT_VAR(GameWidth, 1136), INIT_VAR(GameHeight, 640), INIT_VAR(DeviceWidth, GetGameWidth()), INIT_VAR(DeviceHeight, GetGameHeight()),
INIT_VAR(FactorX, 1.0f), INIT_VAR(FactorY, 1.0f)
{
}


Director::~Director(void)
{
	Scheduler::ReleaseInstance();
	TextureManager::ReleaseInstance();
	FontManager::ReleaseInstance();
}


void Director::Init(void)
{
	Scheduler::GetInstance().Init();
	FontManager::GetInstance().Init();
}

void Director::Resize(s32 width, s32 height)
{
	SetDeviceWidth(width);
	SetDeviceHeight(height);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// left, right, bottom, top
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	//glOrtho(0.0f, 1.0f, 0.0f, (GLfloat)height/width, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SetFactorX((f32)width / GetGameWidth());
	SetFactorY((f32)height / GetGameHeight());
}

void Director::Run(sl32 delta)
{
	Scene* scene = nullptr;

	try {
		scene = m_SceneVec.Top();


		// 스케쥴러가 등록되어있다면 실행한다.
		Scheduler::GetInstance().Run(delta);

		scene->Draw(delta);

		// PopScene 를 호출한 Scene에대해서 삭제를 해준다.
		if (scene->m_IsPop)
		{
			if(m_SceneVec.Pop(scene))
				delete scene;
		}

	}
	catch (...) {  LOG("No Scene");  }
}



void Director::PushScene(Scene* scene)
{
	ASSERT(scene != nullptr);

	m_SceneVec.Push(scene);
}


void Director::PopScene(void)
{
	if (m_SceneVec.Size() > 0)
	{
		try {
			Scene* s = m_SceneVec.Top();
			s->m_IsPop = true;
		}
		catch (std::string& ex)
		{
			LOG("%s", ex.c_str());
		}
	}
}



void Director::HandleTouch(s32 type, s32 touchId, f64 x, f64 y)
{
	Scene* scene = nullptr;
	try {
		scene = m_SceneVec.Top();

		y *= GetAspectRatio();
 		scene->HandleTouch(type, touchId, x/GetDeviceWidth(), y/GetDeviceHeight());
	}
	catch (std::string& ex)
	{ 
		LOG("%s", ex.c_str()); 
	}
}


NS_JMW_END
