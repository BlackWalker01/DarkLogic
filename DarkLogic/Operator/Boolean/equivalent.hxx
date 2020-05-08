/*===--- equivalent.hxx - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements equivalent operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_EQUIVALENT_HXX
#define DARK_LOGIC_EQUIVALENT_HXX

template<Proposition SubPropertyType>
Equivalent<SubPropertyType>::Equivalent(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2):
    Operator<EqFun<SubPropertyType>>(EQUIVALENT,prop1,prop2)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Equivalent operator must be a subTheorem or a subRule" );
}


template<Proposition SubPropertyType>
const ptr<SubPropertyType>& Equivalent<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool Equivalent<SubPropertyType>::operator==(const Equivalent<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<Proposition SubPropertyType>
bool EqFun<SubPropertyType>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() == std::get<1>(m_sonProps)->evaluate();
}

template<Proposition SubPropertyType>
std::string EqFun<SubPropertyType>::symbol()
{
    return EquivalentToStr::s_symbol;
}


template<Proposition SubPropertyType>
std::string EqFun<SubPropertyType>::toString(unsigned short priorityParent) const
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
constexpr unsigned short EqFun<SubPropertyType>::priority()
{
    return 18;
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool EqFun<SubPropertyType>::operator==(const EqFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<Proposition SubPropertyType>
const ptr<SubPropertyType>& EqFun<SubPropertyType>::operator[](const size_t &k) const
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
            throw std::runtime_error("Equivalent operator has only two arguments");
        }
    }
}


template<Proposition SubPropertyType>
EqFun<SubPropertyType>::EqFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2): m_sonProps(prop1,prop2)
{

}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const EqFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Equivalent<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}

#endif
