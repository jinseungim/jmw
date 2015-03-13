#include "jmw.h"
#include "Scheduler.h"
#include "RunTime.h"

NS_JMW_BEGIN


Scheduler *Scheduler::pInstance = nullptr;

Scheduler& Scheduler::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new Scheduler();

	return *pInstance;
}


void Scheduler::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


Scheduler::Scheduler(void)
{
    //ctor
}

Scheduler::~Scheduler(void)
{
    //dtor
	RemoveAllSchedule();

}


void Scheduler::Init(void)
{
}


ScheduleData* Scheduler::InsertSchedule(std::function<void(void*, void*, sl32)> f, void* param, s32 repeat, f64 time)
{
	ScheduleData* s = new ScheduleData;
	s->time = time < 0 ? 0 : time;
	s->func = f;
	s->delta = 0;
	s->param = param;
	s->repeat = repeat;

	m_ScheduleList.push_back(s);

	return s;
}


void Scheduler::Run(sl32 delta)
{
	auto iter = m_ScheduleList.begin();
	while (iter != m_ScheduleList.end())
	{
		sl32 duration = 0;
		ScheduleData *tsd = *iter;
		ScheduleData &s   = *tsd;


		s.delta += delta;
		duration = s.time * (f64)MILLISEC;

		
		if (duration <= s.delta)
		{
			if (s.repeat != SCHEDULE_END && s.func)
				(s.func)(tsd, s.param, delta);


			if (s.repeat >= SCHEDULE_LOOP) {


                // 스케쥴러가 종료되어야할 시점이면 리스트에서 제거한다.
				if (s.repeat == SCHEDULE_END) {

					delete *iter;
					iter = m_ScheduleList.erase(iter);

					continue;
				}
				else {

					s.repeat--;
					if (s.repeat<SCHEDULE_END) s.repeat = SCHEDULE_END;

					s.delta -= duration;
				}
			}
			else if (s.time >= 0.0) {
				s.delta -= duration;

			}

			iter++;
		}
		else
			iter++;
	}
}



bl Scheduler::RemoveSchedule(ScheduleData* tsd)
{
	auto iter = m_ScheduleList.begin();
	for (; iter != m_ScheduleList.end(); ++iter)
	{
		ScheduleData* tmp = *iter;

		if (tsd == tmp)
		{
			tsd->repeat = SCHEDULE_END;
			return true;
		}

	}

	return false;
}



void Scheduler::RemoveAllSchedule(void)
{
	for (auto iter = m_ScheduleList.begin(); iter != m_ScheduleList.end();)
	{
		auto &tmp = *iter;
		delete tmp;
		iter = m_ScheduleList.erase(iter);

	}

	m_ScheduleList.clear();
}

NS_JMW_END
