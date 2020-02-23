#ifndef TERM_H
#define TERM_H
#include "Set/set.h"
#include <string.h>
#include <stdexcept>
#include <unordered_map>
#include "Formula/valuetypeobject.h"
#include "Utils/utils.h"

namespace N_Logic
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

#endif // TERM_H
