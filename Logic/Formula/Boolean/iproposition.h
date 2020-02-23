#ifndef IPROPERTY_H
#define IPROPERTY_H
#include "Formula/formula.h"
#include "Set/booleanset.h"

namespace N_Logic {

class DbVarProp;
class DbVar;

typedef size_t Arity;

class ASubRule;
class ASubTheorem;
class AbstractTerm;

class IProposition: public AbstractFormula<bool>
{
public:
    enum PropType
    {
        AND_PROP,
        BELONGSTO_PROP,
        EQUIV_PROP,
        EQUAL_PROP,
        HYP_PROP,
        IMPL_PROP,
        LET_PROP,
        NOT_PROP,
        OR_PROP,
        VAR_PROP,
        CONST_PROP
    };

    IProposition(const std::string& name_, const PropType type_);
    IProposition(const std::string& name_, const DbVar& dbVar, const PropType type_);
    IProposition(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    IProposition(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);

    PropType type() const;    

    //virtual void setSubVar(const std::string& nameVar, bool val)=0;

    //method to get sub properties
    virtual size_t arity() const =0;

    virtual bool operator==(const ASubRule& prop) const =0;
    virtual bool operator==(const ASubTheorem& prop) const =0;

    virtual ~IProposition() = default;

    static const std::string s_HYP;
protected:
    const PropType m_type;
};

}

#endif // IPROPERTY_H
