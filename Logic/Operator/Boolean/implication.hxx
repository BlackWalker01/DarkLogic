#ifndef IMPLICATION_HXX
#define IMPLICATION_HXX

template<typename SubPropertyType>
Implication<SubPropertyType>::Implication(const ptr<SubPropertyType> &hyp, const ptr<SubPropertyType> &conseq):
    Operator<ImplFun<SubPropertyType>>(IMPLICATION,hyp,conseq)
{
    static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Implication operator must be a subTheorem or a subRule" );
}

template<typename SubPropertyType>
const ptr<const SubPropertyType>& Implication<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool Implication<SubPropertyType>::operator==(const Implication<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}


template<typename SubPropertyType>
bool ImplFun<SubPropertyType>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() ? std::get<1>(m_sonProps)->evaluate() : true;
}

template<typename SubPropertyType>
std::string ImplFun<SubPropertyType>::symbol()
{
    return ImplToStr::s_symbol;
}


template<typename SubPropertyType>
std::string ImplFun<SubPropertyType>::toString(unsigned short priorityParent) const
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
constexpr unsigned short ImplFun<SubPropertyType>::priority()
{
    return 19;
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool ImplFun<SubPropertyType>::operator==(const ImplFun<SubPropertyType2> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename SubPropertyType>
const ptr<const SubPropertyType>& ImplFun<SubPropertyType>::operator[](const size_t &k) const
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
            throw std::runtime_error("Implication operator has only two arguments");
        }
    }
}


template<typename SubPropertyType>
ImplFun<SubPropertyType>::ImplFun(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2): m_sonProps(prop1,prop2)
{

}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const ImplFun<SubPropertyType>& ope)
{
    return std::get<N>(ope.m_sonProps);
}

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const Implication<SubPropertyType>& ope)
{
    return get<N>(ope.m_fun);
}

#endif
