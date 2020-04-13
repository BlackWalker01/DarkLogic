/*===--- isubruleformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ISubRuleFormula in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef ISUBRULEFORMULA_H
#define ISUBRULEFORMULA_H
#include <vector>
#include <string>
#include <stdexcept>
#include "iisubruleformula.h"
#include "Utils/utils.h"
#include "Boolean/asubtheorem.h"

namespace N_DarkLogic
{

class DbVarProp;
class Action;

class ASubTheorem;

template<typename T>
concept SubTheoremType = std::is_same_v<T, ASubTheorem> || std::is_same_v<T, ASubArithmeticTheorem<typename T::ValueType>>;

template<SubTheoremType ATheoremType_>
class ISubRuleFormula: public IISubRuleFormula
{
public:
    using ATheoremType = ATheoremType_;
    using ARuleType = ToRule<ATheoremType>;

    ISubRuleFormula() = default;

    //method used by DarkLogic
    virtual std::pair<ptr<ATheoremType>,bool> apply(const size_t& actionKey, const ptr<ATheoremType>& theorem) const;
    virtual std::vector<size_t> getActions(const ptr<ATheoremType>& prop, size_t& lastActionIndex) const;
    virtual std::vector<Action> getHumanActions() const;

    //create equivalent subRuleFormulas
    virtual std::vector<ptr<ARuleType>> getEquivalentRules() const;

    //store actions method
    virtual void storeActions() const;
    virtual void unstoreActions() const;

    //internal methods
    virtual bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const=0;

    virtual ~ISubRuleFormula() = default;
};


template<SubTheoremType ATheoremType>
std::pair<ptr<ATheoremType>,bool> ISubRuleFormula<ATheoremType>::apply(const size_t &/*actionKey*/, const ptr<ATheoremType> &/*thName*/) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<SubTheoremType ATheoremType>
std::vector<size_t> ISubRuleFormula<ATheoremType>::getActions(const ptr<ATheoremType> &, size_t &) const
{
    throw std::runtime_error("Cannot get possible actions from a subrule formula");
}

template<SubTheoremType ATheoremType>
std::vector<Action> ISubRuleFormula<ATheoremType>::getHumanActions() const
{
    throw std::runtime_error("Cannot get possible human actions from a subrule formula");
}

template<SubTheoremType ATheoremType_>
inline std::vector<ptr<ToRule<ATheoremType_>>> ISubRuleFormula<ATheoremType_>::getEquivalentRules() const
{
    return std::vector<ptr<ARuleType>>();
}

template<SubTheoremType ATheoremType_>
inline void ISubRuleFormula<ATheoremType_>::storeActions() const
{
    throw std::runtime_error("Cannot call storeAction method with subrule formula");
}

template<SubTheoremType ATheoremType_>
inline void ISubRuleFormula<ATheoremType_>::unstoreActions() const
{
    throw std::runtime_error("Cannot call unstoreAction method with subrule formula");
}

}
#endif // ISUBRULEFORMULA_H
