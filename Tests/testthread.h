#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>
#include "AI/event.h"

#define TIMEOUT_MIN 20

class Test;
class TestThread;

void runTestThread(TestThread* thread);

class TestThread
{
public:
	friend void runTestThread(TestThread*);

	TestThread(Test& test_);

	void start();
	void stop();
	void forceStop();
	bool hasStarted() const;
	bool joinable();
	void join();

private:
	void _pushEvent(Event::EventEnum type_);
	void _run();
	template<typename AIType>
	void _start();
	void _stop();
	bool mustStop();

	//Attributes
	Test& m_test;
	bool m_hasStarted;
	std::thread m_thread;

	//Handling events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::queue<Event> m_eventQueue;
	std::atomic<bool> m_mustStop;
};
