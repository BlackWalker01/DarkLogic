/*===--- dbvar.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements dbvar class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "dbvar.h"
#include "avariable.h"

using namespace N_DarkLogic;

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

std::vector<ptr<AVariable>> N_DarkLogic::DbVar::getVars() const
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

std::unordered_map<IDVar, ptr<AVariable>> N_DarkLogic::DbVar::createDb(const DbVar* db)
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
