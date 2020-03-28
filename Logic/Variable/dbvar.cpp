#include "dbvar.h"
#include "term.h"

using namespace N_Logic;

DbVar::DbVar(const std::shared_ptr<const AbstractTerm> &var): db(createDb(var))
{

}

DbVar::DbVar(const DbVar &dbVar): db(dbVar.db)
{

}

DbVar::DbVar(const DbVar &dbVar, const DbVar &dbVar2): db(merge(dbVar,dbVar2))
{

}

DbVar::DbVar(const std::vector<DbVar>& dbVars): db(merge((dbVars)))
{

}


const std::shared_ptr<const AbstractTerm> &DbVar::find(const std::string &name)
{
    return db[name];
}


std::vector<std::string> DbVar::nameVars() const
{
    std::vector<std::string> ret;
    for(auto it=db.begin();it!=db.end();it++)
    {
        ret.push_back(it->first);
    }
    return ret;
}

std::vector<std::shared_ptr<const AbstractTerm>> N_Logic::DbVar::getVars() const
{
	std::vector<std::shared_ptr<const AbstractTerm>> ret;
    for (const auto& var : db)
    {
        ret.push_back(var.second);
    }
    return ret;
}


bool DbVar::contains(const std::string& name) const
{
    return db.find(name)!=db.end();
}

std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> >
DbVar::createDb(const std::shared_ptr<const AbstractTerm> &var)
{
    std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> > ret;
    ret[var->name()]=var;
    return ret;
}

std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> >
DbVar::merge(const DbVar &db, const DbVar &db2)
{
    std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> > ret=db.db;
    ret.insert(db2.db.begin(),db2.db.end());
    return ret;
}

std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> >
DbVar::merge(const std::vector<DbVar> &dbs)
{
    std::unordered_map<std::string, std::shared_ptr<const AbstractTerm> > ret;
    for(const auto& db: dbs)
    {
        ret.insert(db.db.begin(),db.db.end());
    }
    return ret;
}
