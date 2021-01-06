#include "dbaction.h"
#include <random>
#include <ctime>

Id DbAction::push(Id action)
{
	m_idxToActions[action] = action;
	return action;
}

void DbAction::updateValue(Id actionIdx, Val value)
{
    Id action = m_idxToActions[actionIdx];
    if (m_idxToVal.contains(actionIdx))
    {
        Val exValue = m_idxToVal[actionIdx];
        if(exValue != value)
        {
            // remove from latest value
            unsigned short int k = 0;
            for (const auto& exAction : m_valToActions[exValue])
            {
                if (action == exAction)
                {
                    break;
                }
                k += 1;
            }
                
            m_valToActions[exValue].erase(m_valToActions[exValue].begin() + k);
            if (m_valToActions[exValue].size() == 0)
            {
                m_valToActions.erase(exValue);
            }
            // update new value
            if (m_valToActions.contains(value))
            {
                m_valToActions[value].push_back(action);
            }                
            else
            {
                m_valToActions[value] = { action };
            }                
            m_idxToVal[actionIdx] = value;
        }
    }
    else
    {
        // update new value
        if (m_valToActions.contains(value))
        {
            m_valToActions[value].push_back(action);
        }
        else
        {
            m_valToActions[value] = { action };
        }
        m_idxToVal[actionIdx] = value;
    }
        
}

void DbAction::removeIdx(Id actionIdx)
{
    Id action = m_idxToActions[actionIdx];
    Val val = m_idxToVal[actionIdx];
    unsigned int short k = 0;
    for (const auto& crtAction : m_valToActions[val])
    {
        if (action == crtAction)
        {
            break;
        }
        k += 1;
    }
    m_valToActions[val].erase(m_valToActions[val].begin() + k);
    if (m_valToActions[val].size() == 0)
    {
        m_valToActions.erase(val);
    }
    m_idxToActions.erase(actionIdx);
    m_idxToVal.erase(actionIdx);
}

std::vector<Id> DbAction::actions() const
{
	std::vector<Id> ret;
	for (auto it : m_idxToActions)
	{
		ret.push_back(it.first);
	}
	return ret;
}

Id DbAction::getBestAction() const
{    
    const auto& begIt = m_valToActions.begin();
    Val minVal = begIt->first;
    const auto& actionList = m_valToActions.at(minVal);
    return actionList.at(rand() % actionList.size());
}

size_t DbAction::size() const
{
	return m_idxToActions.size();
}

void DbAction::clear()
{
	m_idxToActions.clear();
	m_idxToVal.clear();
	m_valToActions.clear();
}
