#include "stdafx.h"
#include "Lyric.h"


CLyrics::CLyrics(const wstring& file_name) : m_file{ file_name }
{
	DivideLyrics();
	JudgeCode();
	DisposeLyric();
	std::stable_sort(m_lyrics.begin(), m_lyrics.end());		//����ʰ�ʱ���ǩ����ʹ��stable_sort��ȷ����ͬ��Ԫ�����λ�ñ��ֲ��䣬���ڴ��������ĸ��ʱȷ��������ԭ�ĵĺ��棩
}

void CLyrics::DivideLyrics()
{
	ifstream OpenFile{ m_file };
	if (OpenFile.fail()) return;
	string current_line;
	while (!OpenFile.eof())
	{
		std::getline(OpenFile, current_line);		//�Ӹ���ļ��л�ȡһ�и��
		CCommon::StringNormalize(current_line);		//ɾ��һ�и��ǰ��Ŀո�������ַ�
		m_lyrics_str.push_back(current_line);
	}
}

void CLyrics::DisposeLyric()
{
	int index;
	string temp;
	Lyric lyric;
	m_translate = false;
	for (int i{ 0 }; i < m_lyrics_str.size(); i++)
	{
		//����id��ǩ������id��ǩ�����Լ����ϵģ�����Զֻ������ڵ�һ�У�
		if (i == 0)
		{
			index = m_lyrics_str[i].find("[id:");
			if (index != string::npos)
			{
				m_id_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 4, index2 - index - 4);
				m_id = CCommon::StrToUnicode(temp, m_code_type);
			}
		}

		//����ti��ǩ
		if (!m_ti_tag)
		{
			index = m_lyrics_str[i].find("[ti:");
			if (index != string::npos)
			{
				m_ti_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 4, index2 - index - 4);
				m_ti = CCommon::StrToUnicode(temp, m_code_type);
			}
		}

		//����ar��ǩ
		if (!m_ar_tag)
		{
			index = m_lyrics_str[i].find("[ar:");
			if (index != string::npos)
			{
				m_ar_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 4, index2 - index - 4);
				m_ar = CCommon::StrToUnicode(temp, m_code_type);
			}
		}

		//����al��ǩ
		if (!m_al_tag)
		{
			index = m_lyrics_str[i].find("[al:");
			if (index != string::npos)
			{
				m_al_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 4, index2 - index - 4);
				m_al = CCommon::StrToUnicode(temp, m_code_type);
			}
		}

		//����by��ǩ
		if (!m_by_tag)
		{
			index = m_lyrics_str[i].find("[by:");
			if (index != string::npos)
			{
				m_by_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 4, index2 - index - 4);
				m_by = CCommon::StrToUnicode(temp, m_code_type);
			}
		}

		//��ȡƫ����
		if (!m_offset_tag)
		{
			index = m_lyrics_str[i].find("[offset:");		//����ƫ������ǩ
			if (index != string::npos)
			{
				m_offset_tag = true;
				size_t index2 = m_lyrics_str[i].find(']');
				temp = m_lyrics_str[i].substr(index + 8, index2 - index - 8);
				m_offset = atoi(temp.c_str());		//��ȡƫ����
				m_offset_tag_index = i;		//��¼ƫ������ǩ��λ��
			}
		}

		//��ȡ����ı�
		index = m_lyrics_str[i].find_last_of(']');		//�������һ��']'��������ַ���Ϊ����ı�
		if (index == string::npos) continue;
		temp = m_lyrics_str[i].substr(index + 1, m_lyrics_str[i].size() - index - 1);
		CCommon::StringNormalize(temp);		//ɾ������ı�ǰ��Ŀո�������ַ�
		//����ȡ���ĸ���ı�ת����Unicode
		lyric.text = CCommon::StrToUnicode(temp, m_code_type);
		size_t index1;
		index1 = lyric.text.find(L" / ");
		if (index1 != wstring::npos)		//����ҵ��ˡ� / ����˵���þ��ʰ�������
		{
			lyric.translate = lyric.text.substr(index1 + 3);
			lyric.text = lyric.text.substr(0, index1);
			//CCommon::StringNormalize(lyric.text);
			//CCommon::StringNormalize(lyric.translate);
			if(!lyric.translate.empty())
				m_translate = true;
		}
		else
		{
			lyric.translate.clear();
		}

		//if (lyric.text.empty())		//���ʱ���ǩ��û���ı�����ʾΪ��������
		//	lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
		//if (!lyric.text.empty() && lyric.text.back() <= 32) lyric.text.pop_back();		//ɾ�����ĩβ��һ�������ַ���ո�

		//��ȡʱ���ǩ
		index = -1;
		while (true)
		{
			index = m_lyrics_str[i].find_first_of('[', index + 1);		//���ҵ�1����������
			if (index == string::npos) break;		//û���ҵ��������ţ��˳�ѭ��
			else if (index > static_cast<int>(m_lyrics_str[i].size() - 9)) break;		//�ҵ����������ţ����������������ַ����ĵ�����9���ַ��Ժ�Ҳ�˳�ѭ��
			else if ((m_lyrics_str[i][index + 1] > '9' || m_lyrics_str[i][index + 1] < '0') && m_lyrics_str[i][index + 1] != '-') continue;		//�ҵ����������ţ������������ź��治������Ҳ���Ǹ��ţ��˳�����ѭ�����������Ҹ�������һ����������
			
			//����Ѳ��ҵ�ʱ���ǩ�ˣ����ǻ�û���ҵ�offset��ǩ����m_offset_tag_index����Ϊ��1��ʱ���ǩ��λ��
			if (!m_offset_tag && m_offset_tag_index == -1)
			{
				m_offset_tag_index = i;
			}

			int index1, index2, index3;		//��ʱ�ǩ��ð�š�Բ����������ŵ�λ��
			index1 = m_lyrics_str[i].find_first_of(':', index);		//���Ҵ��������ſ�ʼ��1��ð�ŵ�λ��
			index2 = m_lyrics_str[i].find_first_of(".:", index1 + 1);	//���Ҵӵ�1��ð�ſ�ʼ��1��Բ���ð�ŵ�λ�ã����Ӻͺ�����Ӧ����Բ��ָ�������Ҳ������ð�ŷָ��ĸ�ʣ�
			index3 = m_lyrics_str[i].find(']', index);		//�����������ŵ�λ��
			temp = m_lyrics_str[i].substr(index + 1, index1 - index - 1);		//��ȡʱ���ǩ�ķ�����
			lyric.time.min = atoi(temp.c_str());
			temp = m_lyrics_str[i].substr(index1 + 1, index2 - index1 - 1);		//��ȡʱ���ǩ��������
			lyric.time.sec = atoi(temp.c_str());
			temp = m_lyrics_str[i].substr(index2 + 1, index3 - index2 - 1);		//��ȡʱ���ǩ�ĺ�����
			int char_cnt = temp.size();				//��������λ��
			if (char_cnt > 0 && temp[0] == '-')		//�����������ǰ���и��ţ���λ����1
				char_cnt--;
			switch (char_cnt)
			{
			case 0: lyric.time.msec = 0;
			case 1: lyric.time.msec = atoi(temp.c_str()) * 100; break;
			case 2: lyric.time.msec = atoi(temp.c_str()) * 10; break;
			default: lyric.time.msec = atoi(temp.c_str()) % 1000; break;
			}
			m_lyrics.push_back(lyric);
		}
	}
}

void CLyrics::JudgeCode()
{
	if (!m_lyrics_str.empty())		//ȷ����ʲ�Ϊ��
	{
		//��BOM������£�ǰ��3���ֽ�Ϊ0xef(-17), 0xbb(-69), 0xbf(-65)����UTF8����
		if (m_lyrics_str[0].size() >= 3 && (m_lyrics_str[0][0] == -17 && m_lyrics_str[0][1] == -69 && m_lyrics_str[0][2] == -65))	//ȷ��m_lyrics_str[0]�ĳ��ȴ��ڻ����3���Է�ֹ����Խ��
		{
			m_code_type = CodeType::UTF8;
			m_lyrics_str[0] = m_lyrics_str[0].substr(3);		//ȥ��ǰ���BOM
		}
		else				//��BOM�������
		{
			int i, j;
			bool break_flag{ false };
			for (i = 0; i < m_lyrics_str.size(); i++)		//����ÿһ����
			{
				if (m_lyrics_str[i].size() <= 16) continue;		//�����ַ���Ϊ6���µĸ��(ʱ���ǩռ10���ַ�)�����̵��ַ������ܻᵼ�½�ANSI�������ΪUTF8
				for (j = 0; j < m_lyrics_str[i].size(); j++)		//����ÿһ�����е�ÿһ���ַ�
				{
					if (m_lyrics_str[i][j] < 0)		//�ҵ���1����ASCII�ַ�ʱ����ѭ��
					{
						break_flag = true;
						break;
					}
				}
				if (break_flag) break;
			}
			if (i<m_lyrics_str.size() && CCommon::IsUTF8Bytes(m_lyrics_str[i].c_str()))		//�жϳ��ֵ�1����ASCII�ַ����Ǿ����ǲ���UTF8���룬����Ǹ�ʾ���UTF8����
				m_code_type = CodeType::UTF8_NO_BOM;
		}
	}
}

bool CLyrics::IsEmpty() const
{
	return (m_lyrics.size() == 0);
}

CLyrics::Lyric CLyrics::GetLyric(Time time, int offset) const
{
	Lyric ti{};
	ti.text = m_ti;
	for (int i{ 0 }; i < m_lyrics.size(); i++)
	{
		if (m_lyrics[i].GetTime(m_offset) > time)		//����ҵ���һ��ʱ���ǩ��Ҫ��ʾ��ʱ������ʱ���ǩ��ǰһ���ʼ�Ϊ��ǰ���
		{
			if (i + offset - 1 < -1) return Lyric{};
			else if (i + offset - 1 == -1) return ti;		//ʱ���ڵ�һ��ʱ���ǩǰ�棬����ti��ǩ��ֵ
			else if (i + offset - 1 < m_lyrics.size()) return m_lyrics[i + offset - 1];
			else return Lyric{};
		}
	}
	if (m_lyrics.size() + offset - 1 < m_lyrics.size())
		return m_lyrics[m_lyrics.size() + offset - 1];		//���û��ʱ���ǩ��Ҫ��ʾ��ʱ��󣬵�ǰ��ʾ������һ����
	else
		return Lyric{};
}

CLyrics::Lyric CLyrics::GetLyric(int index) const
{
	Lyric ti{};
	ti.text = m_ti;
	if (index < 0)
		return Lyric();
	else if (index == 0)
		return ti;
	else if (index <= m_lyrics.size())
		return m_lyrics[index - 1];
	else
		return Lyric();
}

int CLyrics::GetLyricProgress(Time time) const
{
	if (m_lyrics.empty())
		return 0;

	int lyric_last_time{ 1 };		//timeʱ�����ڵĸ�ʳ�����ʱ��
	int lyric_current_time{ 0 };		//��ǰ�����timeʱ��ʱ�Ѿ�������ʱ��
	int progress{};
	for (int i{ 0 }; i < m_lyrics.size(); i++)
	{
		if (m_lyrics[i].GetTime(m_offset) > time)
		{
			if (i == 0)
			{
				lyric_current_time = time.time2int();
				lyric_last_time = m_lyrics[i].GetTime(m_offset).time2int();
			}
			else
			{
				lyric_last_time = m_lyrics[i].GetTime(m_offset) - m_lyrics[i - 1].GetTime(m_offset);
				lyric_current_time = time - m_lyrics[i - 1].GetTime(m_offset);
			}
			if (lyric_last_time == 0) lyric_last_time = 1;
			progress = lyric_current_time * 1000 / lyric_last_time;
			return progress;
		}
	}
	//������һ����֮���Ѿ�û��ʱ���ǩ���þ���Ĭ����ʾ20��
	lyric_current_time = time - m_lyrics[m_lyrics.size() - 1].GetTime(m_offset);
	lyric_last_time = 20000;
	progress = lyric_current_time * 1000 / lyric_last_time;
	if (progress > 1000) progress = 1000;
	return progress;
}

int CLyrics::GetLyricIndex(Time time) const
{
	for (int i{ 0 }; i < m_lyrics.size(); i++)
	{
		if (m_lyrics[i].GetTime(m_offset)>time)
			return i - 1;
	}
	return m_lyrics.size() - 1;
}

CodeType CLyrics::GetCodeType() const
{
	return m_code_type;
}

wstring CLyrics::GetAllLyricText(bool with_translate) const
{
	wstring all_lyric{};
	for (auto a_lyric : m_lyrics)
	{
		all_lyric += a_lyric.text;
		all_lyric += L"\r\n";
		if(with_translate && !a_lyric.translate.empty())
		{
			all_lyric += a_lyric.translate;
			all_lyric += L"\r\n";
		}
	}
	return all_lyric;
}

wstring CLyrics::GetLyricsString() const
{
	wstring lyric_string{};
	if (m_offset == 0)		//���ʱ��ƫ��Ϊ0���򷵻�ԭʼ�ĸ���ı�
	{
		for (auto str : m_lyrics_str)
		{
			lyric_string += CCommon::StrToUnicode(str, m_code_type);
			lyric_string += L"\r\n";
		}
	}
	else		//���ʱ��ƫ�Ʋ�Ϊ0�����ؽ�ʱ��ƫ��д��ÿ��ʱ���ǩ��ĸ���ı�
	{
		if (m_id_tag) lyric_string += (L"[id:" + m_id + L"]\r\n");
		if (m_ti_tag) lyric_string += (L"[ti:" + m_ti + L"]\r\n");
		if (m_ar_tag) lyric_string += (L"[ar:" + m_ar + L"]\r\n");
		if (m_al_tag) lyric_string += (L"[al:" + m_al + L"]\r\n");
		if (m_by_tag) lyric_string += (L"[al:" + m_al + L"]\r\n");
		if (m_offset_tag) lyric_string += L"[offset:0]\r\n";
		wchar_t time_buff[16];
		for (auto a_lyric : m_lyrics)
		{
			Time a_time{ a_lyric.GetTime(m_offset) };
			swprintf_s(time_buff, L"[%.2d:%.2d.%.2d]", a_time.min, a_time.sec, a_time.msec / 10);
			lyric_string += time_buff;
			lyric_string += a_lyric.text;
			lyric_string += L"\r\n";
		}
	}
	if (lyric_string.size() > 1)
	{
		lyric_string.pop_back();	//���һ�в���Ҫ�ӻس���ɾ��ĩβ��\r\n
		lyric_string.pop_back();
	}
	return lyric_string;
}

void CLyrics::SaveLyric()
{
	if (m_lyrics.size() == 0) return;	//û�и��ʱֱ�ӷ���
	ofstream out_put{ m_file };
	//�����ʱ�����UTF8������ǰ�����BOM
	if (m_code_type == CodeType::UTF8)
	{
		char buff[4];
		buff[0] = -17;
		buff[1] = -69;
		buff[2] = -65;
		buff[3] = 0;
		out_put << buff;
	}
	for (int i{ 0 }; i < m_lyrics_str.size(); i++)
	{
		if (m_offset_tag_index == i)	//���i��ƫ�Ʊ�ǩ��λ�ã�������ʱ���ƫ�Ʊ�ǩ
		{
			out_put << "[offset:" << m_offset << ']' << std::endl;
			if (!m_offset_tag)			//�������û��ƫ�Ʊ�ǩ������ʱ�ǲ���һ��ƫ�Ʊ�ǩ��֮��Ҫ�����ǰ���
				out_put << m_lyrics_str[i] << std::endl;
		}
		else
		{
			out_put << m_lyrics_str[i] << std::endl;
		}
	}
	m_modified = false;
}

void CLyrics::SaveLyric2()
{
	if (m_lyrics.size() == 0) return;	//û�и��ʱֱ�ӷ���
	ofstream out_put{ m_file };
	//�����ʱ�����UTF8������ǰ�����BOM
	if (m_code_type == CodeType::UTF8)
	{
		char buff[4];
		buff[0] = -17;
		buff[1] = -69;
		buff[2] = -65;
		buff[3] = 0;
		out_put << buff;
	}
	//�����ʶ��ǩ
	if(m_id_tag) out_put << "[id:" << CCommon::UnicodeToStr(m_id, m_code_type) << "]" << std::endl;
	if(m_ti_tag) out_put << "[ti:" << CCommon::UnicodeToStr(m_ti, m_code_type) << "]" << std::endl;
	if (m_ar_tag) out_put << "[ar:" << CCommon::UnicodeToStr(m_ar, m_code_type) << "]" << std::endl;
	if (m_al_tag) out_put << "[al:" << CCommon::UnicodeToStr(m_al, m_code_type) << "]" << std::endl;
	if (m_by_tag) out_put << "[by:" << CCommon::UnicodeToStr(m_by, m_code_type) << "]" << std::endl;
	if (m_offset_tag) out_put << "[offset:0]" << std::endl;		//����ƫ���������浽ʱ���ǩ�У�����offset��ǩ�е�ƫ����Ϊ0
	char time_buff[16];
	for (auto a_lyric : m_lyrics)
	{
		Time a_time{ a_lyric.GetTime(m_offset) };
		sprintf_s(time_buff, "[%.2d:%.2d.%.2d]", a_time.min, a_time.sec, a_time.msec / 10);
		out_put << time_buff << CCommon::UnicodeToStr(a_lyric.text, m_code_type);
		if (!a_lyric.translate.empty())
			out_put << " / " << CCommon::UnicodeToStr(a_lyric.translate, m_code_type);
		out_put << std::endl;
	}
	out_put.close();
	m_modified = false;
	m_chinese_converted = false;
}

void CLyrics::CombineSameTimeLyric()
{
	for (int i{}; i < static_cast<int>(m_lyrics.size() - 1); i++)
	{
		if (m_lyrics[i].time == m_lyrics[i + 1].time)	//�ҵ���ͬʱ���ǩ�ĸ��
		{
			if (!m_lyrics[i].text.empty() && !m_lyrics[i + 1].text.empty())		//ֻ��������ͬʱ���ǩ�ĸ�ʶ����ı�ʱ������Ҫ����һ��б��
			{
				m_lyrics[i].text += L" / ";
			}
			m_lyrics[i].text += m_lyrics[i + 1].text;	//�ϲ������ʵ��ı�
			m_lyrics.erase(m_lyrics.begin() + i + 1);	//ɾ������һ����
		}
	}
}

void CLyrics::DeleteRedundantLyric()
{
	for (size_t i{}; i < m_lyrics.size(); i++)
	{
		if (m_lyrics[i].time > Time{ 100,0,0 })		//�ҵ�һ���ʵ�ʱ���ǩ����100����
		{
			m_lyrics.erase(m_lyrics.begin() + i, m_lyrics.end());		//ɾ���þ��ʼ����������и��
			break;
		}
	}
}

void CLyrics::AdjustLyric(int offset)
{
	if (m_lyrics.size() == 0) return;	//û�и��ʱֱ�ӷ���
	m_offset += offset;
	m_modified = true;
}

void CLyrics::ChineseConvertion(bool simplified)
{
	for (auto& lyric : m_lyrics)
	{
		if (m_translate)		//�����ǰ����з��룬��ֻ��ȫ�������ı�ת��
		{
			if (simplified)
				lyric.translate = CCommon::TranslateToSimplifiedChinese(lyric.translate);
			else
				lyric.translate = CCommon::TranslateToTranditionalChinese(lyric.translate);
		}
		else
		{
			if (simplified)
				lyric.text = CCommon::TranslateToSimplifiedChinese(lyric.text);
			else
				lyric.text = CCommon::TranslateToTranditionalChinese(lyric.text);
		}
	}
	m_modified = true;
	m_chinese_converted = true;
	//SaveLyric2();
	//m_lyrics_str.clear();
	//DivideLyrics();
}
