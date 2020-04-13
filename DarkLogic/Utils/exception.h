/*===--- exception.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines exception for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_EXCEPTION_H
#define DARK_LOGIC_EXCEPTION_H
#include <exception>

namespace N_DarkLogic
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