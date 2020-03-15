#ifndef CONSTANTE_HXX
#define CONSTANTE_HXX
template<typename SetType_>
Constante<SetType_>::Constante(const std::string &name_):
    AbstractTerm(name_), m_val(stringToVal(name_))
{

}

template<typename SetType_>
Constante<SetType_>::Constante(const Constante::ValueType &val):
    AbstractTerm(valToString(val)), m_val(val)
{

}

template<typename SetType_>
Constante<SetType_>::Constante(const Constante &cste): AbstractTerm(cste), m_val(cste.m_val)
{

}


template<typename SetType>
constexpr typename Constante<SetType>::ValueType Constante<SetType>::evaluate() const
{
    return m_val;
}

template<typename SetType_>
bool Constante<SetType_>::isConstant() const
{
    return true;
}

/*template<typename SetType> template<typename T>
constexpr bool Constante<SetType>::operator==(const T&) const
{
    return false;
}*/

template<typename SetType>
constexpr bool Constante<SetType>::operator==(const ValueType& val2) const
{
    return m_val==val2;
}

template<typename SetType> template<typename SetType2>
constexpr bool Constante<SetType>::operator==(const Constante<SetType2>&) const
{
    return false;
}

template<typename SetType>
constexpr bool Constante<SetType>::operator==(const Constante<SetType>& var) const
{
    return m_val==var.m_val;
}

template<typename SetType_>
std::string Constante<SetType_>::valToString(const ValueType& val)
{
    std::string s;
    std::stringstream ss;
    ss<<val;
    ss>>s;
    return s;
}

template<typename SetType_>
typename Constante<SetType_>::ValueType Constante<SetType_>::stringToVal(const std::string& str)
{
    ValueType val;
    std::stringstream ss;
    ss<<str;
    ss>>val;
    return val;
}
#endif // CONSTANTE_HXX
