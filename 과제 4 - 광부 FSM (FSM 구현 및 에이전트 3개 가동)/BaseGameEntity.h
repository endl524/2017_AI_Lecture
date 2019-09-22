#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   게임 속 개체의 기본 틀이 되는 클래스.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

class BaseGameEntity
{
private:
	static int m_Next_Valid_ID; // 개체 ID를 '유일하게' 만들어 주기 위한 static 변수.
	int m_ID; // 개체 ID.
	void SetID(const int& val);

public:
	BaseGameEntity(const int& id) { SetID(id); } // 생성자는 '개체 ID'로 사용될 값을 받아와야 한다.
	virtual ~BaseGameEntity() {}

	virtual void Update() = 0; // 개체마다 다른 일을 하므로 개체 갱신은 '순수 가상 함수'로 구현.
	int Get_ID() const { return m_ID; }  // 개체 ID를 반환해준다.
};

#endif