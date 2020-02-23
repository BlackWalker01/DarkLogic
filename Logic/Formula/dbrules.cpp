#include "dbrules.h"
#include "rule.h"

DbRules::DbRules()
{

}


Rule* DbRules::find(std::string name)
{
    auto it=db.find(name);
    if(it!=db.end())
    {
        return it->second;
    }
    return nullptr;
}

void DbRules::insert(Rule *prop)
{
    db[prop->name()]=prop;
}


unsigned int DbRules::size()
{
    return db.size();
}


DbRules::~DbRules()
{
    for(auto it=db.begin();it!=db.end();it++)
    {
        Rule* prop=it->second;
        delete prop;
        prop=nullptr;
    }
    db.clear();
}
