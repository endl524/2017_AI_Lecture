#ifndef NAMES_H
#define NAMES_H
#include <string>

enum // ��ü ���
{
	entity_Miner, // ���ΰ� ����
	entity_MinersWife, // ������ �Ƴ�
	entity_MinersSon // ������ �Ƶ�
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