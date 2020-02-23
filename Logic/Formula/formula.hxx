#ifndef FORMULA_HXX
#define FORMULA_HXX

template<typename ValueType>
AbstractFormula<ValueType>::AbstractFormula(const std::string &name_): m_name(name_)
{

}

template<typename ValueType>
std::string AbstractFormula<ValueType>::name() const
{
    return m_name;
}

template<typename ValueType>
void AbstractFormula<ValueType>::print() const
{
    std::cout<<toString()<<std::endl;
}

template<typename ValueType>
const DbVar &AbstractFormula<ValueType>::getExtVars() const
{
    return m_extVars;
}

template<typename ValueType>
std::vector<std::string> AbstractFormula<ValueType>::nameVars() const
{
    return m_extVars.nameVars();
}

#endif // FORMULA_HXX
