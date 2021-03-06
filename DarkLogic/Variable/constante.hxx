/*===--- allvariable - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file includes implements constant class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_CONSTANTE_HXX
#define DARK_LOGIC_CONSTANTE_HXX
template<SetDerived SetType_>
Constante<SetType_>::Constante(const std::string &name_):
    AbstractTerm(name_), m_val(stringToVal(name_))
{

}

template<SetDerived SetType_>
Constante<SetType_>::Constante(const Constante::ValueType &val):
    AbstractTerm(valToString(val)), m_val(val)
{

}

template<SetDerived SetType_>
Constante<SetType_>::Constante(const Constante &cste): AbstractTerm(cste), m_val(cste.m_val)
{

}


template<SetDerived SetType>
constexpr typename Constante<SetType>::ValueType Constante<SetType>::evaluate() const
{
    return m_val;
}

template<SetDerived SetType_>
bool Constante<SetType_>::isConstant() const
{
    return true;
}

/*template<typename SetType> template<typename T>
constexpr bool Constante<SetType>::operator==(const T&) const
{
    return false;
}*/

template<SetDerived SetType>
constexpr bool Constante<SetType>::operator==(const ValueType& val2) const
{
    return m_val==val2;
}

template<SetDerived SetType> template<SetDerived SetType2>
constexpr bool Constante<SetType>::operator==(const Constante<SetType2>&) const
{
    return false;
}

template<SetDerived SetType>
constexpr bool Constante<SetType>::operator==(const Constante<SetType>& var) const
{
    return m_val==var.m_val;
}

template<SetDerived SetType_>
std::string Constante<SetType_>::valToString(const ValueType& val)
{
    std::string s;
    std::stringstream ss;
    ss<<val;
    ss>>s;
    return s;
}

template<SetDerived SetType_>
typename Constante<SetType_>::ValueType Constante<SetType_>::stringToVal(const std::string& str)
{
    ValueType val;
    std::stringstream ss;
    ss<<str;
    ss>>val;
    return val;
}
#endif // DARK_LOGIC_CONSTANTE_HXX
