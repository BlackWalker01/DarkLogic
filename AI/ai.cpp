#include "ai.h"
#include "logic.h"
#include "MainDarkLogic/action.h"
#include "MainDarkLogic/EnumFun.h"
#include "masteraithread.h"
#include <chrono>
#include <condition_variable>
#include <iostream>

AI::AI(const AIMode type_,const size_t& maxInstanceIdx) : Player("AI"), m_type(type_),
m_masterThread(std::make_shared<MasterAIThread>(maxInstanceIdx,*this)), 
	m_crtNode(std::make_unique<Node>(*this)), m_hasEvents(false), m_lock(m_mutex)
{
	m_masterThread->init();
}

std::shared_ptr<const Action> AI::play()
{	
	//start chrono with cadence=1s
	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	Node* newNode = m_crtNode->getDemoMode();
	std::unique_ptr<size_t> nbNode = nullptr;
	
	//if AI has not found demonstration yet
	if (!newNode)
	{
		m_masterThread->start();
		std::cv_status status = std::cv_status::no_timeout;
		while (!m_hasEvents && (status!=std::cv_status::timeout))
		{
			status = m_condition_var.wait_until(m_lock, start + std::chrono::seconds(120)); //1h			
		}
		if (status == std::cv_status::timeout)
		{
			m_masterThread->stop();
		}

		//waiting for threads to stop
		while (!m_hasEvents)
		{
			m_condition_var.wait(m_lock);
		}
		if (m_eventQueue.size())
		{
			//Consuming event
			m_eventQueue.pop();
		}
		m_hasEvents = false;

		nbNode = std::make_unique<size_t>(m_crtNode->nbNode());
		newNode = m_crtNode->getBestNode();
		if (m_type == MCTS)
		{
			std::cout << "[DEBUG] Nb simulations " << m_masterThread->getTotalRootNbSimu() << std::endl;
		}		
		std::cout<< "[DEBUG] Nb explored node " << *nbNode << std::endl;
	}
	else
	{
		m_masterThread->computeActions();
	}
	
	m_crtNode.reset(newNode);
	m_crtNode->setRoot();
	m_masterThread->updateLogic(m_crtNode->actionId());
	end = std::chrono::high_resolution_clock::now();
	double elapsed_seconds = std::chrono::duration<double>(end - start).count();
	
	if (nbNode)
	{
		std::cout << "[DEBUG] AI play action " << m_crtNode->actionId() << " with value " << m_crtNode->value()
			<< " in " << elapsed_seconds << " seconds" << std::endl;
		std::cout << "[DEBUG] AI exploration speed: " << (*nbNode) / elapsed_seconds << " nodes/s" << std::endl;
	}
		
	return std::make_shared<const Action>(Fun::PUSH_ACTION, m_crtNode->actionId());
}

void AI::pushCrtAction(const size_t& actionId, const unsigned char threadIdx) const
{
	m_crtNode->pushCrtAction(actionId, threadIdx);
}

void AI::stopFromMasterThread()
{
	_pushEvent(Event::EventEnum::STOP);
}

void AI::explore(const std::vector<N_DarkLogic::Action::Id>& actions)
{
	if (m_type == MCTS)
	{
		m_crtNode->explore(actions);
	}
	else
	{
		m_crtNode->exploreDeep(actions);
	}
	
}

bool AI::mustStop(const unsigned char threadIdx) const
{
	return m_masterThread->mustStop(threadIdx);
}

void AI::stopThread(const unsigned char threadIdx) const
{
	m_masterThread->stopThread(threadIdx);
}

std::shared_ptr<MasterAIThread> AI::getMaster() const
{
	return m_masterThread;
}

AI::AIMode AI::type() const
{
	return m_type;
}

size_t AI::getRootNbSimu(const size_t& instanceIdx) const
{
	return m_masterThread->getRootNbSimu(instanceIdx);
}

void AI::incrRootNbSimu(const size_t& instanceIdx)
{
	m_masterThread->incrRootNbSimu(instanceIdx);
}

void AI::_pushEvent(Event::EventEnum type_)
{
	m_mutex.lock();
	m_eventQueue.push(Event(0, type_));
	m_mutex.unlock();
	m_hasEvents = true;
	m_condition_var.notify_all();
}
