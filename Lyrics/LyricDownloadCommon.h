//�������ڶ���ͨ���������ظ����ص�ȫ�ֺ���
//��ʵ����ص��������������ֵ�API
#pragma once
#include "InternetCommon.h"

class CLyricDownloadCommon
{

public:
	CLyricDownloadCommon();
	~CLyricDownloadCommon();


	//���������������и�����id���ظ�ʣ����������result�У�download_translate����ָ���Ƿ����ش�����ĸ��
	static bool DownloadLyric(const wstring& song_id, wstring& result, bool download_translate = true);

	static bool DisposeLryic(wstring& lyric_str);	//�Դ��������������صĸ�ʽ��д���ת������ȷ�ĸ���ı�
	static void AddLyricTag(wstring& lyric_str, const wstring& song_id, const wstring& title = _T(""), const wstring& artist = _T(""), const wstring& album = _T(""));		//�ڸ��ǰ����ϱ�ǩ��Ϣ
};

