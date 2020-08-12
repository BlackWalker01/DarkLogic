/*===--- implication.hxx - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements implication operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IMPLICATION_HXX
#define DARK_LOGIC_IMPLICATION_HXX

template<Proposition SubPropertyType>
Implication<SubPropertyType>::Implication(const ptr<SubPropertyType> &hyp, const ptr<SubPropertyType> &conseq):
    Operator<ImplFun<SubPropertyType>>(IMPLICATION,hyp,conseq)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Implication operator must be a subTheorem or a subRule" );
}

template<Proposition SubPropertyType>
const ptr<const SubPropertyType>& Implication<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool Implication<SubPropertyType>::operator==(const Implication<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}


template<Proposition SubPropertyType>
bool ImplFun<SubPropertyType>::operator()() const
{
    try
    {
        if (!std::get<0>(m_sonProps)->evaluate())
        {
            return true;
        }
    }
    catch (VariableException& e)
    {
        if (std::get<1>(m_sonProps)->evaluate())
        {
            return true;
        }
        else
        {
            throw e;
        }
    }

    return std::get<1>(m_sonProps)->evaluate();
}

template<Proposition SubPropertyType>
std::string ImplFun<SubPropertyType>::symbol()
{
    return ImplToStr::s_symbol;
}


template<Proposition SubPropertyType>
std::string ImplFun<SubPropertyType>::toString(unsigned short priorityParent) const
{
    if(priorityParent<=priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<Proposition SubPropertyType>
constexpr unsigned short ImplFun<SubPropertyType>::priority()
{
    return 19;
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool ImplFun<SubPropertyType>::operator==(const ImplFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<Proposition SubPropertyType>
const ptr<const SubPropertyType>& ImplFun<SubPropertyType>::operator[](const size_t &k) const
{
    switch(k)
    {
        case 0:
        {
            return std::get<0>(m_sonProps);
        }
        case 1:
        {
            return std::get<1>(m_sonProps);
        }
        default:
        {
            throw std::runtime_error("Implication operator has only two arguments");
        }
    }
}


template<Proposition SubPropertyType>
ImplFun<SubPropertyType>::ImplFun(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2): m_sonProps(prop1,prop2)
{

}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const ImplFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Implication<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}

#endif
