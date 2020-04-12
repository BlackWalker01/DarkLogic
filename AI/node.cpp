#include "node.hpp"
#include "node.hpp"
#include "Logic/logic.h"
#include "ai.h"
#include <ctime>
#include <random>
#include <iostream>

AI* Node::s_ai = nullptr;
double Node::s_cste = sqrt(2);

Node::Node(AI& ai):
	m_actionId(0), m_threadId(0), m_depth(0), m_value(VAL_INIT), 
	m_nbSimu(0)
{
	s_ai = &ai;
	std::srand(std::time(nullptr));
}

Node::Node(const size_t& actionId_, const unsigned char& threadId_, const unsigned short& depth_):
	m_actionId(actionId_), m_threadId(threadId_), m_depth(depth_), m_value(VAL_INIT),
	m_nbSimu(0)
{
}

size_t Node::actionId() const
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

unsigned int Node::nbSimu() const
{
	return m_nbSimu;	
}

double Node::getMCTSValue(const unsigned int parentNbSimu) const
{
	return (1000*static_cast<double>(USHRT_MAX-m_value))/USHRT_MAX + s_cste * sqrt(log(parentNbSimu+1)/(m_nbSimu + 1));
}


void Node::pushCrtAction(const size_t& actionId, const unsigned char threadIdx)
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

bool Node::isRoot() const
{
	return m_threadId==0;
}

unsigned short Node::makeSimu()
{
	//play move
	N_Logic::Logic::apply(m_threadId, m_actionId);

	//check if node is terminal
	if (N_Logic::Logic::isAlreadyPlayed(m_threadId) || !N_Logic::Logic::canBeDemonstrated(m_threadId))
	{
		m_value = USHRT_MAX;
	}
	else if (N_Logic::Logic::isDemonstrated(m_threadId))
	{
		//std::cout << "[DEBUG] found demonstration!" << std::endl;
		m_value = 0;
	}
	else if (m_depth >= MAX_DEPTH)
	{
		m_value = VAL_INIT;
		//std::cout << "[DEBUG ]Reach limit" << std::endl;
	}
	else
	{
		if (!s_ai->mustStop(m_threadId))
		{
			//get actions
			std::vector<size_t> actions = N_Logic::Logic::getActions(m_threadId);

			//play random action
			auto newAction = actions[rand() % actions.size()];
			m_sons[newAction] = std::make_unique<Node>(newAction, m_threadId, m_depth + 1);
			m_value = m_sons[newAction]->makeSimu();
			if (m_value < VAL_INIT)
			{
				m_value++;
			}
		}
	}

	//unplay crt move
	N_Logic::Logic::unapply(m_threadId);

	//increment nb simulation
	m_nbSimu++;

	return m_value;
}


unsigned short Node::explore(const std::vector<size_t>& actions)
{	
	unsigned int retValue = 0;
	double maxValue = 0;
	std::vector<Node*> maxNodes;
	for (size_t k=0;k<actions.size();k++)
	{
		auto action = actions[k];
		auto it = m_sons.find(action);
		//if the node has not been created yet, create it!
		if (it == m_sons.end())
		{
			m_sons[action] = std::make_unique<Node>(action, m_threadId, m_depth + 1);
			it = m_sons.find(action);
		}
		auto subNode = it->second.get();
		double mctValue = isRoot()? subNode->getMCTSValue(s_ai->getRootNbSimu(subNode->threadId())) : subNode->getMCTSValue(m_nbSimu);
		if (mctValue > maxValue)
		{
			maxNodes.clear();
			maxNodes.push_back(subNode);
			maxValue = mctValue;
		}
		else if (mctValue == maxValue)
		{
			maxNodes.push_back(subNode);
		}
	}

	if (isRoot())
	{
		//Choose among maxNodes which one to explore
		auto nbNodes = maxNodes.size();
		auto node = maxNodes[rand() % nbNodes];
		if (node->nbSimu())
		{
			node->explore();
		}
		else
		{
			node->makeSimu();
		}
		s_ai->incrRootNbSimu(node->threadId());
	}
	else if (!s_ai->mustStop(m_threadId))
	{
		//Choose among maxNodes which one to explore
		auto nbNodes = maxNodes.size();
		auto node = maxNodes[rand() % nbNodes];
		if (node->nbSimu())
		{
			retValue = node->explore();
			if ((m_value > retValue + 1))
			{
				m_value = retValue + 1;
			}
		}
		else
		{
			retValue = node->makeSimu();
			if ((m_value > retValue + 1))
			{
				m_value = retValue + 1;
			}
			else
			{
				m_value = retValue;
			}
		}


		if (m_value < MAX_DEPTH)
		{
			s_ai->stopThread(m_threadId);
		}
			
		m_nbSimu++;
	}		

	return m_value;
}

/**
* if explore method is called, subNodes have already been created
* return value of selected subNode
*/
unsigned short Node::explore()
{
	//play crt move
	N_Logic::Logic::apply(m_threadId, m_actionId);

	//check if node is terminal
	if (N_Logic::Logic::isAlreadyPlayed(m_threadId) || !N_Logic::Logic::canBeDemonstrated(m_threadId))
	{
		m_value = USHRT_MAX;
		//increment nb simulations
		m_nbSimu++;
	}
	else if(N_Logic::Logic::isDemonstrated(m_threadId))
	{
		m_value = 0;
		//increment nb simulations
		m_nbSimu++;
	}
	else
	{
		//get actions
		std::vector<size_t> actions = N_Logic::Logic::getActions(m_threadId);
		
		//explore one subNode among actions
		explore(actions);		
	}

	//unplay crt move
	N_Logic::Logic::unapply(m_threadId);	

	return m_value;
}

unsigned short Node::exploreDeep(const std::vector<size_t>& actions)
{
	unsigned short maxDepth = minDepth();
	std::cout << "Starting search at MaxDepth="+ numberToString(maxDepth) << std::endl;
	unsigned char threadId = m_sons[actions[0]]->threadId();
	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	while (!s_ai->mustStop(threadId))
	{		
		for (const auto& action : actions)
		{
			auto node = m_sons[action].get();
			unsigned short retValue = USHRT_MAX;
			if (node->value() < USHRT_MAX)
			{
				node->exploreDeep(maxDepth);
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
		end= std::chrono::high_resolution_clock::now();
		double elapsed_seconds = std::chrono::duration<double>(end - start).count();
		std::cout << "[DEBUG] thread id: " +numberToString(static_cast<unsigned int>(threadId))+" has finished depth '"+ numberToString(maxDepth) <<
			"' in "+ numberToString(elapsed_seconds)+ " seconds" <<std::endl;
		maxDepth++;
	}
	return m_value;
}

unsigned short Node::exploreDeep(const unsigned short maxDepth)
{
	//play crt move
	N_Logic::Logic::apply(m_threadId, m_actionId);

	//no need to go deeper
	if (m_depth == maxDepth)
	{
		//check if it is a node which leads to loss
		if (N_Logic::Logic::isAlreadyPlayed(m_threadId) || !N_Logic::Logic::canBeDemonstrated(m_threadId))
		{
			m_value = USHRT_MAX;
		}
		//check if it is a node which leads to win
		else if (N_Logic::Logic::isDemonstrated(m_threadId))
		{
			m_value = 0;
			
			//stop reflexion because AI found a demonstration
			s_ai->stopThread(m_threadId);
		}
	}
	else if(!s_ai->mustStop(m_threadId))
	{
		//get actions
		std::vector<size_t> actions = N_Logic::Logic::getActions(m_threadId);

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
				retValue = node->exploreDeep(maxDepth);
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
	N_Logic::Logic::unapply(m_threadId);

	return m_value;
}

Node* Node::getBestNode()
{
	std::vector<size_t> minNodes;
	unsigned short minVal = USHRT_MAX;
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

void Node::setRoot()
{
	m_threadId = 0;
	_decrDepth();
}

unsigned int Node::getRootNbSimu() const
{
	return m_nbSimu;
}

void Node::incrRootNbSimu()
{
	m_nbSimu++;
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

void Node::_decrDepth()
{

	m_depth--;
	for (auto& son : m_sons)
	{
		son.second->_decrDepth();
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