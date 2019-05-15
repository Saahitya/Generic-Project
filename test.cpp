#include "splay.h"
#include <functional>

using namespace std;

//template<typename T>
/*struct Less
{
	bool(const &T x, const &T y)
};*/

int main() {
	splay_tree<int, greater<int>> st;
	splay_tree<int, greater<int>> cp;
	st.insert(10);
	st.insert(40);
	st.insert(100);
	st.insert(30);
	st.insert(20);

	cp.insert(10);
	cp.insert(40);
	cp.insert(100);
	cp.insert(30);
	//cp.insert(20);
		bool q = st == cp;
cout << "Q: " << q <<endl;
cout << st;
	/*cout <<"DISPLAYING";
	for(auto it = st.begin(); it!=st.end(); ++it)
		cout << *it << " ";
	for(auto it = st.rbegin(); it!=st.rend(); --it)
		cout << *it << " ";
		cout << "DISPLAY";// << *st.begin() << " "<<*st.end();*/
	auto p = st.begin();
	p++;
	cout << *p;
	p++;
	cout << *p;
	p++;
	cout << *p;
	p++;
	cout << *p;
	p++;
	cout << *p;

	if(p == st.end())
		cout <<"END";
	cout << endl << endl;
	cout << "size of st is " << st.size() << endl;
	//for(auto it = st.begin(); it!=st.end(); ++it)
	//	cout << *it << " ";
	auto f10 = st.find(10);
	auto f20 = st.find(20);
	auto f30 = st.find(40);
		auto f40 = st.find(30);
	auto f50 = st.find(100);
	cout <<"Hallelujah "<<  *(f10.first) << endl;
	cout <<"Hallelujah "<<  *(f20.first) << endl;
	cout <<"Hallelujah "<<  *(f30.first) << endl;
	cout <<"Hallelujah "<<  *(f40.first) << endl;
	cout <<"Hallelujah "<<  *(f50.first) << endl;

	cout <<"Hallelujah "<<  (f10.second) << endl;
	cout <<"Hallelujah "<<  (f20.second) << endl;
	cout <<"Hallelujah "<<  (f30.second) << endl;

	st.erase(10);
	f10 = st.find(10);
	cout <<"Hallelujah "<<  *(f10.first) << endl;
	cout <<"Hallelujah "<<  (f10.second) << endl;
	// cout <<"Hallelujah "<<  *(st.find(100).first) << endl;
	// st.erase(100);
	// cout <<"Hallelujah "<<  *(st.find(100).first) << endl;
	//if(st.begin() == st.end())	cout << "Hello" << endl;	//this is clearly wrong
	// auto it = st.begin();
	// ++it;
	// cout << *it << endl;
}
