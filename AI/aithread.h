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
	void pushAction(const size_t& action);
	bool mustStop();
	
	void computeActions();
	void updateLogic(const size_t& actionId);
	unsigned char instanceId() const;

	void setRootNbSimu(const size_t& nbSimu);
	size_t getRootNbSimu() const;
	void incrRootNbSimu();

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
	std::vector<N_DarkLogic::Action::Id> m_crtActions;
	size_t m_rootNbSimu;

	//Handling events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::queue<Event> m_eventQueue;

	std::atomic<bool> m_mustStop;
};
#endif // !AITHREAD_H
