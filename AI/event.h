#ifndef EVENT_H
#define EVENT_H
class Event
{
public:
	enum EventEnum
	{
		START,
		STOP,
		STOP_THREAD
	};

	Event(const unsigned char threadIdx_, EventEnum type_);

	unsigned char threadIdx() const;
	EventEnum type() const;

private:
	unsigned char m_threadIdx; //index from thread origin
	EventEnum m_type;
};
#endif // !EVENT_H
