#ifndef NODE_H
#define NODE_H
#include <unordered_map>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>
#include <climits>
#include "Utils/action.h"
#define MAX_DEPTH 50
#define VAL_INIT std::numeric_limits<unsigned short>::max()-1

class AI;

class Node
{
public:
	using DbNode = std::unordered_map<size_t, std::unique_ptr<Node>>;

	Node(AI& ai); //First Root node
	Node(const size_t& actionId_, const unsigned char& threadId_, const unsigned short& depth_);

	//explore subNodes
	/*
	 * explore one subNode among actions and return its value. called only by AIThread
	 */
	unsigned short explore(const std::vector<N_DarkLogic::Action::Id>& actions);
	unsigned short exploreDeep(const std::vector<N_DarkLogic::Action::Id>& actions); //explore all subNodes among actions
	

	//make simulation on a node until depth = MAX_DEPTH
	unsigned short makeSimu();
	
	size_t actionId() const;
	unsigned char threadId() const;
	unsigned short depth() const;
	unsigned short value() const;
	unsigned int nbSimu() const;
	double getMCTSValue(unsigned int parentNbSimu) const;
	
	
	void pushCrtAction(const size_t& actionId, const unsigned char threadIdx);
	void setThreadIdx(const unsigned char threadIdx);
	bool isRoot() const;

	//only AI uses these methods
	Node* getBestNode();
	Node* getDemoMode();
	void setRoot();
	unsigned int getRootNbSimu() const;
	void incrRootNbSimu();
	size_t nbNode() const;
private:
	unsigned short explore(); //explore one subNode and return its value
	unsigned short exploreDeep(const unsigned short maxDepth); //explore subNodes of current node until maxDepth
	inline void _decrDepth();
	unsigned short minDepth() const;

	size_t m_actionId;
	unsigned char m_threadId;
	unsigned short m_depth;
	unsigned short m_value; //estimation of remain number of actions before end of demonstration
	unsigned int m_nbSimu;
	DbNode m_sons;
	static AI* s_ai;
	static double s_cste;
};

template<typename T>
std::string numberToString(const T& x)
{
	std::stringstream ss;
	ss << x;
	std::string ret;
	ss >> ret;
	return ret;
}
#endif // !NODE_H
