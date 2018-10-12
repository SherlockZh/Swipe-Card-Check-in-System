#include "stdafx.h"
#include "Ado.h"


Ado::Ado()
{
	
}


Ado::~Ado()
{
}


BOOL Ado::Link(CString Data_Source, CString Initial_Catalog, CString User_ID, CString Password)//�������ݿ�
{
	//ʵ������������ָ��
	HRESULT result = Conn.CreateInstance(__uuidof(Connection));
	if (FAILED(result))
		return FALSE;
	result = recordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(result))
		return FALSE;
	
	//���������ַ���
	CString strConn = _T("");
	strConn.Format(_T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s;"),
		Data_Source, Initial_Catalog, User_ID, Password);
	
	//�������ݿ�
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



BOOL Ado::Open(CString strSQL)//�򿪱�
{
	//ִ��SQL���
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

	//�ɱ�Ĳ�������
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


	//���
	recordset->AddNew();
	recordset->Fields->GetItem(long(0))->Value = _variant_t(str1);//����ֵ
	pt = head;
	for (n = 1; n < count; n++)
	{
		recordset->Fields->GetItem(n)->Value = _variant_t(pt->str);//����ֵ
		pt = pt->p;
	}


	//�ͷŶ�̬�ڴ�
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
	//�ɱ�Ĳ�������
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

	//�޸�
	recordset->Fields->GetItem(long(0))->Value = _variant_t(str1);//����ֵ
	pt = head;
	for (n = 1; n < count; n++)
	{
		recordset->Fields->GetItem(n)->Value = _variant_t(pt->str);//����ֵ
		pt = pt->p;
	}

	//�ͷŶ�̬�ڴ�
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