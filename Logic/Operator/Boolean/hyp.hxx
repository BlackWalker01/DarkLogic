#ifndef HYP_HXX
#define HYP_HXX

template<typename SubPropertyType>
Hyp<SubPropertyType>::Hyp(const std::vector<ptr<SubPropertyType> > &props):
    Operator<HypFun<SubPropertyType>>(HYP,props)
{
    if(props.size()<2)
    {
        throw std::runtime_error("arity of hypothsis operator must be greater or equal than 2");
    }
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& Hyp<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool Hyp<SubPropertyType>::operator==(const Hyp<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<typename SubPropertyType>
bool HypFun<SubPropertyType>::operator()() const
{
    bool ret=true;
    for(size_t k=0;k<m_sonProps.size()-1;k++)
    {
        ret=m_sonProps[k]->evaluate();
        if(!ret)
        {
            break;
        }
    }
    return ret? m_sonProps.back()->evaluate(): true;
}

template<typename SubPropertyType>
Arity HypFun<SubPropertyType>::arity() const
{
    return m_sonProps.size();
}

template<typename SubPropertyType>
std::string HypFun<SubPropertyType>::symbol()
{
    return HypToStr::s_symbol;
}

template<typename SubPropertyType>
std::string HypFun<SubPropertyType>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        std::string ret="({";
        size_t k=0;
        ret+=m_sonProps[k]->toString();
        for(k=1;k<m_sonProps.size()-2;k++)
        {
            ret+=","+m_sonProps[k]->toString();
        }
        if(m_sonProps.size()>2)
        {
            ret+=","+m_sonProps[k]->toString();
        }
        ret+="} ";
        ret+=m_sonProps.back()->toString(priority());
        ret+=")";
        return ret;
    }
    else
    {
        std::string ret="{";
        size_t k=0;
        ret+=m_sonProps[k]->toString();
        for(k=1;k<m_sonProps.size()-2;k++)
        {
            ret+=","+m_sonProps[k]->toString();
        }
        if(m_sonProps.size()>2)
        {
            ret+=","+m_sonProps[k]->toString();
        }
        ret+="} ";
        ret+=m_sonProps.back()->toString(priority());
        return ret;
    }
}

template<typename SubPropertyType>
constexpr unsigned short HypFun<SubPropertyType>::priority()
{
    return 18;
}

template<typename SubPropertyType> template<typename SubPropertyType2>
bool HypFun<SubPropertyType>::operator==(const HypFun<SubPropertyType2> &ope) const
{
    if(m_sonProps.size()!=ope.m_sonProps.size())
    {
        return false;
    }
    for(size_t k=0;k<m_sonProps.size()-1;k++)
    {
        if(!(*(m_sonProps[k])==*(ope.m_sonProps[k])))
        {
            return false;
        }
    }
    return *(m_sonProps.back())==*(ope.m_sonProps.back());
}

template<typename SubPropertyType>
const ptr<SubPropertyType>& HypFun<SubPropertyType>::operator[](const size_t &k) const
{
    if(k<arity())
    {
        return m_sonProps[k];
    }
    else
    {
        std::stringstream ss;
        ss<<arity();
        std::string arityStr="";
        ss>>arityStr;
        throw std::runtime_error("this Hyp operator has only "+arityStr+" arguments");
    }
}

template<typename SubPropertyType>
HypFun<SubPropertyType>::HypFun(const std::vector<ptr<SubPropertyType> > &props): m_sonProps(props)
{

}

#endif
