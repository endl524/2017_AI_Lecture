#include "BaseGameEntity.h"
#include <cassert>

int BaseGameEntity::m_Next_Valid_ID = 0; // static 변수 값을 '0' 으로 초기화

void BaseGameEntity::SetID(const int& val)
{
	// 새로 할당되는 개체의 ID는 '유효한 ID' 값 이상이어야 한다.
	assert((val >= m_Next_Valid_ID) && "<BaseGameEntity::SetID>: invalid ID");
	m_ID = val;
	m_Next_Valid_ID = m_ID + 1;
}
