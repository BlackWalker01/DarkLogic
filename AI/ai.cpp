#include "ai.h"
#include "logic.h"
#include "MainDarkLogic/action.h"
#include "MainDarkLogic/EnumFun.h"
#include "masteraithread.h"
#include <chrono>
#include <condition_variable>
#include <iostream>

AI::AI(const size_t& maxInstanceIdx, const size_t& secondTimeout) : Player("BasicAI", 0, 1955),
m_secondTimeout(secondTimeout), m_masterThread(std::make_shared<MasterAIThread>(maxInstanceIdx,*this)), 
	m_crtNode(std::make_unique<Node>(*this)), m_hasEvents(false), m_lock(m_mutex)
{
	m_masterThread->init();
}

AI::AI(const std::string& name_, const size_t& maxInstanceIdx, const size_t timeoutScds) : Player(name_, 0, 1955),
m_secondTimeout(timeoutScds), m_masterThread(std::make_shared<MasterAIThread>(maxInstanceIdx, *this)),
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
			status = m_condition_var.wait_until(m_lock, start + std::chrono::seconds(m_secondTimeout)); 			
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
		std::cout<< "[DEBUG] Nb explored node " << *nbNode << std::endl;
	}
	else
	{
		m_masterThread->computeActions();
	}
	
	m_crtNode.reset(newNode);
	m_crtNode->decrDepth();
	m_masterThread->updateLogic(m_crtNode->actionId());
	end = std::chrono::high_resolution_clock::now();
	double elapsed_seconds = std::chrono::duration<double>(end - start).count();
	
	if (nbNode)
	{
		std::cout << "[DEBUG] AI play action " << m_crtNode->actionId() << " with value " << value()
			<< " in " << elapsed_seconds << " seconds" << std::endl;
		std::cout << "[DEBUG] AI exploration speed: " << (*nbNode) / elapsed_seconds << " nodes/s" << std::endl;
	}
		
	return std::make_shared<const Action>(Fun::PUSH_ACTION, m_crtNode->actionId());
}

void AI::meditate()
{
	Player::meditate();
	m_crtNode = std::make_unique<Node>(*this);
}

void AI::pushCrtAction(const Id& actionId, const unsigned char threadIdx) const
{
	m_crtNode->pushCrtAction(actionId, threadIdx);
}

void AI::stopFromMasterThread()
{
	_pushEvent(Event::EventEnum::STOP);
}

void AI::explore(DbAction& dbActions, unsigned char /*threadIdx*/)
{
	m_crtNode->exploreBasic(dbActions.actions());
}

unsigned short int AI::eval(const std::vector<N_DarkLogic::State>&, const unsigned char)
{
	return 0;
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

size_t AI::timeout() const
{
	return m_secondTimeout;
}

unsigned int AI::value() const
{
	return m_crtNode->value();
}

unsigned short int AI::getValueFromAction(const unsigned short int action)
{
	return m_crtNode->getValueFromAction(action);
}

unsigned short int AI::getRealValueFromAction(const unsigned short int action)
{
	return m_crtNode->getRealValueFromAction(action);
}

void AI::_pushEvent(Event::EventEnum type_)
{
	m_mutex.lock();
	m_eventQueue.push(Event(0, type_));
	m_mutex.unlock();
	m_hasEvents = true;
	m_condition_var.notify_all();
}
