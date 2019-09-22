#ifndef NAMES_H
#define NAMES_H
#include <string>

enum // ��ü ���
{
	entity_Miner, // ���ΰ� ����
	entity_Elsa // ���ΰ��� �Ƴ�
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