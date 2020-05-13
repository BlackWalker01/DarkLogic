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
#include "Utils/action.h"
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
    virtual std::pair<ptr<ASubTheorem>, bool> apply(const Action::Id& actionKey) const;
    virtual ptr<ATheoremType> apply(const Action::Id& actionKey, const ptr<ATheoremType>& theorem, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes) const;
    virtual ptr<ASubTheorem> apply(const Action::Id& actionKey, const ptr<ASubTheorem>& theorem, DbVarProp& dbVarProp,
        std::vector<Arity>& indexes, const size_t& logicIdx) const;
    virtual std::vector<Action::Id> getActions(const ptr<ATheoremType>& prop, Action::Id& lastActionIndex) const;
    virtual std::vector<Action::Id> getActions(const ptr<ATheoremType>& prop, Action::Id& lastActionIndex, const size_t& logicIdx) const;
    virtual std::vector<Action> getHumanActions(const ptr<ASubTheorem>&, Action::Id& lastActionIndex) const;

    //create equivalent subRuleFormulas
    virtual std::vector<ptr<ARuleType>> getEquivalentRules() const;

    //store actions method
    virtual void storeActions() const;
    virtual void unstoreActions() const;

    //internal methods
    virtual bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyFirstPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const = 0;
    virtual ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyPriv(DbVarProp& dbVarProp, const size_t& logicIdx) const = 0;

    virtual ~ISubRuleFormula() = default;
};


template<SubTheoremType ATheoremType_>
inline std::pair<ptr<ASubTheorem>, bool> ISubRuleFormula<ATheoremType_>::apply(const Action::Id& /*actionKey*/) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<SubTheoremType ATheoremType>
ptr<ATheoremType> ISubRuleFormula<ATheoremType>::apply(const Action::Id &/*actionKey*/, const ptr<ATheoremType> &/*thName*/, 
    DbVarProp& dbVarProp, std::vector<Arity>& indexes) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<SubTheoremType ATheoremType_>
inline ptr<ASubTheorem> ISubRuleFormula<ATheoremType_>::apply(const Action::Id& actionKey, 
    const ptr<ASubTheorem>& theorem, DbVarProp& dbVarProp, std::vector<Arity>& indexes, const size_t& logicIdx) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<SubTheoremType ATheoremType>
std::vector<Action::Id> ISubRuleFormula<ATheoremType>::getActions(const ptr<ATheoremType> &, Action::Id &) const
{
    throw std::runtime_error("Cannot get possible actions from a subrule formula");
}

template<SubTheoremType ATheoremType_>
inline std::vector<Action::Id> ISubRuleFormula<ATheoremType_>::getActions(const ptr<ATheoremType>& /*prop*/, Action::Id& /*lastActionIndex*/, 
    const size_t& /*logicIdx*/) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<SubTheoremType ATheoremType>
std::vector<Action> ISubRuleFormula<ATheoremType>::getHumanActions(const ptr<ASubTheorem>&, Action::Id&) const
{
    throw std::runtime_error("Cannot get possible human actions from a subrule formula");
}

template<SubTheoremType ATheoremType_>
inline std::vector<ptr<typename ISubRuleFormula<ATheoremType_>::ARuleType>>
ISubRuleFormula<ATheoremType_>::getEquivalentRules() const
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
