#include "property.h"
#include "Operator/alloperator.h"
#include "../logic.h"
#include "log.h"
#include "Variable/boolean.h"
#include "Variable/constboolean.h"
#include "Variable/dbvar.h"
#include "logichash.h"
#include "rule.h"
#include "dbvarprop.h"
#include "theorem.h"
#include <sstream>
#include <iostream>

using namespace N_LogicGame;

/*

Property::Property(string name_, std::string &cont_):
    m_name(name_),opeFils(nullptr),val(nullptr),extVars(new DbVar()),inVars(new DbVar()), m_value(new VarBool())(new DbProperty())
{
    parseContent(cont_);
}

Property::Property(string name_, std::vector<Operator *> opeList, std::vector<Operator *> orderedOpeList, std::vector<string> *varList):
    m_name(name_),opeFils(nullptr),val(nullptr),extVars(new DbVar()),inVars(new DbVar()), m_value(new VarBool())(new DbProperty())
{
    setOperator(opeList,orderedOpeList,varList);
}


Property::Property(string name_, Operator *opeFils_, DbVar* extVars_, DbVar* inVars_):
    m_name(name_),opeFils(opeFils_),val(nullptr),extVars(new DbVar(*extVars_)),inVars(new DbVar(*inVars_)), m_value(new VarBool())(new DbProperty())
{
    opeFils->newInstance();
}


Property::Property(Rule *rule, std::string name_):
    m_name(name_),opeFils(nullptr),val(nullptr),extVars(nullptr),inVars(nullptr), m_value(new VarBool())(new DbProperty())
{
    Property* ante=rule->antecedent();
    extVars=new DbVar(*(ante->extVars));
    inVars=new DbVar(*(ante->inVars));

    //get hypothesises
    auto anteHypDb=ante->hyps->getDb();
    auto hypImplTab=rule->getHypImpl();
    for(auto it=anteHypDb.begin();it!=anteHypDb.end();it++)
    {
        Property* propHyp=it->second;
        //HYP variables case
        std::string substr=propHyp->m_name.substr(0,3);
        if(substr==HYP)
        {
            //get all properties associate with HYP variable
            for(unsigned int k=0;k<hypImplTab.size();k++)
            {
                Property* hypImpl=hypImplTab[k];
                hyps->insert(new Theorem(*hypImpl));
            }
        }
        //other properties from hypothesises
        else
        {
            if(propHyp->getOpeFils())
            {
                hyps->insert(new Theorem(*propHyp,rule->getIdentif(),hypImplTab));
            }
            else
            {
                Boolean* valHyp=propHyp->getVal();
                Property* associate=rule->getIdentif()->find((Variable*)valHyp);
                hyps->insert(new Theorem(*associate));
            }
        }
    }


    if(ante->getOpeFils())
    {
        Operator* opeAnte=ante->getOpeFils();
        DbVarProp* dbVarProp=rule->getIdentif();
        opeFils=Operator::create(opeAnte,dbVarProp,hypImplTab);
    }
    else
    {
        Boolean* anteVal=ante->getVal();
        if(*anteVal==true)
        {
            val=new ConstBoolean("true",true);
        }
        else if(*anteVal==false)
        {
            val=new ConstBoolean("false",false);
        }
        else
        {
            Property* associate=rule->getIdentif()->find(anteVal);
            if(associate->getOpeFils())
            {
                Operator* opeAssociate=associate->getOpeFils();
                opeFils=Operator::create(opeAssociate);
            }
            else
            {
                Boolean* valAssociate=associate->getVal();
                val=new Boolean(*valAssociate);
            }
        }
    }

}

Property::Property(Property &prop):
    m_name(prop.name()),opeFils(nullptr),val(nullptr),extVars(nullptr),inVars(nullptr), m_value(new VarBool())(nullptr)
{
    if(prop.getOpeFils())
    {
        opeFils=Operator::create(prop.getOpeFils());
    }
    else
    {
        val=new Boolean(*(prop.getVal()));
    }
    extVars=new DbVar(*(prop.extVars));
    inVars=new DbVar(*(prop.inVars));
    hyps=new DbProperty(*(prop.hyps));
}


Property::Property(Property &prop, DbVarProp *dbVarProp, std::vector<Property *> &hypImpl_):
    m_name(prop.name()),opeFils(nullptr),val(nullptr),extVars(nullptr),inVars(nullptr), m_value(new VarBool())(nullptr)
{
    hyps=new DbProperty(*(prop.hyps),dbVarProp,hypImpl_);
    if(prop.getOpeFils())
    {
        opeFils=Operator::create(prop.getOpeFils(),dbVarProp,hypImpl_);
    }
    else
    {
        Boolean* propVal=prop.getVal();
        Property* associate=dbVarProp->find(propVal);
        Operator* opeAssociate=associate->getOpeFils();
        if(opeAssociate)
        {
            opeFils=Operator::create(opeAssociate);
        }
        else
        {
            Boolean* valAssociate=associate->getVal();
            val=new Boolean(*valAssociate);
        }
    }
    extVars=new DbVar(*(prop.extVars));
    inVars=new DbVar(*(prop.inVars));
}
*/


Hyp* N_LogicGame::getHyp(const std::string& name, const std::string &cont, unsigned int &index, unsigned int &numHyp)
{
    Hyp* hypOpe=new Hyp();
    std::string crtProp="";
    char c='\0';
    for(unsigned int k=index;k<cont.size();k++)
    {
        c=cont[k];
        index=k;
        if(c=='}')
        {
            if(crtProp!="")
            {
                std::string nameProp="";
                if(crtProp.substr(0,3).compare(s_HYP)==0)
                {
                    nameProp=crtProp;
                }
                else
                {
                    nameProp=name+"_Hyp"+uintToString(numHyp);
                }
                hypOpe->addProperty(*create(nameProp,crtProp));
                numHyp++;
            }
            break;
        }
        if(c==';')
        {
            if(crtProp!="")
            {
                std::string nameProp="";
                if(crtProp.substr(0,3).compare(s_HYP)==0)
                {
                    nameProp=crtProp;
                }
                else
                {
                    nameProp=name+"_Hyp"+uintToString(numHyp);
                }
                hypOpe->addProperty(*create(nameProp,crtProp));
                numHyp++;
                crtProp="";
            }
            continue;
        }
        crtProp+=c;
    }
    return hypOpe;
}


std::vector<std::pair<IOperator*,unsigned short>> N_LogicGame::orderOperator(std::vector<std::pair<IOperator*,unsigned short>> &opeList)
{
    if(opeList.size()<=1)
    {
        return opeList;
    }
    else
    {
        auto mid=opeList.begin()+opeList.size()/2;
        std::vector<std::pair<IOperator*,unsigned short>> opeList1(opeList.begin(),mid);
        std::vector<std::pair<IOperator*,unsigned short>> opeList2(mid,opeList.end());
        return fusion(orderOperator(opeList1),orderOperator(opeList2));
    }
}


std::vector<std::pair<IOperator*,unsigned short>> N_LogicGame::fusion(const std::vector<std::pair<IOperator *, unsigned short> >& opeList1,
                                                        const std::vector<std::pair<IOperator *, unsigned short> >& opeList2)
{
    if(opeList1.size()==0)
    {
        return opeList2;
    }
    else if(opeList2.size()==0)
    {
        return opeList1;
    }
    else
    {
        if(opeList1[0].second<opeList2[0].second)
        {
            std::vector<std::pair<IOperator*,unsigned short>> t1(opeList1.begin(),opeList1.begin()+1);
            std::vector<std::pair<IOperator*,unsigned short>> queueOpeList1(opeList1.begin()+1,opeList1.end());
            std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(queueOpeList1,opeList2);
            auto ret=t1;
            ret.insert(ret.end(),t2.begin(),t2.end());
            return ret;
        }
        else if(opeList1[0].second>opeList2[0].second)
        {
            std::vector<std::pair<IOperator*,unsigned short>> t1(opeList2.begin(),opeList2.begin()+1);
            std::vector<std::pair<IOperator*,unsigned short>> queueOpeList2(opeList2.begin()+1,opeList2.end());
            std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(opeList1,queueOpeList2);
            auto ret=t1;
            ret.insert(ret.end(),t2.begin(),t2.end());
            return ret;
        }
        else
        {
            IOperator* ope1=opeList1[0].first;
            IOperator* ope2=opeList2[0].first;
            if(ope1->priority()>ope2->priority())
            {
                std::vector<std::pair<IOperator*,unsigned short>> t1(opeList1.begin(),opeList1.begin()+1);
                std::vector<std::pair<IOperator*,unsigned short>> queueOpeList1(opeList1.begin()+1,opeList1.end());
                std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(queueOpeList1,opeList2);
                auto ret=t1;
                ret.insert(ret.end(),t2.begin(),t2.end());
                return ret;
            }
            else if(ope1->priority()<ope2->priority())
            {
                std::vector<std::pair<IOperator*,unsigned short>> t1(opeList2.begin(),opeList2.begin()+1);
                std::vector<std::pair<IOperator*,unsigned short>> queueOpeList2(opeList2.begin()+1,opeList2.end());
                std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(opeList1,queueOpeList2);
                auto ret=t1;
                ret.insert(ret.end(),t2.begin(),t2.end());
                return ret;
            }
            else
            {
                switch(ope1->associativity())
                {
                    case Associativity::LEFT:
                    {
                        std::vector<std::pair<IOperator*,unsigned short>> t1(opeList1.begin(),opeList1.begin()+1);
                        std::vector<std::pair<IOperator*,unsigned short>> queueOpeList1(opeList1.begin()+1,opeList1.end());
                        std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(queueOpeList1,opeList2);
                        auto ret=t1;
                        ret.insert(ret.end(),t2.begin(),t2.end());
                        return ret;
                    }
                    case Associativity::RIGHT:
                    {
                        std::vector<std::pair<IOperator*,unsigned short>> t1(opeList2.begin(),opeList2.begin()+1);
                        std::vector<std::pair<IOperator*,unsigned short>> queueOpeList2(opeList2.begin()+1,opeList2.end());
                        std::vector<std::pair<IOperator*,unsigned short>> t2=fusion(opeList1,queueOpeList2);
                        auto ret=t1;
                        ret.insert(ret.end(),t2.begin(),t2.end());
                        return ret;
                    }
                }
            }
        }
    }
    std::vector<std::pair<IOperator*, unsigned short>> ret;
	return ret;
}

bool isCharInVar(char c)
{
    return (c>='0' && c<='9')||(c>='a' && c<='z')||(c>='A' && c<='Z');
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


/**
 * @brief Property::parseContent
 * @param content
 * Ex: content="a&&b=>!c||d" -> (a&&b)=>((!c)||d)
 */
IProperty *N_LogicGame::create(const std::string &name, const std::string &content)
{
    //parse hypothesis
    unsigned int j=0;    
    char c='\0';
    unsigned int numHyp=0;

    //look for operators and variables
    unsigned int numPar=0;
    std::string crtNameOpe="";
    IOperator* ope=nullptr;
    std::string crtVar="";
    std::vector<std::pair<IOperator*,unsigned short>> opeList;
    //std::vector<unsigned short> parLvlList; //parenthesis level list
    std::vector<std::string> varList;
    for(unsigned int k=j;k<content.size();k++)
    {
        c=content[k];
        switch(c)
        {
            case '(':
            {
                if(crtVar!="")
                {
                    varList.push_back(crtVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=IOperator::createOperator<ASubTheorem>(crtNameOpe)))
                    {
                        opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                numPar++;
                continue;
            }
            case ')':
            {

                if(crtVar!="")
                {
                    varList.push_back(crtVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=IOperator::createOperator<ASubTheorem>(crtNameOpe)))
                    {
                        opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                numPar--;
                continue;
            }
            case ' ':
            {
                if(crtVar!="")
                {
                    varList.push_back(crtVar);
                }
                crtVar="";
                if(crtNameOpe!="")
                {
                    if((ope=IOperator::createOperator<ASubTheorem>(crtNameOpe)))
                    {
                        opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                        ope=nullptr;
                    }
                    else
                    {
                        throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                    }
                    crtNameOpe="";
                }
                continue;
            }
            //hypothesis case
            case '{':
            {
                k++;
                opeList.push_back(std::pair<IOperator*,unsigned short>(getHyp(name,content,k,numHyp),numPar));
                continue;
            }
        }
        if(isCharInVar(c))
        {
            crtVar+=c;
            if(crtNameOpe!="")
            {
                if((ope=IOperator::createOperator<ASubTheorem>(crtNameOpe)))
                {
                    opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                    ope=nullptr;
                }
                else
                {
                    throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                }
                crtNameOpe="";
            }
        }
        else
        {
            crtNameOpe+=c;
            if(IOperator::isOpeName(crtNameOpe.substr(0,crtNameOpe.size()-1)) && !IOperator::isOpeName(crtNameOpe))
            {
                ope=IOperator::createOperator<ASubTheorem>(crtNameOpe.substr(0,crtNameOpe.size()-1));
                opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                ope=nullptr;
                crtNameOpe=c;
            }
            if(crtVar!="")
            {
                varList.push_back(crtVar);
            }
            crtVar="";
        }
        if(k==content.size()-1)
        {
            if(crtVar!="")
            {
                varList.push_back(crtVar);
            }
            crtVar="";
            if(crtNameOpe!="")
            {
                if((ope=IOperator::createOperator<ASubTheorem>(crtNameOpe)))
                {
                    opeList.push_back(std::pair<IOperator*,unsigned short>(ope,numPar));
                    ope=nullptr;
                }
                else
                {
                    throw std::runtime_error("Unknown operator '"+crtNameOpe+"'. Exit...");
                }
                crtNameOpe="";
            }
        }
    }

    //order operators by increasing priority
    std::vector<std::pair<IOperator*,unsigned short>> orderedOpeList=orderOperator(opeList);
    std::vector<IOperator*> opeList_;
    std::vector<IOperator*> orderedOpeList_;
    for(unsigned int k=0;k<opeList.size();k++)
    {
        opeList_.push_back(opeList[k].first);
        orderedOpeList_.push_back(orderedOpeList[k].first);
    }
    if(orderedOpeList_.size()>0)
    {
        switch(orderedOpeList_[0]->name())
        {
            case AND:
            {
                return new SubTheorem<And<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case EQUIVALENT:
            {
                return new SubTheorem<Equivalent<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case HYP:
            {
                return new SubTheorem<Hyp<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case IMPLICATION:
            {
                return new SubTheorem<Implication<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case NOT:
            {
                return new SubTheorem<Not<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
            case OR:
            {
                return new SubTheorem<Or<ASubTheorem>>(name,opeList_,orderedOpeList_,varList);
            }
        }
    }
    else if (varList.size()==1)
    {
        if(varList[0].compare("true")==0)
        {
            return new SubTheorem<ConstBoolean>(name,opeList_,orderedOpeList_,varList);
        }
        else if(varList[0].compare("false")==0)
        {
            return new SubTheorem<ConstBoolean>(name,opeList_,orderedOpeList_,varList);
        }
        else
        {
            return new SubTheorem<Boolean>(name,opeList_,orderedOpeList_,varList);
        }
    }
    throw std::runtime_error("Syntax error in property. Exit...");
}


ASubTheorem *N_LogicGame::copyProperty(ASubTheorem &prop)
{
    switch(prop.type())
    {
        case IProperty::PropType::AND_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<And<ASubTheorem>>*>(&prop);
            return new SubTheorem<And<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::EQ_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Equivalent<ASubTheorem>>*>(&prop);
            return new SubTheorem<Equivalent<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::HYP_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Hyp<ASubTheorem>>*>(&prop);
            return new SubTheorem<Hyp<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::IMPL_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Implication<ASubTheorem>>*>(&prop);
            return new SubTheorem<Implication<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::NOT_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Not<ASubTheorem>>*>(&prop);
            return new SubTheorem<Not<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::OR_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Or<ASubTheorem>>*>(&prop);
            return new SubTheorem<Or<ASubTheorem>>(*propCast);
        }
        case IProperty::PropType::VAR_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<Boolean>*>(&prop);
            return new SubTheorem<Boolean>(*propCast);
        }
        case IProperty::PropType::CONST_PROP:
        {
            auto propCast=reinterpret_cast<SubTheorem<ConstBoolean>*>(&prop);
            return new SubTheorem<ConstBoolean>(*propCast);
        }
    }
    return nullptr;
}

AndFun::AndFun(AndFun &fun)
{
    std::get<0>(m_sonProps)=N_LogicGame::copyProperty(*(fun[0]));
    std::get<1>(m_sonProps)=copyProperty(*(fun[1]));
}

EqFun::EqFun(EqFun &fun)
{
    std::get<0>(m_sonProps)=copyProperty(*(fun[0]));
    std::get<1>(m_sonProps)=copyProperty(*(fun[1]));
}

HypFun::HypFun(HypFun &fun)
{
    for (size_t k=0;k<fun.arity()-1;k++)
    {
        m_sonProps[k]=copyProperty(*(fun[k]));
    }
    m_impl=copyProperty(*(fun[fun.arity()-1]));
}

ImplFun::ImplFun(ImplFun &fun)
{
    std::get<0>(m_sonProps)=copyProperty(*(fun[0]));
    std::get<1>(m_sonProps)=copyProperty(*(fun[1]));
}

NotFun::NotFun(NotFun &fun)
{
    m_sonProp=copyProperty(*(fun.m_sonProp));
}

OrFun::OrFun(OrFun &fun)
{
    std::get<0>(m_sonProps)=copyProperty(*(fun[0]));
    std::get<1>(m_sonProps)=copyProperty(*(fun[1]));
}
