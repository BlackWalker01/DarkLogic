#pragma once
#include "ai.h"

class EvalAI: public AI
{
public:
	EvalAI(const size_t& maxInstanceIdx, const size_t timeoutScds);

	void explore(DbAction& actions, unsigned char threadIdx) override final;
	unsigned short int eval(const std::vector<N_DarkLogic::State>& states, 
		const unsigned char threadIdx) override final;
	unsigned int value() const override final;
};