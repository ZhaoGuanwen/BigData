#ifndef BIG_DATA_H
#define BIG_DATA_H

#include <string>

#define MAX_INT64 (INT64)0x7FFFFFFFFFFFFFFF
#define MIN_INT64 (INT64)0x8000000000000000 //如果不强转，系统会定义为unsigned INT64类型

#define UN_INIT 0xCCCCCCCCCCCCCCCC
typedef long long INT64;

class BigData
{
public:
	BigData(INT64 value);
	BigData(const char* pData);

public:
	BigData operator+(const BigData& bigdata);
	BigData operator-(const BigData& bigdata);
	BigData operator*(const BigData& bigdata);
	BigData operator/(const BigData& bigdata);

protected:
	std::string Add(std::string left, std::string right);
	std::string Sub(std::string left, std::string right);
	std::string Mul(std::string left, std::string right);
	std::string Div(std::string left, std::string right);	

protected:
	bool IsINT64Overflow() const;
	friend std::ostream& operator<<(std::ostream& _cout, const BigData& bigdata);
	void INT64ToString();
	bool IsLeftStrBig(const char* pLeft, int LSize, const char* pRight, int RSize);
	char SubLoop(char* pLeft, int LSize, char* pRight, int RSize);

protected:
	INT64 _value;
	std::string _strData;
};

#endif
