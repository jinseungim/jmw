#include "RedLayer.h"
#include "TestThread.h"


TestThread * pThread;

RedLayer::RedLayer()
{
	EXECUTE_SCHEDULE([this](void*me, void*param, long delta){
		Scene::RemoveLayer(this); 
	}, nullptr, 1, 2);

	//pThread = new TestThread();
	//pThread->Start(nullptr);
}


RedLayer::~RedLayer()
{
	//pThread->Release();
}


void RedLayer::OnDraw(sl32 delta)
{
	glClearColor(0.2f, 0.0f, 0.0f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT);
}
