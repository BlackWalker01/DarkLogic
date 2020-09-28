#ifndef MASTERAITHREAD_H
#define MASTERAITHREAD_H
#include "aithread.h"
#include <unordered_map>

class MasterAIThread;

void runMaster(MasterAIThread* masterThread);

/**
* MasterAIThread class
* thread of AI
* handle slave threads of AI
*/
class MasterAIThread
{
public:
	friend void runMaster(MasterAIThread*);
	MasterAIThread(const size_t& maxInstanceIdx_, AI& ai_);

	void computeActions();
	void updateLogic(const size_t& actionid);

	bool mustStop(const unsigned char threadIdx) const;
	void start();
	void stop();
	void stopFromThread(const unsigned char threadIdx);

	void stopThread(const unsigned char threadIdx);

	void init();

private:
	void _start();
	void _stop();
	void _stopFromThread(const unsigned char threadIdx);
	[[noreturn]] void _run();
	void _pushEvent(const unsigned char threadIdx_, Event::EventEnum type_);

	//Attributes
	AI& m_ai;
	const size_t m_maxInstanceIdx;
	std::vector<std::unique_ptr<AIThread>> m_slaveThreads;
	std::unordered_map<unsigned char,unsigned char> m_threadAlive;
	
	//Handling events
	bool m_hasEvents;
	std::thread m_thread;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::queue<Event> m_eventQueue;
};

#endif // !MASTERAITHREAD_H

