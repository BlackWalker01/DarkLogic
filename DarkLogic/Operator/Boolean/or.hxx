/*===--- or.hxx - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements Or operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_OR_HXX
#define DARK_LOGIC_OR_HXX

template<Proposition SubPropertyType>
Or<SubPropertyType>::Or(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2):
    Operator<OrFun<SubPropertyType>>(OR,prop1,prop2)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Or operator must be a subTheorem or a subRule" );
}

template<Proposition SubPropertyType>
const ptr<SubPropertyType>& Or<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool Or<SubPropertyType>::operator==(const Or<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<Proposition SubPropertyType>
std::string OrFun<SubPropertyType>::symbol()
{
    return OrToStr::s_symbol;
}

template<Proposition SubPropertyType>
bool OrFun<SubPropertyType>::operator()() const
{
    try
    {
        if (std::get<0>(m_sonProps)->testEvaluate())
        {
            return true;
        }
    }
    catch (VariableException&)
    {
        return std::get<1>(m_sonProps)->testEvaluate();
    }
    return std::get<1>(m_sonProps)->testEvaluate();
}

template<Proposition SubPropertyType>
std::string OrFun<SubPropertyType>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<Proposition SubPropertyType>
constexpr unsigned short OrFun<SubPropertyType>::priority()
{
    return 14;
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool OrFun<SubPropertyType>::operator==(const OrFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<Proposition SubPropertyType>
const ptr<SubPropertyType>& OrFun<SubPropertyType>::operator[](const size_t &k) const
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
            throw std::runtime_error("Or operator has only two arguments");
        }
    }
}

template<Proposition SubPropertyType>
OrFun<SubPropertyType>::OrFun(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2):
    m_sonProps(prop1,prop2)
{

}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const OrFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Or<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}

#endif
