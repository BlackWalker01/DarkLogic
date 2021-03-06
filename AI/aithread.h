#ifndef AITHREAD_H
#define AITHREAD_H
#include <thread>
#include <queue>
#include "event.h"
#include <condition_variable>
#include <vector>
#include <stack>
#include <atomic>
#include "Utils/action.h"
#include "dbaction.h"
#include "MainDarkLogic/utils.h"

class AI;

class AIThread;
class MasterAIThread;

void runThread(AIThread* thread);

class AIThread
{
public:
	friend void runThread(AIThread*);
	AIThread(const size_t& instanceId, AI& ai);

	void start();
	void stop();	
	bool hasStarted() const;
	void pushAction(const Id& action);
	bool mustStop();
	
	void computeActions();
	void updateLogic(const Id& actionId);
	unsigned char instanceId() const;

	~AIThread() = default;
private:
	void _start();
	void _stop();
	[[noreturn]] void _run();
	void _pushEvent(const unsigned char threadIdx_, Event::EventEnum type_);

	size_t m_instanceId;
	AI& m_ai;
	std::shared_ptr<MasterAIThread> m_master;
	bool m_hasStarted;
	std::thread m_thread;
	DbAction m_crtActions;

	//Handling events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::queue<Event> m_eventQueue;

	std::atomic<bool> m_mustStop;
};
#endif // !AITHREAD_H
