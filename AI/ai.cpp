#include "ai.h"
#include "Logic/logic.h"
#include "MainBlackLogic/action.h"
#include "MainBlackLogic/EnumFun.h"
#include "masteraithread.h"
#include <chrono>
#include <condition_variable>

AI::AI(const size_t& maxInstanceIdx) : Player("AI"), m_masterThread(std::make_shared<MasterAIThread>(maxInstanceIdx,*this)), 
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
	
	//if AI has not found demonstration yet
	if (!newNode)
	{
		m_masterThread->start();
		std::cv_status status = std::cv_status::no_timeout;
		while (!m_hasEvents && (status!=std::cv_status::timeout))
		{
			status = m_condition_var.wait_until(m_lock, start + std::chrono::seconds(2)); //time is multiplicated per 2 or 3 in Win64			
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

		newNode = m_crtNode->getBestNode();
		std::cout << "[DEBUG] Nb simulations " << m_masterThread->getTotalRootNbSimu() << std::endl;
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
	std::cout << "[DEBUG] AI play action " << m_crtNode->actionId() <<" with value "<<m_crtNode->value()
		<<" in "<<elapsed_seconds<<" seconds"<< std::endl;
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

void AI::explore(const std::vector<size_t>& actions)
{
	m_crtNode->explore(actions);
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
