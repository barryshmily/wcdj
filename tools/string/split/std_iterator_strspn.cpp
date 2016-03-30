#include <cstring>
#include <iterator>
#include <string>

class SplitStrIterator : public std::iterator<std::input_iterator_tag, std::string> {
public:
	SplitStrIterator() : current_(), deliminators_(), tokenLength_() {}

	SplitStrIterator(const char* source, const char* deliminators) : 
		current_(source), deliminators_(deliminators), tokenLength_()
	{
		SplitToken();       // 找出首个 token 的长度
	}

	std::string operator*() const {
		return std::string(current_, tokenLength_);
	}

	SplitStrIterator& operator++() {
		current_ += tokenLength_;
		if (*current_ == '\0')
			current_ = 0;       // 没有更多 token
		else {
			SkipDeliminators(); // 跳过所有分隔符
			SplitToken();       // 找出下一个 token 的长度
		}
		return *this;
	}

	SplitStrIterator operator++(int) {
		SplitStrIterator tmp(*this);
		++*this;
		return tmp;
	}

	bool operator==(const SplitStrIterator& rhs) const {
		return current_ == rhs.current_;
	}

	bool operator!=(const SplitStrIterator& rhs) const {
		return !(*this == rhs);
	}

private:
	void SplitToken() {
		tokenLength_ = std::strcspn(current_, deliminators_);
	}

	void SkipDeliminators() {
		current_ += std::strspn(current_, deliminators_);
	}

	const char* current_;   // 当前 token 的起始位置，0 表示无 token
	const char* deliminators_;
	std::size_t tokenLength_;
};

inline SplitStrIterator SplitStrBegin(const char* source, const char* deliminators) {
	return SplitStrIterator(source, deliminators);
}

inline SplitStrIterator SplitStrEnd() {
	return SplitStrIterator();
}

// 供 C++11 range-for 使用的 wrapper，本身可在 C++98 下编译
class SplitStr {
public:
	SplitStr(const char* source, const char* deliminators) :
		source_(source), deliminators_(deliminators) {}

	SplitStrIterator begin() const { return SplitStrBegin(source_, deliminators_); }
	SplitStrIterator end() const { return SplitStrEnd(); }

private:
	const char* source_;
	const char* deliminators_;
};

// 以下是使用例子

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
	const char test[] = "123;345,abc;,;cdf";
	const char deliminators[] = ",;";

	// 例1: 用迭代器取得每个子字符串
	for (auto itr = SplitStrBegin(test, deliminators); itr != SplitStrEnd(); ++itr)
		cout << *itr << endl;
	cout << endl;

#if 0
	// 例2: C++11 的 range-for
	for (auto s : SplitStr(test, deliminators))
		cout << s << endl;
	cout << endl;
#endif

	// 例3: 储存至 vector
	vector<string> v;
	copy(SplitStrBegin(test, deliminators), SplitStrEnd(), back_inserter(v));

	for (auto itr = v.begin(); itr != v.end(); ++itr)
		cout << *itr << endl;
	cout << endl;

	// 例4: 计算分割后的子字符串数目（无须生成 std::string 及分配内存）
	cout << distance(SplitStrBegin(test, deliminators), SplitStrEnd());

	return 0;
}




