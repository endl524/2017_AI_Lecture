#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   ���� �� ��ü�� �⺻ Ʋ�� �Ǵ� Ŭ����.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

class BaseGameEntity
{
private:
	static int m_Next_Valid_ID; // ��ü ID�� '�����ϰ�' ����� �ֱ� ���� static ����.
	int m_ID; // ��ü ID.
	void SetID(const int& val);

public:
	BaseGameEntity(const int& id) { SetID(id); } // �����ڴ� '��ü ID'�� ���� ���� �޾ƿ;� �Ѵ�.
	virtual ~BaseGameEntity() {}

	virtual void Update() = 0; // ��ü���� �ٸ� ���� �ϹǷ� ��ü ������ '���� ���� �Լ�'�� ����.
	int Get_ID() const { return m_ID; }  // ��ü ID�� ��ȯ���ش�.
};

#endif