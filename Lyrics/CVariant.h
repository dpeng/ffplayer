
#include <iostream>
#include <afx.h>
#include "afxinet.h"
#include "string"
#include <afxwin.h>
#include <vector>
#include <time.h>
#include <string.h>
using namespace std;
#pragma once
class CVariant
{
public:
	CVariant(int value);
	CVariant(unsigned int value);
	CVariant(double value);
	CVariant(LPCTSTR value);
	CVariant(const CString& value);
	CVariant(const wstring& value);

	~CVariant();

	CString ToString() const;

private:
	enum class eType { INT, UINT, DOUBLE, STRING };

	int m_value_int;
	double m_value_double;
	CString m_value_string;
	eType m_type;

};

