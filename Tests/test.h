#ifndef TEST_H
#define TEST_H
#include <thread>
#include <queue>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include "AI/event.h"

class Test
{
	enum Fun
	{
		CheckHumanIdentity,
		CheckHumanDoubleNot,
		CheckHumanExcludedMiddle,
		CheckPerformanceIdentity,
		CheckPerformanceDoubleNot,
		CheckPerformanceNonContradiction
	};	

	Test(const Fun fun);

	
public:
	static bool init(const std::string& testName);
	static int run();
	
	~Test();
private:
	Fun fun() const;
	//Test functions
	void checkHumanIdentity();
	void checkHumanDoubleNot();
	void checkHumanExcludedMiddle();
	void checkPerformanceIdentity();
	void checkPerformanceDoubleNot();
	void checkPerformanceNonContradiction();

	//Generic functions
	void checkPerformance(const std::string& thName, const std::string& thContent, double maxTime);
	static void check(bool expression, const std::string& msg);
	static void testApply(const std::string& actionStr);
	static void testDemonstration(const std::string& name, const std::string& content, unsigned int nbThreads);

	void pushEvent(Event::EventEnum type_);
	void waitForThreadToEnd();

	static std::unique_ptr<Test> s_instance;
	static std::unordered_map<std::string, Fun> s_stringToTest;
	static std::unordered_map<Fun, std::string> s_testToString;

	const Fun m_fun;
	double m_elapsed_seconds;

	//Handling events
	bool m_hasEvents;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condition_var;
	std::thread m_thread;
	std::queue<Event> m_eventQueue;
};

#endif //TEST_H