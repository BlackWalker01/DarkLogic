#ifndef AND_HXX
#define AND_HXX

template<typename SubPropertyType>
And<SubPropertyType>::And(const ptr<SubPropertyType> &prop1,
                          const ptr<SubPropertyType> &prop2):
    Operator<AndFun<SubPropertyType>>(AND,prop1,prop2)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in And operator must be a subTheorem or a subRule" );
}


template<typename SubPropertyType> template<typename SubPropertyType2>
bool And<SubPropertyType>::operator==(const And<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& And<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubPropertyType>
bool AndFun<SubPropertyType>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() && std::get<1>(m_sonProps)->evaluate();
}


template<typename SubPropertyType>
std::string AndFun<SubPropertyType>::symbol()
{
    return AndToStr::s_symbol;
}

template<typename SubPropertyType>
std::string AndFun<SubPropertyType>::toString(unsigned short priorityParent) const
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

template<typename SubPropertyType>
constexpr unsigned short AndFun<SubPropertyType>::priority()
{
    return 13;
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool AndFun<SubPropertyType>::operator==(const AndFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& AndFun<SubPropertyType>::operator[](const size_t &k) const
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
            throw std::runtime_error("And operator has only two arguments");
        }
    }
}


template<typename SubPropertyType>
AndFun<SubPropertyType>::AndFun(const ptr<SubPropertyType> &prop1,
                                const ptr<SubPropertyType> &prop2): m_sonProps(prop1,prop2)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in And operator must be a subTheorem or a subRule" );
}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const AndFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const And<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}



#endif
