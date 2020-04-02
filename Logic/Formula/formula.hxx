#ifndef FORMULA_HXX
#define FORMULA_HXX

template<Type ValueType>
AbstractFormula<ValueType>::AbstractFormula(const std::string &name_): m_name(name_)
{

}

template<Type ValueType>
std::string AbstractFormula<ValueType>::name() const
{
    return m_name;
}

template<Type ValueType>
void AbstractFormula<ValueType>::print() const
{
    std::cout<<toString()<<std::endl;
}

template<Type ValueType>
const DbVar &AbstractFormula<ValueType>::getExtVars() const
{
    return m_extVars;
}

template<Type ValueType>
std::vector<std::string> AbstractFormula<ValueType>::nameVars() const
{
    return m_extVars.nameVars();
}

#endif // FORMULA_HXX
