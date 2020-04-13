/*===--- utils.cpp - source for DarkLogic library --------------*- C++ -*-===*
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

#include "utils.h"
#include <sstream>

std::vector<std::vector<size_t>> N_DarkLogic::permutation(const size_t& n)
{
    std::vector<std::vector<size_t>> ret;
    if (n == 1)
    {
        ret.push_back({0});
    }
    else if (n > 1)
    {
        std::vector<std::vector<size_t>> acc = permutation(n - 1);
        for (size_t i = 0; i < acc.size(); i++)
        {
            for (size_t pos = 0; pos < n; pos++)
            {
                std::vector<size_t> crtRet;
                for (size_t j = 0; j < pos; j++)
                {
                    crtRet.push_back(acc[i][j]);
                }

                crtRet.push_back(n-1);

                for (size_t j = pos; j < acc[i].size(); j++)
                {
                    crtRet.push_back(acc[i][j]);
                }
                ret.push_back(crtRet);
            }
        }        
    }

    return ret;
}

bool isCharInVar(char c)
{
    return (c>='0' && c<='9')||(c>='a' && c<='z')||(c>='A' && c<='Z');
}

bool isSyntaxChar(char c)
{
    return c=='('||c==')'||c=='{'||c=='}'||c==',';
}

std::string uintToString(unsigned int x)
{
    std::string ret="";
    std::stringstream ss;
    ss<<x;
    ss>>ret;
    return ret;
}

std::string sizeToString(const size_t& x)
{
    std::string ret="";
    std::stringstream ss;
    ss<<x;
    ss>>ret;
    return ret;
}

std::string getVarName(const std::string &content, size_t& i)
{
    std::string crtName="";
    char c=content[i];
    //arithmetic case
    if(c>='0' && c<='9')
    {
        crtName+=c;
        size_t k=i+1;
        bool hasDot=false;
        for(;k<content.size();k++)
        {
            c=content[k];
            if(c<'0'||c>'9')
            {
                if(c=='.'&&!hasDot)
                {
                    hasDot=true;
                }
                else if((c>='a' && c<='z')||(c>='A' && c<='Z'))
                {
                    std::string ch="";
                    ch+=c;
                    throw std::runtime_error("Syntax error at index "+sizeToString(i)+", a variable name cannot start with a number");
                }
                else
                {
                    break;
                }
            }
            crtName+=c;
        }
        i=k;

    }
    //other case
    else if((c>='a' && c<='z')||(c>='A' && c<='Z'))
    {
        crtName+=c;
        size_t k=i+1;
        for(;k<content.size();k++)
        {
            c=content[k];
            if(!isCharInVar(c))
            {
                break;
            }
            crtName+=c;
        }
        i=k;
    }

    return crtName;
}
