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

	BOOL Link(CString Data_Source, CString Initial_Catalog, CString User_ID, CString Password);//连接数据库
	BOOL Open(CString strSQL);//打开表
	BOOL Close();//关闭表
	//移动指针
	void MoveNext();
	void MovePrev();
	void MoveFirst();
	void MoveLast();
	//增删、修改
	BOOL Add(long count,CString str1, ...);
	BOOL Delete();
	BOOL Change(long count, CString str1, ...);
	//取值
	CString GetValue(long index);
	//字段数目和字段名字
	int GetFieldCount();
	CString GetFieldInfo(long index);
	//头尾
	BOOL IsBOF();
	BOOL IsEOF();
	//更新
	void Update();


	_RecordsetPtr recordset;

private:
	_ConnectionPtr Conn;
};

