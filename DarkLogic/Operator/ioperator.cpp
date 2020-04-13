/*===--- ioperator.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements some parsing functions for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "ioperator.h"
#include "Utils/utils.h"

using namespace N_DarkLogic;

bool IOperator::isOpeName(const std::string &name)
{
    return s_nameHash.find(name)!=s_nameHash.end();
}

Name IOperator::getOpeName(const std::string &name_)
{
    auto it=s_nameHash.find(name_);
    if(it!=s_nameHash.end())
    {
        return it->second;
    }
    throw std::runtime_error("Operator "+name_+" does not exist");
}


/**
 * @brief IOperator::getNextOpeName
 * @param propCont: property content
 * @param i: current index
 * @return the next operator name if there is one else return NONE
 * @assuming propContent[i:] does not start with '(', '{' or strings to skip (" ", "in", "st" (such that) )
 */
Name IOperator::getNextOpeName(const std::string &propCont, size_t &i)
{
    std::string crtOpe="";
    size_t k=i;
    for(;k<propCont.size();k++)
    {
        char c=propCont[k];
        std::string ch=crtOpe;
        ch+=c;
        if(!mightBeOpeName(ch))
        {
            break;
        }
        crtOpe+=c;
    }

    if(isOpeName(crtOpe))
    {
        i=k;
        if(crtOpe=="let")
        {
            for(;k<propCont.size();k++)
            {
                if(propCont.substr(k,k+2)=="in")
                {
                    return LET;
                }
                /*
                else if(propCont.substr(k,k+2)=="st")
                {
                    return LET_ST;
                }
                */
            }
        }
        else
        {
            return getOpeName(crtOpe);
        }
    }
    else if(crtOpe!="")
    {
        std::string ch="";
        ch+=propCont[i];
        throw std::runtime_error("Unexpected '"+ch+"' character at "+sizeToString(i)+" index");
    }
    return NONE;
}

void IOperator::skipStrings(const std::string &propCont, size_t &i)
{
    std::string mightSkipString="";
    size_t k=i;
    for(;k<propCont.size();k++)
    {
        mightSkipString+=propCont[k];
        if(mightBeSkipped(mightSkipString))
        {
            if(mustBeSkipped(mightSkipString))
            {
                mightSkipString="";
            }
        }
        else
        {
            break;
        }
    }
    i=k;
}

std::unordered_map<std::string, std::string> IOperator::initStringMightSkip()
{
    std::unordered_map<std::string, std::string> ret={};
    for(auto it=s_stringsToSkip.begin();it!=s_stringsToSkip.end();it++)
    {
        std::string key=it->first;
        std::string subKey="";
        for(size_t k=0;k<key.size();k++)
        {
            subKey+=key[k];
            ret[subKey]=subKey;
        }
    }
    return ret;
}

std::unordered_map<std::string, std::string> IOperator::initMightBeOpeName()
{
    std::unordered_map<std::string, std::string> ret={};
    for(auto it=s_nameHash.begin();it!=s_nameHash.end();it++)
    {
        std::string key=it->first;
        std::string subKey="";
        for(size_t k=0;k<key.size();k++)
        {
            subKey+=key[k];
            ret[subKey]=subKey;
        }
    }
    return ret;
}

bool IOperator::mustBeSkipped(const std::string &s)
{
    return s_stringsToSkip.find(s)!=s_stringsToSkip.end();
}

bool IOperator::mightBeSkipped(const std::string &s)
{
    return s_stringsMightSkip.find(s)!=s_stringsMightSkip.end();
}

bool IOperator::mightBeOpeName(const std::string &s)
{
    return s_mightBeOpeName.find(s)!=s_mightBeOpeName.end();
}
