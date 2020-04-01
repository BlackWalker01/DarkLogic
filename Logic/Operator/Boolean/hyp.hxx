#ifndef HYP_HXX
#define HYP_HXX

template<Proposition SubPropertyType>
Hyp<SubPropertyType>::Hyp(const std::vector<ptr<SubPropertyType> > &props):
    Operator<HypFun<SubPropertyType>>(HYP,props)
{
    if(props.size()<2)
    {
        throw std::runtime_error("arity of hypothsis operator must be greater or equal than 2");
    }
}

template<Proposition SubPropertyType>
const ptr<SubPropertyType>& Hyp<SubPropertyType>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
bool Hyp<SubPropertyType>::operator==(const Hyp<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

template<Proposition SubPropertyType>
bool HypFun<SubPropertyType>::operator()() const
{
    bool ret=true;
    std::unique_ptr<std::runtime_error> except=nullptr;
    for(size_t k=0;k<m_sonProps.size()-1;k++)
    {
        try
        {
            ret = m_sonProps[k]->evaluate();
            if (!ret)
            {
                except = nullptr;
                break;
            }
        }
        catch (std::runtime_error& e)
        {
            except = std::make_unique<std::runtime_error>(e);
        }        
    }
    if (except)
    {
        throw *except;
    }
    return ret? m_sonProps.back()->evaluate(): true;
}

template<Proposition SubPropertyType>
Arity HypFun<SubPropertyType>::arity() const
{
    return m_sonProps.size();
}

template<Proposition SubPropertyType>
std::string HypFun<SubPropertyType>::symbol()
{
    return HypToStr::s_symbol;
}

template<Proposition SubPropertyType>
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

template<Proposition SubPropertyType>
constexpr unsigned short HypFun<SubPropertyType>::priority()
{
    return 18;
}

template<Proposition SubPropertyType> template<Proposition SubPropertyType2>
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

template<Proposition SubPropertyType>
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

template<Proposition SubPropertyType>
HypFun<SubPropertyType>::HypFun(const std::vector<ptr<SubPropertyType> > &props): m_sonProps(props)
{

}

#endif
