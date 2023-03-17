/*===--- dbvarprop.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements dbvarprop class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "dbvarprop.h"
#include "Boolean/asubtheorem.h"
#include "Variable/avariable.h"

using namespace N_DarkLogic;

DbVarProp::DbVarProp(const DbVarProp& dbVarProp) : m_db(dbVarProp.m_db)
{

}

DbVarProp::DbVarProp(const std::vector<ptr<AVariable>>& vars)
{
    for(const auto& var : vars)
    {
        if(!var->isHypVariable())
        {
            m_db[var->id()]=nullptr;
        }
        else
        {
            m_hypAssoc[var->id()]={};
        }
    }
}

bool DbVarProp::contains(const IDVar& idVar) const
{
    return (m_db.find(idVar)->second)!=nullptr;
}

bool DbVarProp::containsHyp(const IDVar& idHypVar) const
{
    return m_hypAssoc.find(idHypVar)!=m_hypAssoc.end();
}

const ptr<IISubTheoremFormula>& DbVarProp::operator[](const IDVar& idVar) const
{
    return m_db.at(idVar);
}

ptr<IISubTheoremFormula> &DbVarProp::operator[](const IDVar& idVar)
{
    return m_db[idVar];
}

void DbVarProp::insertHypEmpty(const IDVar& idHypVar)
{
    m_hypAssoc[idHypVar]={};
}

void DbVarProp::insertHypAssoc(const IDVar& idHypVar, const ptr<ASubTheorem>& prop)
{
    m_hypAssoc[idHypVar].push_back(prop);    
}

const std::vector<ptr<ASubTheorem>>& DbVarProp::getHypAssoc(const IDVar& idHypVar) const
{
    return m_hypAssoc.at(idHypVar);
}

bool DbVarProp::isHypAssocEmpty(const IDVar& idHypVar) const
{
    return m_hypAssoc.at(idHypVar).size() == 0;
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

bool DbVarProp::isHypVariable(const ptr<AVariable>& var)
{
    return var->isHypVariable();
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
}
