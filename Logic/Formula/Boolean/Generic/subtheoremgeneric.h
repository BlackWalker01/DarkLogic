#ifndef SUBTHEOREMGENERIC_H
#define SUBTHEOREMGENERIC_H
#include "Formula/Boolean/subtheorem.h"
#include "Formula/Boolean/asubpuretheorem.h"

namespace N_Logic
{
template<typename SubPropertyType>
class SubTheorem: public ASubPureTheorem
{
public:
    SubTheorem(const std::string& name, const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);    

    bool evaluate() const override final;

    bool isEqual(const ASubTheorem& prop) const override final;
    bool operator==(const SubTheorem& prop) const;
    bool isEqual(const ASubRule& prop) const override final;
    bool operator==(const SubRule<SubPropertyType>& prop) const;

    std::string toString(unsigned short priorityParent=1000) const override final;

    size_t arity() const override final;
    ptr<ASubTheorem> copyTheorem() const override final;
    const SubPropertyType& getSon() const;
    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes, const size_t& actionKey) const override;

    ~SubTheorem() override = default;
protected:
    const std::vector<std::vector<Arity> > &computeAllPaths() override final;

protected:
    const ptr<ASubTheorem>& operator[](const size_t& index) const override final;

    const std::unique_ptr<const SubPropertyType> m_son;
};

template<typename SubPropertyType>
const SubPropertyType &SubTheorem<SubPropertyType>::getSon() const
{
    return *(m_son.get());
}

template<typename SubPropertyType>
const std::vector<std::vector<Arity> >& SubTheorem<SubPropertyType>::computeAllPaths()
{
    if(!m_allPaths.size())
    {
        m_allPaths.push_back({});

        //left subProperty
        std::vector<std::vector<Arity>> leftPath=(*m_son)[0]->getAllPaths();
        for(size_t k=0; k<leftPath.size();k++)
        {
            std::vector<Arity> crtPath=leftPath[k];
            crtPath.insert(crtPath.begin(),0);
            m_allPaths.push_back(crtPath);
        }

        //right subProperty
        std::vector<std::vector<Arity>> rightPath=(*m_son)[1]->getAllPaths();
        for(size_t k=0; k<rightPath.size();k++)
        {
            std::vector<Arity> crtPath=rightPath[k];
            crtPath.insert(crtPath.begin(),1);
            m_allPaths.push_back(crtPath);
        }
    }
    return m_allPaths;
}

}
#endif // SUBTHEOREMGENERIC_H
