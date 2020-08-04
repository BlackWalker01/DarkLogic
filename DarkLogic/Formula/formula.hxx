#include "formula.h"
#ifndef DARK_LOGIC_FORMULA_HXX
#define DARK_LOGIC_FORMULA_HXX

template<Type ValueType_>
inline std::string N_DarkLogic::AbstractFormula<ValueType_>::toNormalizedString(unsigned short priorityParent) const
{    
    if (getExtVars())
    {
        std::string ret;
        auto nameVars = getExtVars()->nameVars();
        std::unordered_map<std::string, std::string> idToIdx;
        for (const auto& id : nameVars)
        {
            auto newIdx = idToIdx.size();
            idToIdx[FormulaHelper::getVarNameFromId(id)] = "b" + std::to_string(newIdx);
        }

        std::string content = toString(priorityParent);
        std::string crtVar;
        for (size_t k = 0; k < content.size(); k++)
        {
            if (!isCharInVar(content[k]))
            {
                if (crtVar != "")
                {
                    auto it = idToIdx.find(crtVar);
                    if (it != idToIdx.end())
                    {
                        ret += it->second;
                    }
                    else
                    {
                        ret += crtVar;
                    }
                    crtVar = "";
                }
                ret += content[k];
            }
            else
            {
                crtVar += content[k];
            }
        }
        if (crtVar != "")
        {
            auto it = idToIdx.find(crtVar);
            if (it != idToIdx.end())
            {
                ret += it->second;
            }
            else
            {
                ret += crtVar;
            }
            crtVar = "";
        }
        return ret;
    }
    else
    {
        return toString(priorityParent);
    }
    
}

template<Type ValueType>
void AbstractFormula<ValueType>::print() const
{
    std::cout<<toString()<<std::endl;
}

#endif // DARK_LOGIC_FORMULA_HXX
