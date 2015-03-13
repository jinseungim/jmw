#pragma once


NS_JMW_BEGIN

class Layer;

class Scene : public Message
{
public:
	Scene(void);
	virtual ~Scene(void);


	static Scene* Create(void);
	static void RemoveLayer(Layer*);
	static void RemoveLayerById(Scene*, u64);

public:
	bl					m_IsPop;

	virtual void Draw(sl32);

	// 호출한 순서가 늦을수록 상위에 노출된다.
	u64		AddLayer(Layer*);
	bl		RemoveLayerFromScene(Layer*);
	bl		RemoveLayerFromSceneById(u64);
	bl		PopLayer(void);


	void	HandleTouch(s32, s32, f64, f64);

	bl		ProcessMessage(Layer*);

protected:

	std::vector<Layer*> m_Layers;

	void	RemoveAllLayer(void);
};


NS_JMW_END