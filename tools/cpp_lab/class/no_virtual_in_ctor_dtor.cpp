#include <stdio.h>
#include <iostream>
#include <string>

class Transaction { // base class
public:
	explicit Transaction(const std::string& logInfo);

	// 做出一份因类型不同而不同的日志记录
	void logTransaction(const std::string& logInfo) const {
		std::cout << "Transaction::logTransaction(): " << logInfo << "\n";
	}
};

Transaction::Transaction(const std::string& logInfo)
{
	std::cout << "Transaction()\n";

	// 最后动作是记录日志
	logTransaction(logInfo);
}

class BuyTransaction: public Transaction { // derived class
public:
	BuyTransaction(const std::string &paras)
		: Transaction(createLogString(paras)) // 将log信息传给base class构造函数
	{}
private:
	static std::string createLogString(const std::string& paras);

};

std::string BuyTransaction::createLogString(const std::string& paras)
{
	if (paras == "1") return "1+";
	else if (paras == "2") return "2+";
	else return "+";
}

int main()
{
	BuyTransaction bt1("1");
	BuyTransaction bt2("2");
}
/*
g++ -o no_virtual_in_ctor_dtor no_virtual_in_ctor_dtor.cpp
./no_virtual_in_ctor_dtor
Transaction()
Transaction::logTransaction(): 1+
Transaction()
Transaction::logTransaction(): 2+ 
 */
