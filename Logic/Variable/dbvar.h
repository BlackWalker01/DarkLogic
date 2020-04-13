#ifndef DBVAR_H
#define DBVAR_H
#include "unordered_map"
#include <vector>
#include <memory>
#include "Utils/utils.h"

namespace N_Logic {

class AVariable;

class DbVar
{
public:
    DbVar() = default;
    DbVar(const ptr<AVariable>& var);
    DbVar(const DbVar *dbVar);
    DbVar(const DbVar* dbVar, const DbVar* dbVar2);
    DbVar(const std::vector<const DbVar*>& dbVars);

    std::vector<IDVar> nameVars() const;
    std::vector<ptr<AVariable>> getVars() const;
    const ptr<AVariable>& find(const IDVar &idVar) const;
    bool contains(const IDVar& name) const;

    ~DbVar() = default;

    static std::unordered_map<IDVar, ptr<AVariable> > createDb(const ptr<AVariable> &var);

    static std::unordered_map<IDVar, ptr<AVariable> > createDb(const DbVar* db);

    static std::unordered_map<IDVar, ptr<AVariable> > merge(const DbVar* db, const DbVar* db2);

    static std::unordered_map<IDVar, ptr<AVariable> > merge(const std::vector<const DbVar*>& dbs);

private:
    const std::unordered_map<IDVar, ptr<AVariable>> db;
};

#include "dbvar.hxx"
}

#endif // DBVAR_H
