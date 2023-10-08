/*===--- dbvarprop.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines DbVarProp class which contains associations between variables and SubTheorems for rules in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_DBVARPROP_H
#define DARK_LOGIC_DBVARPROP_H
#include <unordered_map>
#include <vector>
#include "Utils/utils.h"

namespace N_DarkLogic {

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
    const std::vector<ptr<ASubTheorem> >& getHypAssoc(const IDVar& idHypVar) const;
    bool isHypAssocEmpty(const IDVar& idHypVar) const;

    void insertHypEmpty(const IDVar& idHypVar);
    void insertHypAssoc(const IDVar& idHypVar, const ptr<ASubTheorem>& prop);

    bool isTotallyIdentified() const;
    bool applyDefaultPolicy();
    bool applyTruePolicy();
    bool applyTruePolicy(const size_t& logicIdx);
    bool applyFalsePolicy();
    bool applyFalsePolicy(const size_t& logicIdx);
    bool applyVarPolicy(const ptr<Boolean>& varBool);
    bool applyVarPolicy(const ptr<Boolean>& varBool, const size_t& logicIdx);
    static bool isHypVariable(const ptr<AVariable>& var);

    void clear();
    ~DbVarProp() = default;

private:
    //gathers the formula associated to the variable name
    std::unordered_map<IDVar, ptr<IISubTheoremFormula>> m_db;

    std::unordered_map<IDVar,std::vector<ptr<ASubTheorem>>> m_hypAssoc;
};
}
#endif // DARK_LOGIC_DBVARPROP_H
