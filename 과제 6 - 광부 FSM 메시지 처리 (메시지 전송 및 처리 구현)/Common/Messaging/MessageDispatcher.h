#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)

#include <set>

#include "../misc/ConsoleUtils.h"
#include "../Messaging/Telegram.h"

class BaseGameEntity;

const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO   = 0;

#define Dispatch MessageDispatcher::Instance()


class MessageDispatcher
{
private:
	std::set<Telegram> PriorityQ;

	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

	MessageDispatcher() {}
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);

public:
	static MessageDispatcher* Instance();

	void DispatchMessage(double delay, int sender, int receiver, int msg, void* ExtraInfo);
	void DispatchDelayedMessages();
};

#endif