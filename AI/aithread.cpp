#include "aithread.h"
#include "ai.h"
#include "logic.h"
#include <iostream>

AIThread::AIThread(const size_t& instanceId, AI& ai) : m_instanceId(instanceId), 
m_ai(ai), m_master(ai.getMaster()), m_hasStarted(false), m_thread(runThread,this),  
m_rootNbSimu(0), m_hasEvents(false), m_lock(m_mutex), m_mustStop(false)
{
}

void AIThread::start()
{
    m_mustStop = false;
    _pushEvent(0, Event::START);
}

void AIThread::stop()
{
    m_mustStop = true;
}

bool AIThread::hasStarted() const
{
    return m_hasStarted;
}

void AIThread::pushAction(const size_t& action)
{
    m_crtActions.push_back(action);
    m_ai.pushCrtAction(action, m_instanceId);
}

bool AIThread::mustStop()
{
    return m_mustStop.load();
}

void AIThread::computeActions()
{
    N_DarkLogic::DarkLogic::getActions(m_instanceId);
}

void AIThread::updateLogic(const size_t& actionId)
{
    N_DarkLogic::DarkLogic::apply(m_instanceId,actionId);
    m_crtActions.clear();
}

unsigned char AIThread::instanceId() const
{
    return m_instanceId;
}

void AIThread::setRootNbSimu(const size_t& nbSimu)
{
    m_rootNbSimu = nbSimu;
}

size_t AIThread::getRootNbSimu() const
{
    return m_rootNbSimu;
}

void AIThread::incrRootNbSimu()
{
    m_rootNbSimu++;
}

void AIThread::_start()
{
    //std::cout << "[DEBUG] Start thread:"<<m_instanceId << std::endl;
    m_hasStarted = true;
    N_DarkLogic::DarkLogic::getActions(m_instanceId);

    //compute value of given nodes
    while (!mustStop())
    {
        m_ai.explore(m_crtActions);
    }

    //inform master that this thread has finished
    _stop();
}

void AIThread::_stop()
{
    m_hasStarted = false;
    //std::cout << "[DEBUG] Stop thread:" << m_instanceId << std::endl;
    m_master->stopFromThread(m_instanceId);
}

[[noreturn]] void AIThread::_run()
{
    while (true)
    {
        while (!m_hasEvents)
        {
            m_condition_var.wait(m_lock);
        }
        if(m_eventQueue.size())
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
                break;
            }
            }
            m_eventQueue.pop();
        }
        m_hasEvents = false;
    }
}

void AIThread::_pushEvent(const unsigned char threadIdx_, Event::EventEnum type_)
{
    m_mutex.lock();
    m_eventQueue.push(Event(threadIdx_, type_));
    m_mutex.unlock();
    m_hasEvents = true;
    m_condition_var.notify_all();
}

void runThread(AIThread* thread)
{
    thread->_run();
}
