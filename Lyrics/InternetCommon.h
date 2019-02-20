#pragma once
#include "Common.h"

class CInternetCommon
{
#define  NORMAL_CONNECT INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID
#define SUCCESS 0	// �����ɹ�
#define FAILURE 1	// ����ʧ��
#define OUTTIME 2	// ������ʱ

public:
	//һ�������������Ϣ
	struct ItemInfo
	{
		wstring id;		//������ID
		wstring title;		//�����ı���
		wstring artist;		//������������
		wstring album;		//�����ĳ�Ƭ��
	};

	CInternetCommon();
	~CInternetCommon();

	//��һ���ַ���ת����URL���루��UTF8�����ʽ��
	static wstring URLEncode(const wstring& wstr);

	static bool GetURL(const wstring& str_url, wstring& result);

	//��ָ����url����http post���󣬽��������result��
	static int HttpPost(const wstring& str_url, wstring& result);

	static void DeleteStrSlash(wstring& str);		//����ַ����еġ�\"����ɾ���ַ����еķ�б��
	static void DisposeSearchResult(vector<ItemInfo>& down_list, const wstring& search_result, int result_count = 30);		//���������search_result����ȡ����������Ϣ����������down_list������

	/// <summary>
	/// �ַ������ƶ��㷨-�༭���뷨
	/// </summary>
	/// <returns>���ص�ֵΪ0~1��Խ�����ƶ�Խ��</returns>
	static double StringSimilarDegree_LD(const wstring& srcString, const wstring& matchString);

	//���ݲ����ṩ�ĸ������⡢�����ҡ���Ƭ�����ļ�������down_list�����в�����ƥ���һ�������������ֵ
	static int SelectMatchedItem(const vector<ItemInfo>& down_list, const wstring& title, const wstring& artist, const wstring& album, const wstring& filename, bool write_log = false);

	//�Զ������������������ƥ�����ID�����messageΪtrue�������ʧ��ʱ������ʾ
	static ItemInfo SearchSongAndGetMatched(const wstring& title, const wstring& artist, const wstring& album, const wstring& file_name, bool message = true);
};

