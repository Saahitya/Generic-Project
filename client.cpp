#include "splay.h"
#include <functional>

using namespace std;

template<typename T>
struct Less
{
	bool operator()(const T& x, const T& y)
	{
		return x > y;
	}
};

struct MyDate
{
	public:
	MyDate():dd_(0),mm_(0), yy_(0) {} 
	MyDate(int dd, int mm, int yy) : dd_(dd), mm_(mm), yy_(yy) { }
	friend ostream& operator<<(ostream&, const MyDate&);
	friend bool operator<(const MyDate& lhs, const MyDate& rhs);
	friend bool compare_date(const MyDate& lhs, const MyDate& rhs);
	private:
	int dd_;
	int mm_;
	int yy_;
	
};
ostream& operator<<(ostream& o, const MyDate& d)
{
	return o << d.dd_ << "-" << d.mm_ << "-" << d.yy_; // << "\n";
}


bool compare_date(const MyDate& lhs, const MyDate& rhs)
{
	return lhs.dd_ < rhs.dd_;
}

struct MyDatePred
{
	bool operator()(const MyDate& x, const MyDate& y) { return compare_date(x, y); }
};

int main() {

//bidirectional iterators
//traversing this through iterators normaly and reverse
#if 0
	splay_tree<int, greater<int>> st1;
	st1.insert(10);  st1.insert(40);  st1.insert(50); st1.insert(30);  st1.insert(20);  
    auto it = st1.begin();
    while(it != st1.end())
    {
        cout << *it << "\t";
        ++it;
    }
    cout << endl;

    auto jt = st1.rbegin();
    while(jt != st1.rend())
    {
        cout << *jt << "\t";
        --jt;
    }
    cout << endl;

#endif

//insert and find and erase methods
//move constructor and normal constructor
#if 0
	splay_tree<int, greater<int>> st1;
    st1.insert(10);  st1.insert(40);  st1.insert(50); st1.insert(30);  st1.insert(20);  st1.insert(100);
	splay_tree<int, greater<int>> st2(st1);

    st1.erase(100);

	cout << boolalpha;
	cout << st1.find(10).second << endl;
	cout << st1.find(100).second << endl;
    cout << st2.find(10).second << endl;
	cout << st2.find(100).second << endl;

#endif

//move assignment and normal assignment
#if 0
	splay_tree<int, greater<int>> st1;
	splay_tree<int, greater<int>> st2;
    splay_tree<int, greater<int>> st3;
	st1.insert(10);  st1.insert(40);  st1.insert(50); st1.insert(30);  st1.insert(20);

    st2 = st1;
	cout << boolalpha;
	cout << st1.find(10).second << endl;
	cout << st1.find(100).second << endl;
    cout << st2.find(10).second << endl;
	cout << st2.find(100).second << endl;

    st3 = move(st1);
	cout << boolalpha;
	cout << st1.find(10).second << endl;
	cout << st1.find(100).second << endl;
    cout << st3.find(10).second << endl;
	cout << st3.find(100).second << endl;

#endif

//comparison operators
#if 0
	splay_tree<int> st1;
	splay_tree<int> st2;
	st1.insert(10);  st1.insert(40);  st1.insert(50); st1.insert(30);  st1.insert(20);
    st2.insert(10); st2.insert(30);

    cout << boolalpha;
    cout << "== " << (st1 == st2) << endl;
    cout << "!= " << (st1 != st2) << endl;
    cout << "< " << (st1 < st2) << endl;
    cout << "<= " << (st1 <= st2) << endl;
    cout << "> " << (st1 > st2) << endl;
    cout << ">= " << (st1 >= st2) << endl;
#endif

//with different datatype and User defined functor with inserting repeated elements 
#if 0

	MyDate d[] = {
		{26, 12, 2004},
		{11, 9, 2001},	
		{11, 1, 1966},
		{26, 12, 2004},
		{11, 9, 2001},	
		{26, 1, 2001},
		{30, 1, 1948},
		{11, 9, 2001},	
		{26, 12, 2004} 
	};

	splay_tree<MyDate, MyDatePred> s4;
	for(auto it : d)
		s4.insert(it);

	cout << s4;

#endif
}
