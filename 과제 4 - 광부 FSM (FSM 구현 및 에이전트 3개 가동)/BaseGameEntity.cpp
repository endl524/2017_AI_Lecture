#include "BaseGameEntity.h"
#include <cassert>

int BaseGameEntity::m_Next_Valid_ID = 0; // static ���� ���� '0' ���� �ʱ�ȭ

void BaseGameEntity::SetID(const int& val)
{
	// ���� �Ҵ�Ǵ� ��ü�� ID�� '��ȿ�� ID' �� �̻��̾�� �Ѵ�.
	assert((val >= m_Next_Valid_ID) && "<BaseGameEntity::SetID>: invalid ID");
	m_ID = val;
	m_Next_Valid_ID = m_ID + 1;
}
