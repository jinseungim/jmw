#include "jmw.h"
#include "AppDelegate.h"
#include "TestLayer.h"
#include "RedLayer.h"

USING_NS_JMW;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bl AppDelegate::AppDidFinishLaunching()
{
	
	Scene* scene = Scene::Create();
	scene->AddLayer(new TestLayer());
	scene->AddLayer(new RedLayer());
	GetDirector().PushScene(scene);

	Object::PrintCurGID();
	return true;
}