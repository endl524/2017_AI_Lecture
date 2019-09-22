#ifndef NAMES_H
#define NAMES_H
#include <string>

enum // 개체 목록
{
	entity_Miner, // 주인공 광부
	entity_Elsa // 주인공의 아내
};

inline std::string Get_Name_of_Entity(const int& n)
{
	switch(n)
	{
	case entity_Miner: return "Miner";
	case entity_Elsa: return "Elsa";
	default: return "UNKNOWN!";
	}
}

#endif