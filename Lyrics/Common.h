//CCommon��Ϊȫ�ֺ����Ķ���
#pragma once
#include "CVariant.h"
#include <initializer_list>

enum class Command
{
	OPEN,
	PLAY,
	CLOSE,
	PAUSE,
	STOP,
	PLAY_PAUSE,
	FF,	//���
	REW,		//�쵹
	VOLUME_UP,
	VOLUME_DOWN,
	SEEK
};

enum class CodeType
{
	ANSI,
	UTF8,
	UTF8_NO_BOM,
	UTF16,
	AUTO
};

//����
enum class Language
{
	FOLLOWING_SYSTEM,		//����ϵͳ
	ENGLISH,				//Ӣ��
	SIMPLIFIED_CHINESE		//��������
};

class CCommon
{
public:
	CCommon();
	~CCommon();

	//�ж��ļ��Ƿ����
	static bool FileExist(const wstring& file);

	//�ж��ļ����Ƿ����
	static bool FolderExist(const wstring& file);

	//�ж��Ƿ����ļ���
	static bool IsFolder(const wstring& path);

	//�淶���ַ�������ɾ���ַ���ǰ��ͺ���Ŀո������ַ�(ģ������ֻ����string��wstring)
	template<class T>
	static bool StringNormalize(T& str);

	//ɾ���ַ���ĩβ�Ŀո�
	template<class T>
	static bool DeleteEndSpace(T& str);

	//�Զ�����ַ�����������
	static void StringCopy(char* dest, int size, string source);

	//ת���ַ�����Сд�����upperΪtrue����ת���ɴ�д������ת����Сд
	template<class T>
	static bool StringTransform(T& str, bool upper);

	//�ַ����Ƚϣ����Դ�Сд
	template<class T>
	static bool StringCompareNoCase(const T& str1, const T& str2);

	//�ַ������ң����Դ�Сд
	template<class T>
	static size_t StringFindNoCase(const T& str, const T& find_str);

	//�ַ������ң�ȫ��ƥ��
	template<class T>
	static size_t StringNatchWholeWord(const T& str, const T& find_str);

	//�ж�һ���ַ��Ƿ�����ȫ��ƥ��ʱ�ĵ��ʷָ��ַ���������ĸ�����ֺ�256���ϵ�Unicode�ַ�����ַ���
	static bool IsDivideChar(wchar_t ch);

	//�ж�һ���ַ����Ƿ�������
	static bool StrIsNumber(const wstring& str);

	//��һ���ַ����ָ�����ɸ��ַ���
	//str: ԭʼ�ַ���
	//div_ch: ���ڷָ���ַ�
	//result: ���շָ��Ľ��
	static void StringSplit(const wstring& str, wchar_t div_ch,vector<wstring>& results);

	//�����ɸ��ַ����ϲ���һ���ַ���
	//div_ch: ���ڷָ���ַ�
	static wstring StringMerge(const vector<wstring>& strings, wchar_t div_ch);

	//���ķ���ת��
	static wstring TranslateToSimplifiedChinese(const wstring& str);
	static wstring TranslateToTranditionalChinese(const wstring& str);

	//�滻һ���ļ����е���Ч�ַ�
	static void FileNameNormalize(wstring& file_name);

	//�����ļ���С
	static size_t GetFileSize(const wstring& file_name);

	//��ini�ļ�д��һ��int����
	static void WritePrivateProfileIntW(const wchar_t* AppName, const wchar_t* KeyName, int value, const wchar_t* Path);

	//��string���͵��ַ���ת����Unicode�����wstring�ַ���
	static wstring StrToUnicode(const string& str, CodeType code_type = CodeType::AUTO);

	//��Unicode�����wstring�ַ���ת����string���͵��ַ�����������ַ��޷�ת����������char_cannot_convertָ���bool������Ϊtrue
	static string UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert = nullptr);

	//��һ��ֻ��ASCII����ɵ��ַ���ת����Unicode
	static wstring ASCIIToUnicode(const string& ascii);

	//�ж�һ���ַ����Ƿ�UTF8����
	static bool IsUTF8Bytes(const char* data);

	//�ж�һ���ַ����ı����ʽ
	static CodeType JudgeCodeType(const string& str, CodeType default_code = CodeType::ANSI);

	//��ȡ��ǰ����exe�ļ���·��
	static wstring GetExePath();

	//��ȡ�����·��
	static wstring GetDesktopPath();

	//��ȡ��ʱ�ļ���·��
	static wstring GetTemplatePath();

	//��ȡϵͳ�����ļ��е�λ��
	//csidl: ����ͬSHGetSpecialFolderLocation�����Ĳ���
	static wstring GetSpecialDir(int csidl);

	////��ȡһ���б�ؼ���󳤶���Ŀ��ȵ�����ֵ
	//static int GetListWidth(CListBox& list);

	//ɾ��һ���ļ�
	static int DeleteAFile(HWND hwnd, _tstring file);
	//ɾ������ļ�
	static int DeleteFiles(HWND hwnd, const vector<_tstring>& files);

	//����һ���ļ�
	static int CopyAFile(HWND hwnd, _tstring file_from, _tstring file_to);
	//���ƶ���ļ�
	static int CopyFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to);

	//�ƶ�һ���ļ�
	static int MoveAFile(HWND hwnd, _tstring file_from, _tstring file_to);

	//�ƶ�����ļ�
	static int MoveFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to);

	//��һ���ַ������浽������
	static bool CopyStringToClipboard(const wstring& str);

	//�Ӽ������ȡ�ַ���
	static wstring GetStringFromClipboard();

	//д����־
	static void WriteLog(const wchar_t* path, const wstring& content);

	//��ͨ�������в������ݹ����Ķ���ļ�·����֣������浽file���������������ݹ����ĵ�һ���ļ������ļ������ļ��У��򷵻��ļ���·�������򣬷��ؿ��ַ���
	static wstring DisposeCmdLine(const wstring& cmd_line, vector<wstring>& files);

	/*
	�������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ
	����������
	lpszLnkFileDir  ָ��Ŀ¼������ΪNULL��
	lpszFileName    ָ���ļ���ΪNULL��ʾ��ǰ���̵�EXE�ļ���
	lpszLnkFileName ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
	lpszWorkDir		��ݷ�ʽ����Ŀ¼��ΪNULL��ʾ��ݷ�ʽĿ������λ��
	wHotkey         Ϊ0��ʾ�����ÿ�ݼ�
	pszDescription  ��ע
	iShowCmd        ���з�ʽ��Ĭ��Ϊ���洰��
	*/
	static BOOL CreateFileShortcut(LPCTSTR lpszLnkFileDir, LPCTSTR lpszFileName = NULL, LPCTSTR lpszLnkFileName = NULL, LPCTSTR lpszWorkDir = NULL, WORD wHotkey = 0, LPCTSTR lpszDescription = NULL, int iShowCmd = SW_SHOWNORMAL);

	//����ָ���ļ��������ļ���������files����
	//file_name������ D:\\Music\\*abc*.mp3���򽫲���D:\MusicĿ¼�����а���abc��mp3�ļ�
	static void GetFiles(wstring file_name, vector<wstring>& files);

	//����ָ����ͼƬ�ļ�����������files�����У���������ͬGetFiles����
	static void GetImageFiles(wstring file_name, vector<wstring>& files);

	//�����ļ���չ���ж�һ���ļ��Ƿ�ΪͼƬ�ļ�
	static bool FileIsImage(const wstring& file_name);

	//��ȡһ��������ַ���
	static wstring GetRandomString(int length);

	//�ж�һ��Ԫ���Ƿ���vector��
	template<class T>
	static bool IsItemInVector(const vector<T>& items, const T& item);

	//�ж��ļ�����ĩβ�Ƿ���ϡ�(����)������ʽ
	//file_name: Ҫ�жϵ��ļ�������������չ��
	//number: ���������е�����
	//index: �������������ַ����е�����
	static bool IsFileNameNumbered(const wstring& file_name, int& number, size_t& index);

	//ɾ��һ����ģ̬�Ի���
	template<class T>
	static void DeleteModelessDialog(T* dlg);

	//��һ��CSize�����ڱ��ֳ���ȵ���������ţ�ʹ�䳤�ߵ�side
	static void SizeZoom(CSize& size, int side);

	static COLORREF GetWindowsThemeColor();

	//��hSrc�е����в˵�����ӵ��˵�hDst�У����� https://blog.csdn.net/zgl7903/article/details/71077441��
	static int AppendMenuOp(HMENU hDst, HMENU hSrc);

	//����Դ�ļ������ַ��������У�front_str��back_strΪ�����ַ���ʱ��Ҫ��ǰ��������ӵ��ַ���
	static CString LoadText(UINT id, LPCTSTR back_str = nullptr);
	static CString LoadText(LPCTSTR front_str, UINT id, LPCTSTR back_str = nullptr);

	//��ȫ�ĸ�ʽ���ַ�������format_str������<%���%>���ַ����滻�ɳ�ʼ���б�paras�е�Ԫ�أ�Ԫ��֧��int/double/LPCTSTR/CString��ʽ����Ŵ�1��ʼ
	static CString StringFormat(LPCTSTR format_str, const std::initializer_list<CVariant>& paras);

	//����Դ�ļ��������ַ�����������Դ�ַ���������<%���%>���ַ����滻�ɿɱ�����б��еĲ���
	static CString LoadTextFormat(UINT id, const std::initializer_list<CVariant>& paras);

	//�����߳�����
	static void SetThreadLanguage(Language language);

	//��ȫ���ַ������ƺ���
	static void WStringCopy(wchar_t* str_dest, int dest_size, const wchar_t* str_source, int source_size = INT_MAX);

};

template<class T>
inline bool CCommon::StringNormalize(T & str)
{
	if (str.empty()) return false;

	int size = str.size();	//�ַ����ĳ���
	if (size < 0) return false;
	int index1 = 0 ;		//�ַ����е�1�����ǿո������ַ���λ��
	int index2 = size - 1;	//�ַ��������һ�����ǿո������ַ���λ��
	while (index1 < size && str[index1] >= 0 && str[index1] <=32)
		index1++;
	while (index2 >= 0 && str[index2] >= 0 && str[index2] <= 32)
		index2--;
	if (index1 > index2)	//���index1 > index2��˵���ַ���ȫ�ǿո������ַ�
		str.clear();
	else if (index1 == 0 && index2 == size - 1)	//���index1��index2��ֵ�ֱ�Ϊ0��size - 1��˵���ַ���ǰ��û�пո������ַ���ֱ�ӷ���
		return true;
	else
		str = str.substr(index1, index2 - index1 + 1);
	return true;
}

template<class T>
inline bool CCommon::DeleteEndSpace(T & str)
{
	if (str.empty()) return false;
	while (!str.empty() && str.back() == L' ')
		str.pop_back();
	return true;
}

template<class T>
inline bool CCommon::StringTransform(T & str, bool upper)
{
	if (str.empty()) return false;
	//if (upper)
	//	std::transform(str.begin(), str.end(), str.begin(), toupper);
	//else
	//	std::transform(str.begin(), str.end(), str.begin(), tolower);
	for (auto& ch : str)
	{
		if (upper)
		{
			if (ch >= 'a'&&ch <= 'z')
				ch -= 32;
		}
		else
		{
			if (ch >= 'A'&&ch <= 'Z')
				ch += 32;
		}
	}
	return true;
}

template<class T>
inline bool CCommon::StringCompareNoCase(const T & str1, const T & str2)
{
	T _str1{ str1 }, _str2{ str2 };
	StringTransform(_str1, false);
	StringTransform(_str2, false);
	return (_str1 == _str2);
}

template<class T>
inline size_t CCommon::StringFindNoCase(const T & str, const T & find_str)
{
	if (str.empty() || find_str.empty())
		return -1;
	T _str{ str }, _find_str{ find_str };
	StringTransform(_str, false);
	StringTransform(_find_str, false);
	return _str.find(_find_str);
}

template<class T>
inline size_t CCommon::StringNatchWholeWord(const T & str, const T & find_str)
{
	if (str.empty() || find_str.empty())
		return -1;
	//����3���������̫����ʱ���CPU�����ȥ��
	//T _str{ str }, _find_str{ find_str };
	//StringTransform(str, false);
	//StringTransform(find_str, false);
	int index{ -1 };
	int find_str_front_pos, find_str_back_pos;		//�ҵ����ַ�����ԭ�ַ�����ǰ��ͺ���һ���ַ���λ��
	int size = str.size();
	int find_str_size = find_str.size();
	while (true)
	{
		index = str.find(find_str, index + 1);
		if (index == T::npos) break;
		find_str_front_pos = index - 1;
		find_str_back_pos = index + find_str_size;
		if ((find_str_front_pos < 0 || IsDivideChar(str[find_str_front_pos])) && (find_str_back_pos >= size || IsDivideChar(str[find_str_back_pos])))
			return index;
		else
			continue;
	}
	return -1;
}

template<class T>
inline bool CCommon::IsItemInVector(const vector<T>& items, const T & item)
{
	for (const auto& an_item : items)
	{
		if (an_item == item)
			return true;
	}
	return false;
}

template<class T>
inline void CCommon::DeleteModelessDialog(T * dlg)
{
	if (dlg != nullptr)
	{
		dlg->OnCancel();
		delete dlg;
		dlg = nullptr;
	}
}
