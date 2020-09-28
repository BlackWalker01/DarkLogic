#include "node.hpp"
#include "node.hpp"
#include "darklogic.h"
#include "ai.h"
#include <ctime>
#include <random>
#include <iostream>

AI* Node::s_ai = nullptr;
double Node::s_cste = sqrt(2);

Node::Node(AI& ai):
	m_actionId(0), m_threadId(std::numeric_limits<unsigned char>::max()), m_depth(0), m_value(VAL_INIT), m_subValue(SUBVAL_INIT), m_isLoss(false)
{
	s_ai = &ai;
	std::srand(std::time(nullptr));
}

Node::Node(const Id& actionId_, const unsigned char& threadId_, const unsigned short& depth_):
	m_actionId(actionId_), m_threadId(threadId_), m_depth(depth_), m_value(VAL_INIT), m_subValue(SUBVAL_INIT), m_isLoss(false)
{
}

Node::Id Node::actionId() const
{
	return m_actionId;
}

unsigned char Node::threadId() const
{
	return m_threadId;
}

unsigned short Node::depth() const
{
	return m_depth;
}

unsigned short Node::value() const
{
	return m_value;
}

unsigned short int Node::subValue() const
{
	return m_subValue;
}

unsigned short int Node::realValue() const
{
	return (m_value < VAL_INIT)? m_value : (m_value + m_subValue);
}

bool Node::isLoss() const
{
	return m_isLoss;
}

unsigned short int Node::getValueFromAction(const unsigned short int action) const
{
	return m_sons.at(action)->value();
}

unsigned short int Node::getRealValueFromAction(const unsigned short int action) const
{
	return m_sons.at(action)->realValue();
}

void Node::pushCrtAction(const Id& actionId, const unsigned char threadIdx)
{
	auto it = m_sons.find(actionId);
	if (it != m_sons.end())
	{
		//std::cout << "[DEBUG] action " << actionId << " will be handled by thread " << static_cast<unsigned int>(threadIdx) << std::endl;
		it->second->setThreadIdx(threadIdx);
	}
	else
	{
		//std::cout << "[DEBUG] new action " << actionId << " will be handled by thread " << static_cast<unsigned int>(threadIdx) << std::endl;
		m_sons[actionId] = std::make_unique<Node>(actionId, threadIdx, m_depth + 1);
		m_sons[actionId]->eval(0);
	}
}

void Node::setThreadIdx(const unsigned char threadIdx)
{
	m_threadId = threadIdx;
	for (auto& subNode : m_sons)
	{
		subNode.second->setThreadIdx(threadIdx);
	}
}

void Node::eval(const unsigned char threadIdx)
{
	// play crt move
	N_DarkLogic::DarkLogic::apply(threadIdx, m_actionId);

	// check if it is a node which leads to loss
	if (N_DarkLogic::DarkLogic::isAlreadyPlayed(threadIdx))
	{
		m_value = VAL_MAX;
	}
	else if (!N_DarkLogic::DarkLogic::canBeDemonstrated(threadIdx))
	{
		m_value = VAL_MAX;
		if (!N_DarkLogic::DarkLogic::isEvaluated(threadIdx))
		{
			m_isLoss = true;
		}
	}
	// check if it is a node which leads to win
	else if (N_DarkLogic::DarkLogic::isDemonstrated(threadIdx))
	{
		m_value = 0;
		// stop reflexion because AI found a demonstration
		s_ai->stopThread(threadIdx);
	}
	else
	{
		m_subValue = s_ai->eval({ N_DarkLogic::DarkLogic::getState(threadIdx) }, threadIdx);
	}

	// unplay crt move
	N_DarkLogic::DarkLogic::unapply(threadIdx);
}

unsigned short Node::exploreBasic(const std::vector<N_DarkLogic::Action::Id>& actions)
{
	unsigned short maxDepth = minDepth();
	unsigned char threadId = m_sons[actions[0]]->threadId();
	//std::chrono::high_resolution_clock::time_point start, end;
	//start = std::chrono::high_resolution_clock::now();
	while (!s_ai->mustStop(threadId))
	{
		for (const auto& action : actions)
		{
			auto node = m_sons[action].get();
			unsigned short retValue = USHRT_MAX;
			if (node->value() < USHRT_MAX)
			{
				node->exploreBasic(maxDepth);
			}

			if ((m_value > retValue + 1))
			{
				m_value = retValue + 1;
			}

			//if must stop exploration, stop it
			if (s_ai->mustStop(threadId))
			{
				break;
			}
		}
		/*if (maxDepth >= 3)
		{
			end = std::chrono::high_resolution_clock::now();
			double elapsed_seconds = std::chrono::duration<double>(end - start).count();
			std::cout << "[DEBUG] thread id: " + numberToString(static_cast<unsigned int>(threadId)) + " has finished depth '" + numberToString(maxDepth) <<
				"' in " + numberToString(elapsed_seconds) + " seconds" << std::endl;

		}*/
		maxDepth++;
	}
	return m_value;
}

unsigned short Node::exploreBasic(const unsigned short maxDepth)
{
	//play crt move
	N_DarkLogic::DarkLogic::apply(m_threadId, m_actionId);

	//no need to go deeper
	if (m_depth == maxDepth)
	{
		//check if it is a node which leads to loss
		if (N_DarkLogic::DarkLogic::isAlreadyPlayed(m_threadId) || !N_DarkLogic::DarkLogic::canBeDemonstrated(m_threadId))
		{
			m_value = USHRT_MAX;
		}
		//check if it is a node which leads to win
		else if (N_DarkLogic::DarkLogic::isDemonstrated(m_threadId))
		{
			m_value = 0;
			
			//stop reflexion because AI found a demonstration
			s_ai->stopThread(m_threadId);
		}
	}
	else if(!s_ai->mustStop(m_threadId))
	{
		//get actions
		auto actions = N_DarkLogic::DarkLogic::getActions(m_threadId);

		//add all subnodes if they have not been created yet
		if (m_depth == maxDepth - 1)
		{
			for (const auto& action : actions)
			{
				m_sons[action] = std::make_unique<Node>(action, m_threadId, m_depth + 1);
			}			
		}

		//explore subNodes
		bool hasOnlyLosses = true;
		for (const auto& action : actions)
		{
			//explore node associated with action
			auto node = m_sons[action].get();
			unsigned short retValue = USHRT_MAX;
			if (node->value() < USHRT_MAX)
			{
				retValue = node->exploreBasic(maxDepth);
			}

			//update m_value
			if (retValue == USHRT_MAX)
			{
				if (hasOnlyLosses)
				{
					m_value = USHRT_MAX;
				}
			}
			else
			{
				hasOnlyLosses = false;
				if (retValue==VAL_INIT && m_value == USHRT_MAX)
				{
					m_value = VAL_INIT;
				}
				else if ((m_value > retValue + 1))
				{
					m_value = retValue + 1;
				}
			}

			//if must stop exploration, stop it
			if (s_ai->mustStop(m_threadId))
			{
				break;
			}
		}
	}

	//unplay crt move
	N_DarkLogic::DarkLogic::unapply(m_threadId);

	return m_value;
}

void Node::exploreEval(DbAction& dbAction, unsigned char threadIdx)
{
	while (!s_ai->mustStop(threadIdx))
	{
		const auto& action = dbAction.getBestAction();
		auto node = m_sons[action].get();
		unsigned short int value = node->exploreEval();
		dbAction.updateValue(action, value);

		// if must stop exploration, stop it
		if (s_ai->mustStop(threadIdx))
		{
			break;
		}
	}
}

unsigned short Node::exploreEval()
{
	// play crt move
	N_DarkLogic::DarkLogic::apply(m_threadId, m_actionId);
	// get actions
	auto actions = N_DarkLogic::DarkLogic::getActions(m_threadId);
	//no need to go deeper
	if (m_sons.size() != actions.size())
	{
		unsigned short minSubValue = SUBVAL_INIT;
		bool updateValue = true;
		for (const auto action : actions)
		{
			Node* node = nullptr;
			if (!m_sons.contains(action))
			{
				m_sons[action] = std::make_unique<Node>(action, m_threadId, m_depth + 1);
				node = m_sons[action].get();
				node->eval(m_threadId);
				if (node->value() < VAL_MAX)
				{
					auto idx = m_dbActions.push(action);
					m_dbActions.updateValue(idx, node->realValue());
				}
			}
			else
			{
				node = m_sons[action].get();
			}
			unsigned short subValue = node->subValue();
			if (subValue < minSubValue)
			{
				minSubValue = subValue;
			}

			// if must stop exploration, stop it
			if (s_ai->mustStop(m_threadId))
			{
				if (node->value() < VAL_INIT)
				{
					m_value = node->value() + 1;
				}
				updateValue = false;
				break;
			}
		}
		if (updateValue)
		{
			if (m_dbActions.size() == 0)
			{
				//all actions lead to loss
				m_value = VAL_MAX;
			}
			else
			{
				m_subValue = minSubValue + 1;
			}
		}
	}
	else
	{
		auto action = m_dbActions.getBestAction();
		Node* node = m_sons[action].get();
		unsigned short value = node->exploreEval();
		if (node->value() < VAL_MAX)
		{
			m_dbActions.updateValue(action, value);
			action = m_dbActions.getBestAction();
			node = m_sons[action].get();
			m_subValue = node->subValue() + 1;
			if (node->value() < VAL_INIT)
			{
				m_value = node->value() + 1;
			}
		}
		else
		{
			m_dbActions.removeIdx(action);
		}
	}

	// unplay crt action
	N_DarkLogic::DarkLogic::unapply(m_threadId);

	return realValue();
}

Node* Node::getBestNode()
{
	std::vector<size_t> minNodes;
	unsigned short minVal = USHRT_MAX;
	for (auto& son : m_sons)
	{
		if (son.second->realValue() < minVal)
		{
			minVal = son.second->realValue();
			minNodes.clear();
			minNodes.push_back(son.first);
		}
		else if (son.second->realValue() == minVal)
		{
			minNodes.push_back(son.first);
		}
	}
	auto valWinner= minNodes[rand() % minNodes.size()];
	auto winner = m_sons[valWinner].release();
	m_sons.clear();	
	return winner;
}

Node* Node::getDemoMode()
{
	Node* winner = nullptr;
	std::vector<size_t> minNodes;
	unsigned short minVal = VAL_INIT-1;
	for (auto& son : m_sons)
	{
		if (son.second->value() < minVal)
		{
			minVal = son.second->value();
			minNodes.clear();
			minNodes.push_back(son.first);
		}
		else if (son.second->value() == minVal)
		{
			minNodes.push_back(son.first);
		}
	}
	if (minNodes.size())
	{
		auto valWinner = minNodes[rand() % minNodes.size()];
		winner = m_sons[valWinner].release();
		m_sons.clear();
	}	
	return winner;
}

size_t Node::nbNode() const
{
	size_t ret = 1;
	for (const auto& son : m_sons)
	{
		ret += son.second->nbNode();
	}
	return ret;
}

void Node::decrDepth()
{
	m_depth--;
	for (auto& son : m_sons)
	{
		if (son.second)
		{
			son.second->decrDepth();
		}
	}
}

unsigned short Node::minDepth() const
{
	if (m_sons.size())
	{
		unsigned short ret = USHRT_MAX;
		for (auto& son : m_sons)
		{
			if (son.second->value() < USHRT_MAX)
			{
				unsigned short crtDepth = son.second->minDepth();
				if (crtDepth + 1 < ret)
				{
					ret = crtDepth + 1;
				}
			}
			
		}
		return ret;
	}
	else
	{
		return 0;
	}
}