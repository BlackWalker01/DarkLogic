#include "event.h"

Event::Event(const unsigned char threadIdx_, EventEnum type_):
	m_threadIdx(threadIdx_), m_type(type_)
{
}

unsigned char Event::threadIdx() const
{
	return m_threadIdx;
}

Event::EventEnum Event::type() const
{
	return m_type;
}
