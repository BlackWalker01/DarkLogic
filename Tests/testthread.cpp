#include <iostream>
#include "testthread.h"
#include "DarkLogic/darklogic.h"
#include "test.h"
#include "AI/evalai.h"
#include "MainDarkLogic/action.h"


TestThread::TestThread(Test& test_):
	m_test(test_), m_hasStarted(false), m_thread(runTestThread, this), m_hasEvents(false), m_lock(m_mutex)
{

}


void TestThread::start()
{
	m_mustStop = false;
	_pushEvent(Event::EventEnum::START);
}

void TestThread::stop()
{
	m_mustStop = true;
	//_pushEvent(Event::EventEnum::STOP);
}

void TestThread::forceStop()
{
	m_mustStop = true;
	_pushEvent(Event::EventEnum::STOP);
}

bool TestThread::hasStarted() const
{
	return m_hasStarted;
}

bool TestThread::joinable()
{
	return m_thread.joinable();
}

void TestThread::join()
{
	m_thread.join();
}

void TestThread::_pushEvent(Event::EventEnum type_)
{
	m_mutex.lock();
	m_eventQueue.push(Event(0, type_));
	m_mutex.unlock();
	m_hasEvents = true;
	m_condition_var.notify_all();
}

template<typename AIType>
void TestThread::_start()
{
	m_hasStarted = true;
	unsigned int nbThreads = std::thread::hardware_concurrency();
	double maxTime = (m_test.timeout() * 1.15) * (4.0 / nbThreads);
	double aiMaxTime = maxTime > TIMEOUT_MIN ? maxTime : TIMEOUT_MIN;
	auto ai = std::make_unique<AIType>(nbThreads, aiMaxTime);
	size_t numAttempt = 0;
	//std::cout << "TestThread::start, ai timeout is " << aiMaxTime << std::endl;
	while (!N_DarkLogic::DarkLogic::isOver())
	{
		std::cout << "Attempt numero " << numAttempt + 1 << std::endl;
		auto action = ai->play();
		N_DarkLogic::DarkLogic::getActions();
		std::cout << ai->name() << " plays action with id " << action->id() << std::endl;
		N_DarkLogic::DarkLogic::apply(action->id());
		N_DarkLogic::DarkLogic::printTheorem();
		std::cout << "____________________________________________________________________________" << std::endl;

		if (mustStop())
		{
			//s_instance->popEvent();
			break;
		}
		numAttempt++;
	}	

	_stop();
}

void TestThread::_stop()
{
	m_hasStarted = false;
	m_mustStop = false;
	m_test.pushEvent(Event::EventEnum::STOP);
}

bool TestThread::mustStop()
{
	return m_mustStop.load();
}


void TestThread::_run()
{
	while (true)
	{
		while (!m_hasEvents)
		{
			m_condition_var.wait(m_lock);
		}
		if (m_eventQueue.size())
		{
			//Consuming event
			switch (m_eventQueue.front().type())
			{
			case Event::EventEnum::START:
			{
				switch (m_test.playerType())
				{
				case Test::PlayerType::None:
					break;
				case Test::PlayerType::AI:
				{
					_start<AI>();
					break;
				}
				case Test::PlayerType::EvalAI:
				{
					_start<EvalAI>();
					break;
				}
				}
				return;
			}
			case Event::EventEnum::STOP:
			{
				_stop();
				return;
			}
			case Event::EventEnum::STOP_THREAD:
			{
				break;
			}
			}
			m_eventQueue.pop();
		}
		m_hasEvents = false;
	}
}

void runTestThread(TestThread* thread)
{
	thread->_run();
}
