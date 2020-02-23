#include "logichash.h"
#include "property.h"
#include "log.h"

/*
DbProperty::DbProperty(DbProperty &dbProp, DbVarProp *dbVarProp, std::vector<Property *> &hypImpl_)
{
    for(auto it=dbProp.db.begin();it!=dbProp.db.end();it++)
    {
        Property* prop=it->second;
        std::string propName=prop->name();
        if(propName.substr(0,3)==HYP)
        {
            for(unsigned int k=0;k<hypImpl_.size();k++)
            {
                Property* hypProp=hypImpl_[k];
                db[hypProp->name()]=new Property(*hypProp,dbVarProp,hypImpl_);
            }
        }
        else
        {
            db[propName]=new Property(*prop,dbVarProp,hypImpl_);
        }
    }
}*/

/*
std::string DbProperty::toString()
{
    std::string ret="";
    for(auto it=db.begin();it!=db.end();it++)
    {
        Property* prop=it->second;
        ret+=prop->toString();
        ret+=";";
    }
    if(ret!="")
    {
        ret.pop_back();
        ret="{"+ret+"}";
    }
    return ret;
}
*/
