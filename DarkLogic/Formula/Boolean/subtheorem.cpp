/*===--- subtheorem.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements SubRule class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "Formula/Boolean/allproposition.h"
#include "Formula/Arithmetic/allarithformula.h"
#include "logic.h"

using namespace N_DarkLogic;

std::vector<State> getStates(const std::vector < ptr<ASubTheorem>>& subProps)
{
    std::vector<State> ret;
    for (const auto& subProp : subProps)
    {
        ret.push_back(subProp->getState());
    }
    return ret;
}

/**---------------------------------------------------------------
 * computeImplPaths methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
const std::vector<std::vector<Arity> >& SubTheorem<SubPropertyType>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath = (*m_son)[0]->getImplPaths();
        for (size_t k = 0; k < leftPath.size(); k++)
        {
            std::vector<Arity> crtPath = leftPath[k];
            crtPath.insert(crtPath.begin(), 0);
            m_implPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath = (*m_son)[1]->getImplPaths();
        for (size_t k = 0; k < rightPath.size(); k++)
        {
            std::vector<Arity> crtPath = rightPath[k];
            crtPath.insert(crtPath.begin(), 1);
            m_implPaths.push_back(crtPath);
        }
    }
    return m_implPaths;
}

template<>
const std::vector<std::vector<Arity> >& SubTheorem<Implication<ASubTheorem>>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});

        //right subProperty
        std::vector<std::vector<Arity>> rightPath = (*m_son)[1]->getImplPaths();
        for (size_t k = 0; k < rightPath.size(); k++)
        {
            std::vector<Arity> crtPath = rightPath[k];
            crtPath.insert(crtPath.begin(), 1);
            m_implPaths.push_back(crtPath);
        }
    }
    return m_implPaths;
}


/**---------------------------------------------------------------
 * initEval methods
 * ---------------------------------------------------------------
 */
template<>
void SubTheorem<And<ASubTheorem>>::initEval()
{
    const auto& leftProp = (*m_son)[0];
    const auto& rightProp = (*m_son)[1];
    if (leftProp->isEvaluated() && rightProp->isEvaluated())
    {
        m_eval->setValue(leftProp->evaluate() && rightProp->evaluate());
    }
    else if (leftProp->isEvaluated() && !leftProp->evaluate())
    {
        m_eval->setValue(false);
    }
    else if (rightProp->isEvaluated() && !rightProp->evaluate())
    {
        m_eval->setValue(false);
    }
    //else simulate with all possible values for variables
    else
    {
        //m_eval->setExceptMsg(except.what());        

        if (leftProp->canBeDemonstrated() && rightProp->canBeDemonstrated())
        {
            m_eval->setHiddenValue(leftProp->getHiddenValue() && rightProp->getHiddenValue());
        }
        else if (leftProp->canBeDemonstrated())
        {
            if (!leftProp->getHiddenValue())
            {
                m_eval->setHiddenValue(false);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
                for (const auto& config : configRight)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else if (rightProp->canBeDemonstrated())
        {
            if (!rightProp->getHiddenValue())
            {
                m_eval->setHiddenValue(false);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
                for (const auto& config : configLeft)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else
        {
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
            std::unordered_map<IDVar, IDVar> varToEvalLeft = leftProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> varToEvalRight = rightProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> remainVar;
            std::unordered_map<IDVar, IDVar> commonVar;
            if (configLeft.size() > configRight.size())
            {
                //get variables in right property except those from left property
                for (const auto& idVar : varToEvalRight)
                {
                    if (varToEvalLeft.find(idVar.first) == varToEvalLeft.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configLeft)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = rightProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(config.first) && rightProp->testEvaluate(compatibleConfig));
                    }
                }
            }
            else
            {
                //get variables in left property except those from right property
                for (const auto& idVar : varToEvalLeft)
                {
                    if (varToEvalRight.find(idVar.first) == varToEvalRight.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configRight)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = leftProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(compatibleConfig) && rightProp->testEvaluate(config.first));
                    }
                }
            }
        }
    }

    m_eval->endEval();
}

template<>
void SubTheorem<Equivalent<ASubTheorem>>::initEval()
{
    //try to evaluate directly
    const auto& leftProp = (*m_son)[0];
    const auto& rightProp = (*m_son)[1];
    if (leftProp->isEvaluated() && rightProp->isEvaluated())
    {
        m_eval->setValue(leftProp->evaluate() == rightProp->evaluate());
    }
    //else simulate with all possible values for variables
    else
    {
        //m_eval->setExceptMsg(except.what());

        if (leftProp->canBeDemonstrated() && rightProp->canBeDemonstrated())
        {
            m_eval->setHiddenValue(leftProp->getHiddenValue() == rightProp->getHiddenValue());
        }
        else if (leftProp->canBeDemonstrated())
        {
            bool var = leftProp->getHiddenValue();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
            for (const auto& config : configRight)
            {
                m_eval->pushEvalConfig(config.first, config.second == var);
            }
        }
        else if (rightProp->canBeDemonstrated())
        {
            bool var = rightProp->getHiddenValue();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
            for (const auto& config : configLeft)
            {
                m_eval->pushEvalConfig(config.first, config.second == var);
            }
        }
        else
        {
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
            std::unordered_map<IDVar, IDVar> varToEvalLeft = leftProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> varToEvalRight = rightProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> remainVar;
            std::unordered_map<IDVar, IDVar> commonVar;
            if (configLeft.size() > configRight.size())
            {
                //get variables in right property except those from left property
                for (const auto& idVar : varToEvalRight)
                {
                    if (varToEvalLeft.find(idVar.first) == varToEvalLeft.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configLeft)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = rightProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(config.first) == rightProp->testEvaluate(compatibleConfig));
                    }
                }
            }
            else
            {
                //get variables in left property except those from right property
                for (const auto& idVar : varToEvalLeft)
                {
                    if (varToEvalRight.find(idVar.first) == varToEvalRight.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configRight)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = leftProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(compatibleConfig) == rightProp->testEvaluate(config.first));
                    }
                }
            }
        }
    }

    m_eval->endEval();
}

template<>
void SubTheorem<Implication<ASubTheorem>>::initEval()
{
    //try to evaluate directly 
    const auto& leftProp = (*m_son)[0];
    const auto& rightProp = (*m_son)[1];
    if (leftProp->isEvaluated() && rightProp->isEvaluated())
    {
        m_eval->setValue(!leftProp->evaluate() || rightProp->evaluate());
    }
    else if (leftProp->isEvaluated() && !leftProp->evaluate())
    {
        m_eval->setValue(true);
    }
    else if (rightProp->isEvaluated() && rightProp->evaluate())
    {
        m_eval->setValue(true);
    }
    //else simulate with all possible values for variables
    else
    {
        //m_eval->setExceptMsg(except.what());       

        if (leftProp->canBeDemonstrated() && rightProp->canBeDemonstrated())
        {
            m_eval->setHiddenValue(!leftProp->getHiddenValue() || rightProp->getHiddenValue());
        }
        else if (leftProp->canBeDemonstrated())
        {
            if (!leftProp->getHiddenValue())
            {
                m_eval->setHiddenValue(true);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
                for (const auto& config : configRight)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else if (rightProp->canBeDemonstrated())
        {
            if (rightProp->getHiddenValue())
            {
                m_eval->setHiddenValue(true);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
                for (const auto& config : configLeft)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else
        {
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
            std::unordered_map<IDVar, IDVar> varToEvalLeft = leftProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> varToEvalRight = rightProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> remainVar;
            std::unordered_map<IDVar, IDVar> commonVar;
            if (configLeft.size() > configRight.size())
            {
                //get variables in right property except those from left property
                for (const auto& idVar : varToEvalRight)
                {
                    if (varToEvalLeft.find(idVar.first) == varToEvalLeft.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configLeft)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = rightProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, !leftProp->testEvaluate(config.first) || rightProp->testEvaluate(compatibleConfig));
                    }
                }
            }
            else
            {
                //get variables in left property except those from right property
                for (const auto& idVar : varToEvalLeft)
                {
                    if (varToEvalRight.find(idVar.first) == varToEvalRight.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configRight)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = leftProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, !leftProp->testEvaluate(compatibleConfig) || rightProp->testEvaluate(config.first));
                    }
                }
            }
        }
        m_eval->endEval();
    }
    
}

template<>
void SubTheorem<Or<ASubTheorem>>::initEval()
{
    //try to evaluate directly 
    const auto& leftProp = (*m_son)[0];
    const auto& rightProp = (*m_son)[1];
    if (leftProp->isEvaluated() && leftProp->evaluate())
    {
        m_eval->setValue(true);
    }
    else if (rightProp->isEvaluated() && rightProp->evaluate())
    {
        m_eval->setValue(true);
    }
    //else simulate with all possible values for variables
    else
    {
        //m_eval->setExceptMsg(except.what());

        if (leftProp->canBeDemonstrated() && rightProp->canBeDemonstrated())
        {
            m_eval->setHiddenValue(leftProp->getHiddenValue() || rightProp->getHiddenValue());
        }
        else if (leftProp->canBeDemonstrated())
        {
            if (leftProp->getHiddenValue())
            {
                m_eval->setHiddenValue(true);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
                for (const auto& config : configRight)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else if (rightProp->canBeDemonstrated())
        {
            if (rightProp->getHiddenValue())
            {
                m_eval->setHiddenValue(true);
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
                for (const auto& config : configLeft)
                {
                    m_eval->pushEvalConfig(config.first, config.second);
                }
            }
        }
        else
        {
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configLeft = leftProp->getConfigEvals();
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configRight = rightProp->getConfigEvals();
            std::unordered_map<IDVar, IDVar> varToEvalLeft = leftProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> varToEvalRight = rightProp->getVarToEval();
            std::unordered_map<IDVar, IDVar> remainVar;
            std::unordered_map<IDVar, IDVar> commonVar;
            if (configLeft.size() > configRight.size())
            {
                //get variables in right property except those from left property
                for (const auto& idVar : varToEvalRight)
                {
                    if (varToEvalLeft.find(idVar.first) == varToEvalLeft.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configLeft)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = rightProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(config.first) || rightProp->testEvaluate(compatibleConfig));
                    }
                }
            }
            else
            {
                //get variables in left property except those from right property
                for (const auto& idVar : varToEvalLeft)
                {
                    if (varToEvalRight.find(idVar.first) == varToEvalRight.end())
                    {
                        remainVar[idVar.first] = idVar.first;
                    }
                    else
                    {
                        commonVar[idVar.first] = idVar.first;
                    }
                }

                for (const auto& config : configRight)
                {
                    //create commonConfig
                    Evaluater::ConfigEval commonConfig;
                    for (const auto& it : config.first)
                    {
                        if (commonVar.find(it.first) != commonVar.end())
                        {
                            commonConfig[it.first] = it.second;
                        }
                    }

                    std::vector<Evaluater::ConfigEval> compatibleConfigs = leftProp->getCompatibleConfigs(commonConfig, remainVar);
                    for (auto& compatibleConfig : compatibleConfigs)
                    {
                        Evaluater::ConfigEval newConfig = config.first;
                        newConfig.merge(compatibleConfig);
                        m_eval->pushEvalConfig(newConfig, leftProp->testEvaluate(compatibleConfig) || rightProp->testEvaluate(config.first));
                    }
                }
            }
        }
        m_eval->endEval();
    }
    
}

void SubTheorem<Hyp<ASubTheorem>>::initEval()
{
    //try to evaluate directly 
    std::unique_ptr<bool> value = std::make_unique<bool>(true);
    if ((*m_son)[arity() - 1]->isEvaluated() && (*m_son)[arity() - 1]->evaluate())
    {
        m_eval->setValue(true);
        return;
    }
    else
    {
        for (size_t k = 0; k < arity() - 1; k++)
        {
            if ((*m_son)[k]->isEvaluated())
            {
                if (!(*m_son)[k]->evaluate())
                {
                    m_eval->setValue(true);
                    return;
                }
            }
            else
            {
                value = nullptr;
            }
        }
        if (value != nullptr)
        {
            if ((*m_son)[arity() - 1]->isEvaluated() && !(*m_son)[arity() - 1]->evaluate())
            {
                m_eval->setValue(false);
                return;
            }
            else
            {
                value = nullptr;
            }
        }
    }

    //else simulate with all possible values for variables
    if(value == nullptr)
    {
        //m_eval->setExceptMsg(except.what());
        if ((*m_son)[arity() - 1]->canBeDemonstrated() && (*m_son)[arity() - 1]->getHiddenValue())
        {
            m_eval->setHiddenValue(true);
        }
        else
        {
            std::vector<std::pair<Evaluater::ConfigEval, bool>> configsDone;
            std::map<Evaluater::ConfigEval, bool> configsDoneHash;
            std::unordered_map<IDVar, IDVar> varToEvalDone;            
            std::unordered_map<IDVar, IDVar> remainVar;
            std::unordered_map<IDVar, IDVar> commonVar;
            std::unique_ptr<bool> hiddenValue = std::make_unique<bool>(true);

            //first sub-property
            if ((*m_son)[0]->canBeDemonstrated())
            {
                if (!(*m_son)[0]->getHiddenValue())
                {
                    m_eval->setHiddenValue(true);
                    return;
                }
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configToDo = (*m_son)[0]->getConfigEvals();
                for (const auto& config : configToDo)
                {
                    configsDoneHash[config.first] = config.second;
                    configsDone.push_back({ config.first, config.second });
                }
            }

            for (size_t k = 1; k < arity() - 1; k++)
            {
                const auto& subProp = (*m_son)[k];
                if (hiddenValue != nullptr && subProp->canBeDemonstrated())
                {
                    if (!subProp->getHiddenValue())
                    {
                        m_eval->setHiddenValue(true);
                        return;
                    }
                }
                //general case (it is no use to do anything if subProp is true
                else if(!(subProp->canBeDemonstrated() && subProp->getHiddenValue()))
                {
                    hiddenValue = nullptr;
                    std::vector<std::pair<Evaluater::ConfigEval, bool>> newConfigsDone;
                    std::map<Evaluater::ConfigEval, bool> newConfigsDoneHash;
                    const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configToDo = subProp->getConfigEvals();
                    std::unordered_map<IDVar, IDVar> varToEvalToDo = subProp->getVarToEval();
                    if (configsDone.size() > configToDo.size())
                    {
                        //get variables in right property except those from left property
                        for (const auto& idVar : varToEvalToDo)
                        {
                            if (varToEvalDone.find(idVar.first) == varToEvalDone.end())
                            {
                                remainVar[idVar.first] = idVar.first;
                            }
                            else
                            {
                                commonVar[idVar.first] = idVar.first;
                            }
                        }

                        for (const auto& config : configsDone)
                        {
                            //create commonConfig
                            Evaluater::ConfigEval commonConfig;
                            for (const auto& it : config.first)
                            {
                                if (commonVar.find(it.first) != commonVar.end())
                                {
                                    commonConfig[it.first] = it.second;
                                }
                            }

                            std::vector<Evaluater::ConfigEval> compatibleConfigs = subProp->getCompatibleConfigs(commonConfig, remainVar);
                            for (auto& compatibleConfig : compatibleConfigs)
                            {
                                Evaluater::ConfigEval newConfig = config.first;
                                newConfig.merge(compatibleConfig);
                                bool var = configsDoneHash[config.first] && subProp->testEvaluate(compatibleConfig);
                                newConfigsDone.push_back({ newConfig,  var});
                                newConfigsDoneHash[config.first] = var;
                            }
                        }
                    }
                    else if (configsDone.size() == 0)
                    {
                        newConfigsDone = configToDo;
                        for (const auto& config : configToDo)
                        {
                            newConfigsDoneHash[config.first] = config.second;
                        }
                    }
                    else
                    {
                        //get variables in left property except those from right property
                        for (const auto& idVar : varToEvalDone)
                        {
                            if (varToEvalToDo.find(idVar.first) == varToEvalToDo.end())
                            {
                                remainVar[idVar.first] = idVar.first;
                            }
                            else
                            {
                                commonVar[idVar.first] = idVar.first;
                            }
                        }

                        for (const auto& config : configToDo)
                        {
                            //create commonConfig
                            Evaluater::ConfigEval commonConfig;
                            for (const auto& it : config.first)
                            {
                                if (commonVar.find(it.first) != commonVar.end())
                                {
                                    commonConfig[it.first] = it.second;
                                }
                            }

                            std::vector<Evaluater::ConfigEval> compatibleConfigs = Evaluater::getCompatibleConfigs(configsDoneHash, 
                                commonConfig, remainVar);
                            for (auto& compatibleConfig : compatibleConfigs)
                            {
                                Evaluater::ConfigEval newConfig = config.first;
                                newConfig.merge(compatibleConfig);
                                bool var = configsDoneHash[compatibleConfig] && subProp->testEvaluate(config.first);
                                newConfigsDone.push_back({ newConfig , var });                                
                                newConfigsDoneHash[newConfig] = var;
                            }
                        }
                    }

                    //update configsDone
                    configsDone = newConfigsDone;
                    configsDoneHash = newConfigsDoneHash;
                }                

            }

            //conclusion case
            const auto& subProp = (*m_son)[arity() - 1];
            //if conclusion can be demonstrated and we come until here, conclusion is necessary false
            if (subProp->canBeDemonstrated())
            {
                if (configsDone.size())
                {
                    for (const auto& config : configsDone)
                    {
                        m_eval->pushEvalConfig(config.first, !config.second);
                    }
                }
                else
                {
                    m_eval->setHiddenValue(*hiddenValue);
                }
                
            }
            else
            {
                const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configToDo = subProp->getConfigEvals();
                std::unordered_map<IDVar, IDVar> varToEvalToDo = subProp->getVarToEval();
                if (configsDone.size() > configToDo.size())
                {
                    //get variables in right property except those from left property
                    for (const auto& idVar : varToEvalToDo)
                    {
                        if (varToEvalDone.find(idVar.first) == varToEvalDone.end())
                        {
                            remainVar[idVar.first] = idVar.first;
                        }
                        else
                        {
                            commonVar[idVar.first] = idVar.first;
                        }
                    }

                    for (const auto& config : configsDone)
                    {
                        //create commonConfig
                        Evaluater::ConfigEval commonConfig;
                        for (const auto& it : config.first)
                        {
                            if (commonVar.find(it.first) != commonVar.end())
                            {
                                commonConfig[it.first] = it.second;
                            }
                        }

                        std::vector<Evaluater::ConfigEval> compatibleConfigs = subProp->getCompatibleConfigs(commonConfig, remainVar);
                        for (auto& compatibleConfig : compatibleConfigs)
                        {
                            Evaluater::ConfigEval newConfig = config.first;
                            newConfig.merge(compatibleConfig);
                            bool var = !configsDoneHash[config.first] || subProp->testEvaluate(compatibleConfig);
                            m_eval->pushEvalConfig(newConfig, var);
                        }
                    }
                }
                else
                {
                    //get variables in left property except those from right property
                    for (const auto& idVar : varToEvalDone)
                    {
                        if (varToEvalToDo.find(idVar.first) == varToEvalToDo.end())
                        {
                            remainVar[idVar.first] = idVar.first;
                        }
                        else
                        {
                            commonVar[idVar.first] = idVar.first;
                        }
                    }

                    for (const auto& config : configToDo)
                    {
                        //create commonConfig
                        Evaluater::ConfigEval commonConfig;
                        for (const auto& it : config.first)
                        {
                            if (commonVar.find(it.first) != commonVar.end())
                            {
                                commonConfig[it.first] = it.second;
                            }
                        }

                        std::vector<Evaluater::ConfigEval> compatibleConfigs = Evaluater::getCompatibleConfigs(configsDoneHash,
                            commonConfig, remainVar);
                        for (auto& compatibleConfig : compatibleConfigs)
                        {
                            Evaluater::ConfigEval newConfig = config.first;
                            newConfig.merge(compatibleConfig);
                            bool var = !configsDoneHash[compatibleConfig] || subProp->testEvaluate(config.first);
                            m_eval->pushEvalConfig(newConfig, var);
                        }
                    }
                }
            }            
        }
    }

    m_eval->endEval();
}

void SubTheorem<Not<ASubTheorem>>::initEval()
{
    //try to evaluate directly
    const auto& subProp = (*m_son)[0];
    if(subProp->isEvaluated())
    {
        m_eval->setValue(!subProp->evaluate());
    }
    //else simulate with all possible values for variables
    else
    {
        //m_eval->setExceptMsg(except.what());
        
        if (subProp->canBeDemonstrated())
        {
            m_eval->setHiddenValue(!subProp->getHiddenValue());
        }
        else
        {
            const std::vector<std::pair<Evaluater::ConfigEval, bool>>& configSub = subProp->getConfigEvals();
            for (const auto& config : configSub)
            {
                m_eval->pushEvalConfig(config.first, !config.second);
            }
        }
    }

    m_eval->endEval();
}

void SubTheorem<Boolean>::initEval()
{
    //m_eval->setExceptMsg(except.what());
    m_eval->pushEvalConfig({ {m_son->id(),false} }, false);
    m_eval->pushEvalConfig({ {m_son->id(),true} }, true);
}

/**---------------------------------------------------------------
 * Constructor methods from a vector of subProperties
 * ---------------------------------------------------------------
 */
template<>
SubTheorem<And<ASubTheorem>>::SubTheorem(const ptr<ASubTheorem>& leftSubProp,
                                         const ptr<ASubTheorem>& rightSubProp):
    m_son(std::make_unique<And<ASubTheorem>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()), m_eval(std::make_unique<Evaluater>()),
    m_state(AND, leftSubProp->getState(), rightSubProp->getState())
{
    computeAllPaths();
    initEval();
}

template<>
SubTheorem<Equivalent<ASubTheorem>>::SubTheorem(const ptr<ASubTheorem>& leftSubProp,
                                                const ptr<ASubTheorem>& rightSubProp):
    m_son(std::make_unique<Equivalent<ASubTheorem>>(leftSubProp,rightSubProp)),
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()), m_eval(std::make_unique<Evaluater>()),
    m_state(EQUIVALENT, leftSubProp->getState(), rightSubProp->getState())
{
    computeAllPaths();
    initEval();
}

SubTheorem<Hyp<ASubTheorem>>::SubTheorem(const std::vector<ptr<ASubTheorem>>& subProps):
    m_son(std::make_unique<Hyp<ASubTheorem>>(subProps)), m_extVars(getDbVarFromTheorems(subProps)), 
    m_eval(std::make_unique<Evaluater>()),
    m_state(HYP, getStates(subProps))
{
    computeAllPaths();
    computeImplPaths();
    initEval();
}

template<>
SubTheorem<Implication<ASubTheorem>>::SubTheorem(const ptr<ASubTheorem>& leftSubProp,
                                                 const ptr<ASubTheorem>& rightSubProp):
    m_son(std::make_unique<Implication<ASubTheorem>>(leftSubProp,rightSubProp)), 
    m_extVars(leftSubProp->getExtVars(), rightSubProp->getExtVars()), m_eval(std::make_unique<Evaluater>()),
    m_state(IMPLICATION, leftSubProp->getState(), rightSubProp->getState())
{
    computeAllPaths();
    computeImplPaths();
    initEval();
}

SubTheorem<Not<ASubTheorem>>::SubTheorem(const ptr<ASubTheorem>& subProp):
    m_son(std::make_unique<Not<ASubTheorem>>(subProp)), m_extVars(subProp->getExtVars()), 
    m_eval(std::make_unique<Evaluater>()),
    m_state(NOT, subProp->getState())
{
    computeAllPaths();
    computeImplPaths();
    initEval();
}

template<>
SubTheorem<Or<ASubTheorem>>::SubTheorem(const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp):
    m_son(std::make_unique<Or<ASubTheorem>>(leftSubProp,rightSubProp)), m_extVars(leftSubProp->getExtVars(),rightSubProp->getExtVars()), 
    m_eval(std::make_unique<Evaluater>()),
    m_state(OR, leftSubProp->getState(), rightSubProp->getState())
{
    computeAllPaths();
    computeImplPaths();
    initEval();
}

SubTheorem<Boolean>::SubTheorem(const std::shared_ptr<Boolean>& son):
    m_son(son), m_extVars(son), m_eval(std::make_unique<Evaluater>()),
    m_state(son->id(), BOOL_TYPE)
{
    computeAllPaths();
    computeImplPaths();
    initEval();
}

SubTheorem<ConstBoolean>::SubTheorem(const bool& val):
    m_son(std::make_unique<ConstBoolean>(val)),
    m_state(BOOL_TYPE, val)
{
    computeAllPaths();
    computeImplPaths();
}


SubTheorem<ConstBoolean>::SubTheorem(const SubRule<ConstBoolean>& prop):
    m_son(std::make_unique<ConstBoolean>(prop.evaluate())),
    m_state(BOOL_TYPE, prop.evaluate())
{
    computeAllPaths();
    computeImplPaths();
}

/**---------------------------------------------------------------
 * operator[] methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
const ptr<ASubTheorem> &SubTheorem<SubPropertyType>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubTheorem> &SubTheorem<Hyp<ASubTheorem>>::operator[](const size_t& index) const
{
    return (*m_son)[index];
}

const ptr<ASubTheorem> &SubTheorem<Not<ASubTheorem>>::operator[](const size_t&) const
{
    return (*m_son)[0];
}


/**---------------------------------------------------------------
 * arity methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
size_t SubTheorem<SubPropertyType>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Hyp<ASubTheorem>>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Not<ASubTheorem>>::arity() const
{
    return m_son->arity();
}

size_t SubTheorem<Boolean>::arity() const
{
    return 0;
}

size_t SubTheorem<ConstBoolean>::arity() const
{
    return 0;
}

/**---------------------------------------------------------------
 * getExtVars methods
 * ---------------------------------------------------------------
 */

const DbVar* SubTheorem<Hyp<ASubTheorem>>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubTheorem<Not<ASubTheorem>>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubTheorem<Boolean>::getExtVars() const
{
    return &m_extVars;
}

const DbVar* SubTheorem<ConstBoolean>::getExtVars() const
{
    return nullptr;
}


/**---------------------------------------------------------------
 * getConfigEvals methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
std::vector<std::pair<Evaluater::ConfigEval, bool>> SubTheorem<SubPropertyType>::getConfigEvals() const
{
    return m_eval->getConfigEvals();
}

std::vector<std::pair<Evaluater::ConfigEval, bool>> SubTheorem<Hyp<ASubTheorem>>::getConfigEvals() const
{
    return m_eval->getConfigEvals();
}

std::vector<std::pair<Evaluater::ConfigEval, bool>> SubTheorem<Not<ASubTheorem>>::getConfigEvals() const
{
    return m_eval->getConfigEvals();
}

std::vector<std::pair<Evaluater::ConfigEval, bool>> SubTheorem<Boolean>::getConfigEvals() const
{
    return m_eval->getConfigEvals();
}


/**---------------------------------------------------------------
 * evaluate methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::evaluate() const
{
    return m_eval->evaluate();
}

bool SubTheorem<Hyp<ASubTheorem>>::evaluate() const
{
    return m_eval->evaluate();
}

bool SubTheorem<Not<ASubTheorem>>::evaluate() const
{
    return m_eval->evaluate();
}

bool SubTheorem<Boolean>::evaluate() const
{
    return m_son->evaluate();
}

bool SubTheorem<ConstBoolean>::evaluate() const
{
    return m_son->evaluate();
}

/**---------------------------------------------------------------
 * testEvaluate methods
 * ---------------------------------------------------------------
 */
bool SubTheorem<Hyp<ASubTheorem>>::testEvaluate(const Evaluater::ConfigEval& configEval) const
{
    return m_eval->evalWithConfig(configEval);
}

bool SubTheorem<Not<ASubTheorem>>::testEvaluate(const Evaluater::ConfigEval& configEval) const
{
    return m_eval->evalWithConfig(configEval);
}

bool SubTheorem<Boolean>::testEvaluate(const Evaluater::ConfigEval& configEval) const
{
    return m_eval->evalWithConfig(configEval);
}

/**---------------------------------------------------------------
 * canBeDemonstrated methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::canBeDemonstrated() const
{
    return m_eval->canBeDemonstrated();
}

bool SubTheorem<Hyp<ASubTheorem>>::canBeDemonstrated() const
{
    return m_eval->canBeDemonstrated();
}

bool SubTheorem<Not<ASubTheorem>>::canBeDemonstrated() const
{
    return m_eval->canBeDemonstrated();
}

bool SubTheorem<Boolean>::canBeDemonstrated() const
{
    return false;
}

bool SubTheorem<ConstBoolean>::canBeDemonstrated() const
{
    return true;
}

/**---------------------------------------------------------------
 * getHiddenValue methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::getHiddenValue() const
{
    return m_eval->getHiddenValue();
}

bool SubTheorem<Hyp<ASubTheorem>>::getHiddenValue() const
{
    return m_eval->getHiddenValue();
}

bool SubTheorem<Not<ASubTheorem>>::getHiddenValue() const
{
    return m_eval->getHiddenValue();
}

bool SubTheorem<Boolean>::getHiddenValue() const
{
    return m_eval->getHiddenValue();
}

/**---------------------------------------------------------------
 * getVarToEval methods
 * ---------------------------------------------------------------
 */
std::unordered_map<IDVar, IDVar> SubTheorem<Hyp<ASubTheorem>>::getVarToEval() const
{
    return m_eval->getVarToEval();
}

std::unordered_map<IDVar, IDVar> SubTheorem<Not<ASubTheorem>>::getVarToEval() const
{
    return m_eval->getVarToEval();
}

std::unordered_map<IDVar, IDVar> SubTheorem<Boolean>::getVarToEval() const
{
    return m_eval->getVarToEval();
}

/**---------------------------------------------------------------
 * getCompatibleConfigs methods
 * ---------------------------------------------------------------
 */
inline std::vector<Evaluater::ConfigEval> SubTheorem<Hyp<ASubTheorem>>::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
    const std::unordered_map<IDVar, IDVar>& internalVars) const
{
    return m_eval->getCompatibleConfigs(commonConfig, internalVars);
}

inline std::vector<Evaluater::ConfigEval> SubTheorem<Not<ASubTheorem>>::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
    const std::unordered_map<IDVar, IDVar>& internalVars) const
{
    return m_eval->getCompatibleConfigs(commonConfig, internalVars);
}

inline std::vector<Evaluater::ConfigEval> SubTheorem<Boolean>::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
    const std::unordered_map<IDVar, IDVar>& internalVars) const
{
    return m_eval->getCompatibleConfigs(commonConfig, internalVars);
}


/**---------------------------------------------------------------
 * getState methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
const State& SubTheorem<SubPropertyType>::getState() const
{
    return m_state;
}

const State& SubTheorem<Hyp<ASubTheorem>>::getState() const
{
    return m_state;
}

const State& SubTheorem<Not<ASubTheorem>>::getState() const
{
    return m_state;
}

const State& SubTheorem<Boolean>::getState() const
{
    return m_state;
}

const State& SubTheorem<ConstBoolean>::getState() const
{
    return m_state;
}


/**---------------------------------------------------------------
 * operator== methods
 * ---------------------------------------------------------------
 */

template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Hyp<ASubTheorem>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}


bool SubTheorem<Not<ASubTheorem>>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<Boolean>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

bool SubTheorem<ConstBoolean>::isEqual(const ASubTheorem &prop) const
{
    auto propCast=dynamic_cast<const SubTheorem*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<>
bool SubTheorem<Let<ASubTheorem, ASubTheorem> >::
isEqual(const ASubRule &prop) const
{
    //FIX IT!!!!!!!!! not SubPropertyType
    auto propCast=dynamic_cast<const SubRule<Let<ASubRule, ASubRule> >*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}

template<>
bool SubTheorem<Let<ASubArithmeticTheorem<void>, ASubTheorem> >::
isEqual(const ASubRule &prop) const
{
    //FIX IT!!!!!!!!! not SubPropertyType
    auto propCast=dynamic_cast<const SubRule<Let<ASubArithmeticRule<void>, ASubRule> >*>(&prop);
    if(propCast)
    {
        return *this->m_son==(propCast->getSon());
    }
    else
    {
        return false;
    }
}


/**---------------------------------------------------------------
 * isEvaluated methods
 * ---------------------------------------------------------------
 */
bool SubTheorem<Hyp<ASubTheorem>>::isEvaluated() const
{
    return m_eval->isEvaluated();
}

bool SubTheorem<Not<ASubTheorem>>::isEvaluated() const
{
    return m_eval->isEvaluated();
}

bool SubTheorem<Boolean>::isEvaluated() const
{
    return m_eval->isEvaluated();
}

bool SubTheorem<ConstBoolean>::isEvaluated() const
{
    return true;
}


/**---------------------------------------------------------------
 * toString methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
std::string SubTheorem<SubPropertyType>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Hyp<ASubTheorem>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Not<ASubTheorem>>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<Boolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

std::string SubTheorem<ConstBoolean>::toString(unsigned short priorityParent) const
{
    return m_son->toString(priorityParent);
}

/**---------------------------------------------------------------
 * copyTheorem methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
ptr<ASubTheorem> SubTheorem<SubPropertyType>::copyTheorem() const
{
    return std::make_shared<const Theorem<SubPropertyType>>((*m_son)[0],(*m_son)[1]);
}

ptr<ASubTheorem> SubTheorem<Hyp<ASubTheorem>>::copyTheorem() const
{
    std::vector<ptr<ASubTheorem>> subProps;
    for(size_t k=0;k<arity();k++)
    {
        subProps.push_back((*m_son)[k]);
    }
    return std::make_shared<const Theorem<Hyp<ASubTheorem>>>(subProps);
}

ptr<ASubTheorem> SubTheorem<Not<ASubTheorem>>::copyTheorem() const
{
    return std::make_shared<const Theorem<Not<ASubTheorem>>>((*m_son)[0]);
}

ptr<ASubTheorem> SubTheorem<Boolean>::copyTheorem() const
{
    return std::make_shared<const Theorem<Boolean>>(m_son);
}

ptr<ASubTheorem> SubTheorem<ConstBoolean>::copyTheorem() const
{
    return std::make_shared<const Theorem<ConstBoolean>>(m_son->evaluate());
}

/**---------------------------------------------------------------
 * variableId methods
 * ---------------------------------------------------------------
 */
IDVar SubTheorem<Boolean>::variableId() const
{
    return m_son->id();
}

/**---------------------------------------------------------------
 * ruleApply methods
 * ---------------------------------------------------------------
 */
template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> SubTheorem<SubPropertyType>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    if(index==0)
    {
        return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(
        std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,(*m_son)[0],dbVarProp,indexes)), 
            (*m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(
        (*m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,(*m_son)[1],dbVarProp,
            indexes)));
    }
}

template<SubTheoremProperty SubPropertyType>
ptr<IISubTheoremFormula> SubTheorem<SubPropertyType>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    if (index == 0)
    {
        return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(logicIdx,
            std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, (*m_son)[0], dbVarProp, indexes, 
                logicIdx)), (*m_son)[1]);
    }
    else
    {
        return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(logicIdx,
            (*m_son)[0], std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, (*m_son)[1], dbVarProp,
                indexes, logicIdx)));
    }
}

ptr<IISubTheoremFormula> SubTheorem<Hyp<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey) const
{
    Arity index=indexes[0];
    indexes.erase(indexes.begin());
    std::vector<ptr<ASubTheorem>> sons;
    for(size_t k=0;k<index;k++)
    {
        sons.push_back((*m_son)[k]);
    }

    sons.push_back(std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,((*m_son)[index]),
        dbVarProp,indexes)));

    for(size_t k=index+1;k<m_son->arity();k++)
    {
        sons.push_back((*m_son)[k]);
    }

    return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(sons);
}

ptr<IISubTheoremFormula> SubTheorem<Hyp<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    Arity index = indexes[0];
    indexes.erase(indexes.begin());
    std::vector<ptr<ASubTheorem>> sons;
    for (size_t k = 0; k < index; k++)
    {
        sons.push_back((*m_son)[k]);
    }

    sons.push_back(std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, ((*m_son)[index]),
        dbVarProp, indexes, logicIdx)));

    for (size_t k = index + 1; k < m_son->arity(); k++)
    {
        sons.push_back((*m_son)[k]);
    }

    return Logic::make_theorem_formula<SubTheorem<SubPropertyType>>(logicIdx, sons);
}

ptr<IISubTheoremFormula> SubTheorem<Not<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey) const
{
    indexes.erase(indexes.begin());
    return Logic::make_theorem_formula<SubTheorem<Not<ASubTheorem>>>(
    std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey,(*m_son)[0],
        dbVarProp, indexes)));
}

ptr<IISubTheoremFormula> SubTheorem<Not<ASubTheorem>>::ruleApply(const IISubRuleFormula& rule, DbVarProp& dbVarProp,
    std::vector<Arity>& indexes, const Action::Id& actionKey, const size_t& logicIdx) const
{
    indexes.erase(indexes.begin());
    return Logic::make_theorem_formula<SubTheorem<Not<ASubTheorem>>>(logicIdx,
        std::static_pointer_cast<const ASubTheorem>(static_cast<const ASubRule*>(&rule)->applyAnnexe(actionKey, (*m_son)[0], 
            dbVarProp, indexes, logicIdx)));
}

ptr<IISubTheoremFormula> SubTheorem<Boolean>::ruleApply(const IISubRuleFormula&, DbVarProp&,
    std::vector<Arity>&, const Action::Id&) const
{
    throw std::runtime_error("SubTheorem Boolean cannot call ruleApply method");
}

ptr<IISubTheoremFormula> SubTheorem<Boolean>::ruleApply(const IISubRuleFormula&, DbVarProp&,
    std::vector<Arity>&, const Action::Id&, const size_t&) const
{
    throw std::runtime_error("SubTheorem Boolean cannot call ruleApply method");
}

ptr<IISubTheoremFormula> SubTheorem<ConstBoolean>::ruleApply(const IISubRuleFormula&, DbVarProp&,
    std::vector<Arity>&, const Action::Id&) const
{
    throw std::runtime_error("SubTheorem ConstBoolean cannot call ruleApply method");
}

ptr<IISubTheoremFormula> SubTheorem<ConstBoolean>::ruleApply(const IISubRuleFormula&, DbVarProp&,
    std::vector<Arity>&, const Action::Id&, const size_t&) const
{
    throw std::runtime_error("SubTheorem ConstBoolean cannot call ruleApply method");
}

template<SubTheoremProperty SubPropertyType>
bool SubTheorem<SubPropertyType>::operator==(const SubTheorem &prop) const
{
    return *m_son==(prop->getSon());
}

/**---------------------------------------------------------------
 * computeAllPaths methods
 * ---------------------------------------------------------------
 */
const std::vector<std::vector<Arity> >& SubTheorem<Hyp<ASubTheorem>>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
        for(Action::Id i=0;i<m_son->arity();i++)
        {
            std::vector<std::vector<Arity>> sonPath=(*m_son)[i]->getAllPaths();
            if(sonPath.size())
            {
                for(size_t k=0; k<sonPath.size();k++)
                {
                    std::vector<Arity> crtPath=sonPath[k];
                    crtPath.insert(crtPath.begin(),i);
                    m_allPaths.push_back(crtPath);
                }
            }
            else
            {
                m_allPaths.push_back({i});
            }
        }
    }
    return m_allPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<Not<ASubTheorem>>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
        std::vector<std::vector<Arity>> sonPath=(*m_son)[0]->getAllPaths();
        for(size_t k=0; k<sonPath.size();k++)
        {
            std::vector<Arity> crtPath=sonPath[k];
            crtPath.insert(crtPath.begin(),0);
            m_allPaths.push_back(crtPath);
        }
    }
    return m_allPaths;
}


const std::vector<std::vector<Arity> >& SubTheorem<Boolean>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
    }
    return m_allPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<ConstBoolean>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});
    }
    return m_allPaths;
}


/**---------------------------------------------------------------
 * computeImplPaths methods
 * ---------------------------------------------------------------
 */

const std::vector<std::vector<Arity> >& SubTheorem<Hyp<ASubTheorem>>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});
        auto implIdx = static_cast<Action::Id>(arity() - 1);
        std::vector<std::vector<Arity>> sonPath = (*m_son)[implIdx]->getImplPaths();
        if (sonPath.size())
        {
            for (size_t k = 0; k < sonPath.size(); k++)
            {
                std::vector<Arity> crtPath = sonPath[k];
                crtPath.insert(crtPath.begin(), implIdx);
                m_implPaths.push_back(crtPath);
            }
        }
        else
        {
            m_implPaths.push_back({ implIdx });
        }
    }
    return m_implPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<Not<ASubTheorem>>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});
    }
    return m_implPaths;
}


const std::vector<std::vector<Arity> >& SubTheorem<Boolean>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});
    }
    return m_implPaths;
}

const std::vector<std::vector<Arity> >& SubTheorem<ConstBoolean>::computeImplPaths()
{
    if (!m_implPaths.size())
    {
        m_implPaths.push_back({});
    }
    return m_implPaths;
}


/**---------------------------------------------------------------
 * getSon methods
 * ---------------------------------------------------------------
 */
const Hyp<ASubTheorem> &SubTheorem<Hyp<ASubTheorem>>::getSon() const
{
    return *(m_son.get());
}

const Not<ASubTheorem> &SubTheorem<Not<ASubTheorem>>::getSon() const
{
    return *(m_son.get());
}
const Boolean &SubTheorem<Boolean>::getSon() const
{
    return *(m_son.get());
}

const ConstBoolean &SubTheorem<ConstBoolean>::getSon() const
{
    return *(m_son.get());
}
