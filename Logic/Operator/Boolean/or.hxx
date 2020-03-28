#ifndef OR_HXX
#define OR_HXX

template<typename SubPropertyType>
Or<SubPropertyType>::Or(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2):
    Operator<OrFun<SubPropertyType>>(OR,prop1,prop2)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Or operator must be a subTheorem or a subRule" );
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& Or<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool Or<SubPropertyType>::operator==(const Or<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<typename SubPropertyType>
std::string OrFun<SubPropertyType>::symbol()
{
    return OrToStr::s_symbol;
}

template<typename SubPropertyType>
bool OrFun<SubPropertyType>::operator()() const
{
    try
    {
        if (std::get<0>(m_sonProps)->evaluate())
        {
            return true;
        }
        else
        {
            return std::get<1>(m_sonProps)->evaluate();
        }
    }
    catch (std::runtime_error&)
    {
        return std::get<1>(m_sonProps)->evaluate();
    }
}

template<typename SubPropertyType>
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

template<typename SubPropertyType>
constexpr unsigned short OrFun<SubPropertyType>::priority()
{
    return 14;
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool OrFun<SubPropertyType>::operator==(const OrFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename SubPropertyType>
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

template<typename SubPropertyType>
OrFun<SubPropertyType>::OrFun(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2):
    m_sonProps(prop1,prop2)
{

}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const OrFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const Or<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}

#endif
