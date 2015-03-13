#pragma once


NS_JMW_BEGIN

class Drawable : public Node, public Touchable
{
protected:
	shared_ptr<Image> m_Img;
	static GLfloat m_vertices[36];


	CMD_LISTENER		m_CmdListener;
	TOUCH_LISTENER		m_TouchUpListener;
	TOUCH_LISTENER		m_TouchDownListener;
	TOUCH_LISTENER		m_TouchMoveListener;


	void CalcTex(void);
	void DrawTex(void);

public:
	Drawable(void);
	virtual ~Drawable(void);

	virtual void OnEnter(void) {};
	virtual void OnExit(void) {};
	virtual void OnCalc(sl32);
	virtual void OnDraw(sl32);
	virtual bl   OnCommand(Msg* msg); // true를 리턴하게 되면 메세지는 이후의 layer에게 전달되지 않고 삭제된다.

	virtual bl OnTouchUp(Point);
	virtual bl OnTouchDown(Point);
	virtual bl OnTouchMove(Point);
	


	void SetCmdListener(CMD_LISTENER listener) { m_CmdListener = listener; }

	void SetTouchUpListener(TOUCH_LISTENER listener) { m_TouchUpListener = listener;  }
	void SetTouchDownListener(TOUCH_LISTENER listener) { m_TouchDownListener = listener; }
	void SetTouchMoveListener(TOUCH_LISTENER listener) { m_TouchMoveListener = listener; }

};

NS_JMW_END