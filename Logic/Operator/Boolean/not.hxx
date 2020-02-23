#ifndef NOT_HXX
#define NOT_HXX

template<typename SubPropertyType>
Not<SubPropertyType>::Not(const ptr<SubPropertyType> &son):
    Operator<NotFun<SubPropertyType>>(NOT,son)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Not operator must be a subTheorem or a subRule" );
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& Not<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool Not<SubPropertyType>::operator==(const Not<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<typename SubPropertyType>
bool NotFun<SubPropertyType>::operator()() const
{
    return !m_sonProp->evaluate();
}

template<typename SubPropertyType>
std::string NotFun<SubPropertyType>::symbol()
{
    return NotToStr::s_symbol;
}

template<typename SubPropertyType>
std::string NotFun<SubPropertyType>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+symbol()+m_sonProp->toString(priority())+")";
    }
    else
    {
        return symbol()+m_sonProp->toString(priority());
    }
}

template<typename SubPropertyType>
constexpr unsigned short NotFun<SubPropertyType>::priority()
{
    return 3;
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool NotFun<SubPropertyType>::operator==(const NotFun<SubPropertyType2> &ope) const
{
    return m_sonProp==ope.m_sonProp;
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& NotFun<SubPropertyType>::operator[](const size_t &k) const
{
    switch(k)
    {
        case 0:
        {
            return m_sonProp;
        }
        default:
        {
            throw std::runtime_error("Not operator has only one arguments");
        }
    }
}


template<typename SubPropertyType>
NotFun<SubPropertyType>::NotFun(const ptr<SubPropertyType> &prop1): m_sonProp(prop1)
{

}

template<typename SubPropertyType>
const ptr<SubPropertyType>&
get(const NotFun<SubPropertyType>& ope)
{
    return ope.m_sonProp;
}

template<typename SubPropertyType>
const ptr<SubPropertyType>&
get(const Not<SubPropertyType>& ope)
{
    return get(ope.m_fun);
}

#endif
