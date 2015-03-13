#pragma once

NS_JMW_BEGIN

class Node : public Object
{
private:
	Point m_Pos;
	Size  m_Size;
	bl    m_Visible;

	s32   m_Zorder;

	// 텍스처를 그릴때 GL_COLOR_ARRAY를 사용할지 설정한다.
	GETSET(bl, EnableColor);
	// 기준점적용(0,0 : 좌측하단, 0,1 : 좌측상단, 1,0 : 우측하단, 1,1 : 우측상단, 0.5,0.5 : 가운데 
	GETSET(Point, AnchorPoint);

	GLfloat m_Color[4][4];
public:
	Node(void);
	virtual ~Node(void);

	void SetVisible(bl v) { m_Visible = v; }
	bl   GetVisible(void) const { return m_Visible; }

	void SetZorder(s32 zorder) { m_Zorder = zorder; }
	s32  GetZorder(void) const { return m_Zorder; }

	void SetPos(const Point&);
	void SetSize(const Size&);

	f64  GetPosX(void) const;
	f64  GetPosY(void) const;
	f64  GetW(void) const;
	f64  GetH(void) const;


	bl   Collision(const Point&);

	void SetColor(u32);
	void SetColor(u32, u32, u32, u32);

};

NS_JMW_END
