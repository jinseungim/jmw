#include "jmw.h"
#include "Layer.h"


NS_JMW_BEGIN

Layer::Layer(void) : m_Parent(nullptr), m_Type(VIEW)
{
}


Layer::~Layer(void)
{
	RemoveAllChild();
}


void Layer::OnEnter(void)
{
	Drawable::OnEnter();
}


void Layer::OnExit(void)
{
	Drawable::OnExit();
}


void Layer::SortChaild(void)
{
	if (m_Childs.size() > 0)
		m_Childs.sort(Ascsort());
}

void Layer::OnCalc(sl32 delta)
{

	Drawable::OnCalc(delta);

	for (auto &c : m_Childs)
	{
		if (c->GetVisible())
			c->OnCalc(delta);
	}
}

void Layer::OnDraw(sl32 delta)
{
	Drawable::OnDraw(delta);

	for (auto &c : m_Childs)
	{
		if (c->GetVisible())
			c->OnDraw(delta);
	}

}


bl Layer::OnCommand(Msg* msg)
{
	bl ret = false;

	do {
		if (Drawable::OnCommand(msg) == true)
			ret = true;
		else
		{
			if (msg->what == TOUCH_COMMAND)
			{
				TouchPoint* touch = dynamic_cast<TouchPoint*>(msg);
				if (touch) {

					// 맨위에 있는 차일드부터 터치가 먹히게 정렬한다.
					if (m_Childs.size() > 0)
						m_Childs.sort(Descsort());

					for (auto &c : m_Childs)
					{
						if (c->GetVisible() && c->Collision(touch->pos))
						{
							if (TOUCH_UP == touch->kindOf)
								ret = c->OnTouchUp(touch->pos);
							else if (TOUCH_DOWN == touch->kindOf)
								ret = c->OnTouchDown(touch->pos);
							else if (TOUCH_MOVE == touch->kindOf)
								ret = c->OnTouchMove(touch->pos);

							LOG("kindof: %d touch object: %lld", touch->kindOf, c->GetID());
							break;
						}
					}
				}
			}
		}
	} while (0);

	return ret;
}


void Layer::SetParent(Scene* parent)
{
	ASSERT(parent != nullptr);

	m_Parent = parent;
}


Scene* Layer::GetParent(void) const
{
	return m_Parent;
}


u64 Layer::AddChild(Drawable* elem, s32 zorder)
{
	ASSERT(elem != nullptr);

	elem->SetZorder(zorder);
	elem->OnEnter();
	m_Childs.push_back(elem);
	return elem->GetID();
}


void Layer::RemoveAllChild(void)
{

	for (auto iter = m_Childs.begin(); iter != m_Childs.end();)
	{
		(*iter)->OnExit();
		(*iter)->Release();

		iter = m_Childs.erase(iter);

	}

	m_Childs.clear();
}

bl Layer::RemoveChildByID(u64 id)
{
	bl isRemove = false;
	for (auto iter = m_Childs.begin(); iter != m_Childs.end();)
	{

		if ((*iter)->GetID() == id)
		{
			(*iter)->OnExit();
			(*iter)->Release();

			iter = m_Childs.erase(iter);
		}
		else
			++iter;

	}


	return isRemove;
}




NS_JMW_END
