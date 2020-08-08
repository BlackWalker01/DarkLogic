/*===--- parser.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements parsing functions for DarkLogic library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "parser.h"

using namespace N_DarkLogic;

Parser::DbVarContainer Parser::s_dbVar;
const std::unordered_map<std::string, Name> Parser::s_nameHash = {
    {"&&",AND},
    {"<=>",EQUIVALENT},
    {"=>",IMPLICATION},
    {"!",NOT},
    {"||",OR},
    {"{",HYP},
    {"==",EQUAL},
    {"E",BELONGSTO},
    {"=",SETEQUAL},
    {"in",LET}, //let ... in ...
    {"+",PLUS} };

const std::unordered_map<std::string, std::string> Parser::s_mightBeOpeName = initMightBeOpeName();

const std::unordered_map<std::string, std::string> Parser::s_stringsToSkip =
{ {" "," "},{"let","let"} };

const std::unordered_map<std::string, std::string> Parser::s_stringsMightSkip = initStringMightSkip();

bool Parser::isOpeName(const std::string& name)
{
    return s_nameHash.find(name) != s_nameHash.end();
}

Name Parser::getOpeName(const std::string& name_)
{
    auto it = s_nameHash.find(name_);
    if (it != s_nameHash.end())
    {
        return it->second;
    }
    throw std::runtime_error("Operator " + name_ + " does not exist");
}


/**
 * @brief Parser::getNextOpeName
 * @param propCont: property content
 * @param i: current index
 * @return the next operator name if there is one else return NONE
 * @assuming propContent[i:] does not start with '(', '{' or strings to skip (" ", "in", "st" (such that) )
 */
Name Parser::getNextOpeName(const std::string& propCont, size_t& i)
{
    std::string crtOpe = "";
    size_t k = i;
    for (; k < propCont.size(); k++)
    {
        char c = propCont[k];
        std::string ch = crtOpe;
        ch += c;
        if (!mightBeOpeName(ch))
        {
            break;
        }
        crtOpe += c;
    }

    if (isOpeName(crtOpe))
    {
        i = k;
        if (crtOpe == "let")
        {
            for (; k < propCont.size(); k++)
            {
                if (propCont.substr(k, k + 2) == "in")
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
    else if (crtOpe != "")
    {
        std::string ch = "";
        ch += propCont[i];
        throw std::runtime_error("Unexpected '" + ch + "' character at " + sizeToString(i) + " index");
    }
    return NONE;
}

void Parser::skipStrings(const std::string& propCont, size_t& i)
{
    std::string mightSkipString = "";
    size_t k = i;
    for (; k < propCont.size(); k++)
    {
        mightSkipString += propCont[k];
        if (mightBeSkipped(mightSkipString))
        {
            if (mustBeSkipped(mightSkipString))
            {
                mightSkipString = "";
            }
        }
        else
        {
            break;
        }
    }
    i = k;
}

void N_DarkLogic::Parser::addAbstractTerm(const std::string& varName, std::vector<std::shared_ptr<Parser::VariableContainer>>& varList)
{
    std::shared_ptr<VariableContainer> varContainer = nullptr;
    std::shared_ptr<AbstractTerm> var = nullptr;
    if (s_dbVar.contains(varName))
    {
        varContainer = s_dbVar.find(varName);
    }
    else
    {
        var = AbstractTerm::create(varName);
        varContainer = std::make_shared<VariableContainer>(var);
        bool isConst = true;
        //bool isReal=false;
        for (size_t k = 0; k < var->name().size(); k++)
        {
            char c = var->name()[k];
            if (c < '0' || c>'9')
            {
                isConst = false;
                break;
            }
            if (c == '.')
            {
                //isReal=true;
            }
        }
        if (!isConst && var->name() != "true" && var->name() != "false")
        {
            s_dbVar.insert(varContainer);
        }
    }
    varList.push_back(varContainer);
}

std::unordered_map<std::string, std::string> Parser::initStringMightSkip()
{
    std::unordered_map<std::string, std::string> ret = {};
    for (auto it = s_stringsToSkip.begin(); it != s_stringsToSkip.end(); it++)
    {
        std::string key = it->first;
        std::string subKey = "";
        for (size_t k = 0; k < key.size(); k++)
        {
            subKey += key[k];
            ret[subKey] = subKey;
        }
    }
    return ret;
}

std::unordered_map<std::string, std::string> Parser::initMightBeOpeName()
{
    std::unordered_map<std::string, std::string> ret = {};
    for (auto it = s_nameHash.begin(); it != s_nameHash.end(); it++)
    {
        std::string key = it->first;
        std::string subKey = "";
        for (size_t k = 0; k < key.size(); k++)
        {
            subKey += key[k];
            ret[subKey] = subKey;
        }
    }
    return ret;
}

bool Parser::mustBeSkipped(const std::string& s)
{
    return s_stringsToSkip.find(s) != s_stringsToSkip.end();
}

bool Parser::mightBeSkipped(const std::string& s)
{
    return s_stringsMightSkip.find(s) != s_stringsMightSkip.end();
}

bool Parser::mightBeOpeName(const std::string& s)
{
    return s_mightBeOpeName.find(s) != s_mightBeOpeName.end();
}

Parser::OperatorOrdering::OperatorOrdering(Name ope_, const size_t& nbPar_, const size_t& nbBracket_, const std::vector<size_t>& idx_):
	ope(ope_), nbPar(nbPar_), nbBracket(nbBracket_), indexes(idx_)
{
}

unsigned int Parser::OperatorOrdering::priority() const
{
    switch (ope)
    {
    case NONE:
    {
        throw std::runtime_error("Unknown operator");
    }
    case AND:
    {
        return AndFun<ASubTheorem>::priority();
    }
    case EQUIVALENT:
    {
        return EqFun<ASubTheorem>::priority();
    }
    case IMPLICATION:
    {
        return ImplFun<ASubTheorem>::priority();
    }
    case NOT:
    {
        return NotFun<ASubTheorem>::priority();
    }
    case OR:
    {
        return OrFun<ASubTheorem>::priority();
    }
    case HYP:
    {
        return HypFun<ASubTheorem>::priority();
    }
    case SETEQUAL:
    {
        return SetEqualFun<SubArithmeticTheorem<NaturalInteger>>::priority();
    }
    case EQUAL:
    {
        return EqualFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::priority();
    }
    case LET:
    {
        return LetFun<SubArithmeticTheorem<NaturalInteger>, ASubTheorem>::priority();
    }
    case BELONGSTO:
    {
        return BelongsToFun<ASubArithmeticRule<size_t>, ASubArithmeticRule<NaturalIntegerSet>>::priority();
    }
    case PLUS:
    {
        return PlusFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::priority();
    }
    }
}

Associativity N_DarkLogic::Parser::OperatorOrdering::associativity() const
{
    switch (ope)
    {
    case NONE:
    {
        throw std::runtime_error("Unknown operator");
    }
    case AND:
    {
        return AndFun<ASubTheorem>::associativity();
    }
    case EQUIVALENT:
    {
        return EqFun<ASubTheorem>::associativity();
    }
    case IMPLICATION:
    {
        return ImplFun<ASubTheorem>::associativity();
    }
    case NOT:
    {
        return NotFun<ASubTheorem>::associativity();
    }
    case OR:
    {
        return OrFun<ASubTheorem>::associativity();
    }
    case HYP:
    {
        return HypFun<ASubTheorem>::associativity();
    }
    case SETEQUAL:
    {
        return SetEqualFun<SubArithmeticTheorem<NaturalInteger>>::associativity();
    }
    case EQUAL:
    {
        return EqualFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::associativity();
    }
    case LET:
    {
        return LetFun<SubArithmeticTheorem<NaturalInteger>, ASubTheorem>::associativity();
    }
    case BELONGSTO:
    {
        return BelongsToFun<ASubArithmeticRule<size_t>, ASubArithmeticRule<NaturalIntegerSet>>::associativity();
    }
    case PLUS:
    {
        return PlusFun<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t>>::associativity();
    }
    }
}

bool N_DarkLogic::Parser::OperatorOrdering::operator<(const OperatorOrdering& opeOrder)
{
    if (nbPar < opeOrder.nbPar)
    {
        return true;
    }
    else if (nbPar > opeOrder.nbPar)
    {
        return false;
    }
    else
    {
        if (priority() > opeOrder.priority())
        {
            return true;
        }
        else if (priority() < opeOrder.priority())
        {
            return false;
        }
        else
        {
            if (associativity() == Associativity::LEFT)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}


Parser::VariableContainer::VariableContainer(const std::shared_ptr<AbstractTerm>& term) : type(VALUE_TYPE::VOID_TYPE), var(term)
{

}

void Parser::VariableContainer::init()
{
    std::string name = var->name();
    switch (type)
    {
    case VOID_TYPE:
    {
        throw std::runtime_error("Cannot initialize variable with no type");
    }
    case BOOL_TYPE:
    {
        if (name.substr(0, 3) == "HYP")
        {
            var = std::make_shared<HypBoolean>(name);
        }
        else
        {
            var = std::make_shared<Boolean>(name);
        }

        break;
    }
    case NATURAL_INT_TYPE:
    {
        var = std::make_shared<NaturalInteger>(name);
        break;
    }
    case BOOLEANSET_TYPE:
    {
        var = std::make_shared<VarSet<BooleanSet>>(name);
        break;
    }
    case NATURAL_INTEGERSET_TYPE:
    {
        var = std::make_shared<VarSet<NaturalIntegerSet>>(name);
        break;
    }
    }
}


/**
 * @brief DbVarContainer
 */

void Parser::DbVarContainer::insert(const std::shared_ptr<VariableContainer>& var)
{
    db[var->var->name()] = var;
}

std::shared_ptr<Parser::VariableContainer>& Parser::DbVarContainer::find(const std::string& name)
{
    return db[name];
}

bool Parser::DbVarContainer::contains(const std::string& name) const
{
    return db.find(name) != db.end();
}
