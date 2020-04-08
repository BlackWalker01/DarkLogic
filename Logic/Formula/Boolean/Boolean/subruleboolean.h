#ifndef SUBRULEBOOLEAN_H
#define SUBRULEBOOLEAN_H
#include "Formula/Boolean/subrule.h"
#include "Variable/Boolean/boolean.h"

namespace N_Logic
{
template<>
class SubRule<Boolean>: public ASubRule
{
public:
    typedef Boolean SubPropertyType;

    SubRule(const std::shared_ptr<Boolean>& val);

    bool evaluate() const override final;
    constexpr PropType type() const override final
    {
        return PropType::VAR_PROP;
    }

    bool isEqual(const ASubRule& prop) const override final;
    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubRule& prop) const;
    bool operator==(const SubTheorem<SubPropertyType>& prop) const;


    std::string toString(unsigned short priorityParent=1000) const override final;
    const DbVar* getExtVars() const override final;
    const SubPropertyType& getSon() const;

    ~SubRule() override = default;

protected:
    bool identifyPriv(const ptr<ASubTheorem>& prop, DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyPriv(DbVarProp& dbVarProp) const override final;
    ptr<ASubTheorem> applyFirstPriv(DbVarProp& dbVarProp) const override final;
    size_t arity() const override final;

protected:
    const std::shared_ptr<SubPropertyType> m_son;
    const DbVar m_extVars;
};
}

#endif // SUBRULEBOOLEAN_H
