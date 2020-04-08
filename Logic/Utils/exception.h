#ifndef LOGICEXCEPTION_H
#define LOGICEXCEPTION_H
#include <exception>

namespace N_Logic
{
	template<unsigned short Id_>
	class Exception: public std::exception
	{
	public:
		Exception() = default;

		constexpr unsigned short Id()
		{
			return Id_;
		}
	};
}

#endif