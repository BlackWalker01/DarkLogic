#ifndef TEST_H
#define TEST_H
#include <thread>
#include <queue>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AI/event.h"
#include "MainDarkLogic/utils.h"
#include "testthread.h"


class TestThread;

class Test
{
	enum class Fun
	{
		None,
		CheckHumanIdentity,
		CheckHumanDoubleNot,
		CheckHumanExcludedMiddle,
		CheckPerformanceIdentity,
		CheckPerformanceDoubleNot,
		CheckPerformanceNonContradiction,
		CheckEvalPerformanceIdentity,
		CheckEvalPerformanceDoubleNot,
		CheckEvalPerformanceNonContradiction
	};	

	enum class PlayerType
	{
		None,
		AI,
		EvalAI
	};

	Test(const Fun fun);
	Test(const std::string& name_, const std::string& content_, PlayerType playerType_, const double& timeout_);

	friend class TestThread;
	
public:
	static bool init(const std::unordered_map<ConfigType, std::string>& config);
	static int run();
	void _run();
	
	~Test();
private:
	Fun fun() const;
	PlayerType playerType() const;
	std::string name() const;
	std::string content() const;
	double timeout() const;

	//Test functions
	void checkHumanIdentity();
	void checkHumanDoubleNot();
	void checkHumanExcludedMiddle();
	void checkPerformanceIdentity();
	void checkPerformanceDoubleNot();
	void checkPerformanceNonContradiction();
	void checkEvalPerformanceIdentity();
	void checkEvalPerformanceDoubleNot();
	void checkEvalPerformanceNonContradiction();

	//Generic functions
	void checkPerformance();
	static void check(bool expression, const std::string& msg);
	static void testApply(const std::string& actionStr);

	bool hasEvents();
	void pushEvent(Event::EventEnum type_);
	void popEvent();
	void waitForThreadToEnd();

	static std::unique_ptr<Test> s_instance;
	static std::unordered_map<std::string, Fun> s_stringToTest;
	static std::unordered_map<Fun, std::string> s_testToString;


	//Attributes
	const Fun m_fun;

	const std::string m_name;
	const std::string m_content;
	const PlayerType m_playerType;
	const double m_timeout;

	double m_elapsed_seconds;

	//Handling events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::unique_ptr<TestThread> m_thread;
	std::queue<Event> m_eventQueue;
};

#endif //TEST_H