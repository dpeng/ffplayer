#pragma once
class Time
{
public:
	int min{};
	int sec{};
	int msec{};

	Time()
	{}

	Time(int _min, int _sec, int _msec) : min{_min}, sec{_sec}, msec{_msec}
	{}

	Time(int time)
	{
		int2time(time);
	}

	~Time()
	{}

	//��int���͵�ʱ�䣨��������ת����Time�ṹ
	void int2time(int time)
	{
		msec = time % 1000;
		sec = time / 1000 % 60;
		min = time / 1000 / 60;
	}

	//��Time�ṹת����int���ͣ���������
	int time2int() const
	{
		return msec + sec * 1000 + min * 60000;
	}

	bool operator>(const Time& time) const
	{
		if (min != time.min)
			return (min > time.min);
		else if (sec != time.sec)
			return(sec > time.sec);
		else if (msec != time.msec)
			return(msec > time.msec);
		else return false;
	}

	bool operator==(const Time& time) const
	{
		return (min == time.min && sec == time.sec && msec == time.msec);
	}
	
	bool operator!=(const Time& time) const
	{
		return !(*this == time);
	}

	bool operator>=(const Time& time) const
	{
		if (min != time.min)
			return (min > time.min);
		else if (sec != time.sec)
			return(sec > time.sec);
		else if (msec != time.msec)
			return(msec > time.msec);
		else return true;
	}

	//��������������ڼ�������Time�����ʱ������int���ͣ���λΪ����
	int operator-(const Time& time) const
	{
		return (min - time.min) * 60000 + (sec - time.sec) * 1000 + (msec - time.msec);
	}

	//�ӷ���ֵ������������ڵ�ǰʱ���ϼ���һ��int���͵ĺ�����
	Time operator+=(int time)
	{
		int added = this->time2int();
		added += time;
		this->int2time(added);
		return *this;
	}

	//�ӷ�������������ڵ�ǰʱ���ϼ���һ��int���͵ĺ�����������Time����
	Time operator+(int time) const
	{
		int added = this->time2int();
		added += time;
		return Time{ added };
	}

	//��ʱ��ת�����ַ�������ʽ����:�룩
	wstring time2str() const
	{
		wchar_t buff[16];
		if (*this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--");
		else
			swprintf_s(buff, L"%d:%.2d", min, sec);
		return wstring(buff);
	}

	//��ʱ��ת�����ַ�������ʽ����:��.���룩
	wstring time2str2() const
	{
		wchar_t buff[16];
		if (*this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--");
		else
			swprintf_s(buff, L"%d:%.2d.%.3d", min, sec, msec);
		return wstring(buff);
	}

	//��ʱ��ת�����ַ�������ʽ��ʱ:��:�룩
	wstring time2str3() const
	{
		int hour, min1;
		hour = min / 60;
		min1 = min % 60;
		wchar_t buff[16];
		if (*this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--:--");
		else
			swprintf_s(buff, L"%d:%.2d:%.2d", hour, min1, sec);
		return wstring(buff);
	}

	//�ж�ʱ���Ƿ�Ϊ0
	bool isZero() const
	{
		return (min == 0 && sec == 0 && msec == 0);
	}
};

