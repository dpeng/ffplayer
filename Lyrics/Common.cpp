#include "stdafx.h"
#include "Common.h"
#include <io.h>

#pragma warning (disable : 4267)
#pragma warning (disable : 4244)

CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}


//void CCommon::GetAllFormatFiles(wstring path, vector<wstring>& files, const vector<wstring>& format, size_t max_file)
//{
//	//�ļ���� 
//	int hFile = 0;
//	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
//	_wfinddata_t fileinfo;
//	wstring file_path;
//	for (auto a_format : format)
//	{
//		if ((hFile = _wfindfirst(file_path.assign(path).append(L"\\*.").append(a_format).c_str(), &fileinfo)) != -1)
//		{
//			do
//			{
//				if (files.size() >= max_file) break;
//				files.push_back(file_path.assign(fileinfo.name));  //���ļ�������
//			} while (_wfindnext(hFile, &fileinfo) == 0);
//		}
//		_findclose(hFile);
//	}
//	std::sort(files.begin(), files.end());		//����������ļ�����������
//}

bool CCommon::FileExist(const wstring & file)
{
	_wfinddata_t fileinfo;
	return (_wfindfirst(file.c_str(), &fileinfo) != -1);
}

bool CCommon::FolderExist(const wstring & file)
{
	DWORD dwAttrib = GetFileAttributes(file.c_str());
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool CCommon::IsFolder(const wstring& path)
{
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

//bool CCommon::FileIsMidi(const wstring & file_name)
//{
//	wstring type;
//	type = file_name.substr(file_name.size() - 4, 4);			//��ȡ��չ��
//	std::transform(type.begin(), type.end(), type.begin(), tolower);		//����չ��ת����Сд
//	return (type == L".mid");
//}

void CCommon::StringCopy(char * dest, int size, string source)
{
	int source_size = source.size();
	for (int i{}; i < size && i<source_size; i++)
	{
		dest[i] = source[i];
	}
}

//bool CCommon::StringCompareNoCase(const wstring & str1, const wstring & str2)
//{
//	wstring _str1{ str1 }, _str2{ str2 };
//	StringTransform(_str1, false);
//	StringTransform(_str2, false);
//	return (_str1 == _str2);
//}

//size_t CCommon::StringFindNoCase(const wstring & str, const wstring & find_str)
//{
//	wstring _str{ str }, _find_str{ find_str };
//	StringTransform(_str, false);
//	StringTransform(_find_str, false);
//	return _str.find(_find_str);
//}

bool CCommon::IsDivideChar(wchar_t ch)
{
	if ((ch >= L'0' && ch <= L'9') || (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch > 255)
		return false;
	else
		return true;
}

bool CCommon::StrIsNumber(const wstring & str)
{
	for (const auto& ch : str)
	{
		if (ch<L'0' || ch > L'9')
			return false;
	}
	return true;
}

void CCommon::StringSplit(const wstring & str, wchar_t div_ch, vector<wstring>& results)
{
	results.clear();
	size_t split_index = -1 ;
	size_t last_split_index = -1;
	while (true)
	{
		split_index = str.find(div_ch, split_index + 1);
		wstring split_str = str.substr(last_split_index + 1, split_index - last_split_index - 1);
		StringNormalize(split_str);
		if(!split_str.empty())
			results.push_back(split_str);
		if (split_index == wstring::npos)
			break;
		last_split_index = split_index;
	}
}

wstring CCommon::StringMerge(const vector<wstring>& strings, wchar_t div_ch)
{
	wstring result;
	for (const auto& str : strings)
	{
		result.append(str).push_back(div_ch);
	}
	result.pop_back();
	return result;
}

wstring CCommon::TranslateToSimplifiedChinese(const wstring & str)
{
	wstring result;
	size_t size{ str.size() };
	if (size == 0) return wstring();
	wchar_t* out_buff = new wchar_t[size + 1];
	WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
	int rtn = LCMapString(Locale, LCMAP_SIMPLIFIED_CHINESE, str.c_str(), -1, out_buff, size * sizeof(wchar_t));
	result.assign(out_buff);
	delete[] out_buff;
	return result;
}

wstring CCommon::TranslateToTranditionalChinese(const wstring & str)
{
	wstring result;
	size_t size{ str.size() };
	if (size == 0) return wstring();
	wchar_t* out_buff = new wchar_t[size + 1];
	WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
	int rtn = LCMapString(Locale, LCMAP_TRADITIONAL_CHINESE, str.c_str(), -1, out_buff, size * sizeof(wchar_t));
	result.assign(out_buff);
	delete[] out_buff;
	return result;
}

void CCommon::FileNameNormalize(wstring & file_name)
{
	//�����ַ����е���Ч�ַ����������滻�ɡ�_��
	const wstring invalid_chars{ L"\\\"/:*?<>|\a\b\f\n\r\t\v" };
	int index{ -1 };
	while (true)
	{
		index = file_name.find_first_of(invalid_chars, index + 1);
		if (index == wstring::npos)
			return;
		else
			file_name[index] = L'_';
	}
}

size_t CCommon::GetFileSize(const wstring & file_name)
{
	int l, m;
	ifstream file(file_name, std::ios::in | std::ios::binary);
	l = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::end);
	m = static_cast<int>(file.tellg());
	file.close();
	return m - l;
}

void CCommon::WritePrivateProfileIntW(const wchar_t * AppName, const wchar_t * KeyName, int value, const wchar_t * Path)
{
	wchar_t buff[16];
	_itow_s(value, buff, 10);
	WritePrivateProfileStringW(AppName, KeyName, buff, Path);
}

wstring CCommon::StrToUnicode(const string & str, CodeType code_type)
{
	if (str.empty()) return wstring();

	if (code_type == CodeType::AUTO)
	{
		//code_typeΪAUTOʱ�Զ��жϱ�������
		//���ǰ����UTF8��BOM�����������ΪUTF8
		if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
			code_type = CodeType::UTF8;
		//���ǰ����UTF16��BOM�����������ΪUTF16
		else if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
			code_type = CodeType::UTF16;
		//else if (IsUTF8Bytes(str.c_str()))		//���û���ҵ�UTF8��UTF16��BOM�����ж��ַ����Ƿ���UTF8���������
		//	code_type = CodeType::UTF8_NO_BOM;
		else
			code_type = CodeType::ANSI;
	}

	wstring result;
	int size;
	if (code_type == CodeType::ANSI)
	{
		size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		if (size <= 0) return wstring();
		wchar_t* str_unicode = new wchar_t[size + 1];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str_unicode, size);
		result.assign(str_unicode);
		delete[] str_unicode;
	}
	else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
	{
		string temp;
		//���ǰ����BOM����ȥ��BOM
		if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
			temp = str.substr(3);
		else
			temp = str;
		size = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, NULL, 0);
		if (size <= 0) return wstring();
		wchar_t* str_unicode = new wchar_t[size + 1];
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, str_unicode, size);
		result.assign(str_unicode);
		delete[] str_unicode;
	}
	else if (code_type == CodeType::UTF16)
	{
		string temp;
		//���ǰ����BOM����ȥ��BOM
		if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
			temp = str.substr(2);
		else
			temp = str;
		if (temp.size() % 2 == 1)
			temp.pop_back();
		temp.push_back('\0');
		result = (const wchar_t*)temp.c_str();
	}
	return result;
}

string CCommon::UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert)
{
	if (wstr.empty()) return string();
	if (char_cannot_convert != nullptr)
		*char_cannot_convert = false;
	BOOL UsedDefaultChar{ FALSE };
	string result;
	int size{ 0 };
	if (code_type == CodeType::ANSI)
	{
		size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (size <= 0) return string();
		char* str = new char[size + 1];
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str, size, NULL, &UsedDefaultChar);
		result.assign(str);
		delete[] str;
	}
	else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
	{
		result.clear();
		if (code_type == CodeType::UTF8)
		{
			result.push_back(-17);
			result.push_back(-69);
			result.push_back(-65);
		}
		size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (size <= 0) return string();
		char* str = new char[size + 1];
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str, size, NULL, NULL);
		result.append(str);
		delete[] str;
	}
	else if (code_type == CodeType::UTF16)
	{
		result.clear();
		result.push_back(-1);	//��ǰ�����UTF16��BOM
		result.push_back(-2);
		result.append((const char*)wstr.c_str(), (const char*)wstr.c_str() + wstr.size() * 2);
		result.push_back('\0');
	}
	if (char_cannot_convert != nullptr)
		*char_cannot_convert = (UsedDefaultChar != FALSE);
	return result;
}

wstring CCommon::ASCIIToUnicode(const string & ascii)
{
	wstring result;
	for (const char& ch: ascii)
	{
		result.push_back(ch);
	}
	return result;
}

bool CCommon::IsUTF8Bytes(const char * data)
{
	int charByteCounter = 1;  //���㵱ǰ���������ַ�Ӧ���е��ֽ���
	unsigned char curByte; //��ǰ�������ֽ�.
	bool ascii = true;
	int length = strlen(data);
	for (int i = 0; i < length; i++)
	{
		curByte = static_cast<unsigned char>(data[i]);
		if (charByteCounter == 1)
		{
			if (curByte >= 0x80)
			{
				ascii = false;
				//�жϵ�ǰ
				while (((curByte <<= 1) & 0x80) != 0)
				{
					charByteCounter++;
				}
				//���λ��λ��Ϊ��0 ��������2��1��ʼ ��:110XXXXX...........1111110X 
				if (charByteCounter == 1 || charByteCounter > 6)
				{
					return false;
				}
			}
		}
		else
		{
			//����UTF-8 ��ʱ��һλ����Ϊ1
			if ((curByte & 0xC0) != 0x80)
			{
				return false;
			}
			charByteCounter--;
		}
	}
	if (ascii) return false;		//���ȫ��ASCII�ַ�������false
	else return true;
}

CodeType CCommon::JudgeCodeType(const string & str, CodeType default_code)
{
	//���ǰ����UTF8��BOM�����������ΪUTF8
	if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
		return CodeType::UTF8;
	//���ǰ����UTF16��BOM�����������ΪUTF16
	else if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
		return CodeType::UTF16;
	//else if (IsUTF8Bytes(str.c_str()))		//���û���ҵ�UTF8��UTF16��BOM�����ж��ַ����Ƿ���UTF8���������
	//	return CodeType::UTF8_NO_BOM;
	else
		return default_code;
}

wstring CCommon::GetExePath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);
	size_t index;
	wstring current_path{ path };
	index = current_path.find_last_of(L'\\');
	current_path = current_path.substr(0, index + 1);
	return current_path;
}

wstring CCommon::GetDesktopPath()
{
	LPITEMIDLIST ppidl;
	TCHAR pszDesktopPath[MAX_PATH];
	if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
	{
		SHGetPathFromIDList(ppidl, pszDesktopPath);
		CoTaskMemFree(ppidl);
	}
	return wstring(pszDesktopPath);
}

wstring CCommon::GetTemplatePath()
{
	wstring result;
	wchar_t buff[MAX_PATH];
	GetTempPath(MAX_PATH, buff);		//��ȡ��ʱ�ļ��е�·��
	result = buff;
	if (result.back() != L'\\' && result.back() != L'/')		//ȷ��·��������б��
		result.push_back(L'\\');
	return result;
}

wstring CCommon::GetSpecialDir(int csidl)
{
	LPITEMIDLIST ppidl;
	TCHAR folder_dir[MAX_PATH];
	if (SHGetSpecialFolderLocation(NULL, csidl, &ppidl) == S_OK)
	{
		SHGetPathFromIDList(ppidl, folder_dir);
		CoTaskMemFree(ppidl);
	}
	return wstring(folder_dir);
}

//int CCommon::GetListWidth(CListBox & list)
//{
//	CDC *pDC = list.GetDC();
//	if (NULL == pDC)
//	{
//		return 0;
//	}
//	int nCount = list.GetCount();
//	if (nCount < 1)
//		return 0;
//	int nMaxExtent = 0;
//	CString szText;
//	for (int i = 0; i < nCount; ++i)
//	{
//		list.GetText(i, szText);
//		CSize &cs = pDC->GetTextExtent(szText);
//		if (cs.cx > nMaxExtent)
//		{
//			nMaxExtent = cs.cx;
//		}
//	}
//	return nMaxExtent;
//}


int CCommon::DeleteAFile(HWND hwnd, _tstring file)
{
	file.push_back(_T('\0'));	//pFrom����������\0��β
	//LPCTSTR strTitle = CCommon::LoadText(IDS_DELETE);	//�ļ�ɾ�����ȶԻ������
	LPCTSTR strTitle = file.c_str();
	SHFILEOPSTRUCT FileOp{};	//����SHFILEOPSTRUCT�ṹ����
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_DELETE;	//ִ���ļ�ɾ������;
	FileOp.pFrom = file.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;	//�˱�־ʹɾ���ļ����ݵ�Windows����վ
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = strTitle;
	return SHFileOperation(&FileOp);	//ɾ���ļ�
}

int CCommon::DeleteFiles(HWND hwnd, const vector<_tstring>& files)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return DeleteAFile(hwnd, file_list);
}

int CCommon::CopyAFile(HWND hwnd, _tstring file_from, _tstring file_to)
{
	file_from.push_back(_T('\0'));	//pFrom����������\0��β
	file_to.push_back(_T('\0'));	//pTo����������\0��β
	SHFILEOPSTRUCT FileOp{};
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_COPY;
	FileOp.pFrom = file_from.c_str();
	FileOp.pTo = file_to.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;
	FileOp.hNameMappings = NULL;
	//FileOp.lpszProgressTitle = LoadText(IDS_COPY);
	FileOp.lpszProgressTitle = file_from.c_str();
	return SHFileOperation(&FileOp);
}

int CCommon::CopyFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return CopyAFile(hwnd, file_list, file_to);
}

int CCommon::MoveAFile(HWND hwnd, _tstring file_from, _tstring file_to)
{
	file_from.push_back(_T('\0'));	//pFrom����������\0��β
	file_to.push_back(_T('\0'));	//pTo����������\0��β
	SHFILEOPSTRUCT FileOp{};
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_MOVE;
	FileOp.pFrom = file_from.c_str();
	FileOp.pTo = file_to.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;
	FileOp.hNameMappings = NULL;
	//FileOp.lpszProgressTitle = LoadText(IDS_MOVE);
	FileOp.lpszProgressTitle = file_from.c_str();
	return SHFileOperation(&FileOp);
}

int CCommon::MoveFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return MoveAFile(hwnd, file_list, file_to);
}

bool CCommon::CopyStringToClipboard(const wstring & str)
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		EmptyClipboard();
		size_t size = (str.size() + 1) * 2;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, size);
		memcpy_s(GlobalLock(clipbuffer), size, str.c_str(), size);
		GlobalUnlock(clipbuffer);
		if(SetClipboardData(CF_UNICODETEXT, clipbuffer)==NULL)
			return false;
		CloseClipboard();
		return true;
	}
	else return false;
}

wstring CCommon::GetStringFromClipboard()
{
	if (OpenClipboard(NULL))
	{
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HANDLE hClip;
			wchar_t *pBuf;
			hClip = GetClipboardData(CF_UNICODETEXT); //��ȡ����������
			pBuf = (wchar_t*)GlobalLock(hClip);
			CloseClipboard();
			return wstring(pBuf);
		}
	}
	return wstring();
}

void CCommon::WriteLog(const wchar_t* path, const wstring & content)
{
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	char buff[32];
	sprintf_s(buff, "%d/%.2d/%.2d %.2d:%.2d:%.2d.%.3d ", cur_time.wYear, cur_time.wMonth, cur_time.wDay,
		cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
	ofstream out_put{ path, std::ios::app };
	out_put << buff << CCommon::UnicodeToStr(content, CodeType::UTF8_NO_BOM) << std::endl;
}

wstring CCommon::DisposeCmdLine(const wstring & cmd_line, vector<wstring>& files)
{
	files.clear();
	if (cmd_line.empty()) return wstring();
	wstring path;
	//���ҳ��ַ����е��ļ���·�����������в������ݹ������ļ��϶�����ͬһ���ļ����µ�
	if (cmd_line[0] == L'\"')		//�����һ���ļ���˫���Ű�������
	{
		int index1 = cmd_line.find(L'\"', 1);		//���Һ͵�1��˫����ƥ���˫����
		int index2 = cmd_line.rfind(L'\\', index1);		//��ǰ���ҷ�б��
		path = cmd_line.substr(1, index2);		//��ȡ�ļ���·�����������һ����б�ܣ�
		files.push_back(cmd_line.substr(1, index1 - 1));
	}
	else		//�����һ���ļ�û����˫���Ű�����������˵��·���в������ո�
	{
		int index1 = cmd_line.find(L' ');		//���Һ͵�1�ո�
		int index2 = cmd_line.rfind(L'\\', index1);		//��ǰ���ҷ�б��
		path = cmd_line.substr(0, index2 + 1);		//��ȡ�ļ���·�����������һ����б�ܣ�
		files.push_back(cmd_line.substr(0, index1));
	}
	int path_size = path.size();
	if (path_size < 2) return wstring();
	if (files[0].size() > 4 && files[0][files[0].size() - 4] != L'.' && files[0][files[0].size() - 5] != L'.')
	{
		//�����1���ļ������ļ������ļ��У���ֱ�ӻظ��ļ��е�·��
		return files[0];
	}
	int index{};
	while (true)
	{
		index = cmd_line.find(path, index + path_size);		//�ӵ�2����ʼ����·�����ֵ�λ��
		if (index == string::npos) break;
		if (index > 0 && cmd_line[index - 1] == L'\"')		//���·��ǰ��һ���ַ���˫����
		{
			int index1 = cmd_line.find(L'\"', index);
			files.push_back(cmd_line.substr(index, index1 - index));
		}
		else
		{
			int index1 = cmd_line.find(L' ', index);
			files.push_back(cmd_line.substr(index, index1 - index));
		}
	}
	return wstring();
	//CString out_info;
	//out_info += _T("�����в�����");
	//out_info += cmd_line.c_str();
	//out_info += _T("\r\n");
	//out_info += _T("·����");
	//out_info += path.c_str();
	//out_info += _T("\r\n");
	//CCommon::WriteLog(L".\\command.log", wstring{ out_info });
}

BOOL CCommon::CreateFileShortcut(LPCTSTR lpszLnkFileDir, LPCTSTR lpszFileName, LPCTSTR lpszLnkFileName, LPCTSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd)
{
	if (lpszLnkFileDir == NULL)
		return FALSE;

	HRESULT hr;
	IShellLink     *pLink;  //IShellLink����ָ��
	IPersistFile   *ppf; //IPersisFil����ָ��

	//����IShellLink����
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(hr))
		return FALSE;

	//��IShellLink�����л�ȡIPersistFile�ӿ�
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr))
	{
		pLink->Release();
		return FALSE;
	}

	TCHAR file_path[MAX_PATH];
	GetModuleFileName(NULL, file_path, MAX_PATH);

	//Ŀ��
	if (lpszFileName == NULL)
		pLink->SetPath(file_path);
	else
		pLink->SetPath(lpszFileName);

	//����Ŀ¼
	if (lpszWorkDir != NULL)
	{
		pLink->SetWorkingDirectory(lpszWorkDir);
	}
	else
	{
		//���ù���Ŀ¼Ϊ��ݷ�ʽĿ������λ��
		TCHAR workDirBuf[MAX_PATH];
		if (lpszFileName == NULL)
			wcscpy_s(workDirBuf, file_path);
		else
			wcscpy_s(workDirBuf, lpszFileName);
		LPTSTR pstr = wcsrchr(workDirBuf, _T('\\'));
		*pstr = _T('\0');
		pLink->SetWorkingDirectory(workDirBuf);
	}

	//��ݼ�
	if (wHotkey != 0)
		pLink->SetHotkey(wHotkey);

	//��ע
	if (lpszDescription != NULL)
		pLink->SetDescription(lpszDescription);

	//��ʾ��ʽ
	pLink->SetShowCmd(iShowCmd);


	//��ݷ�ʽ��·�� + ����
	wchar_t szBuffer[MAX_PATH];
	if (lpszLnkFileName != NULL) //ָ���˿�ݷ�ʽ������
		swprintf_s(szBuffer, L"%s\\%s", lpszLnkFileDir, lpszLnkFileName);
	else
	{
		//û��ָ�����ƣ��ʹ�ȡָ���ļ����ļ�����Ϊ��ݷ�ʽ���ơ�
		const wchar_t *pstr;
		if (lpszFileName != NULL)
			pstr = wcsrchr(lpszFileName, L'\\');
		else
			pstr = wcsrchr(file_path, L'\\');

		if (pstr == NULL)
		{
			ppf->Release();
			pLink->Release();
			return FALSE;
		}
		//ע���׺��Ҫ��.exe��Ϊ.lnk
		swprintf_s(szBuffer, L"%s\\%s", lpszLnkFileDir, pstr);
		int nLen = wcslen(szBuffer);
		szBuffer[nLen - 3] = L'l';
		szBuffer[nLen - 2] = L'n';
		szBuffer[nLen - 1] = L'k';
	}
	//�����ݷ�ʽ��ָ��Ŀ¼��
	//WCHAR  wsz[MAX_PATH];  //����Unicode�ַ���
	//MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, wsz, MAX_PATH);

	hr = ppf->Save(szBuffer, TRUE);

	ppf->Release();
	pLink->Release();
	return SUCCEEDED(hr);
}

void CCommon::GetFiles(wstring file_name, vector<wstring>& files)
{
	files.clear();
	//�ļ����
	int hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	if ((hFile = _wfindfirst(file_name.c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(fileinfo.name);
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CCommon::GetImageFiles(wstring file_name, vector<wstring>& files)
{
	files.clear();
	//�ļ����
	int hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	if ((hFile = _wfindfirst(file_name.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (FileIsImage(fileinfo.name))
				files.push_back(fileinfo.name);
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

bool CCommon::FileIsImage(const wstring & file_name)
{
	size_t index;
	index = file_name.find_last_of(L'.');
	wstring type;
	if (index != string::npos)
		type = file_name.substr(index);			//��ȡ��չ��
	std::transform(type.begin(), type.end(), type.begin(), tolower);		//����չ��ת����Сд
	return (type == L".jpg" || type == L".jpeg" || type == L".png" || type == L".gif" || type == L".bmp");
}

wstring CCommon::GetRandomString(int length)
{
	wstring result;
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//��ȡ��ǰʱ��
	srand(current_time.wMilliseconds);		//�õ�ǰʱ��ĺ��������ò��������������
	int char_type;		//��ǰҪ���ɵ��ַ����� 0�����֣�1��Сд��ĸ��2����д��ĸ
	for (int i{}; i < length; i++)
	{
		char_type = rand() % 3;		//���ȷ��Ҫ���ɵ��ַ�����
		wchar_t current_char;
		switch (char_type)
		{
		case 0:
			current_char = L'0' + (rand() % 10);
			break;
		case 1:
			current_char = L'a' + (rand() % 26);
			break;
		case 2:
			current_char = L'A' + (rand() % 26);
			break;
		}
		result.push_back(current_char);
	}
	return result;
}

bool CCommon::IsFileNameNumbered(const wstring & file_name, int& number, size_t& index)
{
	if (file_name.empty())
		return false;
	if (file_name.back() != L')')
		return false;
	size_t size{ file_name.size() };
	//size_t index;
	index = file_name.rfind(L'(');		//��ǰ����������
	if (index == wstring::npos || index == 0)		//�����Ų������ַ�����ͷ
		return false;
	wstring number_str{ file_name.substr(index + 1, size - index - 2) };		//��ȡ��������֮����ı�
	if (StrIsNumber(number_str))				//�ж��ı��Ƿ�������
	{
		number = _wtoi(number_str.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

void CCommon::SizeZoom(CSize & size, int side)
{
	float width_height_ratio{ static_cast<float>(size.cx) / size.cy };	//�����
	if (width_height_ratio > 1)		//����ȴ���1��1
	{
		size.cx = side;
		size.cy = static_cast<int>(side / width_height_ratio);
	}
	else if (width_height_ratio < 1)
	{
		size.cy = side;
		size.cx = static_cast<int>(side * width_height_ratio);
	}
	else
	{
		size.cx = size.cy = side;
	}
}

COLORREF CCommon::GetWindowsThemeColor()
{
	return RGB(0, 120, 215);
}


int CCommon::AppendMenuOp(HMENU hDst, HMENU hSrc)
{
	int iCnt = 0;
	ASSERT(hDst && hSrc);

	for (int iSrc = 0, iDst = GetMenuItemCount(hDst); iSrc < GetMenuItemCount(hSrc); iSrc++)
	{
		TCHAR szMenuStr[256] = { 0 };
		MENUITEMINFO mInfo = { 0 };
		mInfo.cbSize = sizeof(mInfo);
		mInfo.fMask = 0
			| MIIM_CHECKMARKS //Retrieves or sets the hbmpChecked and hbmpUnchecked members. 
			| MIIM_DATA //Retrieves or sets the dwItemData member. 
			| MIIM_ID //Retrieves or sets the wID member. 
			| MIIM_STATE //Retrieves or sets the fState member. 
			| MIIM_SUBMENU //Retrieves or sets the hSubMenu member. 
			| MIIM_TYPE //Retrieves or sets the fType and dwTypeData members. 
			| 0;
		mInfo.dwTypeData = szMenuStr;
		mInfo.cch = _countof(szMenuStr);

		VERIFY(GetMenuItemInfo(hSrc, iSrc, TRUE, &mInfo));

		if (mInfo.hSubMenu)
		{
			HMENU hSub = CreatePopupMenu();
			AppendMenuOp(hSub, mInfo.hSubMenu);
			mInfo.hSubMenu = hSub;
		}

		InsertMenuItem(hDst, iDst++, TRUE, &mInfo);
		iCnt++;
	}

	return iCnt;
}

CString CCommon::LoadText(UINT id, LPCTSTR back_str)
{
	CString str;
	str.LoadString(id);
	if (back_str != nullptr)
		str += back_str;
	return str;
}

CString CCommon::LoadText(LPCTSTR front_str, UINT id, LPCTSTR back_str)
{
	CString str;
	str.LoadString(id);
	if (back_str != nullptr)
		str += back_str;
	if (front_str != nullptr)
		str = front_str + str;
	return str;
}

CString CCommon::StringFormat(LPCTSTR format_str, const std::initializer_list<CVariant>& paras)
{
	CString str_rtn = format_str;
	int index = 1;
	for (const auto& para : paras)
	{
		CString para_str = para.ToString();
		CString format_para;
		format_para.Format(_T("<%%%d%%>"), index);
		str_rtn.Replace(format_para, para_str);

		index++;
	}
	return str_rtn;
}

CString CCommon::LoadTextFormat(UINT id, const std::initializer_list<CVariant>& paras)
{
	CString str;
	str.LoadString(id);
	return StringFormat(str.GetString(), paras);
}


void CCommon::SetThreadLanguage(Language language)
{
	switch (language)
	{
	case Language::ENGLISH: SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)); break;
	case Language::SIMPLIFIED_CHINESE: SetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)); break;
	//case Language::TRADITIONAL_CHINESE: SetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)); break;
	default: break;
	}
}

void CCommon::WStringCopy(wchar_t * str_dest, int dest_size, const wchar_t * str_source, int source_size)
{
	if (dest_size <= 0)
		return;
	if (source_size <= 0 || str_source == nullptr)
	{
		str_dest[0] = L'\0';
		return;
	}
	int i;
	for (i = 0; i < dest_size && i < source_size && str_source[i] != L'\0'; i++)
		str_dest[i] = str_source[i];
	//ȷ��Ŀ���ַ���ĩβ��һ��\0
	int copy_cnt = i;
	if (copy_cnt < dest_size)
		str_dest[copy_cnt] = L'\0';
	else
		str_dest[dest_size - 1] = L'\0';
}

