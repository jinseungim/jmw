#include "jmw.h"
#include "Rect.h"
NS_JMW_BEGIN


bl Rect::Collision(const Point& point)
{
	return m_Pos.xpos <= point.xpos && m_Size.width >= point.xpos && m_Pos.ypos <= point.ypos && m_Size.height >= point.ypos;
}

NS_JMW_END