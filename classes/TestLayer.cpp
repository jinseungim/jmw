#include "TestLayer.h"

TestLayer::TestLayer()
{

}


TestLayer::~TestLayer()
{
}


void TestLayer::OnEnter()
{
	//SetCmdListener(SET_CMDLISTENER(TestLayer::MyCmd));
	PrintClassName();

	
	Drawable *s0 = Sprite::Create("Penguins.png");
	s0->SetPos(Point(0, 0));
	s0->SetTouchDownListener(SET_TOUCHLISTENER(TestLayer::Sprite0TouchDown));
	s0->SetColor(ABGR(255, 128, 0, 255));
	AddChild(s0, 0);

	s0 = Sprite::Create("gl.png");
	s0->SetAnchorPoint(Point(0, 0));
	s0->SetPos(Point(0, 0));
	AddChild(s0, 1);
	
	s0 = Text::Create("ttt.txt", 1);
	s0->SetAnchorPoint(Point(0, 0));
	s0->SetPos(Point(0, 0));
	AddChild(s0, 2);

}

bl TestLayer::Sprite0TouchDown(Point pos)
{
	LOG("Penguins Touch\n");
	return true;
}


void TestLayer::OnDraw(sl32 delta)
{
	//glClearColor(1.f, 1.f, 1.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT);

	Layer::OnDraw(delta);
}
/*
// 모든메세지를 받을경우
bl TestLayer::MyCmd(Msg* msg)
{

	if (msg->what == TOUCH_COMMAND)
	{
		TouchPoint* touch = dynamic_cast<TouchPoint*>(msg);
		if (touch) {

			if (TOUCH_DOWN == touch->kindOf)
			{
				// 맨위에 있는 차일드부터 터치가 먹히게 정렬한다.
				if (m_Childs.size() > 0)
					m_Childs.sort(Descsort());

				for (auto &c : m_Childs)
				{
					if (c->GetVisible())
					{
						if (c->Collision(touch->pos) == true)
						{
							return true;
						}
					}
				}
			}
		}
	}


	return true;
}
*/
