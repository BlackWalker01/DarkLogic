#ifndef DBVARPROP_H
#define DBVARPROP_H
#include <unordered_map>
#include <vector>
#include "Utils/utils.h"

namespace N_Logic {

class IISubTheoremFormula;
class ASubTheorem;

class DbVarProp
{
public:
    DbVarProp();
    DbVarProp(const DbVarProp& dbVarProp);
    DbVarProp(const std::vector<std::string>& dbVar);

    bool contains(const std::string& nameHypVar) const;
    bool containsHyp(const std::string& hypProp) const;
    const ptr<IISubTheoremFormula>& operator[](const std::string& nameVar) const;
    ptr<IISubTheoremFormula>& operator[](const std::string& nameVar);
    std::vector<ptr<ASubTheorem> > getHypAssoc(const std::string& nameHypVar);

    void insert(const std::string &nameVar, const ptr<ASubTheorem> &prop);
    void insertHypEmpty(const std::string& nameHypVar);
    void insertHypAssoc(const std::string& nameHypVar, const ptr<ASubTheorem>& prop);

    bool isTotallyIdentified() const;
    static bool isHypVariable(const std::string& nameVar);

    void clear();
    ~DbVarProp() = default;

private:
    //gathers the formula associated to the variable name
    std::unordered_map<std::string, ptr<IISubTheoremFormula>> m_db;

    std::unordered_map<std::string,std::vector<std::string>> m_hypMapOrdered;
    std::unordered_map<std::string,std::unordered_map<std::string, ptr<ASubTheorem>>> m_hypAssoc;
};
}
#endif // DBVARPROP_H
