#include "dbvarprop.h"
#include "Boolean/asubtheorem.h"

using namespace N_Logic;

DbVarProp::DbVarProp()
{

}

DbVarProp::DbVarProp(const DbVarProp& dbVarProp): m_db(dbVarProp.m_db)
{

}

DbVarProp::DbVarProp(const std::vector<std::string>& dbVar)
{
    for(size_t k=0;k<dbVar.size();k++)
    {
        if(!isHypVariable(dbVar[k]))
        {
            m_db[dbVar[k]]=nullptr;
        }
        else
        {
            m_hypAssoc[dbVar[k]]={};
        }
    }
}

bool DbVarProp::contains(const std::string& nameHypVar) const
{
    return m_hypAssoc.find(nameHypVar)!=m_hypAssoc.end();
}

bool DbVarProp::containsHyp(const std::string& hypProp) const
{
    return m_hypAssoc.find(hypProp)!=m_hypAssoc.end();
}

const ptr<IISubTheoremFormula>& DbVarProp::operator[](const std::string& nameVar) const
{
    return m_db.find(nameVar)->second;
}

ptr<IISubTheoremFormula> &DbVarProp::operator[](const std::string &nameVar)
{
    return m_db[nameVar];
}


void DbVarProp::insert(const std::string &nameVar, const ptr<ASubTheorem>& prop)
{
    m_db[nameVar]=prop;
}

void DbVarProp::insertHypEmpty(const std::string& nameHypVar)
{
    m_hypAssoc[nameHypVar]={};
}

void DbVarProp::insertHypAssoc(const std::string& nameHypVar, const ptr<ASubTheorem>& prop)
{
    std::string key=prop->toString();
    m_hypMapOrdered[nameHypVar].push_back(key);
    m_hypAssoc[nameHypVar][key]=prop;
}

std::vector<ptr<ASubTheorem>> DbVarProp::getHypAssoc(const std::string& nameHypVar)
{
    std::vector<ptr<const ASubTheorem>> ret;
    for(size_t k=0;k<m_hypMapOrdered[nameHypVar].size();k++)
    {
        ret.push_back(m_hypAssoc[nameHypVar][m_hypMapOrdered[nameHypVar][k]]);
    }
    return ret;
}

bool DbVarProp::isTotallyIdentified() const
{
    for(auto it=m_db.begin();it!=m_db.end();it++)
    {
        if(!(it->second))
        {
            return false;
        }
    }
    return true;
}

bool DbVarProp::isHypVariable(const std::string& nameVar)
{
    return nameVar.substr(0,3)=="HYP";
}

void DbVarProp::clear()
{
    for(auto it=m_db.begin();it!=m_db.end();it++)
    {
        m_db[it->first]=nullptr;
    }

    for(auto it=m_hypAssoc.begin();it!=m_hypAssoc.end();it++)
    {
        m_hypAssoc[it->first]={};
    }

    for(auto it=m_hypMapOrdered.begin();it!=m_hypMapOrdered.end();it++)
    {
        m_hypMapOrdered[it->first]={};
    }
}
