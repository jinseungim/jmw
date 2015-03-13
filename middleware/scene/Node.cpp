#include "jmw.h"
#include "Node.h"

NS_JMW_BEGIN

Node::Node(void) : m_Visible(true), m_Zorder(0), INIT_ZERO(EnableColor), INIT_VAR(AnchorPoint, Point(0.5f, 0.5f))
{
	memset(m_Color, 0, sizeof(m_Color));
}


Node::~Node(void)
{
}


void Node::SetPos(const Point& pos)
{
	
	m_Pos.xpos = (pos.xpos*GetDirector().GetFactorX()) / GetDirector().GetDeviceWidth();
	m_Pos.ypos = (pos.ypos*GetDirector().GetFactorY()) / GetDirector().GetDeviceHeight();
}

void Node::SetSize(const Size& size)
{
	m_Size.width = (size.width*GetDirector().GetFactorX()) / GetDirector().GetDeviceWidth();
	m_Size.height = (size.height*GetDirector().GetFactorY()) / GetDirector().GetDeviceHeight();
}

f64  Node::GetPosX(void) const
{
	return m_Pos.xpos;
}


f64  Node::GetPosY(void) const
{
	return m_Pos.ypos;
}
f64  Node::GetW(void) const
{
	return m_Size.width;
}

f64  Node::GetH(void) const
{
	return m_Size.height;
}

bl  Node::Collision(const Point& point)
{
	Point p(m_Pos.xpos - GetW()*GetAnchorPoint().xpos, m_Pos.ypos - GetH()*GetAnchorPoint().ypos);
	Rect area(p, Size(p.xpos + GetW(), p.ypos + GetH()));

	return area.Collision(point);
}


void Node::SetColor(u32 color)
{
	GLfloat R = GETR(color) / 255.f;
	GLfloat G = GETG(color) / 255.f;
	GLfloat B = GETB(color) / 255.f;
	GLfloat A = GETALPAH(color) / 255.f;

	m_Color[0][0] = m_Color[1][0] = m_Color[2][0] = m_Color[3][0] = R;
	m_Color[0][1] = m_Color[1][1] = m_Color[2][1] = m_Color[3][1] = G;
	m_Color[0][2] = m_Color[1][2] = m_Color[2][2] = m_Color[3][2] = B;
	m_Color[0][3] = m_Color[1][3] = m_Color[2][3] = m_Color[3][3] = A;

	SetEnableColor(true);
	
}


void Node::SetColor(u32 c1, u32 c2, u32 c3, u32 c4)
{
	GLfloat R = GETR(c1) / 255.f;
	GLfloat G = GETG(c1) / 255.f;
	GLfloat B = GETB(c1) / 255.f;
	GLfloat A = GETALPAH(c1) / 255.f;

	m_Color[0][0] = R;
	m_Color[0][1] = G;
	m_Color[0][2] = B;
	m_Color[0][3] = A;


	R = GETR(c2) / 255.f;
	G = GETG(c2) / 255.f;
	B = GETB(c2) / 255.f;
	A = GETALPAH(c2) / 255.f;

	m_Color[1][0] = R;
	m_Color[1][1] = G;
	m_Color[1][2] = B;
	m_Color[1][3] = A;

	R = GETR(c3) / 255.f;
	G = GETG(c3) / 255.f;
	B = GETB(c3) / 255.f;
	A = GETALPAH(c3) / 255.f;

	m_Color[2][0] = R;
	m_Color[2][1] = G;
	m_Color[2][2] = B;
	m_Color[2][3] = A;

	R = GETR(c4) / 255.f;
	G = GETG(c4) / 255.f;
	B = GETB(c4) / 255.f;
	A = GETALPAH(c4) / 255.f;

	m_Color[3][0] = R;
	m_Color[3][1] = G;
	m_Color[3][2] = B;
	m_Color[3][3] = A;


	SetEnableColor(true);
}

NS_JMW_END
