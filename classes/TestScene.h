#pragma once


#include "jmw.h"
#include "Scene.h"

USING_NS_JMW;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();


	void Draw(sl32);


	void ScenePopTest(void*, void*, sl32);
};

