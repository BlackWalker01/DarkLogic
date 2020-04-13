#include "dbvar.h"
#include "avariable.h"

using namespace N_Logic;

DbVar::DbVar(const ptr<AVariable> &var): db(createDb(var))
{

}

DbVar::DbVar(const DbVar *dbVar): db(createDb(dbVar))
{

}

DbVar::DbVar(const DbVar *dbVar, const DbVar *dbVar2): db(merge(dbVar,dbVar2))
{

}

DbVar::DbVar(const std::vector<const DbVar*>& dbVars): db(merge((dbVars)))
{

}


const ptr<AVariable> &DbVar::find(const IDVar &idVar) const
{
    return db.at(idVar);
}


std::vector<IDVar> DbVar::nameVars() const
{
    std::vector<IDVar> ret;
    for(auto it=db.begin();it!=db.end();it++)
    {
        ret.push_back(it->first);
    }
    return ret;
}

std::vector<ptr<AVariable>> N_Logic::DbVar::getVars() const
{
	std::vector<ptr<AVariable>> ret;
    for (const auto& var : db)
    {
        ret.push_back(var.second);
    }
    return ret;
}


bool DbVar::contains(const IDVar& idVar) const
{
    return db.find(idVar)!=db.end();
}

std::unordered_map<IDVar, ptr<AVariable> >
DbVar::createDb(const ptr<AVariable> &var)
{
    std::unordered_map<IDVar, ptr<AVariable> > ret;
    ret[var->id()]=var;
    return ret;
}

std::unordered_map<IDVar, ptr<AVariable>> N_Logic::DbVar::createDb(const DbVar* db)
{
    std::unordered_map<IDVar, ptr<AVariable>> ret;
    if (db)
    {
        ret.insert(db->db.begin(), db->db.end());
    }
    return ret;
}

std::unordered_map<IDVar, ptr<AVariable> >
DbVar::merge(const DbVar *db, const DbVar *db2)
{
    std::unordered_map<IDVar, ptr<AVariable> > ret;
    if (db)
    {
        ret.insert(db->db.begin(), db->db.end());
    }
    if (db2)
    {
        ret.insert(db2->db.begin(), db2->db.end());
    }
    return ret;
}

std::unordered_map<IDVar, ptr<AVariable> >
DbVar::merge(const std::vector<const DbVar*> &dbs)
{
    std::unordered_map<IDVar, ptr<AVariable> > ret;
    for(const auto& db: dbs)
    {
        if (db)
        {
            ret.insert(db->db.begin(), db->db.end());
        }        
    }
    return ret;
}
