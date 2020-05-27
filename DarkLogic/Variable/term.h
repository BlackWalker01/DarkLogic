/*===--- term.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines AbstractTerm class which is abstract class for all formula terms in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_TERM_H
#define DARK_LOGIC_TERM_H
#include "Set/set.h"
#include <string.h>
#include <stdexcept>
#include <unordered_map>
#include "Formula/valuetypeobject.h"
#include "Utils/utils.h"

namespace N_DarkLogic
{

enum ConstantEnum
{
    FALSE_TYPE,
    TRUE_TYPE,
    BOOLEAN_SET_TYPE,
    NATURALINTEGERSET_TYPE
};

class AbstractTerm: public ValueTypeObject
{
public:
    static std::shared_ptr<AbstractTerm> create(const std::string &name);

    AbstractTerm(const std::string& name);
    AbstractTerm(const AbstractTerm& term);

    std::string name() const;
    std::string toString(unsigned short =1000) const;
    virtual bool isConstant() const=0;

    virtual ~AbstractTerm() = default;
protected:
    const std::string m_name;
    static const std::unordered_map<std::string,ConstantEnum> s_constantHash;
}; 
}

#endif // DARK_LOGIC_TERM_H
