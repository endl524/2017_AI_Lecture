#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_HiHoneyImHome,
	Msg_StewReady,
	Msg_ComebackHome,
	Msg_ImGoing
};

inline std::string MsgToStr(int msg)
{
	switch (msg)
	{
	case Msg_HiHoneyImHome: return "HiHoneyImHome";

	case Msg_StewReady: return "StewReady";

	case Msg_ComebackHome: return "ComebackHome";

	case Msg_ImGoing: return "ImGoing";

	default: return "Not recognized!";
	}
}

#endif