#ifndef AI_H
#define AI_H
#include "MainDarkLogic/player.h"
#include "node.hpp"
#include "masteraithread.h"

class AI: public Player
{
public:
	friend class AIThread;
	friend class MasterAIThread;
	friend class Node;
	enum AIMode
	{
		MCTS,
		DEEP
	};

	AI(const AIMode type_, const size_t& maxInstanceIdx);

	std::shared_ptr<const Action> play() override;
	AIMode type() const;

	~AI() = default;

private:
	void pushCrtAction(const size_t& actionId, const unsigned char threadIdx) const;
	void stopFromMasterThread();
	void explore(const std::vector<size_t>& actions);
	bool mustStop(const unsigned char threadIdx) const;
	void stopThread(const unsigned char threadIdx) const;
	std::shared_ptr<MasterAIThread> getMaster() const;	

	size_t getRootNbSimu(const size_t& instanceIdx) const;
	void incrRootNbSimu(const size_t& instanceIdx);

	void _pushEvent(Event::EventEnum type_);


	//Attributes
	const AIMode m_type;
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
