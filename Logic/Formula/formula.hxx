#ifndef FORMULA_HXX
#define FORMULA_HXX

template<Type ValueType>
void AbstractFormula<ValueType>::print() const
{
    std::cout<<toString()<<std::endl;
}

#endif // FORMULA_HXX
