#ifndef LOGICVARAIBLEEXCEPTION_H
#define LOGICVARAIBLEEXCEPTION_H
#include "Utils/exception.h"
#include <string>

namespace N_Logic
{
class VariableException : public Exception<1>
{
public:
	VariableException(const std::string& nameVar);

	std::string whatStr() const;
private:
	const std::string m_nameVar;
	static const std::string s_endWhat;
};
}


#endif // !LOGICVARAIBLEEXCEPTION_H