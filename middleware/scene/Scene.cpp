#include "jmw.h"
#include "Scene.h"

NS_JMW_BEGIN

Scene::Scene(void) : m_IsPop(false)
{
}


Scene::~Scene(void)
{
	RemoveAllLayer();
}


Scene* Scene::Create(void)
{
	return new Scene();
}


void Scene::RemoveLayer(Layer* layer)
{
	ASSERT(layer != nullptr);

	layer->GetParent()->RemoveLayerFromScene(layer);
}


void Scene::RemoveLayerById(Scene* parent, u64 id)
{
	ASSERT(parent != nullptr);

	parent->RemoveLayerFromSceneById(id);
}

void Scene::Draw(sl32 delta)  
{
	bl delMsg = false;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto &l : m_Layers)
	{
		if (l->GetVisible())
		{
			delMsg = ProcessMessage(l);
			l->SortChaild();
			l->OnCalc(delta);
			l->OnDraw(delta);
		}
	}

	// layer들중에서 해당메세지를 아무도 처리하지 않았다면 메세지를 폐기한다.
	if (delMsg == false)
	{
		Msg* msg = PopMag();
		SAFE_DELETE_NULL(&msg);
	}
}


u64 Scene::AddLayer(Layer* layer)
{
	ASSERT(layer != nullptr);

	layer->SetParent(this);
	layer->SetPos(Point());
	layer->SetSize(Size(GetDirector().GetDeviceWidth(), GetDirector().GetDeviceHeight()));
	layer->OnEnter();
	m_Layers.push_back(layer);
	
	return layer->GetID();
}

bl Scene::RemoveLayerFromScene(Layer* layer)
{
	ASSERT(layer != nullptr);

	bl isRemove = false;

	for (auto iter = m_Layers.begin(); iter != m_Layers.end();){
		if (*iter == layer) {

			(*iter)->OnExit();
			SAFE_RELEASE(*iter);
			
			iter = m_Layers.erase(iter);
			isRemove = true;
			
			break;
		}
		else
			++iter;
	}

	return isRemove;
}


bl Scene::RemoveLayerFromSceneById(u64 id)
{
	bl isRemove = false;

	for (auto iter = m_Layers.begin(); iter != m_Layers.end();){
		if ((*iter)->GetID() == id) {

			(*iter)->OnExit();
			SAFE_RELEASE(*iter);

			iter = m_Layers.erase(iter);
			isRemove = true;

			break;
		}
		else
			++iter;
	}

	return isRemove;
}


bl Scene::PopLayer(void)
{
	bl isRemove = false;
	if (m_Layers.size() > 0)
	{
		Layer* l = m_Layers.back();
		l->OnExit();
		SAFE_RELEASE(l);
		m_Layers.pop_back();

		isRemove = true;
	}

	return isRemove;
}


void Scene::RemoveAllLayer(void)
{
	for (auto iter = m_Layers.begin(); iter != m_Layers.end();){

		(*iter)->OnExit();
		SAFE_RELEASE(*iter);
		
		iter = m_Layers.erase(iter);
	}
}



void Scene::HandleTouch(s32 type, s32 touchId, f64 x, f64 y)
{
	TouchPoint* msg = dynamic_cast<TouchPoint*>(MakeMsg(TOUCH_COMMAND, new TouchPoint));
	if (msg) {
		msg->kindOf = type;
		msg->touchId = touchId;
		msg->pos.xpos = x;
		msg->pos.ypos = y;

		PutMsg(msg);
	}
	else
	{
		throw("Error to make message.");
	}
}


bl Scene::ProcessMessage(Layer* layer)
{
	bl ret = false;
	
	// 메세지만 살짝 가져온다.
	Msg* msg = PeekMsg();
	if (msg != nullptr)
	{
		if (layer->GetType() == POPUP)
		{
			// 팝업이 메세지 우선순위를 가지며 팝업이 처리하든 안하든 무조건 폐기한다.
			layer->OnCommand(msg);
			ret = msg->isDel = true;
		}
		else
		{
			ret = msg->isDel = layer->OnCommand(msg); // 해당레이어에서 메세지를 처리해서 true가 리턴된다면
		}

		// 메세지큐에서 메세지를 제거후 메모리해제를 한다.
		// 이부분을 잘활용하면 여러개의 layer들중에서 서로 처리할 메세지에 우선순위를 가지고 해결할수 있다.
		// 맨위 layer를 UI로 하고 그 밑layer을 게임화면으로 구성할경우
		// 맨위 layer에서 필요한 메세지에서 true를 리턴하면 그 밑에 있는 레이어로는 메세지가 가지 않는다.
		if (msg->isDel)
		{
			PopMag();
			SAFE_DELETE_NULL(&msg);
		}
	}

	return ret;
}

NS_JMW_END