#include "evalai.h"

EvalAI::EvalAI(const size_t& maxInstanceIdx, const size_t timeoutScds):
	AI("EvalAI", maxInstanceIdx, timeoutScds)
{
    m_elo = 2500;
}

void EvalAI::explore(DbAction& actions, unsigned char threadIdx)
{
    m_crtNode->exploreEval(actions, threadIdx);
}

unsigned short int EvalAI::eval(const std::vector<N_DarkLogic::State>& states, const unsigned char threadIdx)
{
    const N_DarkLogic::State& state = states[0];
    const auto& opeList = state.priorityOpe();
    // first: opeSize
    unsigned short int opeSize = static_cast<unsigned short int>(opeList.size());
    // second : nb of double not
    unsigned short int nbDoubleNots = 0;
    for (const auto& ope : opeList)
    {
        const std::vector<N_DarkLogic::State::ParentOperator>& parentOpes = ope.parentOperators();
        unsigned short int consecNots = 0;
        if (ope.name() == N_DarkLogic::Name::NOT)
        {
            consecNots += 1;
        }
        for (size_t k = 0; k < parentOpes.size(); k++)
        {
            const auto& parentOpe = parentOpes[parentOpes.size() - k - 1];
            if (parentOpe.name() == N_DarkLogic::Name::NOT)
            {
                consecNots += 1;
            }
            else
            {
                unsigned short int quo = consecNots / 2;
                nbDoubleNots = (nbDoubleNots >= quo) ? nbDoubleNots : quo;
                consecNots = 0;
            }
        }
        if (consecNots != 0)
        {
            unsigned short int quo = consecNots / 2;
            nbDoubleNots = (nbDoubleNots >= quo) ? nbDoubleNots : quo;
            consecNots = 0;
        }        
    }
    return opeSize + nbDoubleNots;
}

unsigned int EvalAI::value() const
{
    return m_crtNode->realValue();
}
