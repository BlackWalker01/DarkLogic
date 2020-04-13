#ifndef DARK_LOGIC_FORMULA_HXX
#define DARK_LOGIC_FORMULA_HXX

template<Type ValueType>
void AbstractFormula<ValueType>::print() const
{
    std::cout<<toString()<<std::endl;
}

#endif // DARK_LOGIC_FORMULA_HXX
