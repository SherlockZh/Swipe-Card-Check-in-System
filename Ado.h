#pragma once
#import"C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","end")
struct _Link{
	CString str;
	_Link *p;
};

class Ado
{
public:
	Ado();
	~Ado();

	BOOL Link(CString Data_Source, CString Initial_Catalog, CString User_ID, CString Password);//�������ݿ�
	BOOL Open(CString strSQL);//�򿪱�
	BOOL Close();//�رձ�
	//�ƶ�ָ��
	void MoveNext();
	void MovePrev();
	void MoveFirst();
	void MoveLast();
	//��ɾ���޸�
	BOOL Add(long count,CString str1, ...);
	BOOL Delete();
	BOOL Change(long count, CString str1, ...);
	//ȡֵ
	CString GetValue(long index);
	//�ֶ���Ŀ���ֶ�����
	int GetFieldCount();
	CString GetFieldInfo(long index);
	//ͷβ
	BOOL IsBOF();
	BOOL IsEOF();
	//����
	void Update();


	_RecordsetPtr recordset;

private:
	_ConnectionPtr Conn;
};

