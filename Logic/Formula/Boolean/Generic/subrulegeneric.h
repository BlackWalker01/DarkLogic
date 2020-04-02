#ifndef SUBRULEGENERIC_H
#define SUBRULEGENERIC_H
#include "Formula/Boolean/subrule.h"
#include "Formula/Boolean/asubpurerule.h"
#include "Operator/Boolean/and.h"
#include "Operator/Boolean/equivalent.h"
#include "Operator/Boolean/implication.h"
#include "Operator/Boolean/or.h"

namespace N_Logic
{
template<SubRuleProperty SubPropertyType>
class SubRule: public ASubPureRule
{
public:
    SubRule(const std::string& name, const ptr<ASubRule>& leftSubProp, const ptr<ASubRule>& rightSubProp);

    bool evaluate() const override final;

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<SubPropertyType>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;
    const SubPropertyType& getSon() const;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const override final;

    const ptr<ASubRule>& operator[](const size_t& index) const override final;
    size_t arity() const override final;

protected:
    const std::unique_ptr<const SubPropertyType> m_son;
};

/**
 * IdentifyPriv methods
 */
template<>
bool SubRule<And<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Equivalent<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Implication<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;

template<>
bool SubRule<Or<ASubRule>>::identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const;


/**
 * applyPriv methods
 */
template<>
ptr<ASubTheorem> SubRule<And<ASubRule>>::applyPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Equivalent<ASubRule>>::applyPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Implication<ASubRule>>::applyPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Or<ASubRule>>::applyPriv(const std::string& thName, DbVarProp& dbVarProp) const;

/**
 * applyFirstPriv methods
 */
template<>
ptr<ASubTheorem> SubRule<And<ASubRule>>::applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Equivalent<ASubRule>>::applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Implication<ASubRule>>::applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const;

template<>
ptr<ASubTheorem> SubRule<Or<ASubRule>>::applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const;

}

#endif // SUBRULEGENERIC_H
