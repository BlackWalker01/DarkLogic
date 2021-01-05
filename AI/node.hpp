#ifndef NODE_H
#define NODE_H
#include <unordered_map>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>
#include <climits>
#include "Utils/action.h"
#include "dbaction.h"
#include "MainDarkLogic/utils.h"

#define MAX_DEPTH 50
#define VAL_INIT 100
#define VAL_MAX VAL_INIT + 1
#define SUBVAL_INIT (USHRT_MAX - VAL_INIT - 1)

class AI;
class DbAction;

class Node
{
public:
	using DbNode = std::unordered_map<size_t, std::unique_ptr<Node>>;

	Node(AI& ai); //First Root node
	Node(const Id& actionId_, const unsigned char& threadId_, const unsigned short& depth_);

	//explore subNodes
	/*
	 * explore one subNode among actions and return its value. called only by AIThread
	 */
	unsigned short exploreBasic(const std::vector<N_DarkLogic::Action::Id>& actions); //explore all subNodes among actions for BasicAI
	void exploreEval(DbAction& actions, unsigned char threadIdx); //explore all subNodes among actions for EvalAI
	
	Id actionId() const;
	unsigned char threadId() const;
	unsigned short depth() const;
	unsigned short value() const;
	unsigned short int subValue() const;
	unsigned short int realValue() const;
	bool isLoss() const;
	unsigned short int getValueFromAction(const unsigned short int action) const;
	unsigned short int getRealValueFromAction(const unsigned short int action) const;
	
	void pushCrtAction(const Id& actionId, const unsigned char threadIdx);
	void setThreadIdx(const unsigned char threadIdx);
	void eval(const unsigned char threadIdx);

	//only AI uses these methods
	Node* getBestNode();
	Node* getDemoMode();
	size_t nbNode() const;
	void decrDepth();
private:
	unsigned short exploreBasic(const unsigned short maxDepth); //explore subNodes of current node until maxDepth
	unsigned short exploreEval(); //explore all subNodes among actions for EvalAI
	unsigned short minDepth() const;

	// Attributes
	Id m_actionId;
	DbNode m_sons;
	DbAction m_dbActions;
	unsigned char m_threadId;
	unsigned short m_depth;
	unsigned short m_value; //estimation of remain number of actions before end of demonstration
	unsigned short m_subValue; //evaluation of current node by AI
	bool m_isLoss;

	// Static Attributes
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
