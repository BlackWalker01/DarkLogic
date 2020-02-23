#ifndef ISUBRULEFORMULA_H
#define ISUBRULEFORMULA_H
#include <vector>
#include <string>
#include <stdexcept>
#include "iisubruleformula.h"

namespace N_Logic
{

class DbVarProp;

template<typename ATheoremType_>
class ISubRuleFormula: public IISubRuleFormula
{
public:
    typedef ATheoremType_ ATheoremType;

    ISubRuleFormula() = default;

    //method used by Logic
    virtual ptr<ATheoremType> apply(const size_t& actionKey, const ptr<ATheoremType>& theorem) const;
    virtual void unapply() const;
    virtual std::vector<size_t> getActions(const ptr<ATheoremType>& prop, const size_t& nbActions, size_t& lastActionIndex) const;

    //internal methods
    virtual bool identifyPriv(const ptr<ATheoremType>& prop, DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyFirstPriv(const std::string& thName, DbVarProp& dbVarProp) const=0;
    virtual ptr<ATheoremType> applyPriv(const std::string& thName, DbVarProp& dbVarProp) const=0;

    virtual ~ISubRuleFormula() = default;
};


template<typename ATheoremType>
ptr<ATheoremType> ISubRuleFormula<ATheoremType>::apply(const size_t &/*actionKey*/, const ptr<ATheoremType> &/*thName*/) const
{
    throw std::runtime_error("Cannot apply subrule formula");
}

template<typename ATheoremType>
void ISubRuleFormula<ATheoremType>::unapply() const
{
    throw std::runtime_error("Cannot unapply subrule formula");
}

template<typename ATheoremType>
std::vector<size_t> ISubRuleFormula<ATheoremType>::getActions(const ptr<ATheoremType> &, const size_t &, size_t &) const
{
    throw std::runtime_error("Cannot get possible actions from a subrule formula");
}

}
#endif // ISUBRULEFORMULA_H
