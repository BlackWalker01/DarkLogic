/*===--- parser.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines parsing functions for DarkLogic library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "Formula/Arithmetic/allarithformula.h"
#include "Formula/Boolean/allproposition.h"
#include "logic.h"

namespace N_DarkLogic
{
class Parser
{
public:
	template<typename FormulaType>
    static ptr<FormulaType> createFormula(const std::string& name, const std::string& content, const size_t& depth = 0, const size_t& startIdx = 0);    

private:
    struct VariableContainer;

    static bool isOpeName(const std::string& name);
    static Name getOpeName(const std::string& name_);
    static Name getNextOpeName(const std::string& propCont, size_t& i);
    static void skipStrings(const std::string& propCont, size_t& i);
    static void addAbstractTerm(const std::string& varName, std::vector<std::shared_ptr<Parser::VariableContainer> >& varList);


    static const std::unordered_map<std::string, Name>  s_nameHash;
    static const std::unordered_map<std::string, std::string> s_mightBeOpeName;
    static const std::unordered_map<std::string, std::string> s_stringsToSkip;
    static const std::unordered_map<std::string, std::string> s_stringsMightSkip;

    static std::unordered_map<std::string, std::string> initStringMightSkip();
    static std::unordered_map<std::string, std::string> initMightBeOpeName();

    static bool mustBeSkipped(const std::string & s);
    static bool mightBeSkipped(const std::string & s);
    static bool mightBeOpeName(const std::string & s);

	struct OperatorOrdering
	{
		OperatorOrdering(Name ope_, const size_t& nbPar_, const size_t& nbBracket_, const std::vector<size_t>& idx_);

		unsigned int priority() const;
		Associativity associativity() const;
        bool operator<(const OperatorOrdering& ope); // true if and only if this operator must be called first

		Name ope; // enum of oprator to order
		size_t nbPar; // number of unclosed parenthesis before the operator
        size_t nbBracket; // number of unclosed brace brackets before the operator
        std::vector<size_t> indexes; // indexes where subFormulas of operator can be separated
	};

    struct VariableContainer
    {
        VariableContainer(const std::shared_ptr<AbstractTerm>& term);

        void init();

        ~VariableContainer() = default;

        //attributes
        VALUE_TYPE type;
        std::shared_ptr<AbstractTerm> var;
    };

    class DbVarContainer
    {
    public:
        DbVarContainer() = default;

        void insert(const std::shared_ptr<VariableContainer>& var);

        std::shared_ptr<VariableContainer>& find(const std::string& name);
        bool contains(const std::string& name) const;

        ~DbVarContainer() = default;

    private:
        std::unordered_map<std::string, std::shared_ptr<VariableContainer>> db;
    };

    static DbVarContainer s_dbVar;
};

template<typename FormulaType>
struct ToSubStruct;

template<>
struct ToSubStruct<ASubTheorem>
{
    using Type = ASubTheorem;
};

template<>
struct ToSubStruct<ASubRule>
{
    using Type = ASubRule;
};

template<typename T>
struct ToSubStruct<ASubArithmeticTheorem<T>>
{
    using Type = ASubArithmeticTheorem<T>;
};

template<typename T>
struct ToSubStruct<ASubArithmeticRule<T>>
{
    using Type = ASubArithmeticRule<T>;
};

template<typename FormulaType>
using toSub = typename ToSubStruct<FormulaType>::Type;



template<typename FormulaType>
inline ptr<FormulaType> Parser::createFormula(const std::string& name, const std::string& content, const size_t& depth, const size_t& startIdx)
{
	char c = '\0'; // current character of content
	size_t nbPar = 0; // number of unclosed parenthesis until here
	size_t nbBracket = 0; // number of unclosed brace brackets until here
    std::unique_ptr<OperatorOrdering> crtOpe = nullptr; // current operator
    std::vector<std::shared_ptr<Parser::VariableContainer>> varList;
    bool hasFoundCcl = false; // true if crtOpe is hypothesis operator and parser found its conclusion formula
    Name crtNameOpe; // current name of operator
    std::string crtVar; // latest variable seen by parser
    for (size_t k = 0; k < content.size(); k++)
    {
        c = content[k];
        switch (c)
        {
        case '(':
        {
            nbPar++;
            continue;
        }
        case ')':
        {
            if (nbPar == 0 && depth == 0)
            {
                throw std::runtime_error("Unexpected ')' character at " + sizeToString(k + startIdx) + " index");
            }
            nbPar--;
            continue;
        }
        case ' ':
        {
            continue;
        }
        //hypothesis cases
        case '{':
        {            
            if (k != content.size() - 1)
            {
                if (crtOpe == nullptr)
                {
                    crtOpe = std::make_unique<OperatorOrdering>(HYP, nbPar, nbBracket, std::vector<size_t>({k+1}));
                }
                else
                {
                    OperatorOrdering opeOrder(HYP, nbPar, nbBracket, { k + 1 });
                    if (opeOrder < *crtOpe)
                    {
                        hasFoundCcl = false;
                        crtOpe = std::make_unique<OperatorOrdering>(opeOrder);
                    }
                }
            }
            else
            {
                throw std::runtime_error("Unexpected '{' character at " + sizeToString(k + startIdx) + " index");
            }
            nbBracket++;
            continue;
        }
        case ',':
        {
            if (nbBracket > 0 && k < content.size() -1)
            {
                if (crtOpe->ope == HYP && nbBracket == crtOpe->nbBracket + 1 && !hasFoundCcl)
                {
                    crtOpe->indexes.push_back(k + 1);
                }
            }
            else
            {
                throw std::runtime_error("unexpected ',' character at " + sizeToString(k + startIdx) + " index");
            }
            continue;
        }
        case '}':
        {            
            if (nbBracket != 0)
            {
                nbBracket--;
                if (crtOpe->ope == HYP && nbBracket == 0 && !hasFoundCcl)
                {
                    if (crtOpe->indexes.size() == 0)
                    {
                        throw std::runtime_error("Unexpected '}' character at " + sizeToString(k + startIdx) + " index. "
                        +"Hypothesis operator is empty!");
                    }
                    crtOpe->indexes.push_back(k + 1);
                    hasFoundCcl = true;
                }
            }
            else
            {
                throw std::runtime_error("Unexpected '}' character at " + sizeToString(k + startIdx) + " index");
            }
            continue;
        }
        default:
            break;
        }

        //skip strings if it is possible
        Parser::skipStrings(content, k);
        size_t crtK = k;
        if ((crtNameOpe = Parser::getNextOpeName(content, k)))
        {
            k--;
            if (crtOpe == nullptr)
            {
                crtOpe = std::make_unique<OperatorOrdering>(crtNameOpe, nbPar, nbBracket, std::vector<size_t>({ crtK }));
            }
            else
            {
                OperatorOrdering opeOrder(crtNameOpe, nbPar, nbBracket, { crtK });
                if (opeOrder < *crtOpe)
                {
                    crtOpe = std::make_unique<OperatorOrdering>(opeOrder);
                }
            }
        }
        else
        {
            crtVar = getVarName(content, k);
            if (crtVar != "")
            {
                Parser::addAbstractTerm(crtVar, varList);
                crtVar = "";
                k--;
            }
            else if (k == crtK)
            {
                std::string ch = "";
                ch += c;
                throw std::runtime_error("Unexpected '" + ch + "' character at " + sizeToString(k + startIdx) + " index");
            }
        }
    }

    if (crtOpe != nullptr)
    {
        switch (crtOpe->ope)
        {
        case NONE:
        {
            throw std::runtime_error("Unknown operator");
        }
        case AND:
        {
            if constexpr (Proposition< FormulaType>)
            {
                size_t opeSize = 2;
                if (crtOpe->indexes[0] + opeSize >= content.size())
                {
                    throw std::runtime_error("&& operator cannot be at end of formula (at " + sizeToString(content.size() - 1) + " index");
                }
                size_t sep = crtOpe->indexes[0] + opeSize;
                std::string contLeft = content.substr(0, crtOpe->indexes[0]);
                std::string contRight = content.substr(sep);
                auto leftFml = createFormula<toSub<FormulaType>>(name, contLeft, depth + 1, startIdx);
                auto rightFml = createFormula<toSub<FormulaType>>(name, contRight, depth + 1, sep);
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<And<ASubTheorem >>>(leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<And<ASubTheorem >>>(leftFml, rightFml);
                    }

                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got && operator");
                    }
                    else
                    {
                        return std::make_shared<const SubRule<And<ASubRule >>>(leftFml, rightFml);
                    }
                }
                else
                {
                    return std::make_shared<const FormulaType>>(leftFml, rightFml);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case EQUIVALENT:
        {
            if constexpr (Proposition< FormulaType>)
            {
                size_t opeSize = 3;
                if (crtOpe->indexes[0] + opeSize >= content.size())
                {
                    throw std::runtime_error("<=> operator cannot be at end of formula (at " + sizeToString(content.size() - 1) + " index");
                }
                size_t sep = crtOpe->indexes[0] + opeSize;
                std::string contLeft = content.substr(0, crtOpe->indexes[0]);
                std::string contRight = content.substr(sep);
                auto leftFml = createFormula<toSub<FormulaType>>(name, contLeft, depth + 1, startIdx);
                auto rightFml = createFormula<toSub<FormulaType>>(name, contRight, depth + 1, sep);
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<Equivalent<ASubTheorem >>>(leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<Equivalent<ASubTheorem >>>(leftFml, rightFml);
                    }

                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Rule<Equivalent<ASubRule >>>(name, leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubRule<Equivalent<ASubRule >>>(leftFml, rightFml);
                    }
                }
                else
                {
                    return std::make_shared<const FormulaType >> (leftFml, rightFml);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case IMPLICATION:
        {
            if constexpr (Proposition< FormulaType>)
            {
                size_t opeSize = 2;
                if (crtOpe->indexes[0] + opeSize >= content.size())
                {
                    throw std::runtime_error("=> operator cannot be at end of formula (at " + sizeToString(content.size() - 1) + " index");
                }
                size_t sep = crtOpe->indexes[0] + opeSize;
                std::string contLeft = content.substr(0, crtOpe->indexes[0]);
                std::string contRight = content.substr(sep);
                auto leftFml = createFormula<toSub<FormulaType>>(name, contLeft, depth + 1, startIdx);
                auto rightFml = createFormula<toSub<FormulaType>>(name, contRight, depth + 1, sep);
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<Implication<ASubTheorem >>>(leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<Implication<ASubTheorem >>>(leftFml, rightFml);
                    }

                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Rule<Implication<ASubRule >>>(name, leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubRule<Implication<ASubRule >>>(leftFml, rightFml);
                    }
                }
                else
                {
                    return std::make_shared<const FormulaType >> (leftFml, rightFml);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case NOT:
        {
            if constexpr (Proposition< FormulaType>)
            {
                size_t opeSize = 1;
                if (crtOpe->indexes[0] + opeSize >= content.size())
                {
                    throw std::runtime_error("! operator cannot be at end of formula (at " + sizeToString(content.size() - 1) + " index");
                }
                std::string contRight = content.substr(crtOpe->indexes[0] + opeSize);
                auto rightFml = createFormula<toSub<FormulaType>>(name, contRight, depth + 1, crtOpe->indexes[0] + opeSize);
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<Not<ASubTheorem >>>(rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<Not<ASubTheorem >>>(rightFml);
                    }

                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got ! operator");
                    }
                    else
                    {
                        return std::make_shared<const SubRule<Not<ASubRule >>>(rightFml);
                    }
                }
                else
                {
                    return std::make_shared<const FormulaType >> (rightFml);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case OR:
        {
            if constexpr (Proposition< FormulaType>)
            {
                size_t opeSize = 1;
                if (crtOpe->indexes[0] + opeSize >= content.size())
                {
                    throw std::runtime_error("|| operator cannot be at end of formula (at " + sizeToString(content.size() - 1) + " index");
                }
                size_t sep = crtOpe->indexes[0] + opeSize + 1;
                std::string contLeft = content.substr(0, crtOpe->indexes[0]);
                std::string contRight = content.substr(sep);
                auto leftFml = createFormula<toSub<FormulaType>>(name, contLeft, depth + 1, startIdx);
                auto rightFml = createFormula<toSub<FormulaType>>(name, contRight, depth + 1, sep);
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<Or<ASubTheorem >>>(leftFml, rightFml);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<Or<ASubTheorem >>>(leftFml, rightFml);
                    }
                    
                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got || operator");
                    }
                    else
                    {
                        return std::make_shared<const SubRule<Or<ASubRule >>>(leftFml, rightFml);
                    }                    
                }
                else
                {
                    return std::make_shared<const FormulaType >> (leftFml, rightFml);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case HYP:
        {
            if constexpr (Proposition< FormulaType>)
            {
                std::vector<ptr<toSub<FormulaType>>> sons;
                size_t k = 0;
                for (; k < crtOpe->indexes.size() - 1; k++)
                {
                    std::string cont = content.substr(crtOpe->indexes[k], crtOpe->indexes[k + 1] - crtOpe->indexes[k] - 1);
                    auto fml = createFormula<toSub<FormulaType>>(name, cont, depth + 1, crtOpe->indexes[k]);
                    sons.push_back(fml);
                }

                std::string cont = content.substr(crtOpe->indexes[k]);
                auto fml = createFormula<toSub<FormulaType>>(name, cont, depth + 1, crtOpe->indexes[k]);
                sons.push_back(fml);
                
                if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
                {
                    if (depth == 0)
                    {
                        return std::make_shared<const Theorem<Hyp<ASubTheorem >>>(sons);
                    }
                    else
                    {
                        return std::make_shared<const SubTheorem<Hyp<ASubTheorem >>>(sons);
                    }
                }
                else if constexpr (std::is_same_v<FormulaType, ASubRule>)
                {
                    if (depth == 0)
                    {
                        throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got Hypothesis operator");
                    }
                    else
                    {
                        return std::make_shared<const SubRule<Hyp<ASubRule >>>(sons);
                    }
                }
                else
                {
                    return std::make_shared<const FormulaType >> (sons);
                }
            }
            else
            {
                throw std::runtime_error("Unexpected boolean formula at " + sizeToString(startIdx) + " index");
            }
        }
        case SETEQUAL:
        {
            /*
            if (varList.size() < 2)
            {
                throw std::runtime_error("€ operator expects 2 arguments but got only " + sizeToString(varList.size()));
            }
            std::shared_ptr<VariableContainer> firstVar = nullptr;
            ptr<ValueTypeObject> leftOpe = nullptr;
            ptr<ValueTypeObject> rightOpe = nullptr;
            if (leftOpeList.size() == 0)
            {
                firstVar = varList[0];
                varList.erase(varList.begin());
                rightOpe = createSubTheorem(name + "€R", rightOpeList, rightOrderedOpeList, varList, nbHyps);
                varList.insert(varList.begin(), firstVar);
                if (firstVar->type == VALUE_TYPE::VOID_TYPE)
                {
                    switch (rightOpe->valueType())
                    {
                    case NATURAL_INTEGERSET_TYPE:
                    {
                        firstVar->type = NATURAL_INT_TYPE;
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("BelongsTo operator expects set as right operand");
                    }
                    }
                    firstVar->init();
                }
                else if (firstVar->type != rightOpe->valueType())
                {
                    throw std::runtime_error(firstVar->var->name() + " is not of the same type of right operand in € operator");
                }
                leftOpe = createSubTheorem(name + "€L", leftOpeList, leftOrderedOpeList, varList, nbHyps);
            }
            else
            {
                leftOpe = createSubTheorem(name + "€L", leftOpeList, leftOrderedOpeList, varList, nbHyps);
                rightOpe = createSubTheorem(name + "€R", rightOpeList, rightOrderedOpeList, varList, nbHyps);
            }
            switch (leftOpe->valueType())
            {
            case NATURAL_INT_TYPE:
            {
                switch (rightOpe->valueType())
                {
                case NATURAL_INTEGERSET_TYPE:
                {
                    auto leftOpeCast = std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                    auto rightOpeCast = std::static_pointer_cast<const ASubArithmeticTheorem<NaturalIntegerSet>>(rightOpe);
                    return std::make_shared<const SubTheorem<BelongsTo<ASubArithmeticTheorem<size_t>,
                        ASubArithmeticTheorem<NaturalIntegerSet> >>>(leftOpeCast, rightOpeCast);
                }
                default:
                {
                    throw std::runtime_error("BelongsTo operator expects a set as right operand");
                }
                }
            }
            default:
            {
                throw std::runtime_error("BelongsTo operator does not handle bool variable");
            }
            }
            */
            throw std::runtime_error("= operator not supported for the moment");
        }
        case EQUAL:
        {
            /*
            auto leftOpe=createSubTheorem(name+"==L",leftOpeList,leftOrderedOpeList,varList,nbHyps);
            auto rightOpe=createSubTheorem(name+"==R",rightOpeList,rightOrderedOpeList,varList,nbHyps);
            switch(leftOpe->valueType())
            {
                case NATURAL_INT_TYPE:
                {
                    switch(rightOpe->valueType())
                    {
                        case NATURAL_INT_TYPE:
                        {
                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                            return std::make_shared<const SubTheorem<Equal<ASubArithmeticTheorem<size_t>, ASubArithmeticTheorem<size_t> >>>
                            (leftOpeCast,rightOpeCast);
                        }
                        default:
                        {
                            throw std::runtime_error("Equal operator does not handle bool variable");
                        }
                    }
                }
                default:
                {
                    throw std::runtime_error("Equal operator does not handle bool variable");
                }
            }
            */
            throw std::runtime_error("== operator not supported for the moment");
        }
        case LET:
        {
            /*
            auto leftOpe=createSubTheorem(name+"letL",leftOpeList,leftOrderedOpeList,varList,nbHyps);
            auto rightOpe=createSubTheorem(name+"letR",rightOpeList,rightOrderedOpeList,varList,nbHyps);
            switch(leftOpe->valueType())
            {
                case VOID_TYPE:
                {
                    switch(rightOpe->valueType())
                    {
                        case BOOL_TYPE:
                        {
                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<void>>(leftOpe);
                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                            return std::make_shared<const Theorem<Let<ASubArithmeticTheorem<void>,ASubTheorem>>>
                                                                            (leftOpeCast,rightOpeCast);
                        }
                        default:
                        {
                            throw std::runtime_error("Let operator handles only boolean as right operand");
                        }
                    }
                }
                case BOOL_TYPE:
                {
                    switch(rightOpe->valueType())
                    {
                        case BOOL_TYPE:
                        {
                            auto leftOpeCast=std::static_pointer_cast<const ASubTheorem>(leftOpe);
                            auto rightOpeCast=std::static_pointer_cast<const ASubTheorem>(rightOpe);
                            return std::make_shared<const Theorem<Let<ASubTheorem,ASubTheorem>>>
                                                                (leftOpeCast,rightOpeCast);
                        }
                        default:
                        {
                            throw std::runtime_error("Let operator handles only boolean as right operand");
                        }
                    }
                }
                default:
                {
                    throw std::runtime_error("Let operator does not handle numbers as left operand");
                }
            }
            */
            throw std::runtime_error("let operator not supported for the moment");
        }
        case BELONGSTO:
        {
            /*
            if(varList.size()<2)
            {
                throw std::runtime_error("€ operator expects 2 arguments but got only "+sizeToString(varList.size()));
            }
            std::shared_ptr<VariableContainer> firstVar=nullptr;
            ptr<ValueTypeObject> leftOpe=nullptr;
            ptr<ValueTypeObject> rightOpe=nullptr;
            if(leftOpeList.size()==0)
            {
                firstVar=varList[0];
                varList.erase(varList.begin());
                rightOpe=createSubTheorem(name+"€R",rightOpeList,rightOrderedOpeList,varList,nbHyps);
                varList.insert(varList.begin(),firstVar);
                if(firstVar->type==VALUE_TYPE::VOID_TYPE)
                {
                    switch (rightOpe->valueType())
                    {
                        case NATURAL_INTEGERSET_TYPE:
                        {
                            firstVar->type=NATURAL_INT_TYPE;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("BelongsTo operator expects set as right operand");
                        }
                    }
                    firstVar->init();
                }
                else if(firstVar->type!=rightOpe->valueType())
                {
                    throw std::runtime_error(firstVar->var->name()+" is not of the same type of right operand in € operator");
                }
                leftOpe=createSubTheorem(name+"€L",leftOpeList,leftOrderedOpeList,varList,nbHyps);
            }
            else
            {
                leftOpe=createSubTheorem(name+"€L",leftOpeList,leftOrderedOpeList,varList,nbHyps);
                rightOpe=createSubTheorem(name+"€R",rightOpeList,rightOrderedOpeList,varList,nbHyps);
            }
            switch(leftOpe->valueType())
            {
                case NATURAL_INT_TYPE:
                {
                    switch(rightOpe->valueType())
                    {
                        case NATURAL_INTEGERSET_TYPE:
                        {
                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<NaturalIntegerSet>>(rightOpe);
                            return std::make_shared<const SubTheorem<BelongsTo<ASubArithmeticTheorem<size_t>,
                                    ASubArithmeticTheorem<NaturalIntegerSet> >>>(leftOpeCast,rightOpeCast);
                        }
                        default:
                        {
                            throw std::runtime_error("BelongsTo operator expects a set as right operand");
                        }
                    }
                }
                default:
                {
                    throw std::runtime_error("BelongsTo operator does not handle bool variable");
                }
            }
            */
            throw std::runtime_error("E operator not supported for the moment");
        }
        case PLUS:
        {
            /*
            auto leftOpe=createSubTheorem(name+"+L",leftOpeList,leftOrderedOpeList,varList,nbHyps);
            auto rightOpe=createSubTheorem(name+"+R",rightOpeList,rightOrderedOpeList,varList,nbHyps);
            switch(leftOpe->valueType())
            {
                case NATURAL_INT_TYPE:
                {
                    switch(rightOpe->valueType())
                    {
                        case NATURAL_INT_TYPE:
                        {
                            auto leftOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(leftOpe);
                            auto rightOpeCast=std::static_pointer_cast<const ASubArithmeticTheorem<size_t>>(rightOpe);
                            return std::make_shared<const SubArithmeticTheorem<Plus<ASubArithmeticTheorem<size_t>,
                                    ASubArithmeticTheorem<size_t> >>>(leftOpeCast,rightOpeCast);
                        }
                        default:
                        {
                            throw std::runtime_error("Plus operator does not handle bool variable");
                        }
                    }
                }
                default:
                {
                    throw std::runtime_error("Plus operator does not handle bool variable");
                }
            }
            */
            throw std::runtime_error("+ operator not supported for the moment");
        }
        }
    }
    else if (varList.size() == 1)
    {
    auto var = varList[0]->var;
    varList.erase(varList.begin());
    if (var->isConstant())
    {
        switch (var->valueType())
        {
        case VALUE_TYPE::BOOL_TYPE:
        {
            if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
            {
                if (depth == 0)
                {
                    return std::make_shared<const Theorem<ConstBoolean>>(
                        std::static_pointer_cast<ConstBoolean>(var)->evaluate());
                }
                else
                {
                    return std::make_shared<const SubTheorem<ConstBoolean>>(
                        std::static_pointer_cast<ConstBoolean>(var)->evaluate());
                }
                
            }
            else if constexpr (std::is_same_v<FormulaType, ASubRule>)
            {
                if (depth == 0)
                {
                    throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got ConstBoolean variable");
                }
                else
                {
                    return std::make_shared<const SubRule<ConstBoolean>>(
                        std::static_pointer_cast<ConstBoolean>(var)->evaluate());
                }
            }
            else
            {
                return std::make_shared<const FormulaType>(
                    std::static_pointer_cast<ConstBoolean>(var)->evaluate());
            }            
        }
        case VALUE_TYPE::NATURAL_INT_TYPE:
        {
            /*return std::make_shared<const ConstSubArithmeticTheorem<ConstNaturalInteger>>(
                std::static_pointer_cast<ConstNaturalInteger>(var)->evaluate());*/
            throw std::runtime_error("natural integer constants not supported for the moment");
        }
        case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
        {
            /*return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<NaturalIntegerSet>>>(
                std::static_pointer_cast<ConstSet<NaturalIntegerSet>>(var)->evaluate());*/
            throw std::runtime_error("natural integer set constants not supported for the moment");
        }
        case VALUE_TYPE::BOOLEANSET_TYPE:
        {
            /*return std::make_shared<const ConstSubArithmeticTheorem<ConstSet<BooleanSet>>>(
                std::static_pointer_cast<ConstSet<BooleanSet>>(var)->evaluate());*/
            throw std::runtime_error("boolean set constants not supported for the moment");
        }
        default:
        {
            throw std::runtime_error("No void variable in arithmetic formula");
        }
        }
    }
    else
    {
        switch (var->valueType())
        {
        case VALUE_TYPE::BOOL_TYPE:
        {
            if constexpr (std::is_same_v<FormulaType, ASubTheorem>)
            {
                if (depth == 0)
                {
                    return std::make_shared<const Theorem<Boolean>>(
                        std::static_pointer_cast<Boolean>(var));
                }
                else
                {
                    return std::make_shared<const SubTheorem<Boolean>>(
                        std::static_pointer_cast<Boolean>(var));
                }
            }
            else if constexpr (std::is_same_v<FormulaType, ASubRule>)
            {
                if (depth == 0)
                {
                    throw std::runtime_error("Rule must have either '<=>' or '=>' as top operator but got ConstBoolean variable");
                }
                else
                {
                    return std::make_shared<const SubRule<Boolean>>(
                        std::static_pointer_cast<Boolean>(var));
                }                
            }
            else
            {
                return std::make_shared<const FormulaType>(
                    std::static_pointer_cast<Boolean>(var));
            }
            
        }
        case VALUE_TYPE::NATURAL_INT_TYPE:
        {
            /*return std::make_shared<const SubArithmeticTheorem<NaturalInteger>>(
                std::static_pointer_cast<NaturalInteger>(var));*/
            throw std::runtime_error("natural integer variables not supported for the moment");
        }
        case VALUE_TYPE::NATURAL_INTEGERSET_TYPE:
        {
            /*return std::make_shared<const SubArithmeticTheorem<VarSet<NaturalIntegerSet>>>(
                std::static_pointer_cast<VarSet<NaturalIntegerSet>>(var));*/
            throw std::runtime_error("natural integer set variables not supported for the moment");
        }
        case VALUE_TYPE::BOOLEANSET_TYPE:
        {
            /*return std::make_shared<const SubArithmeticTheorem<VarSet<BooleanSet>>>(
                std::static_pointer_cast<VarSet<BooleanSet>>(var));*/
            throw std::runtime_error("boolean set variables not supported for the moment");
        }
        default:
        {
            throw std::runtime_error("No void variable in arithmetic formula");
        }
        }
    }
    }
    else if(varList.size() > 1)
    {
        throw std::runtime_error("Too much variables in theorem at " + sizeToString(startIdx) + " index");
    }
    else
    {
        throw std::runtime_error("Empty theorem");
    }

	return ptr<FormulaType>();
}

}