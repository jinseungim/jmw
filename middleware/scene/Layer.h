#pragma once

NS_JMW_BEGIN


typedef enum
{
	VIEW,
	POPUP
}Layer_Type;

class Layer : public Drawable
{
public:
	Layer(void);
	~Layer(void);


	virtual void OnEnter(void);
	virtual void OnExit(void);
	virtual void OnCalc(sl32);
	virtual void OnDraw(sl32);
	virtual bl   OnCommand(Msg*);


	void SetParent(Scene*);
	Scene* GetParent(void) const;

	void SortChaild(void);

	Layer_Type GetType(void) const { return m_Type; }
	void SetType(Layer_Type t) { m_Type = t;  }

protected:
	Scene* m_Parent;
	std::list< Drawable* >	m_Childs;
	Layer_Type m_Type;

	u64 AddChild(Drawable* elem, s32 zorder = 0);
	void RemoveAllChild(void);
	bl RemoveChildByID(u64);


	class Ascsort
	{
	public:
		bool operator()(const Drawable* a, const Drawable* b) const
		{
			return a->GetZorder() < b->GetZorder();
		}
	};


	class Descsort
	{
	public:
		bool operator()(const Drawable* a, const Drawable* b) const
		{
			return a->GetZorder() > b->GetZorder();
		}
	};


};


NS_JMW_END