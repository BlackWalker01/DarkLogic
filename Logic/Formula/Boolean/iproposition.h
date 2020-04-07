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

    IProposition() = default;

    virtual constexpr PropType type() const=0;

    //method to get sub properties
    virtual size_t arity() const =0;

    virtual bool isEqual(const ASubRule& prop) const =0;
    virtual bool isEqual(const ASubTheorem& prop) const =0;

    virtual ~IProposition() = default;
};

inline bool operator==(const ASubRule& rule, const ASubTheorem& thm);
inline bool operator==(const ASubTheorem& th, const ASubTheorem& th2);
inline bool operator==(const ASubRule& th, const ASubRule& th2);
}

#endif // IPROPERTY_H
