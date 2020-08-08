/*===--- ioperator.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines operator interface which is base class for operators in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IOPERATOR_H
#define DARK_LOGIC_IOPERATOR_H
#include <cstddef>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Utils/utils.h"

namespace N_DarkLogic {

class IOperator
{
public:
    IOperator() = default;
    IOperator(const IOperator& ope) = default;

    virtual Name name() const=0;
    virtual Arity arity() const=0;
    virtual unsigned short priority() const=0;
    virtual Associativity associativity() const=0;
    virtual std::string symbol() const=0;

    virtual ~IOperator() = default;
};
}
#endif // DARK_LOGIC_IOPERATOR_H
