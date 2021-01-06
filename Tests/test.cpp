#include "test.h"
#include <iostream>
#include <sstream>
#include "darklogic.h"
#include "AI/ai.h"
#include "AI/evalai.h"
#include "MainDarkLogic/action.h"
#include "MainDarkLogic/logicgame.h"

std::unordered_map<std::string, Test::Fun> Test::s_stringToTest =
{
	{"checkHumanIdentity" , Test::Fun::CheckHumanIdentity},
	{"checkHumanDoubleNot" , Test::Fun::CheckHumanDoubleNot},
	{"checkHumanExcludedMiddle" , Test::Fun::CheckHumanExcludedMiddle},
	{"checkPerformanceIdentity" , Test::Fun::CheckPerformanceIdentity},
	{"checkPerformanceDoubleNot" , Test::Fun::CheckPerformanceDoubleNot},
	{"checkPerformanceNonContradiction" , Test::Fun::CheckPerformanceNonContradiction},
	{"checkEvalPerformanceIdentity" , Test::Fun::CheckEvalPerformanceIdentity},
	{"checkEvalPerformanceDoubleNot" , Test::Fun::CheckEvalPerformanceDoubleNot},
	{"checkEvalPerformanceNonContradiction" , Test::Fun::CheckEvalPerformanceNonContradiction}
};
std::unordered_map<Test::Fun, std::string> Test::s_testToString = 
{
	{Test::Fun::CheckHumanIdentity, "checkHumanIdentity" },
	{Test::Fun::CheckHumanDoubleNot, "checkHumanDoubleNot" },
	{Test::Fun::CheckHumanExcludedMiddle, "checkHumanExcludedMiddle" },
	{Test::Fun::CheckPerformanceIdentity, "checkPerformanceIdentity"},
	{Test::Fun::CheckPerformanceDoubleNot, "checkPerformanceDoubleNot"},
	{Test::Fun::CheckPerformanceNonContradiction, "checkPerformanceNonContradiction"},
	{Test::Fun::CheckEvalPerformanceIdentity, "checkEvalPerformanceIdentity"},
	{Test::Fun::CheckEvalPerformanceDoubleNot, "checkEvalPerformanceDoubleNot"},
	{Test::Fun::CheckEvalPerformanceNonContradiction, "checkEvalPerformanceNonContradiction"}
};

std::unique_ptr<Test> Test::s_instance = nullptr;


Test::Test(const Fun fun) : m_fun(fun), m_name(""), m_content(""), m_playerType(PlayerType::None), m_timeout(0.0),
m_elapsed_seconds(0), m_hasEvents(false), m_lock(m_mutex), m_thread(nullptr)
{

}

Test::Test(const std::string& name_, const std::string& content_, PlayerType playerType_, const double& timeout_) : 
	m_fun(Fun::None), m_name(name_), m_content(content_), m_playerType(playerType_), m_timeout(timeout_),
m_elapsed_seconds(0), m_hasEvents(false), m_lock(m_mutex), m_thread(new TestThread(*this))
{

}

bool Test::init(const std::unordered_map<ConfigType, std::string>& config)
{
	std::string testName = "";
	auto itName = config.find(ConfigType::TESTNAME);
	if (itName != config.end())
	{
		testName = itName->second;
	}
	else
	{
		std::cout << "[ERROR] test mode needs testName argument! Exit..." << std::endl;
		return false;
	}
	auto it = s_stringToTest.find(testName);
	if (it != s_stringToTest.end())
	{
		s_instance = std::unique_ptr<Test>(new Test(it->second));
		return true;
	}
	else
	{
		std::string content = "";
		auto itContent = config.find(ConfigType::CONTENT);
		if (itContent != config.end())
		{
			content = itContent->second;
		}
		else
		{
			std::cout << "[ERROR] test mode needs 'content' argument when testName is not registered! Exit..." << std::endl;
			return false;
		}

		PlayerType playerType = PlayerType::None;
		auto itPlayer = config.find(ConfigType::PLAYER);
		if (itPlayer != config.end())
		{
			std::string playerStr = itPlayer->second;
			if (playerStr == "ai" || playerStr == "Ai" || playerStr == "AI")
			{
				playerType = PlayerType::AI;
			}
			else if (playerStr == "evalai" || playerStr == "evalAi" || playerStr == "EvalAi" || playerStr == "EvalAI" || playerStr == "EVALAI")
			{
				playerType = PlayerType::EvalAI;
			}
			else
			{
				std::cout << "[ERROR] player argument should be 'AI' or 'EvalAI'! Exit..." << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "[ERROR] test mode needs 'player' argument when testName is not registered! Exit..." << std::endl;
		}

		double timeout = 0.0;
		auto itTimeout = config.find(ConfigType::TIMEOUT);
		if (itTimeout != config.end())
		{
			std::stringstream ss;
			ss << itTimeout->second;
			ss >> timeout;
		}
		else
		{
			std::cout << "[ERROR] test mode needs 'timeout' argument when testName is not registered! Exit..." << std::endl;
			return false;
		}
		s_instance = std::unique_ptr<Test>(new Test(testName, content, playerType, timeout));
		return true;
	}	
}

int Test::run()
{
	try
	{
		switch (s_instance->fun())
		{
		case Fun::None:
		{
			s_instance->_run();
			break;
		}
		case Fun::CheckHumanIdentity:
		{
			s_instance->checkHumanIdentity();
			break;
		}
		case Fun::CheckHumanDoubleNot:
		{
			s_instance->checkHumanDoubleNot();
			break;
		}
		case Fun::CheckHumanExcludedMiddle:
		{
			s_instance->checkHumanExcludedMiddle();
			break;
		}
		case Fun::CheckPerformanceIdentity:
		{
			//s_instance->checkPerformanceIdentity();
			break;
		}
		case Fun::CheckPerformanceDoubleNot:
		{
			//s_instance->checkPerformanceDoubleNot();
			break;
		}
		case Fun::CheckPerformanceNonContradiction:
		{
			//s_instance->checkPerformanceNonContradiction();
			break;
		}
		case Fun::CheckEvalPerformanceIdentity:
		{
			//s_instance->checkEvalPerformanceIdentity();
			break;
		}
		case Fun::CheckEvalPerformanceDoubleNot:
		{
			//s_instance->checkEvalPerformanceDoubleNot();
			break;
		}
		case Fun::CheckEvalPerformanceNonContradiction:
		{
			//s_instance->checkEvalPerformanceNonContradiction();
			break;
		}
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "[ERROR] " << e.what() << std::endl;
		if (s_instance->fun() == Fun::None)
		{
			std::cout << "Test: " + s_instance->name() + " [FAIL]" << std::endl;
		}
		else
		{
			std::cout << "Test: " + s_testToString[s_instance->fun()] + " [FAIL]" << std::endl;
		}
		std::cout << "****************************************************************************" << std::endl;
		std::cout << "----------------------------------------------------------------------------" << std::endl;
		std::cout << "****************************************************************************" << std::endl;
		return -1;
	}

	if (s_instance->fun() == Fun::None)
	{
		std::cout << "Test: " + s_instance->name() + " [SUCCESS]" << std::endl;
	}
	else
	{
		std::cout << "Test: " + s_testToString[s_instance->fun()] + " [SUCCESS]" << std::endl;
	}
	std::cout << "****************************************************************************" << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	std::cout << "****************************************************************************" << std::endl;
	return 0;
}

void Test::_run()
{
	checkPerformance();
}

Test::~Test()
{
	if (m_thread)
	{
		if (m_thread->hasStarted())
		{
			m_thread->stop();
		}
		waitForThreadToEnd();
	}	
}

Test::Fun Test::fun() const
{
	return m_fun;
}

Test::PlayerType Test::playerType() const
{
	return m_playerType;
}

std::string Test::name() const
{
	return m_name;
}

std::string Test::content() const
{
	return m_content;
}

double Test::timeout() const
{
	return m_timeout;
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

/*void Test::checkPerformanceIdentity()
{
	checkPerformance<AI>("identity", "a<=>a", 1.5);
}

void Test::checkPerformanceDoubleNot()
{
	checkPerformance<AI>("doubleNot", "a<=>!!a", 5.2); //normally 1.7 
}

void Test::checkPerformanceNonContradiction()
{
	checkPerformance<AI>("non-contradiction", "!(a&&!a)", 0.016);
}

void Test::checkEvalPerformanceIdentity()
{
	checkPerformance<EvalAI>("identity", "a<=>a", 0.2);
}

void Test::checkEvalPerformanceDoubleNot()
{
	checkPerformance<EvalAI>("doubleNot", "a<=>!!a", 2.0); //normally 1.7 
}

void Test::checkEvalPerformanceNonContradiction()
{
	checkPerformance<EvalAI>("non-contradiction", "!(a&&!a)", 0.016);
}*/

void Test::checkPerformance()
{	
	std::cout << "Check performance on " + m_name + " theorem : " << m_content << std::endl;
		
	auto nbThreads = std::thread::hardware_concurrency();
	double maxTime = (m_timeout) * (4.0 / nbThreads);
	double realMaxTime = (maxTime > TIMEOUT_MIN)? maxTime : TIMEOUT_MIN;
	
	//Initialize AI environment
	std::string aiName = "";
	switch (m_playerType)
	{
	case PlayerType::None:
	{
		break;
	}
	case PlayerType::AI:
	{
		aiName = "BasicAI";
		break;
	}
	case PlayerType::EvalAI:
	{
		aiName = "EvalAI";
		break;
	}
	}
	std::cout << "Test "+aiName+" on " + m_name + " theorem ("<<m_content<<") with " << nbThreads << " cores" << std::endl;
	N_DarkLogic::DarkLogic::DarkLogic::init(nbThreads);
	Test::check(N_DarkLogic::DarkLogic::makeTheorem(m_name, m_content), "cannot make " + m_name + " theorem");
	
	//start AI
	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	m_thread->start();
		
	std::cv_status status = std::cv_status::no_timeout;
	while (!m_hasEvents && (status != std::cv_status::timeout))
	{
		status = m_condition_var.wait_until(m_lock, start + std::chrono::seconds(static_cast<size_t>(realMaxTime)));			
	}
	if (status == std::cv_status::timeout)
	{
		m_thread->stop();
	}

	//waiting for thread to stop
	while (!m_hasEvents)
	{
		m_condition_var.wait(m_lock);
	}
	if (m_eventQueue.size())
	{
		//Consuming event
		m_eventQueue.pop();
	}
	m_hasEvents = false;

	end = std::chrono::high_resolution_clock::now();
	double elapsed_seconds = std::chrono::duration<double>(end - start).count();

	std::cout << "Timeout is " << maxTime << std::endl;
	std::cout << "Attempt duration is " << elapsed_seconds << " seconds" << std::endl;
	check(N_DarkLogic::DarkLogic::isDemonstrated(), m_name + " cannot be demonstrated");
	check(elapsed_seconds < maxTime, "Timeout!!");

	if (N_DarkLogic::DarkLogic::hasAlreadyPlayed())
	{
		if (N_DarkLogic::DarkLogic::isDemonstrated())
		{
			std::cout << aiName+" won! "+aiName+" finished the demonstration!" << std::endl;
		}
		else if (N_DarkLogic::DarkLogic::isAlreadyPlayed())
		{
			std::cout << aiName + " lost! Repetition of theorem!" << std::endl;
		}
		else if (N_DarkLogic::DarkLogic::isEvaluated())
		{
			std::cout << aiName + " lost! Cannot (\"back-\")demonstrate that a theorem is false with implications!" << std::endl;
		}
		else if (!N_DarkLogic::DarkLogic::canBeDemonstrated())
		{
			std::cout << aiName + " lost! This theorem cannot be demonstrated! "
				"It can be true or false according to the values of its variables" << std::endl;
		}
		else
		{
			std::cout << aiName + " lost! "
				"It could not find demonstration before timeout :/" << std::endl;
		}
	}
	else
	{
		if (N_DarkLogic::DarkLogic::isDemonstrated())
		{
			std::cout << "Game Over! the demonstration is already finished!" << std::endl;
		}
		else
		{
			std::cout << "Game Over! This theorem cannot be demonstrated! "
				"It can be true or false according to the values of its variables" << std::endl;
		}
	}

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

bool Test::hasEvents()
{
	bool ret = false;
	m_mutex.lock();
	ret = m_hasEvents;
	m_mutex.unlock();
	return ret;
}

void Test::pushEvent(Event::EventEnum type_)
{
	m_mutex.lock();
	m_eventQueue.push(Event(0, type_));
	m_mutex.unlock();
	m_hasEvents = true;
	m_condition_var.notify_all();
}

void Test::popEvent()
{
	m_mutex.lock();
	m_eventQueue.pop();
	m_mutex.unlock();
}

void Test::waitForThreadToEnd()
{
	if (m_thread->joinable())
	{
		m_thread->join();
	}
}
