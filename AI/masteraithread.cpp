#include "masteraithread.h"
#include "Logic/logic.h"
#include "ai.h"

MasterAIThread::MasterAIThread(const size_t& maxInstanceIdx_, AI& ai_): 
    m_ai(ai_), m_maxInstanceIdx(maxInstanceIdx_), m_hasEvents(false), m_thread(runMaster, this), m_lock(m_mutex)
{
    if (maxInstanceIdx_ < 2)
    {
        throw std::runtime_error("AI needs at least one instance (maxInstanceIdx must be at least 2)!");
    }	
}

void MasterAIThread::computeActions()
{
    for (auto& slave : m_slaveThreads)
    {
        slave->computeActions();
    }
}

void MasterAIThread::updateLogic(const size_t& actionid)
{
    for (auto& slave : m_slaveThreads)
    {
        slave->updateLogic(actionid);
    }
}

bool MasterAIThread::mustStop(const unsigned char threadIdx) const
{
    return m_slaveThreads[threadIdx-1]->mustStop();
}

void MasterAIThread::start()
{
    _pushEvent(0, Event::START);
}

void MasterAIThread::stop()
{
    _pushEvent(0, Event::STOP);
}

void MasterAIThread::stopFromThread(const unsigned char threadIdx)
{
    _pushEvent(threadIdx, Event::STOP_THREAD);
}

void MasterAIThread::stopThread(const unsigned char threadIdx)
{    
    m_slaveThreads[threadIdx-1]->stop();
}

void MasterAIThread::init()
{
    if (m_slaveThreads.size() == 0)
    {
        for (size_t k = 1; k < m_maxInstanceIdx; k++)
        {
            m_slaveThreads.push_back(std::make_unique<AIThread>(k, m_ai));
        }
    }
}

size_t MasterAIThread::getTotalRootNbSimu() const
{
    size_t ret = 0;
    for (auto& slaveThread : m_slaveThreads)
    {
        ret += slaveThread->getRootNbSimu();
    }
    return ret;
}

void MasterAIThread::setRootNbSimu(const size_t& instanceIdx, const size_t& nbSimu)
{
    m_slaveThreads[instanceIdx - 1]->setRootNbSimu(nbSimu);
}

size_t MasterAIThread::getRootNbSimu(const size_t& instanceIdx) const
{
    return m_slaveThreads[instanceIdx - 1]->getRootNbSimu();
}

void MasterAIThread::incrRootNbSimu(const size_t& instanceIdx)
{
    m_slaveThreads[instanceIdx - 1]->incrRootNbSimu();
}

void MasterAIThread::_start()
{
    //dispatch actions between slave threads
    auto actions = N_Logic::Logic::getActions(0);
    for (size_t k = 0; k < actions.size(); k++)
    {
        m_slaveThreads[k % m_slaveThreads.size()]->pushAction(actions[k]);
    }
    //init number of simuations
    for (auto& slaveThread : m_slaveThreads)
    {
        //slaveThread->setRootNbSimu(m_ai.m_crtNode->nbSimu());
        slaveThread->setRootNbSimu(0);
    }

    //start slave threads
    if (actions.size() > m_slaveThreads.size())
    {
        for (auto& slaveThread : m_slaveThreads)
        {
            slaveThread->setRootNbSimu(m_ai.m_crtNode->nbSimu());
            slaveThread->start();
            m_threadAlive[slaveThread->instanceId()] = slaveThread->instanceId();
        }
    }
    else
    {
        for (unsigned char k = 0; k < actions.size(); k++)
        {
            m_slaveThreads[k]->start();
            m_threadAlive[k] = k;
        }
    }
    
}

void MasterAIThread::_stop()
{
    for (auto& slaveThread : m_slaveThreads)
    {
        slaveThread->stop();
    }
}

void MasterAIThread::_stopFromThread(const unsigned char threadIdx)
{
    //remove this thread from "living threads"
    m_threadAlive.erase(threadIdx);    

    //if no thread is alive, warn AI
    if (m_threadAlive.size() == 0)
    {
        m_ai.stopFromMasterThread();
    }
    //else check if the other threads know they must stop
    else
    {
        for (const auto& threadIdx : m_threadAlive)
        {
            if (!m_slaveThreads[threadIdx.second - 1]->mustStop())
            {
                m_slaveThreads[threadIdx.second - 1]->stop();
            }
        }
    }
}

[[noreturn]] void MasterAIThread::_run()
{
	while (true)
	{
        while (!m_hasEvents)
        {
            m_condition_var.wait(m_lock);
        }
        while (m_eventQueue.size())
        {
            //Consuming event
            switch (m_eventQueue.front().type())
            {
            case Event::START:
            {
                _start();
                break;
            }
            case Event::STOP:
            {
                _stop();
                break;
            }
            case Event::STOP_THREAD:
            {
                _stopFromThread(m_eventQueue.front().threadIdx());
                break;
            }
            }
            m_eventQueue.pop();
        }
        m_hasEvents = false;
	}
}

void MasterAIThread::_pushEvent(const unsigned char threadIdx_, Event::EventEnum type_)
{
    m_mutex.lock();
    m_eventQueue.push(Event(threadIdx_,type_));
    m_mutex.unlock();
    m_hasEvents = true;
    m_condition_var.notify_all();
}

void runMaster(MasterAIThread* masterThread)
{
	masterThread->_run();
}
