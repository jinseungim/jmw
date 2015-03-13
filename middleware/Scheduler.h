#pragma once


NS_JMW_BEGIN


#define SCHEDULE_LOOP	 (-1)
#define SCHEDULE_END	 (0)

#define EXECUTE_SCHEDULE(f, p, r, d)   Scheduler::GetInstance().InsertSchedule(f, p , r, d)
#define REMOVE_SCHEDULE(s)             Scheduler::GetInstance().RemoveSchedule(s)
#define EXECUTE_CMSchedule(f, p, r, d) Scheduler::GetInstance().InsertSchedule(std::bind(&f, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), p , r, d)

//
// 람다로 호출하는 방식
// EXECUTE_SCHEDULE([](void*me, void*param, long delta){ LOG("ramda call"); }, nullptr, 4, 1);
//
// 일반함수로 호출하는방식
// EXECUTE_SCHEDULE(call , nullptr, 4, 1);
//
// 클래스의 멤버함수로 호출하는 방식
// EXECUTE_SCHEDULE(std::bind(&AppMain::Acall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), (void*)30, 4, 1);
//


typedef struct _tagScheduleData
{
	std::function<void (void*,void*,sl32)>   func;
	f64									     time;
	sl32									 delta;
	void*									 param;
	s32										 repeat;

} ScheduleData;




class Scheduler
{
private:
	Scheduler(void);
	~Scheduler(void);

	static Scheduler *pInstance;


	std::list<ScheduleData*> m_ScheduleList;

private:
	void RemoveAllSchedule(void);

public:
	static Scheduler& GetInstance(void);
	static void ReleaseInstance(void);

	void Init();

	//
	// ScheduleData*  InsertSchedule(SCHEDULE_CALL f, void* param, int repeat, double time)
	//			  일정시간이 되면 등록된 f함수가 호출된다.
	//
	// f		: time시간이 되면 호출될 콜백함수
	// param	: f이 호출될때 넘겨줄 변수
	// repeat	: 몇번호출될것인지 지정
	//			  repeat값이 0이면 f 함수는 호출되지 않으며 리스트에서 사라진다.
	//			  repeat값이 -이면 무한히 반복된다.
	// time		: f를 호출할 시간 간격
	//			  time값이 0이거나 -값이면 시간간격이 없이 repeat만큼 호출된다.
	//			  time값은 초이며 double값이므로 1.2초, 1.5초 이런식으로 지정하면 된다.
	// 리턴값	: 정상적으로 등록된 스케쥴의 포인터가 리턴된다.
	//		      f값이 nullptr 이면 nullptr을 리턴한다.
	ScheduleData*  InsertSchedule(std::function<void(void*, void*, sl32)>, void*, s32, f64);

	//
	// bool RemoveSchedule(ScheduleData* sd)
	//            스케쥴을 삭제할때 사용되며 repeat값을 SCHEDULE_END값으로 셋팅해서 다음 호출될때 f를 호출하지 않고
	//            m_ScheduleList에서 삭제되도록 한다.
	//
	// sd		: 삭제할 스케쥴
	// 리턴값	: true 이면 삭제성공, false 등록된 스케쥴이 아니거나 없을때의 값
	bl		       RemoveSchedule(ScheduleData*);

	//
	// void Run(long delta)
	//            등록된 스케쥴을 실행시킨다.
	//
	// delta	: AppMain에서 Run함수에서 루프가 한바퀴돈 델타값
	//
	void		   Run(sl32);

};

NS_JMW_END

