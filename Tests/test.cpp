#include "test.h"
#include <iostream>
#include "darklogic.h"
#include "AI/ai.h"
#include "MainDarkLogic/action.h"
#include "MainDarkLogic/logicgame.h"

std::unordered_map<std::string, Test::Fun> Test::s_stringToTest =
{
	{"checkHumanIdentity" , Test::Fun::CheckHumanIdentity},
	{"checkHumanDoubleNot" , Test::Fun::CheckHumanDoubleNot},
	{"checkHumanExcludedMiddle" , Test::Fun::CheckHumanExcludedMiddle},
	{"checkPerformanceIdentity" , Test::Fun::CheckPerformanceIdentity},
	{"checkPerformanceDoubleNot" , Test::Fun::CheckPerformanceDoubleNot},
	{"checkPerformanceNonContradiction" , Test::Fun::CheckPerformanceNonContradiction}
};
std::unordered_map<Test::Fun, std::string> Test::s_testToString = 
{
	{Test::Fun::CheckHumanIdentity, "checkHumanIdentity" },
	{Test::Fun::CheckHumanDoubleNot, "checkHumanDoubleNot" },
	{Test::Fun::CheckHumanExcludedMiddle, "checkHumanExcludedMiddle" },
	{Test::Fun::CheckPerformanceIdentity, "checkPerformanceIdentity"},
	{Test::Fun::CheckPerformanceDoubleNot, "checkPerformanceDoubleNot"},
	{Test::Fun::CheckPerformanceNonContradiction, "checkPerformanceNonContradiction"}
};

std::unique_ptr<Test> Test::s_instance = nullptr;


Test::Test(const Fun fun) : m_fun(fun), m_elapsed_seconds(0), m_hasEvents(false), m_lock(m_mutex)
{

}

bool Test::init(const std::string& testName)
{
	auto it = s_stringToTest.find(testName);
	if (it != s_stringToTest.end())
	{
		s_instance = std::unique_ptr<Test>(new Test(it->second));
		return true;
	}
	else
	{
		return false;
	}	
}

int Test::run()
{
	try
	{
		switch (s_instance->fun())
		{
		case CheckHumanIdentity:
		{
			s_instance->checkHumanIdentity();
			break;
		}
		case CheckHumanDoubleNot:
		{
			s_instance->checkHumanDoubleNot();
			break;
		}
		case CheckHumanExcludedMiddle:
		{
			s_instance->checkHumanExcludedMiddle();
			break;
		}
		case CheckPerformanceIdentity:
		{
			s_instance->checkPerformanceIdentity();
			break;
		}
		case CheckPerformanceDoubleNot:
		{
			s_instance->checkPerformanceDoubleNot();
			break;
		}
		case CheckPerformanceNonContradiction:
		{
			s_instance->checkPerformanceNonContradiction();
			break;
		}
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "[ERROR] " << e.what() << std::endl;
		std::cout << "Test: " + s_testToString[s_instance->fun()] + " [FAIL]" << std::endl;
		std::cout << "****************************************************************************" << std::endl;
		std::cout << "----------------------------------------------------------------------------" << std::endl;
		std::cout << "****************************************************************************" << std::endl;
		return -1;
	}

	std::cout << "Test: " + s_testToString[s_instance->fun()] + " [SUCCESS]" << std::endl;
	std::cout << "****************************************************************************" << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	std::cout << "****************************************************************************" << std::endl;
	return 0;
}

Test::~Test()
{
	waitForThreadToEnd();
}

Test::Fun Test::fun() const
{
	return m_fun;
}

void Test::checkHumanIdentity()
{
	N_DarkLogic::DarkLogic::init(0);
	std::cout << "Identity Demonstration" << std::endl;
	N_DarkLogic::DarkLogic::makeTheorem("identity", "a<=>a");
	N_DarkLogic::DarkLogic::printTheorem();
	std::cout << "__________________________________________________" << std::endl;

	testApply("arr,[0]");

	testApply("arr,[0]");

	testApply("arr,[1]");

	testApply("arr_True,[]");

	check(N_DarkLogic::DarkLogic::isDemonstrated(), "identity theorem was not demonstrated");
}

void Test::checkHumanDoubleNot()
{
	N_DarkLogic::DarkLogic::init(0);
	std::cout << "DoubleNot Demonstration" << std::endl;
	N_DarkLogic::DarkLogic::makeTheorem("doubleNot", "a<=>!!a");
	N_DarkLogic::DarkLogic::printTheorem();
	std::cout << "__________________________________________________" << std::endl;

	testApply("FE,[1]");

	testApply("arr,[0]");

	testApply("arr,[0]");

	testApply("arr_True");

	check(N_DarkLogic::DarkLogic::isDemonstrated(), "DoubleNot theorem was not demonstrated");
}

void Test::checkHumanExcludedMiddle()
{
	N_DarkLogic::DarkLogic::init(0);
	std::cout << "ExcludedMiddle Demonstration" << std::endl;
	N_DarkLogic::DarkLogic::makeTheorem("ExcludedMiddle", "p||!p");
	N_DarkLogic::DarkLogic::printTheorem();
	std::cout << "__________________________________________________" << std::endl;

	testApply("arr");

	testApply("FI!");

	testApply("||Ig_Annexe_0");

	testApply("FE");

	testApply("FI!_Annexe_7");

	testApply("||Ig");

	testApply("ax_Annexe_1");

	check(N_DarkLogic::DarkLogic::isDemonstrated(), "ExcludedMiddle theorem was not demonstrated");
}

void Test::checkPerformanceIdentity()
{
	checkPerformance("identity", "a<=>a", 1.5);
}

void Test::checkPerformanceDoubleNot()
{
	checkPerformance("doubleNot", "a<=>!!a", 5.2); //normally 1.7 
}

void Test::checkPerformanceNonContradiction()
{
	checkPerformance("non-contradiction", "!(a&&!a)", 0.016);
}

void Test::checkPerformance(const std::string& thName, const std::string& thContent, double maxTime)
{	
	std::cout << "CheckPerformance Test on " + thName + " theorem : " << thContent << std::endl;
		
	auto nbThreads = std::thread::hardware_concurrency();
	double realMaxTime = (maxTime * 1.15) * (4.0 / nbThreads);
	
	//start AI
	m_thread = std::thread(Test::testDemonstration, thName, thContent, nbThreads);
	std::chrono::high_resolution_clock::time_point start;
	start = std::chrono::high_resolution_clock::now();	
	std::cv_status status = std::cv_status::no_timeout;
	while (!m_hasEvents && (status != std::cv_status::timeout))
	{
		status = m_condition_var.wait_until(m_lock, start + std::chrono::seconds(20));			
	}
	if (m_eventQueue.size())
	{
		//Consuming event
		m_eventQueue.pop();
	}
	m_hasEvents = false;	

	check(N_DarkLogic::DarkLogic::isDemonstrated(), thName + " cannot be demonstrated");
	std::cout << "Demonstration duration: " << m_elapsed_seconds << std::endl;
	std::stringstream ss;
	ss << realMaxTime;
	std::string maxTimeStr;
	ss >> maxTimeStr;
	check(m_elapsed_seconds < realMaxTime, "Demonstration of "+ thName +" theorem must be done in less than "
		+maxTimeStr+" seconds");

	//clear Logic state
	N_DarkLogic::DarkLogic::clearAll();

}

void Test::check(bool expression, const std::string& msg)
{
	if (!expression)
	{
		throw std::runtime_error(msg);
	}
}

void Test::testApply(const std::string& actionStr)
{
	std::cout << "Apply {" + actionStr + "}" << std::endl;
	check(N_DarkLogic::DarkLogic::applyStr(actionStr), "cannot apply  {" + actionStr + "}");
	N_DarkLogic::DarkLogic::printTheorem();
	std::cout << "__________________________________________________" << std::endl;
}

void Test::testDemonstration(const std::string& name, const std::string& content, unsigned int nbThreads)
{	
	std::cout << "Test AI on " + name + " theorem with " << nbThreads << " cores" << std::endl;
	N_DarkLogic::DarkLogic::DarkLogic::init(nbThreads);
	auto ai = std::make_unique<AI>(nbThreads, AI_TIMEOUT);
	check(N_DarkLogic::DarkLogic::makeTheorem(name, content), "cannot make " + name + " theorem");
	N_DarkLogic::DarkLogic::printTheorem();

	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	while (!N_DarkLogic::DarkLogic::isOver())
	{
		auto action = ai->play();
		N_DarkLogic::DarkLogic::getActions();
		std::cout << ai->name() << " plays action with id " << action->id() << std::endl;
		N_DarkLogic::DarkLogic::apply(action->id());
		N_DarkLogic::DarkLogic::printTheorem();
		std::cout << "____________________________________________________________________________" << std::endl;
	}
	end = std::chrono::high_resolution_clock::now();

	if (N_DarkLogic::DarkLogic::hasAlreadyPlayed())
	{
		if (N_DarkLogic::DarkLogic::isDemonstrated())
		{
			std::cout << ai->name() << " won! " << ai->name() << " finished the demonstration!" << std::endl;
			s_instance->m_elapsed_seconds = std::chrono::duration<double>(end - start).count();
		}
		else if (N_DarkLogic::DarkLogic::isAlreadyPlayed())
		{
			std::cout << ai->name() << " lost! Repetition of theorem!" << std::endl;
		}
		else if (N_DarkLogic::DarkLogic::isEvaluated())
		{
			std::cout << ai->name() << " lost! Cannot (\"back-\")demonstrate that a theorem is false with implications!" << std::endl;
		}
		else /*if (!canBeDemonstrated())*/
		{
			std::cout << ai->name() << " lost! This theorem cannot be demonstrated! "
				"It can be true or false according to the values of its variables" << std::endl;
		}
	}
	else
	{
		if (N_DarkLogic::DarkLogic::isDemonstrated())
		{
			std::cout << "Game Over! the demonstration is already finished!" << std::endl;
		}
		else /*if (!canBeDemonstrated())*/
		{
			std::cout << "Game Over! This theorem cannot be demonstrated! "
				"It can be true or false according to the values of its variables" << std::endl;
		}
	}
	s_instance->pushEvent(Event::STOP);
}

void Test::pushEvent(Event::EventEnum type_)
{
	m_mutex.lock();
	m_eventQueue.push(Event(0, type_));
	m_mutex.unlock();
	m_hasEvents = true;
	m_condition_var.notify_all();
}

void Test::waitForThreadToEnd()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}
