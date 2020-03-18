#include "booleanset.h"
#include "Formula/Boolean/rule.h"
#include "set_impl.h"

using namespace N_Logic;

void BooleanSet::init()
{
    //AXIOME Rule
    auto ax=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("ax","true<=>({p,HYP}p)"));
    insert(ax);

    //AND rules
    auto andI=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("&&I","(({HYP}p)&&({HYP}q))<=>({HYP}p&&q)"));
    insert(andI);
    auto andE=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("&&E","({HYP}p&&q)<=>(({HYP}p)&&({HYP}q))"));
    insert(andE);

    auto andEl=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("&&Eg","({HYP}p&&q)=>({HYP}p)")); //complicated to use!
    insert(andEl);
    auto andEr=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("&&Ed","({HYP}p&&q)=>({HYP}q)")); //complicated to use!
    insert(andEr);

    //OR Rules
    auto orI=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("||I","(({HYP}p)||({HYP}q))<=>({HYP}p||q)"));
    insert(orI);
    auto orIl=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||Ig","({HYP}p)=>({HYP}p||q)"));
    insert(orIl);
    auto orIr=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||Id","({HYP}q)=>({HYP}p||q)"));
    insert(orIr);
    auto orE=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||E","(({HYP}p||q)&&({p,HYP}r)&&({q,HYP}r))=>(({HYP}r)")); //complicated to use!
    insert(orE);

    //IMPLICATION Rules
    auto implI=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("=>I","({p,HYP}q)=>({HYP}p=>q)"));
    insert(implI);
    auto implE=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("=>E","(({HYP}p)&&({HYP}p=>q))=>({p,p=>q,HYP}q)"));
    insert(implE);


    //FALSE Rules
    auto fi=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("FI","({HYP}(p&&!p))=>({HYP}false)"));
    insert(fi);
    auto fe=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("FE","({p,HYP}false)<=>({HYP}!p)"));
    insert(fe);
    auto fiNot=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("FI!","({!p,HYP}p)=>({!p,HYP}false)"));
    insert(fiNot);

    //WEAKENING Rule
    auto weak=std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("weakening","({HYP}p)=>({q,HYP}p)"));
    insert(weak);

    //ABSURD REASONNING Rules (ARR)
    auto arr=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("arr","({!p,HYP}false)<=>({HYP}p)"));
    insert(arr);
    auto doubleNot=std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("!!","({HYP}!!p)<=>({HYP}p)"));
    insert(doubleNot);
}