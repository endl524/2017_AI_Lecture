#ifndef NAMES_H
#define NAMES_H
#include <string>

enum // 개체 목록
{
	entity_Miner, // 주인공 광부
	entity_MinersWife, // 광부의 아내
	entity_MinersSon // 광부의 아들
};

inline std::string Get_Name_of_Entity(const int& n)
{
	switch (n)
	{
	case entity_Miner: return "Bob (Miner)";
	case entity_MinersWife: return "Elsa (Wife)";
	case entity_MinersSon: return "David (Son)";
	default: return "UNKNOWN!";
	}
}

#endif