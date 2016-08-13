#include "BigData.h"
#include <assert.h>

BigData::BigData(INT64 value)
	:_value(value)
{
	INT64ToString(); //将_value中的值保存在_strData中
}

BigData::BigData(const char* pData)
{
	//要处理的输入："12345678"   "234567qwe"    "+"    "    "     "0000123456"

	if (NULL == pData)
	{
		assert(false);
		return;
	}

	//处理符号位
	char* pStr = (char*)pData; 
	char cSymbol = pData[0];
	if ('+' == cSymbol || '-' == cSymbol)
		pStr++;
	else if (cSymbol >= '0' && cSymbol <= '9')
		cSymbol = '+';
	else
		return;

	//"0000123456"
	int iDataLen = strlen(pData);
	if (iDataLen > 1)
	{
		while ('0' == *pStr)
			pStr++;
	}

	_strData.resize(strlen(pData)+1); //为_strData分配空间
	_strData[0] = cSymbol; //第0位保存符号

	//"123456qwe"
	_value = 0;
	int iCount = 1;
	while (*pStr >= '0' && *pStr <= '9')
	{
		_value = _value*10 + (*pStr - '0');
		_strData[iCount++] = *pStr;
		pStr++;
	}

	_strData.resize(iCount);

	if (cSymbol == '-')
		_value = 0 - _value;
}

bool BigData::IsINT64Overflow() const
{
	std::string temp("+9223372036854775807");
	
	if (_strData[0] == '-')
		temp = "-9223372036854775808";

	if (_strData.size() < temp.size())
		return false;
	else if (_strData.size() == temp.size() && _strData <= temp)
		return false;

	return true;
}

void BigData::INT64ToString()
{
	//处理符号位
	char cSymbol = '+';
	if (_value < 0)
		cSymbol = '-';

	INT64 temp = _value;
	while (temp)
	{
		if (cSymbol == '+')
			_strData.append(1, temp%10 + '0');
		else
			_strData.append(1, -(temp%10) + '0');

		temp /= 10;
	}
	_strData.append(1, cSymbol);
	
	std::reverse(_strData.begin(), _strData.end());
}

bool BigData::IsLeftStrBig(const char* pLeft, int LSize, const char* pRight, int RSize)
{
	assert(pLeft != NULL && pRight != NULL);

	if ((LSize > RSize) || 
		(LSize == RSize && strcmp(pLeft, pRight) >= 0))
		return true;
	else
		return false;
}

char BigData::SubLoop(char* pLeft, int LSize, char* pRight, int RSize)
{
	assert(pLeft != NULL && pRight != NULL);

	char cRet = '0';

	while (true)
	{
		if (!IsLeftStrBig(pLeft, LSize, pRight, RSize))
			break;
		
		int iLIdx = LSize - 1;
		int iRIdx = RSize - 1;
		while (iLIdx >= 0 && iRIdx >= 0)
		{
			char ret = pLeft[iLIdx] - '0';
			ret -= pRight[iRIdx] - '0';
			
			if (ret < 0)
			{
				pLeft[iLIdx - 1] -= 1;
				ret += 10;
			}
			
			pLeft[iLIdx] = ret + '0';
			iLIdx--;
			iRIdx--;
		}

		while (*pLeft == '0' && LSize > 0)
		{
			pLeft++;
			LSize--;
		}

		cRet++;
	}

	return cRet;
}

std::ostream& operator<<(std::ostream& _cout, const BigData& bigdata)
{
	if (!bigdata.IsINT64Overflow()) //_value没有溢出
	{
		_cout<<bigdata._value;
	}
	else //_value溢出
	{
		char* pData = (char*)bigdata._strData.c_str();
		if ('+' == pData[0])
			pData++;

		_cout<<pData;
	}

	return _cout;
}

BigData BigData::operator+(const BigData& bigdata)
{
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow()) //两个数都没有溢出
	{
		if (_strData[0] != bigdata._strData[0]) //两个数异号
		{
			return BigData(_value + bigdata._value);
		}
		else //两个数同号
		{
			if ((_value >= 0 && bigdata._value <= MAX_INT64 - _value) || 
				(_value < 0 && bigdata._value >= MIN_INT64 - _value)) //相加后的和没有溢出
				return BigData(_value + bigdata._value);
		}
	}

	//两个数至少有一个溢出
	//相加后的和溢出
	if (_strData[0] == bigdata._strData[0]) //同号相加，调用加法
		return BigData(Add(_strData, bigdata._strData).c_str());
	else //异号相加，调用减法
		return BigData(Sub(_strData, bigdata._strData).c_str());
}

BigData BigData::operator-(const BigData& bigdata)
{
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow()) //两个数都没有溢出
	{
		if (_strData[0] == bigdata._strData[0])//同号
		{
			return BigData(_value - bigdata._value);
		}
		else //异号
		{
			if ((_value >= 0 && MAX_INT64 + bigdata._value >= _value) || 
				(_value < 0 && MIN_INT64 + bigdata._value <= _value)) //结果没有溢出
				return BigData(_value - bigdata._value);
		}
	}

	//至少有一个数溢出或结果溢出
	if (_strData[0] != bigdata._strData[0]) //异号相减，调用加法
		return BigData(Add(_strData, bigdata._strData).c_str());
	else //同号相减，调用减法
		return BigData(Sub(_strData, bigdata._strData).c_str());
}

BigData BigData::operator*(const BigData& bigdata)
{
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow()) //都没有溢出
	{
		if (_value == 0 || bigdata._value == 0)
		{
			return BigData((INT64)0);
		}

		if (_strData[0] == bigdata._strData[0]) //同号，积为正
		{
			if ((_value > 0 && MAX_INT64/_value >= bigdata._value) || 
				(_value < 0 && MAX_INT64/_value <= bigdata._value)) //积没有溢出
			{
				return BigData(_value * bigdata._value);
			}
		}
		else //异号,积为负
		{
			if ((_value > 0 && MIN_INT64/_value <= bigdata._value) || 
				(_value < 0 && MIN_INT64/_value >= bigdata._value)) //积没有溢出
			{
				return BigData(_value * bigdata._value);
			}
		}
	}

	return BigData(Mul(_strData, bigdata._strData).c_str());
}

BigData BigData::operator/(const BigData& bigdata)
{
	//除数不能为0
	if ('0' == bigdata._strData[1])
		assert(false);

	//	都没溢出


	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
		return BigData(_value / bigdata._value);

	//	至少有一个溢出


	//除数 == ±1
	if (bigdata._strData == "+1" && bigdata._strData == "-1") 
	{
		std::string strRet = _strData;

		if (_strData[0] != bigdata._strData[0])
			strRet[0] = '-';

		return BigData(strRet.c_str());
	}

	//左 < 右
	if ( (_strData.size() < bigdata._strData.size()) || 
		 (_strData.size() == bigdata._strData.size() && strcmp(_strData.c_str()+1, bigdata._strData.c_str()+1) < 0) ) 
		return BigData(INT64(0));

	//左 == 右
	if (strcmp(_strData.c_str()+1, bigdata._strData.c_str()+1) == 0) //左==右
	{
		std::string strRet = "+1";
		if (_strData[0] != bigdata._strData[0])
			strRet[0] = '-';
		else
			return BigData(strRet.c_str());
	}

	//左 > 右
	return BigData(Div(_strData, bigdata._strData).c_str());
}

std::string BigData::Add(std::string left, std::string right)
{
	//让left保存位数大的数
	int iLeftSize = left.size();
	int iRightSize = right.size();
	if (iLeftSize < iRightSize)
	{
		std::swap(left, right);
		std::swap(iLeftSize, iRightSize);
	}

	//相加
	std::string strRet;
	strRet.resize(iLeftSize + 1);
	strRet[0] = left[0]; //符号位
	char Step = 0; //进位
	for (int iIdx = 1; iIdx < iLeftSize; ++iIdx)
	{
		char cRet = left[iLeftSize - iIdx] - '0' + Step;

		if (iIdx < iRightSize)
			cRet += right[iRightSize - iIdx] - '0';

		strRet[iLeftSize - iIdx + 1] = cRet % 10 + '0';
		Step = cRet / 10;
	}

	strRet[1] = Step + '0';
	
	return strRet;
}

std::string BigData::Sub(std::string left, std::string right)
{
	int iLeftSize = left.size();
	int iRightSize = right.size();
	char cSymbol = left[0]; //差的符号位

	if ((iLeftSize < iRightSize) || 
		(iLeftSize == iRightSize && left < right))
	{
		std::swap(left, right);
		std::swap(iLeftSize, iRightSize);

		if (cSymbol == '+')
			cSymbol = '-';
		else
			cSymbol = '+';
	}

	//相减
	std::string strRet; //保存差
	strRet.resize(left.size()); //先设置和左操作数一样大的空间
	strRet[0] = cSymbol; //符号位

	for (int Idx = 1; Idx < iLeftSize; ++Idx)
	{
		char cRet = left[iLeftSize - Idx] - '0';
		
		if (Idx < iRightSize)
			cRet -= (right[iRightSize - Idx] - '0');

		if (cRet < 0) //有借位
		{
			left[iLeftSize - Idx - 1] -= 1;
			cRet += 10;
		}

		strRet[iLeftSize - Idx] = cRet + '0';
	}

	return strRet;
}

std::string BigData::Mul(std::string left, std::string right)
{
	//确定符号位
	char cSymbol = '+';
	if (left[0] != right[0])
		cSymbol = '-';

	//使左操作数位数小于右操作数位数
	int iLeftSize = left.size();
	int iRightSize = right.size();
	if (iLeftSize > iRightSize)
	{
		std::swap(left, right);
		std::swap(iLeftSize, iRightSize);
	}

	std::string strRet;
	//strRet.resize(iLeftSize+iRightSize-1);
	strRet.assign(iLeftSize+iRightSize-1, '0');
	strRet[0] = cSymbol;
	int iRetLen = strRet.size();
	int iOffset = 0; //偏移

	for (int iLeftIndex = 1; iLeftIndex < iLeftSize; ++iLeftIndex)
	{
		char cLeft = left[iLeftSize - iLeftIndex] - '0';
		
		if (cLeft == 0) //当左操作数中有0时，直接左移，不计算
		{
			++iOffset;
			continue;
		}

		char cStep = 0;
		for (int iRightIndex = 1; iRightIndex < iRightSize; ++iRightIndex)
		{
			char cRet = cLeft*(right[iRightSize - iRightIndex] - '0') + cStep;
			cRet += strRet[iRetLen - iRightIndex - iOffset] - '0';
			strRet[iRetLen - iRightIndex - iOffset] = (cRet%10) + '0';
			cStep = cRet/10;
		}
		strRet[iRetLen-iRightSize -iOffset] += cStep;
		++iOffset;
	}

	return strRet;
}

std::string BigData::Div(std::string left, std::string right)
{
	std::string strRet;
	
	//确定符号位
	strRet.append(1, '+');
	if (left[0] != right[0])
		strRet[0] = '-';

	char* pLeft = (char*)(left.c_str()+1);
	char* pRight = (char*)(right.c_str()+1);
	int iLSize = left.size()-1; //被除数长度
	int iDataLen = right.size()-1; //取iDataLen位被除数

	int iIdx = 0; //iIdx和pLeft同步走,用来判断是否走到被除数结尾
	while (iIdx < iLSize)
	{
		if (*pLeft == '0')
		{
			strRet.append(1, '0');
			pLeft++;
			iIdx++;
			continue;
		}

		if (!IsLeftStrBig(pLeft, iDataLen, pRight, right.size()-1)) //取的iDataLen位被除数 < 除数
		{
			strRet.append(1, '0');
			iDataLen++;
			if (iDataLen + iIdx > iLSize)
				break;
		}
		else
		{
			//循环相减
			strRet.append(1, SubLoop(pLeft, iDataLen, pRight, right.size()-1)); 
			while (*pLeft == '0')
			{
				pLeft++;
				iIdx++;
				iDataLen--;
			}

			iDataLen++;
		}
	}

	return strRet;
}
