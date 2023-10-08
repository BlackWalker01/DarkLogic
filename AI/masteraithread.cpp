#include "masteraithread.h"
#include "darklogic.h"
#include "ai.h"
#include <iostream>

MasterAIThread::MasterAIThread(const size_t& maxInstanceIdx_, AI& ai_): 
    m_ai(ai_), m_maxInstanceIdx(maxInstanceIdx_), m_hasEvents(false), m_thread(runMaster, this), m_lock(m_mutex)
{
    if (maxInstanceIdx_ < 1)
    {
        throw std::runtime_error("AI needs at least one instance!");
    }	
}

void MasterAIThread::computeActions()
{
    for (auto& slave : m_slaveThreads)
    {
        slave->computeActions();
    }
}

void MasterAIThread::updateLogic(const Id& actionid)
{
    for (auto& slave : m_slaveThreads)
    {
        slave->updateLogic(actionid);
    }
}

bool MasterAIThread::mustStop(const unsigned char threadIdx) const
{
    return m_slaveThreads[threadIdx]->mustStop();
}

void MasterAIThread::start()
{
    _pushEvent(0, Event::EventEnum::START);
}

void MasterAIThread::stop()
{
    _pushEvent(0, Event::EventEnum::STOP);
}

void MasterAIThread::stopFromThread(const unsigned char threadIdx)
{
    _pushEvent(threadIdx, Event::EventEnum::STOP_THREAD);
}

void MasterAIThread::stopThread(const unsigned char threadIdx)
{    
    m_slaveThreads[threadIdx]->stop();
}

void MasterAIThread::init()
{
    if (m_slaveThreads.size() == 0)
    {
        for (size_t k = 0; k < m_maxInstanceIdx; k++)
        {
            m_slaveThreads.push_back(std::make_unique<AIThread>(k, m_ai));
        }
    }
}

void MasterAIThread::_start()
{
    //dispatch actions between slave threads
    size_t nbActions = 0;
    bool foundDemo = false;
    auto actions = N_DarkLogic::DarkLogic::getActions(0);
    for (const auto action : actions)
    {
        unsigned char threadIdx = static_cast<unsigned char>(nbActions % m_slaveThreads.size());
        m_ai.pushCrtAction(action, threadIdx);
        auto val = m_ai.getValueFromAction(action);
        if (val == 0)
        {
            foundDemo = true;
            break;
        }
        else if (val == VAL_MAX)
        {
            continue;
        }
        else
        {
            m_slaveThreads[threadIdx]->pushAction(action);
            nbActions += 1;
        }
    }

    if (!foundDemo)
    {
        //start slave threads
        if (nbActions > m_slaveThreads.size())
        {
            for (auto& slaveThread : m_slaveThreads)
            {
                slaveThread->start();
                m_threadAlive[slaveThread->instanceId()] = slaveThread->instanceId();
            }
        }
        else
        {
            for (unsigned char k = 0; k < nbActions; k++)
            {
                m_slaveThreads[k]->start();
                m_threadAlive[m_slaveThreads[k]->instanceId()] = m_slaveThreads[k]->instanceId();
            }
        }
    }
    else
    {
        m_ai.stopFromMasterThread();
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
            if (!m_slaveThreads[threadIdx.second ]->mustStop())
            {
                m_slaveThreads[threadIdx.second]->stop();
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
            case Event::EventEnum::START:
            {
                _start();
                break;
            }
            case Event::EventEnum::STOP:
            {
                _stop();
                break;
            }
            case Event::EventEnum::STOP_THREAD:
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
