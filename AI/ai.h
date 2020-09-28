#ifndef AI_H
#define AI_H
#include "MainDarkLogic/player.h"
#include "node.hpp"
#include "masteraithread.h"
#include "Utils/state.h"

class AI: public Player
{
public:
	friend class AIThread;
	friend class MasterAIThread;
	friend class Node;
	enum AIMode
	{
		BASIC,
		EVAL
	};

	AI(const size_t& maxInstanceIdx, const size_t& timeoutScds);
protected:
	AI(const std::string& name_, const size_t& maxInstanceIdx, const size_t timeoutScds);

public:
	std::shared_ptr<const Action> play() override;
	void meditate() override final;
	size_t timeout() const;

	~AI() = default;

protected:
	virtual void explore(DbAction& actions, unsigned char threadIdx);
	virtual unsigned short int eval(const std::vector<N_DarkLogic::State>& states, const unsigned char threadIdx);

	void pushCrtAction(const size_t& actionId, const unsigned char threadIdx) const;
	void stopFromMasterThread();
	
	bool mustStop(const unsigned char threadIdx) const;
	void stopThread(const unsigned char threadIdx) const;
	std::shared_ptr<MasterAIThread> getMaster() const;	

	virtual unsigned int value() const;
	unsigned short int getValueFromAction(const unsigned short int action);
	unsigned short int getRealValueFromAction(const unsigned short int action);

	void _pushEvent(Event::EventEnum type_);


	//Attributes
	const size_t m_secondTimeout;
	const std::shared_ptr<MasterAIThread> m_masterThread;
	std::unique_ptr<Node> m_crtNode;
	
	//handle events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::queue<Event> m_eventQueue;
};

#endif
