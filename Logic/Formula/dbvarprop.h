#ifndef DBVARPROP_H
#define DBVARPROP_H
#include <unordered_map>
#include <vector>
#include "Utils/utils.h"

namespace N_Logic {

class IISubTheoremFormula;
class ASubTheorem;
class AVariable;

class DbVarProp
{
public:
    DbVarProp() = default;
    DbVarProp(const DbVarProp& dbVarProp);
    DbVarProp(const std::vector<ptr<AVariable>>& vars);

    bool contains(const IDVar& idVar) const;
    bool containsHyp(const IDVar& idHyp) const;
    const ptr<IISubTheoremFormula>& operator[](const IDVar& idVar) const;
    ptr<IISubTheoremFormula>& operator[](const IDVar& idVar);
    std::vector<ptr<ASubTheorem> > getHypAssoc(const IDVar& idHypVar);

    void insertHypEmpty(const IDVar& idHypVar);
    void insertHypAssoc(const IDVar& idHypVar, const ptr<ASubTheorem>& prop);

    bool isTotallyIdentified() const;
    static bool isHypVariable(const ptr<AVariable>& var);

    void clear();
    ~DbVarProp() = default;

private:
    //gathers the formula associated to the variable name
    std::unordered_map<IDVar, ptr<IISubTheoremFormula>> m_db;

    std::unordered_map<IDVar,std::vector<ptr<ASubTheorem>>> m_hypAssoc;
};
}
#endif // DBVARPROP_H
