#ifndef PROPERTY_H
#define PROPERTY_H
#include "string"
#include "vector"
#include "iproperty.h"
#include "Set/booleanset.h"
#include "Variable/dbvar.h"
#include "Operator/alloperator.h"
#include "Variable/boolean.h"
#include "Variable/constboolean.h"
#include "Property/dbvarprop.h"


namespace N_LogicGame {

class IOperator;

//template<typename SubPropertyType, bool isTheorem, typename HeadSetType, typename ...QueueSetType>
//class Property;

IProperty* create(const std::string& name, const std::string& content);
IProperty* copyProperty(IProperty& prop);
std::vector<std::pair<IOperator*,unsigned short>> orderOperator(std::vector<std::pair<IOperator*,unsigned short>>& opeList);
std::vector<std::pair<IOperator*,unsigned short>> fusion(const std::vector<std::pair<IOperator *, unsigned short> >& opeList1,
                                                         const std::vector<std::pair<IOperator *, unsigned short> >& opeList2);
Hyp* getHyp(const std::string& name, const std::string &cont, unsigned int &index, unsigned int &numHyp);

const std::string s_HYP="HYP";

/**
 * Theorem case of Boolean Property
 * SubPropertyType is Operator, Variable or Constant
 */
template<typename SubPropertyType>
class Property<SubPropertyType,true,BooleanSet>: public IProperty
{
public:

    Property(const std::string& name_, std::vector<IOperator*>& opeList,
             std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList);

    Property(const Property& prop);

    Property(const std::string& name, const std::vector<IProperty*>& subProps);

    Property(const Property<ConstBoolean,false,BooleanSet>& prop);

    bool evaluate() const override final
    {
        return m_son->evaluate();
    }

    void setSubVar(const std::string& /*nameVar*/, bool /*val*/) override final
    {

    }
    bool operator==(IProperty& prop) const override final
    {
        Property* propCast=dynamic_cast<Property*>(&prop);
        if(propCast)
        {
            return *this->m_son==*(propCast->m_son);
        }
        else
        {
            return false;
        }
    }
    bool operator==(const Property& prop) const
    {
        return *m_son==*(prop.m_son);
    }

    /*template<typename SubPropertyType2, typename SetType>
    bool operator==(const Property<SubPropertyType2,true,SetType>& prop) const;
    template<typename SubPropertyType2, typename SetType>
    bool operator==(const Property<SubPropertyType2,false,SetType>& prop) const;*/

    std::string toString(unsigned short priorityParent=1000) const override final
    {
        return m_son->toString(priorityParent);
    }

    DbVar<BooleanSet>& getExtVars() override final
    {
        return m_extVars;
    }

    SubPropertyType* getSon()
    {
        return m_son;
    }

    void setImpl(IProperty& impl);

    ~Property() override
    {
        delete m_son;
        m_son=nullptr;
    }
private:
    void setOperator(std::vector<IOperator*>& opeList,
                     std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList);
    void setSubProperty(const std::string& name, std::vector<IOperator*>& opeList,
                     std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList);

    void setUpExtVars(DbVar<BooleanSet>& extvars)
    {
        m_extVars.insert(extvars);
    }

protected:
    bool identifyPriv(IProperty&, DbVarProp<BooleanSet>&) override final
    {
        return false;
    }

    IProperty* apply(const size_t&, const std::string&) override final
    {
        throw std::runtime_error("Cannot call apply with theorems");
    }

    IProperty* applyPriv(const std::string&, DbVarProp<BooleanSet>&) override final
    {
        throw std::runtime_error("Cannot call applyPriv with theorems");
    }    

    IProperty* operator[](const size_t& index) override final;
    size_t arity() const override final;

    SubPropertyType* m_son;
    DbVar<BooleanSet> m_extVars;
};

template<>
void Property<Not, true, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList);

template<>
void Property<Hyp, true, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList);

template<>
void Property<Boolean, true, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList);

template<>
void Property<ConstBoolean, true, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList);

/**
 *Property constructor
 */
template<>
Property<And, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::AND_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Equivalent, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::EQ_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Hyp, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::HYP_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Implication, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::IMPL_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Not, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::NOT_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Or, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::OR_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Boolean, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::VAR_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<ConstBoolean, true, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::CONST_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}


/**
 * Property copy constructor
 */
template<>
Property<And,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new And(*(prop.m_son)))
{

}

template<>
Property<Equivalent,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Equivalent(*(prop.m_son)))
{

}

template<>
Property<Hyp,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Hyp(*(prop.m_son)))
{

}

template<>
Property<Implication,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Implication(*(prop.m_son)))
{

}

template<>
Property<Not,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Not(*(prop.m_son)))
{

}

template<>
Property<Or,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Or(*(prop.m_son)))
{

}

template<>
Property<Boolean,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new Boolean(*(prop.m_son)))
{

}

template<>
Property<ConstBoolean,true,BooleanSet>::Property(const Property& prop):
    IProperty(prop), m_son(new ConstBoolean(*(prop.m_son)))
{

}


/**
 * Property constructor from subProperties
 */
template<>
Property<And,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,AND_PROP), m_son(new And(*(subProps[0]),*(subProps[1])))
{

}

template<>
Property<Equivalent,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,EQ_PROP), m_son(new Equivalent(*(subProps[0]),*(subProps[1])))
{

}

template<>
Property<Hyp,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,HYP_PROP), m_son(new Hyp(subProps))
{

}

template<>
Property<Implication,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,IMPL_PROP), m_son(new Implication(*(subProps[0]),*(subProps[1])))
{

}

template<>
Property<Not,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,NOT_PROP), m_son(new Not(*(subProps[0])))
{

}

template<>
Property<Or,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>& subProps):
    IProperty(name_,OR_PROP), m_son(new Or(*(subProps[0]),*(subProps[1])))
{

}

template<>
Property<Boolean,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>&):
    IProperty(name_,VAR_PROP)
{

}

template<>
Property<ConstBoolean,true,BooleanSet>::Property(const std::string& name_, const std::vector<IProperty*>&):
    IProperty(name_,CONST_PROP)
{

}


/**
 * setOperator methods
 */
template<typename SubPropertyType>
void Property<SubPropertyType, true, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<SubPropertyType*>(orderedOpeList[0]);
    std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
    std::vector<IOperator*> leftOpeList;
    std::vector<IOperator*> rightOpeList;
    for(unsigned int k=0; k<opeList.size();k++)
    {
        if(m_son==dynamic_cast<SubPropertyType*>(opeList[k]))
        {
            leftOpeList.insert(leftOpeList.begin(),opeList.begin(),opeList.begin()+k);
            rightOpeList.insert(rightOpeList.begin(),opeList.begin()+k+1,opeList.end());
            break;
        }
    }

    //store remaining operators to the left of current operator
    std::unordered_map<IOperator*,IOperator*> hashLeftOpe;
    for(unsigned int i=0;i<leftOpeList.size();i++)
    {
        IOperator* crtOpe=leftOpeList[i];
        hashLeftOpe[crtOpe]=crtOpe;
    }

    //store remaining operators to the right of current operator
    std::unordered_map<IOperator*,IOperator*> hashRightOpe;
    for(unsigned int i=0;i<rightOpeList.size();i++)
    {
        IOperator* crtOpe=rightOpeList[i];
        hashRightOpe[crtOpe]=crtOpe;
    }

    //separate operators in queueOrderedOpeList to the left and to the right and order them in each list
    std::vector<IOperator*> leftOrderedOpeList;
    std::vector<IOperator*> rightOrderedOpeList;
    for(unsigned int i=0;i<queueOrderedOpeList.size();i++)
    {
        IOperator* crtOpe=queueOrderedOpeList[i];
        auto itLeft=hashLeftOpe.find(crtOpe);
        if(itLeft!=hashLeftOpe.end())
        {
            leftOrderedOpeList.push_back(crtOpe);
            continue;
        }
        auto itRight=hashRightOpe.find(crtOpe);
        if(itRight!=hashRightOpe.end())
        {
            rightOrderedOpeList.push_back(crtOpe);
            continue;
        }
    }

    //left sub-Property
    setSubProperty(m_name+m_son->symbol()+"L",leftOpeList,leftOrderedOpeList,varList);

    //right sub-Property
    setSubProperty(m_name+m_son->symbol()+"R",rightOpeList,rightOrderedOpeList,varList);
}


template<>
void Property<Not, true, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<Not*>(orderedOpeList[0]);
    if(m_son==opeList[0])
    {
        std::vector<IOperator*> queueOpeList(opeList.begin()+1,opeList.end());
        std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
        setSubProperty(m_name+m_son->symbol(),queueOpeList,queueOrderedOpeList,varList);
    }
    else
    {
        throw std::runtime_error("Operator with one argument in the middle of the expression!");
    }
}

template<>
void Property<Hyp, true, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<Hyp*>(orderedOpeList[0]);
    if(m_son==orderedOpeList[0])
    {
        m_son=nullptr;
        std::vector<IOperator*> queueOpeList(opeList.begin()+1,opeList.end());
        std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
        //setOperator(queueOpeList,queueOrderedOpeList,varList);
        if(queueOrderedOpeList.size()>0)
        {
            switch(queueOrderedOpeList[0]->name())
            {
                case AND:
                {
                    auto prop=new Property<And,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case EQUIVALENT:
                {
                    auto prop=new Property<Equivalent,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case HYP:
                {
                    auto prop=new Property<Hyp,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case IMPLICATION:
                {
                    auto prop=new Property<Implication,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case NOT:
                {
                    auto prop=new Property<Not,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case OR:
                {
                    auto prop=new Property<Or,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
            }
        }
        else if (varList.size()>0)
        {
            if(varList[0].compare("true")==0)
            {
                auto prop=new Property<ConstBoolean,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else if(varList[0].compare("false")==0)
            {
                auto prop=new Property<ConstBoolean,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else
            {
                auto prop=new Property<Boolean,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
        }
        else
        {
            throw std::runtime_error("Operator with one argument in the middle of the expression!");
        }
    }
    else
    {
        throw std::runtime_error("Operator with one argument in the middle of the expression!");
    }


}


template<>
void Property<Boolean, true, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList)
{
    m_son=new Boolean(varList[0]);
    m_extVars.insert(*m_son);
    varList.erase(varList.begin());
}


template<>
void Property<ConstBoolean, true, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList)
{
    if(varList[0].compare("true")==0)
    {
        m_son=new ConstBoolean("true",true);
        varList.erase(varList.begin());
    }
    else
    {
        m_son=new ConstBoolean("false",false);
        varList.erase(varList.begin());
    }
}


template<typename SubProperty>
void Property<SubProperty, true, BooleanSet>::setSubProperty(const std::string& name, std::vector<IOperator*>& opeList,
                                                             std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList)
{
    if(orderedOpeList.size()>0)
    {
        switch(orderedOpeList[0]->name())
        {
            case AND:
            {
                auto prop=new Property<And,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
            case EQUIVALENT:
            {
                auto prop=new Property<Equivalent,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
            case HYP:
            {
                auto prop=new Property<Hyp,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
            case IMPLICATION:
            {
                auto prop=new Property<Implication,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
            case NOT:
            {
                auto prop=new Property<Not,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
            case OR:
            {
                auto prop=new Property<Or,true,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
                break;
            }
        }
    }
    else if (varList.size()>0)
    {
        if(varList[0].compare("true")==0)
        {
            auto prop=new Property<ConstBoolean,true,BooleanSet>(name,opeList,orderedOpeList,varList);
            m_son->addProperty(*prop);
            setUpExtVars(prop->getExtVars());
        }
        else if(varList[0].compare("false")==0)
        {
            auto prop=new Property<ConstBoolean,true,BooleanSet>(name,opeList,orderedOpeList,varList);
            m_son->addProperty(*prop);
            setUpExtVars(prop->getExtVars());
        }
        else
        {
            auto prop=new Property<Boolean,true,BooleanSet>(name,opeList,orderedOpeList,varList);
            m_son->addProperty(*prop);
            setUpExtVars(prop->getExtVars());
        }
    }
    else
    {
        throw(std::runtime_error("No variable or operator in Property"));
    }
}

template<>
void Property<Hyp,true,BooleanSet>::setImpl(IProperty& impl)
{
    m_son->setImpl(impl);
}

template<typename SubPropertyType>
IProperty* Property<SubPropertyType,true,BooleanSet>::operator[](const size_t& index)
{
    return (*m_son)[index];
}

template<>
IProperty* Property<Boolean,true,BooleanSet>::operator[](const size_t&)
{
    throw std::runtime_error("Cannot access to sub property in Boolean Property");
}

template<>
IProperty* Property<ConstBoolean,true,BooleanSet>::operator[](const size_t&)
{
    throw std::runtime_error("Cannot access to sub property in ConstBoolean Property");
}

template<typename SubPropertyType>
size_t Property<SubPropertyType,true,BooleanSet>::arity() const
{
    return m_son->arity();
}

template<>
size_t Property<Boolean,true,BooleanSet>::arity() const
{
    return 0;
}

template<>
size_t Property<ConstBoolean,true,BooleanSet>::arity() const
{
    return 0;
}

/**
 * Other cases of Boolean Property
 * SubPropertyType is Operator, Variable or Constant
 */
template<typename SubPropertyType>
class Property<SubPropertyType,false,BooleanSet>: public IProperty
{
public:

    Property(const std::string& name_, std::vector<IOperator*>& opeList,
             std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList);    

    bool evaluate() const override final
    {
        return true;
    }
    void setSubVar(const std::string& /*nameVar*/, bool /*val*/) override final
    {

    }

    bool operator==(IProperty& prop) const override final
    {
        Property* propCast=dynamic_cast<Property*>(&prop);
        if(propCast)
        {
            return *this->m_son==*(propCast->m_son);
        }
        else
        {
            return false;
        }
    }
    bool operator==(const Property& prop) const
    {
        return *m_son==*(prop.m_son);
    }

    std::string toString(unsigned short priorityParent=1000) const override final
    {
        return m_son->toString(priorityParent);
    }

    DbVar<BooleanSet>& getExtVars() override final
    {
        return m_extVars;
    }

    std::vector<std::string> nameVars() const
    {
        return m_extVars.nameVars();
    }

    ~Property() override
    {
        delete m_son;
        m_son=nullptr;
    }

protected:
    bool identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp) override final;
    IProperty* apply(const size_t&, const std::string&) override
    {
        throw std::runtime_error("Cannot call apply with Properties that are not rules");
    }
    IProperty* applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp) override final;
    IProperty* applyPrivWithVect(const std::string& thName, std::vector<size_t>& indexes,
                                       DbVarProp<BooleanSet>& dbVarProp) override final;
    IProperty* operator[](const size_t& index) override final;
    size_t arity() const override final;

private:
    void setOperator(std::vector<IOperator*>& opeList,
                     std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList);
    void setSubProperty(const std::string& name, std::vector<IOperator*>& opeList,
                     std::vector<IOperator*>& orderedOpeList, std::vector<std::string>& varList)
    {
        if(orderedOpeList.size()>0)
        {
            switch(orderedOpeList[0]->name())
            {
                case AND:
                {
                    auto prop=new Property<And,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case EQUIVALENT:
                {
                    auto prop=new Property<Equivalent,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case HYP:
                {
                    auto prop=new Property<Hyp,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case IMPLICATION:
                {
                    auto prop=new Property<Implication,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case NOT:
                {
                    auto prop=new Property<Not,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
                case OR:
                {
                    auto prop=new Property<Or,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                    m_son->addProperty(*prop);
                    setUpExtVars(prop->getExtVars());
                    break;
                }
            }
        }
        else if (varList.size()>0)
        {
            if(varList[0].compare("true")==0)
            {
                auto prop=new Property<ConstBoolean,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else if(varList[0].compare("false")==0)
            {
                auto prop=new Property<ConstBoolean,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else
            {
                auto prop=new Property<Boolean,false,BooleanSet>(name,opeList,orderedOpeList,varList);
                m_son->addProperty(*prop);
                setUpExtVars(prop->getExtVars());
            }
        }
        else
        {
            throw(std::runtime_error("No variable or operator in Property"));
        }
    }


    void setUpExtVars(DbVar<BooleanSet>& extvars)
    {
        m_extVars.insert(extvars);
    }
protected:
    SubPropertyType* m_son;
    DbVar<BooleanSet> m_extVars;
};

/*
 * setOperator method declarations
 */
template<>
void Property<Not, false, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList);

template<>
void Property<Hyp, false, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList);

template<>
void Property<Boolean, false, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList);

template<>
void Property<ConstBoolean, false, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList);


/*
 * identifyPriv method declarations
 */
template<>
bool Property<And, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Equivalent, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Implication, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Hyp, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Not, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Or, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<Boolean, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

template<>
bool Property<ConstBoolean, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp);

/*
 * applyPriv method declarations
 */
template<>
IProperty* Property<And, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Equivalent, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Implication, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Hyp, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Not, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Or, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<Boolean, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);

template<>
IProperty* Property<ConstBoolean, false, BooleanSet>::applyPriv(const std::string& thName, DbVarProp<BooleanSet>& dbVarProp);


template<>
Property<And, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::AND_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Equivalent, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::EQ_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Hyp, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::HYP_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Implication, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::IMPL_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Not, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::NOT_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Or, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::OR_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<Boolean, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::VAR_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}

template<>
Property<ConstBoolean, false, BooleanSet>::Property(const std::string &name_, std::vector<IOperator *> &opeList,
                                          std::vector<IOperator *> &orderedOpeList, std::vector<std::string> &varList):
    IProperty(name_,PropType::CONST_PROP)
{
    setOperator(opeList,orderedOpeList,varList);
}


template<typename SubPropertyType>
void Property<SubPropertyType, false, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<SubPropertyType*>(orderedOpeList[0]);
    std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
    std::vector<IOperator*> leftOpeList;
    std::vector<IOperator*> rightOpeList;
    for(unsigned int k=0; k<opeList.size();k++)
    {
        if(m_son==dynamic_cast<SubPropertyType*>(opeList[k]))
        {
            leftOpeList.insert(leftOpeList.begin(),opeList.begin(),opeList.begin()+k);
            rightOpeList.insert(rightOpeList.begin(),opeList.begin()+k+1,opeList.end());
            break;
        }
    }

    //store remaining operators to the left of current operator
    std::unordered_map<IOperator*,IOperator*> hashLeftOpe;
    for(unsigned int i=0;i<leftOpeList.size();i++)
    {
        IOperator* crtOpe=leftOpeList[i];
        hashLeftOpe[crtOpe]=crtOpe;
    }

    //store remaining operators to the right of current operator
    std::unordered_map<IOperator*,IOperator*> hashRightOpe;
    for(unsigned int i=0;i<rightOpeList.size();i++)
    {
        IOperator* crtOpe=rightOpeList[i];
        hashRightOpe[crtOpe]=crtOpe;
    }

    //separate operators in queueOrderedOpeList to the left and to the right and order them in each list
    std::vector<IOperator*> leftOrderedOpeList;
    std::vector<IOperator*> rightOrderedOpeList;
    for(unsigned int i=0;i<queueOrderedOpeList.size();i++)
    {
        IOperator* crtOpe=queueOrderedOpeList[i];
        auto itLeft=hashLeftOpe.find(crtOpe);
        if(itLeft!=hashLeftOpe.end())
        {
            leftOrderedOpeList.push_back(crtOpe);
            continue;
        }
        auto itRight=hashRightOpe.find(crtOpe);
        if(itRight!=hashRightOpe.end())
        {
            rightOrderedOpeList.push_back(crtOpe);
            continue;
        }
    }

    //left sub-Property
    setSubProperty(m_name+m_son->symbol()+"L",leftOpeList,leftOrderedOpeList,varList);

    //right sub-Property
    setSubProperty(m_name+m_son->symbol()+"R",rightOpeList,rightOrderedOpeList,varList);
}


template<>
void Property<Not, false, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<Not*>(orderedOpeList[0]);
    if(m_son==opeList[0])
    {
        std::vector<IOperator*> queueOpeList(opeList.begin()+1,opeList.end());
        std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
        setSubProperty(m_name+m_son->symbol(),queueOpeList,queueOrderedOpeList,varList);
    }
    else
    {
        throw std::runtime_error("Operator with one argument in the middle of the expression!");
    }
}

template<>
void Property<Hyp, false, BooleanSet>::setOperator(std::vector<IOperator *> &opeList, std::vector<IOperator *> &orderedOpeList,
                                                              std::vector<std::string> &varList)
{
    m_son=dynamic_cast<Hyp*>(orderedOpeList[0]);
    if(m_son==orderedOpeList[0])
    {
        //m_hyps=m_son;
        //m_son=nullptr;
        std::vector<IOperator*> queueOpeList(opeList.begin()+1,opeList.end());
        std::vector<IOperator*> queueOrderedOpeList(orderedOpeList.begin()+1,orderedOpeList.end());
        if(queueOrderedOpeList.size()>0)
        {
            switch(queueOrderedOpeList[0]->name())
            {
                case AND:
                {
                    auto prop=new Property<And,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
                case EQUIVALENT:
                {
                    auto prop=new Property<Equivalent,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
                case HYP:
                {
                    auto prop=new Property<Hyp,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
                case IMPLICATION:
                {
                    auto prop=new Property<Implication,true,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
                case NOT:
                {
                    auto prop=new Property<Not,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
                case OR:
                {
                    auto prop=new Property<Or,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                    m_son->setImpl(*prop);
                    setUpExtVars(m_son->getExtVars());
                    break;
                }
            }
        }
        else if (varList.size()>0)
        {
            if(varList[0].compare("true")==0)
            {
                auto prop=new Property<ConstBoolean,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else if(varList[0].compare("false")==0)
            {
                auto prop=new Property<ConstBoolean,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
            else
            {
                auto prop=new Property<Boolean,false,BooleanSet>(m_name,queueOpeList,queueOrderedOpeList,varList);
                m_son->setImpl(*prop);
                setUpExtVars(prop->getExtVars());
            }
        }
        else
        {
            throw(std::runtime_error("No variable or operator in Property"));
        }
    }
    else
    {
        throw std::runtime_error("Operator with one argument in the middle of the expression!");
    }
}


template<>
void Property<Boolean, false, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList)
{
    m_son=new Boolean(varList[0]);
    m_extVars.insert(*m_son);
    varList.erase(varList.begin());
}


template<>
void Property<ConstBoolean, false, BooleanSet>::setOperator(std::vector<IOperator *> &, std::vector<IOperator *> &,
                                                              std::vector<std::string> &varList)
{
    if(varList[0].compare("true")==0)
    {
        m_son=new ConstBoolean("true",true);
        varList.erase(varList.begin());
    }
    else
    {
        m_son=new ConstBoolean("false",false);
        varList.erase(varList.begin());
    }
}

template<>
bool Property<And, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::AND_PROP)
    {
        Property<And,true,BooleanSet>* propCast=reinterpret_cast<Property<And,true,BooleanSet>*>(&prop);
        And* ope=propCast->getSon();
        return (*m_son)[0]->identifyPriv(*(*ope)[0],dbVarProp) && (*m_son)[1]->identifyPriv(*(*ope)[1],dbVarProp);
    }
    return false;
}

template<>
bool Property<Equivalent, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::EQ_PROP)
    {
        Property<Equivalent,true,BooleanSet>* propCast=reinterpret_cast<Property<Equivalent,true,BooleanSet>*>(&prop);
        Equivalent* ope=propCast->getSon();
        return (*m_son)[0]->identifyPriv(*(*ope)[0],dbVarProp) && (*m_son)[1]->identifyPriv(*(*ope)[1],dbVarProp);
    }
    return false;
}

template<>
bool Property<Implication, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::IMPL_PROP)
    {
        Property<Implication,true,BooleanSet>* propCast=reinterpret_cast<Property<Implication,true,BooleanSet>*>(&prop);
        Implication* ope=propCast->getSon();
        return (*m_son)[0]->identifyPriv(*(*ope)[0],dbVarProp) && (*m_son)[1]->identifyPriv(*(*ope)[1],dbVarProp);
    }
    return false;
}

template<>
bool Property<Hyp, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::HYP_PROP)
    {
        Property<Hyp,true,BooleanSet>* propCast=reinterpret_cast<Property<Hyp,true,BooleanSet>*>(&prop);
        Hyp* ope=propCast->getSon();
        if(m_son->arity()>=ope->arity()-1)
        {
            bool ret=false;
            //Identify all properties before HYP variable and conclusion
            for(size_t k=0;k<m_son->arity()-2;k++)
            {
                ret=(*m_son)[k]->identifyPriv(*(*ope)[k],dbVarProp);
                if(!ret)
                {
                    return false;
                }
            }
            if(ret)
            {
                std::string nameHypVar=(*m_son)[m_son->arity()-2]->toString();
                size_t indexHyp=m_son->arity()-2;
                if(dbVarProp.containsHyp(nameHypVar))
                {
                    //check if old mapping is the same as the current one
                    auto props=dbVarProp.getHypAssoc(nameHypVar);
                    for(size_t k=indexHyp;k<ope->arity()-1;k++)
                    {
                        ret=*(props[k-indexHyp])==*(*ope)[k];
                        if(!ret)
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    //map HYP variable with prop subProperties
                    for(size_t k=indexHyp;k<ope->arity()-1;k++)
                    {
                        dbVarProp.insertHypAssoc(nameHypVar,*(*ope)[k]);
                    }
                }

                ret=(*m_son)[m_son->arity()-1]->identifyPriv(*(*ope)[ope->arity()-1],dbVarProp);
            }
            return ret;
        }
    }
    else if(m_son->arity()==2)
    {
        std::string nameHypVar=(*m_son)[0]->toString();
        dbVarProp.insertHypEmpty(nameHypVar);
        return (*m_son)[1]->identifyPriv(prop,dbVarProp);
    }
    return false;
}

template<>
bool Property<Not, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::NOT_PROP)
    {
        Property<Not,true,BooleanSet>* propCast=reinterpret_cast<Property<Not,true,BooleanSet>*>(&prop);
        Not* ope=propCast->getSon();
        return (*m_son)[0]->identifyPriv(*(*ope)[0],dbVarProp);
    }
    return false;
}

template<>
bool Property<Or, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(prop.type()==PropType::OR_PROP)
    {
        Property<Or,true,BooleanSet>* propCast=reinterpret_cast<Property<Or,true,BooleanSet>*>(&prop);
        Or* ope=propCast->getSon();
        return (*m_son)[0]->identifyPriv(*(*ope)[0],dbVarProp) && (*m_son)[1]->identifyPriv(*(*ope)[1],dbVarProp);
    }
    return false;
}

template<>
bool Property<Boolean, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>& dbVarProp)
{
    if(!dbVarProp.contains(m_son->name()))
    {
        dbVarProp[m_son->name()]=&prop;
        return true;
    }
    else
    {
        auto exProp=dbVarProp[m_son->name()];
        return prop==*exProp;
    }
}

template<>
bool Property<ConstBoolean, false, BooleanSet>::identifyPriv(IProperty& prop, DbVarProp<BooleanSet>&)
{
    return (*this)==prop;
}

template<typename SubPropertyType>
IProperty* Property<SubPropertyType,false,BooleanSet>::operator[](const size_t& index)
{
    return (*m_son)[index];
}

template<>
IProperty* Property<Boolean,false,BooleanSet>::operator[](const size_t&)
{
    throw std::runtime_error("Cannot access to sub property in Boolean Property");
}

template<>
IProperty* Property<ConstBoolean,false,BooleanSet>::operator[](const size_t&)
{
    throw std::runtime_error("Cannot access to sub property in ConstBoolean Property");
}

template<typename SubPropertyType>
size_t Property<SubPropertyType,false,BooleanSet>::arity() const
{
    return m_son->arity();
}

template<>
size_t Property<Boolean,false,BooleanSet>::arity() const
{
    return 0;
}

template<>
size_t Property<ConstBoolean,false,BooleanSet>::arity() const
{
    return 0;
}

/**
 * SubPropertyType is Operator, Variable or Constant
 */
/*
template<typename SubPropertyType, typename HeadSetType, typename ...QueueSetType>
class Property: public IProperty<typename SubPropertyType::ValueType>
{

private:
    SubPropertyType* m_sonProp;
};*/


}



/**
 * @brief isCharInVar
 * @param c
 * @return true if c can be in a name of a variable
 */
bool isCharInVar(char c);
std::string uintToString(unsigned int x);
std::string sizeToString(const size_t& x);

#endif // PROPERTY_H
