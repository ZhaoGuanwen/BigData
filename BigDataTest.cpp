#include <iostream>
using namespace std;
#include "BigData.h"

void AddTest()
{
	//BigData b1("12345678");
	//BigData b2("12345qwe");
	//BigData b3("+");
	//BigData b4("00001234");
	//BigData b5("   ");
	//BigData b6("99999999999999999999999999999999999999999999999999999999");
	//BigData b7 ("-123213");

	//cout<<b1<<endl;
	//cout<<b2<<endl;
	//cout<<b3<<endl;
	//cout<<b4<<endl;
	//cout<<b5<<endl;
	//cout<<b6<<endl;
	//cout<<b7<<endl;

	//BigData left1(1234);
	//BigData right1(4321);
	//cout<<left1+right1<<endl;

	//BigData left2(9223372036854775807);
	//BigData right2(3);
	//cout<<left2+right2<<endl;

	//BigData left3(-9223372036854775808);
	//BigData right3(-3);
	//cout<<left3+right3<<endl;

	//BigData left3("99999999999999999999999999999999999");
	//BigData right3("11111111111111111111111111111111111");
	//cout<<left3+right3<<endl;
}

void SubTest()
{
	//BigData left("1111111111111111111111111111111100");
	//BigData right("99");
	//cout<<left-right<<endl;

	//BigData left1("-2222222222222222222222222222222222222222");
	//BigData right1("22222");
	//cout<<left1+right1<<endl;
}

void MulTest()
{
	//BigData left("77777777777777777777777777777777777777777777777777777777777");
	//BigData right("66");
	//cout<<left*right<<endl;

	//BigData left1("99");
	//BigData right1("9999999999999999999999999999999999999999999");
	//cout<<left1*right1<<endl;


	//BigData left2("11111111111111111111111111111111111111111111111111111111111111111");
	//BigData right2("-99");
	//cout<<left2*right2<<endl;

	//BigData left3("-8");
	//BigData right3("66");
	//cout<<left3*right3<<endl;

	//BigData left4("10001");
	//BigData right4("666666666666666666666666666666666666");
	//cout<<left4*right4<<endl;
}

void DivTest()
{
	//BigData left("222222222222222222222222222222222222222222222222");
	//BigData right("33");
	//cout<<left/right<<endl;

	//BigData left1("-222222222222222222222222222222222222222222222222");
	//BigData right1("33");
	//cout<<left1/right1<<endl;

	//BigData left3("33333333333333333333333333333333333333333333333333333");
	//BigData right3("0");
	//cout<<left3/right3<<endl;

	//BigData left4("111");
	//BigData right4("222222222222222222222222222222222222222222222222");
	//cout<<left4/right4<<endl;

	//BigData left5("11111111111111111111111111111111111111111111111111");
	//BigData right5("-11111111111111111111111111111111111111111111111111");
	//cout<<left5/right5<<endl;

	BigData left5("5555555555555555555555555555555555555555555555");
	BigData right5("-5");
	cout<<left5/right5<<endl;
}

void PrintMenu()
{
	cout<<"------------------------------------------------------------------------"<<endl;
	cout<<"|                            大 数 计 算 器                            |"<<endl;
	cout<<"------------------------------------------------------------------------"<<endl;
	cout<<"请输入：";
}

void Demo()
{
	while (true)
	{
		PrintMenu();
		string input;
		cin>>input;

		if (input == "quit")
		{
			break;
		}

		if (input.find('+') != -1)
		{
			string left = input.substr(0, input.find('+'));
			string right = input.substr(input.find('+')+1, input.length()-1);
			BigData bigdata1(left.c_str());
			BigData bigdata2(right.c_str());
			cout<<"结果：  "<<bigdata1+bigdata2<<endl;
			continue;
		}

		if (input.find('-') != -1)
		{
			string left = input.substr(0, input.find('-'));
			string right = input.substr(input.find('-')+1, input.length()-1);
			BigData bigdata1(left.c_str());
			BigData bigdata2(right.c_str());
			cout<<"结果：  "<<bigdata1-bigdata2<<endl;
			continue;
		}

		if (input.find('*') != -1)
		{
			string left = input.substr(0, input.find('*'));
			string right = input.substr(input.find('*')+1, input.length()-1);
			BigData bigdata1(left.c_str());
			BigData bigdata2(right.c_str());
			cout<<"结果：  "<<bigdata1*bigdata2<<endl;
			continue;
		}

		if (input.find('/') != -1)
		{
			string left = input.substr(0, input.find('/'));
			string right = input.substr(input.find('/')+1, input.length()-1);
			BigData bigdata1(left.c_str());
			BigData bigdata2(right.c_str());
			cout<<"结果：  "<<bigdata1/bigdata2<<endl;
			continue;
		}

	}
}

int main()
{
	//AddTest();
	//SubTest();
	//MulTest();
	DivTest();
	//Demo();

	return 0;
}
