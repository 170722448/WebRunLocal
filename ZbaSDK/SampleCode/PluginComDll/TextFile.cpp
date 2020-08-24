// TextFile.cpp : CTextFile ��ʵ��

#include "stdafx.h"
#include <shlobj.h>
#include "TextFile.h"

// CTextFile
STDMETHODIMP CTextFile::RecText(BSTR bstrContent)
{
	// TODO: Add your implementation code here
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
	{
		return E_POINTER;
	}
	HRESULT hRet = m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);

	/// �봦���յ����ı���Ϣ��������ʾֱ�ӻظ�ԭ����
	hRet = m_spiSocketConnect->AsynSendText(bstrContent,NULL);
	ATLASSERT(SUCCEEDED(hRet));
	/// Ҳ����ֱ�ӷ��Ͷ�����������������Ѷ����������б�����ٴ���
//	BYTE szContent[] = "TestByte";
//	hRet = m_spiSocketConnect->AsynSendByte(szContent,strlen((char*)szContent),NULL);
	return hRet;
}

ATL::CString GetSpecialFolderPath(long lFoldID)
{
	TCHAR szSpecialPath[MAX_PATH];
	::memset(szSpecialPath,0,MAX_PATH*sizeof(TCHAR));
	BOOL bGetFlag = ::SHGetSpecialFolderPath(NULL,szSpecialPath,lFoldID,TRUE);
	if(!bGetFlag)
	{
		return _T("");
	}

	ATL::CString strSpecialPath(szSpecialPath);
	///·�����涼��"\"
	if(!strSpecialPath.IsEmpty() && 0 != strSpecialPath.Right(1).CompareNoCase(_T("\\")))
		strSpecialPath+=_T("\\");
	return strSpecialPath;
}

STDMETHODIMP CTextFile::RecJson(ULONG nReqID,BSTR bstrReqName,BSTR bstrContent)
{
	// TODO: Add your implementation code here
	ATLASSERT(NULL != m_spiSocketConnect);
	if(NULL == bstrContent || NULL == m_spiSocketConnect)
		return E_POINTER;
	m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);
	IJsonServicePtr spiJsonService = NULL;
	HRESULT hRet = spiJsonService.CreateInstance(__uuidof(JsonService));
	if(NULL == spiJsonService)
		return hRet;/// ����JSON����������ʧ��
	VARIANT_BOOL bLoadFlag = VARIANT_FALSE;
	spiJsonService->put_CodingType(CODINGTYPE_US2);
	hRet = spiJsonService->ParseString(bstrContent,&bLoadFlag);
	if(VARIANT_FALSE == bLoadFlag)
	{
		spiJsonService = NULL;
		return hRet;
	}
	/// �봦���յ�����ͨJSON���ݰ�
	/// �������������������Ӧ
	CString strReqName(bstrReqName);
	if(0 == strReqName.CompareNoCase(L"Demo_WriteFile"))
	{
		CString strFilePath = GetSpecialFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"Name"),&bstrVal);;
		strFilePath += bstrVal.m_str;
		bstrVal.Empty();
		spiJsonService->GetStringValue(CComBSTR(L"Content"),&bstrVal);;
		CString strContent = bstrVal.m_str;
		bstrVal.Empty();

		CTextFileWrite WriteFile(strFilePath);
		WriteFile.Write(strContent);
		WriteFile.Close();
		CString strReturn;
		strContent.Replace(L"\"",L"\\\"");
		strFilePath.Replace(L"\\",L"/");
		strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"Content\":\"%s\"}}"), \
			nReqID,strFilePath + L" �ļ�д�����");
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn),NULL);
	}
	else if(0 == strReqName.CompareNoCase(L"Demo_ReadFile"))
	{
		CString strFilePath = GetSpecialFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(L"Name"),&bstrVal);;
		strFilePath += bstrVal.m_str;
		bstrVal.Empty();

		CString strReturn;
		CTextFileRead ReadFile(strFilePath);
		string strLine;
		while(ReadFile.ReadLine(strLine))
		{
			if(!strLine.length())
				continue;/// ���в�����
			strReturn.Empty();
			strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"ReadLine\":\"%s\"}}"), \
				nReqID,CString(strLine.c_str()));
			m_spiSocketConnect->AsynSendText(CComBSTR(strReturn),NULL);
			strLine.clear();
		}
		ReadFile.Close();
		strReturn.Empty();
		strReturn.Format(_T("{\"rid\":%ld,\"data\":{\"End\":\"%s\"}}"), \
			nReqID,L"��ȡ���");
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn), NULL);
	}
	else
	{
		CString strReturn,strContent(bstrContent);
		strContent.Replace(L"\"",L"\\\"");
		strReturn.Format(L"{\"rid\":%ld,\"data\":{\"Req\":\"%s\",\"Content\":\"%s\",\"Status\":\"��ʶ�������\"}}",\
			nReqID,strReqName,strContent);
		m_spiSocketConnect->AsynSendText(CComBSTR(strReturn), NULL);
	}
	return S_OK;
}