#include "stdafx.h"
#include "InternetCommon.h"
#pragma warning (disable : 4101)
#pragma warning (disable : 4267)
CInternetCommon::CInternetCommon()
{
}


CInternetCommon::~CInternetCommon()
{
}

wstring CInternetCommon::URLEncode(const wstring & wstr)
{
	string str_utf8;
	wstring result{};
	wchar_t buff[4];
	str_utf8 = CCommon::UnicodeToStr(wstr, CodeType::UTF8_NO_BOM);
	for (const auto& ch : str_utf8)
	{
		if (ch == ' ')
			result.push_back(L'+');
		else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
			result.push_back(static_cast<wchar_t>(ch));
		else if (ch == '-' || ch == '_' || ch == '.' || ch == '!' || ch == '~' || ch == '*'/* || ch == '\''*/ || ch == '(' || ch == ')')
			result.push_back(static_cast<wchar_t>(ch));
		else
		{
			swprintf_s(buff, L"%%%x", static_cast<unsigned char>(ch));
			result += buff;
		}
	}
	return result;
}

bool CInternetCommon::GetURL(const wstring & str_url, wstring & result)
{
	bool sucessed{ false };
	CInternetSession session{};
	CHttpFile* pfile{};
	try
	{
		pfile = (CHttpFile *)session.OpenURL(str_url.c_str());
		DWORD dwStatusCode;
		pfile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode == HTTP_STATUS_OK)
		{
			CString content;
			CString data;
			while (pfile->ReadString(data))
			{
				content += data;
			}
			result = CCommon::StrToUnicode(string{ (const char*)content.GetString() }, CodeType::UTF8);	//��ȡ��ҳ���ݣ���ת����Unicode����
			sucessed = true;
		}
		pfile->Close();
		delete pfile;
		session.Close();
	}
	catch (CInternetException* e)
	{
		if (pfile != nullptr)
		{
			pfile->Close();
			delete pfile;
		}
		session.Close();
		sucessed = false;
	}
	return sucessed;
}


int CInternetCommon::HttpPost(const wstring & str_url, wstring & result)
{
	CInternetSession session;
	CHttpConnection* pConnection{};
	CHttpFile* pFile{};
	CString strServer;
	CString strObject;
	DWORD dwServiceType;
	INTERNET_PORT nPort;

	AfxParseURL(str_url.c_str(), dwServiceType, strServer, strObject, nPort);

	if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
		return FAILURE;

	try
	{
		pConnection = session.GetHttpConnection(strServer,
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
			nPort);
		pFile = pConnection->OpenRequest(_T("POST"), strObject,
			NULL, 1, NULL, NULL,
			(dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));

		pFile->SendRequest(NULL, 0, NULL, 0);

		CString content;
		CString data;
		while (pFile->ReadString(data))
		{
			content += data;
		}
		result = CCommon::StrToUnicode(string{ (const char*)content.GetString() }, CodeType::UTF8);


		pFile->Close();
		delete pFile;
		pConnection->Close();
		delete pConnection;
		session.Close();
	}
	catch (CInternetException* e)
	{
		pFile->Close();
		delete pFile;
		pConnection->Close();
		delete pConnection;
		session.Close();
		DWORD dwErrorCode = e->m_dwError;
		e->Delete();
		//DWORD dwError = GetLastError();
		//PRINT_LOG("dwError = %d", dwError, 0);

		if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
			return OUTTIME;
		else
			return FAILURE;
	}
	return SUCCESS;
}


void CInternetCommon::DeleteStrSlash(wstring & str)
{
	for (int i{}; i < static_cast<int>(str.size() - 1); i++)
	{
		if (str[i] == '\\' && str[i + 1] == '\"')
		{
			str.erase(i, 1);
		}
	}
}

void CInternetCommon::DisposeSearchResult(vector<ItemInfo>& down_list, const wstring& search_result, int result_count)
{
	down_list.clear();
	ItemInfo item;
	int index1{}, index2{}, index3{}, index4{};
	//while (true)
	for (int i{}; i < result_count; i++)
	{
		//��ȡ������ID
		if (i == 0)
		{
			index1 = search_result.find(L"\"songs\":[{\"id\":", index1 + 1);
			if (index1 == string::npos) break;
			index2 = search_result.find(L',', index1);
			item.id = search_result.substr(index1 + 15, index2 - index1 - 15);
		}
		else
		{
			index1 = search_result.find(L",{\"id\":", index1 + 1);
			if (index1 == string::npos) break;
			index2 = search_result.find(L',', index1 + 1);
			item.id = search_result.substr(index1 + 7, index2 - index1 - 7);
		}

		//��ȡ��������
		index2 = search_result.find(L"name", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"\",\"", index2);
		wstring title = search_result.substr(index2 + 7, index3 - index2 - 7);
		if (search_result.substr(index3 + 3, 6) == L"picUrl")	//����ҵ��ġ�name��������ַ����ǡ�picUrl����˵������name��ֵ����
		{														//��һ�׸�ı��⣬������һ�׸�������ң���һ�׸��ж��������
			if (!down_list.empty())
			{
				down_list.back().artist += L'/';
				down_list.back().artist += title;
			}
			continue;
		}
		else
		{
			item.title = title;
		}

		//��ȡ������������
		index2 = search_result.find(L"artists", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"name", index2);
		index4 = search_result.find(L"\",\"", index3);
		item.artist = search_result.substr(index3 + 7, index4 - index3 - 7);

		//��ȡ�����ĳ�Ƭ��
		index2 = search_result.find(L"\"album\"", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"name", index2);
		index4 = search_result.find(L"\",\"", index3);
		item.album = search_result.substr(index3 + 7, index4 - index3 - 7);

		DeleteStrSlash(item.title);
		DeleteStrSlash(item.artist);
		DeleteStrSlash(item.album);
		down_list.push_back(item);
	}
}

double CInternetCommon::StringSimilarDegree_LD(const wstring & srcString, const wstring & matchString)
{
	int n = srcString.size();
	int m = matchString.size();
	//int[, ] d = new int[n + 1, m + 1]; // matrix
	vector<vector<int>> d(n + 1, vector<int>(m + 1));
	int cost; // cost
			  // Step 1���������һ���ַ�������Ϊ0�������ƶ�Ϊ1����
			  //if (n == 0) return (double)m / max(srcString.size(), matchString.size());
			  //if (m == 0) return (double)n / max(srcString.size(), matchString.size());
	if (n == 0 || m == 0) return 0.0;	//�������һ���ַ�������Ϊ0�������ƶ�Ϊ0
										// Step 2
	for (int i = 0; i <= n; d[i][0] = i++);
	for (int j = 0; j <= m; d[0][j] = j++);
	// Step 3
	for (int i = 1; i <= n; i++)
	{
		//Step 4
		for (int j = 1; j <= m; j++)
		{
			// Step 5
			cost = (matchString.substr(j - 1, 1) == srcString.substr(i - 1, 1) ? 0 : 1);
			// Step 6
			d[i][j] = min(min(d[i - 1][j] + 1, d[i][j - 1] + 1), d[i - 1][j - 1] + cost);
		}
	}

	// Step 7
	double ds = 1 - (double)d[n][m] / max(srcString.size(), matchString.size());

	return ds;
}


int CInternetCommon::SelectMatchedItem(const vector<ItemInfo>& down_list, const wstring & title, const wstring & artist, const wstring & album, const wstring & filename, bool write_log)
{
	/*
	ƥ��ȼ��㣺
	ͨ���������µ�ƥ��ȣ�������Ȩֵ����ÿһ���ƥ��ȳ���Ȩֵ����ӣ��õ���ֵ���ľ�����ƥ���һ��
	��Ŀ			 Ȩֵ
	���⡪������         0.4
	�����ҡ���������     0.4
	��Ƭ��������Ƭ��     0.3
	�ļ�����������       0.3
	�ļ�������������     0.2
	�б��е�����       0.05
	*/
	if (down_list.empty()) return -1;
	vector<double> weights;		//�����б���ÿһ���Ȩֵ

								//����ÿһ���Ȩֵ
	for (size_t i{}; i<down_list.size(); i++)
	{
		double weight;
		weight = 0;
		weight += (StringSimilarDegree_LD(title, down_list[i].title) * 0.4);
		weight += (StringSimilarDegree_LD(artist, down_list[i].artist) * 0.4);
		weight += (StringSimilarDegree_LD(album, down_list[i].album) * 0.3);
		weight += (StringSimilarDegree_LD(filename, down_list[i].title) * 0.3);
		weight += (StringSimilarDegree_LD(filename, down_list[i].artist) * 0.3);
		weight += ((1 - i * 0.02) * 0.05);			//�б���˳���Ȩֵ��һ����˵�����������ֵ���������ķ��ؽ����
													//����Խǰ��Ĺ����Ⱦ�Խ�ߣ�����ȡ��һ��Ϊ1��֮��ÿһ���0.02������ٳ���0.05
		weights.push_back(weight);
	}

	//����Ȩֵ������
	double max_weight = weights[0];
	int max_index{};
	for (size_t i{ 1 }; i < weights.size(); i++)
	{
		if (weights[i] > max_weight)
		{
			max_weight = weights[i];
			max_index = i;
		}
	}

	//���Ȩֵ������ȨֵС��0.3�����ж�û��ƥ��������-1
	if (max_weight < 0.3)
		max_index = -1;

#ifdef DEBUG
	if (write_log)
	{
		CString out_info{ _T("\r\n==============================================================================\r\n") };
		CString tmp;
		out_info += _T("\r\n������Ϣ��\r\n");
		out_info += _T("�ļ�����");
		out_info += filename.c_str();
		out_info += _T("\r\n���⣺");
		out_info += title.c_str();
		out_info += _T("\r\n�����ң�");
		out_info += artist.c_str();
		out_info += _T("\r\n��Ƭ����");
		out_info += album.c_str();

		out_info += _T("\r\n���������\r\n");
		out_info += _T("���\t����ID\t����\t������\t��Ƭ��\r\n");
		for (size_t i{}; i<down_list.size(); i++)
		{
			tmp.Format(_T("%d\t%s\t%s\t%s\t%s\r\n"), i + 1, down_list[i].id.c_str(), down_list[i].title.c_str(), down_list[i].artist.c_str(), down_list[i].album.c_str());
			out_info += tmp;
		}

		out_info += _T("����Ȩֵ��\r\n");
		for (size_t i{}; i < weights.size(); i++)
		{
			tmp.Format(_T("%d\t%f\r\n"), i + 1, weights[i]);
			out_info += tmp;
		}

		tmp.Format(_T("���ƥ���%d\r\n\r\n"), max_index + 1);
		out_info += tmp;

		CCommon::WriteLog(L".\\search.log", wstring{ out_info });
	}
#endif // DEBUG

	return max_index;
}

CInternetCommon::ItemInfo CInternetCommon::SearchSongAndGetMatched(const wstring & title, const wstring & artist, const wstring & album, const wstring & file_name, bool message)
{
	//���������ؼ���
	wstring search_result;		//���Ҹ������صĽ��
	wstring keyword;		//���ҵĹؼ���
	keyword = artist + L' ' + title;

	//��������
	wstring keyword_url = CInternetCommon::URLEncode(keyword);		//�������ؼ���ת����URL����
	wchar_t buff[1024];
	swprintf_s(buff, L"http://music.163.com/api/search/get/?s=%s&limit=20&type=1&offset=0", keyword_url.c_str());
	int rtn = CInternetCommon::HttpPost(buff, search_result);		//�����������ֵĸ�������API����http��POST����
	if (rtn != 0)
	{
		if(message)
			MessageBox(NULL, L"connect failed", NULL, MB_ICONWARNING);
		return CInternetCommon::ItemInfo();
	}

	//�����ؽ��
	vector<CInternetCommon::ItemInfo> down_list;
	CInternetCommon::DisposeSearchResult(down_list, search_result);		//�����صĲ��ҽ�����������������down_list������
	if (down_list.empty())
	{
		if (message)
			MessageBox(NULL, L"couldnot found lyrics", NULL, MB_ICONWARNING);
		return CInternetCommon::ItemInfo();
	}

	//�������ѡ����
	wstring _title = title;
	wstring _artist = artist;
	wstring _album = album;
	if (_title.empty())
		_title = keyword;
	int best_matched = CInternetCommon::SelectMatchedItem(down_list, _title, _artist, _album, file_name, true);
	if (best_matched < 0)
	{
		if (message)
			MessageBox(NULL, L"couldnot found song", NULL, MB_ICONWARNING);
		return CInternetCommon::ItemInfo();
	}

	//�񷵻����ƥ����
	return down_list[best_matched];
}
