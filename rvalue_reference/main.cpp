#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Foo {
	//void foo()& { std::cout << "lvalue" << std::endl; }
	//void foo()&&{ std::cout << "rvalue" << std::endl; }
};

class X{
public:
	int * m_pResourse;
	int f();
	X(){
		m_pResourse = new int(0);
	}
	X(int *p){
		m_pResourse = p;
	}
private:
	X(const X&){ // Uncopyable
	}
	X& operator=(const X&){}
public:
	X(X&& x){
		m_pResourse = x.m_pResourse;
		x.m_pResourse = nullptr;
	}

	X& operator=(X&& x){//move operator
		int *tmp = m_pResourse;
		m_pResourse = x.m_pResourse;
		x.m_pResourse = tmp;
		return *this;
	}
};

template<typename T>
struct RemoveReference
{
	typedef T			Type;
};

template<typename T>
struct RemoveReference<T&>
{
	typedef T			Type;
};

template<typename T>
struct RemoveReference<T&&>
{
	typedef T			Type;
};

template<typename T>
typename RemoveReference<T>::Type&& MoveValue(T&& value)
{
	return reinterpret_cast<typename RemoveReference<T>::Type&&>(value);
}

int main(){
	auto a = X(new int(1));
	auto b = X(new int(2));
	auto d = reinterpret_cast<int&&>(*new int);
	a = MoveValue(b);
	X c(MoveValue(b));

	//Foo foo;
	//foo.foo();
	//Foo().foo();

	std::string str = "Hello";
	std::vector<std::string> v;

	v.push_back(str);
	std::cout << "After copy, str is \"" << str << "\"\n";

	v.push_back(MoveValue(str));

	std::cout << "After move, str is \"" << str << "\"\n";

	v.push_back(std::string(str));
	std::cout << "The contents of the vector are \"" << v[0]
		<< "\", \"" << v[1] << "\"\n";

	int num = 10;

	std::vector<int> intv;

	intv.push_back(num);

	intv.push_back(MoveValue(num));

	int num2 = 5;
	num2 = MoveValue(num);
	return 0;
}