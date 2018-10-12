#include "stdafx.h"
#include "Ado.h"


Ado::Ado()
{
	
}


Ado::~Ado()
{
}


BOOL Ado::Link(CString Data_Source, CString Initial_Catalog, CString User_ID, CString Password)//连接数据库
{
	//实例化两个智能指针
	HRESULT result = Conn.CreateInstance(__uuidof(Connection));
	if (FAILED(result))
		return FALSE;
	result = recordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(result))
		return FALSE;
	
	//构造连接字符串
	CString strConn = _T("");
	strConn.Format(_T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s;"),
		Data_Source, Initial_Catalog, User_ID, Password);
	
	//连接数据库
	try{
		result = Conn->Open((_bstr_t)strConn, "", "", adConnectUnspecified);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	if (FAILED(result))
		return FALSE;
	return TRUE;
}



BOOL Ado::Open(CString strSQL)//打开表
{
	//执行SQL语句
	HRESULT result = recordset->Open(_bstr_t(strSQL), (IDispatch *)Conn, adOpenKeyset, adLockOptimistic, adCmdText);
	if (FAILED(result))
		return FALSE;
	return TRUE;
}


BOOL Ado::Close()
{
	recordset->Close();
	return TRUE;
}


void Ado::MoveNext()
{
	recordset->MoveNext();
}



void Ado::MovePrev()
{
	recordset->MovePrevious();
}



void Ado::MoveFirst()
{
	recordset->MoveFirst();
}



void Ado::MoveLast()
{
	recordset->MoveLast();
}


BOOL Ado::Add(long count, CString str1, ...)
{
	if (!recordset->Supports(adAddNew))
		return FALSE;

	//可变的参数处理
	va_list var_arg;
	long n = 0;
	va_start(var_arg, str1);
	_Link *head, *pt,*p0;
	head = pt = p0 = NULL;

	for (; n < count-1; n++)
	{
		p0 = new _Link;
		p0->str = va_arg(var_arg, CString);
		p0->p = NULL;
		if (0 == n)
		{
			head = p0;
			pt = p0;
		}
		else
		{
			pt->p = p0;
			pt = pt->p;
		}
	}
	va_end(var_arg);


	//添加
	recordset->AddNew();
	recordset->Fields->GetItem(long(0))->Value = _variant_t(str1);//设置值
	pt = head;
	for (n = 1; n < count; n++)
	{
		recordset->Fields->GetItem(n)->Value = _variant_t(pt->str);//设置值
		pt = pt->p;
	}


	//释放动态内存
	while (NULL!=head)
	{
		pt = head;
		head = head->p;
		delete pt;
	}

	return TRUE;
}


BOOL Ado::Delete()
{
	if (!recordset->Supports(adDelete))
	{
		return FALSE;
	}
	else
		recordset->Delete(adAffectCurrent);
	return TRUE;
}


BOOL Ado::Change(long count, CString str1, ...)
{
	//可变的参数处理
	va_list var_arg;
	long n = 0;
	va_start(var_arg, str1);
	_Link *head, *pt, *p0;
	head = pt = p0 = NULL;

	for (; n < count - 1; n++)
	{
		p0 = new _Link;
		p0->str = va_arg(var_arg, CString);
		p0->p = NULL;
		if (0 == n)
		{
			head = p0;
			pt = p0;
		}
		else
		{
			pt->p = p0;
			pt = pt->p;
		}
	}
	va_end(var_arg);

	//修改
	recordset->Fields->GetItem(long(0))->Value = _variant_t(str1);//设置值
	pt = head;
	for (n = 1; n < count; n++)
	{
		recordset->Fields->GetItem(n)->Value = _variant_t(pt->str);//设置值
		pt = pt->p;
	}

	//释放动态内存
	while (NULL != head)
	{
		pt = head;
		head = head->p;
		delete pt;
	}

	return TRUE;
}



CString Ado::GetValue(long index)
{
	
	_bstr_t str = recordset->Fields->GetItem(index)->GetValue();
	return (LPCTSTR)str;
}


int Ado::GetFieldCount()
{
	return recordset->Fields->GetCount();
}



CString Ado::GetFieldInfo(long index)
{
	_bstr_t str = recordset->Fields->GetItem(index)->GetName();
	return (LPCTSTR)str;
}



BOOL Ado::IsBOF()
{
	if (recordset->GetBOF())
		return TRUE;
	else
		return FALSE;
}



BOOL Ado::IsEOF()
{
	if (recordset->Getend())
		return TRUE;
	else
		return FALSE;
}


void Ado::Update()
{
	recordset->Update();
}