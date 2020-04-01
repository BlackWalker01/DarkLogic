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

    virtual bool isEqual(const ASubRule& prop) const =0;
    virtual bool isEqual(const ASubTheorem& prop) const =0;

    virtual ~IProposition() = default;

    static const std::string s_HYP;
protected:
    const PropType m_type;
};

template<typename T>
concept Proposition = std::is_same_v<T,ASubTheorem> || std::is_same_v<T,ASubRule>;

inline bool operator==(const ASubRule& rule, const ASubTheorem& thm);
inline bool operator==(const ASubTheorem& th, const ASubTheorem& th2);
inline bool operator==(const ASubRule& th, const ASubRule& th2);
}

#endif // IPROPERTY_H
