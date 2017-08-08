#include"Info.h"
#include<string>

using namespace std;
//member function for class info
string info::get_BookName() const
{
	return BookName;
}

void info::set_BookName(const string name)
{
	BookName=name;
}

string info::get_writer() const;
{
	return writer;
}

void set_writer(const string w)
{
	writer=w;
}

int get_ISBN() const
{
	return ISBN;
}

void set_ISBN(const int isbn)
{
	ISBN=isbn;
}

bool info::operator==(const info &i2)//return true if the ISBN of two info's are equal
{
	ISBN==i2.get_ISBN()?return true:return false;
}

ostream& operator<<(ostream &os,const info &i)
{
	os
	<<i.get_BookName()<<endl
	<<i.get_writer()<<endl
	<<i.get_ISBN()<<endl;
	
	return os;
}

istream& operator>>(istream &is,info &i)
{
	string bn,w;//BookName,writer
	int isbn;//ISBN
	is>>bn>>w>>isbn;
	i.set_BookName(bn);
	i.set_writer(w);
	i.set_ISBN(isbn);
	return is;
}

//member function for class record
int record::get_borrow_time() const
{
	return borrow_time;
}

void record::set_borrow_time(const int borrow_t)
{
	borrow_time=borrow_t;
}

int reccord::get_user_number() const
{
	return user_number;
}

void reccord::set_user_number(const int user_n)
{
	user_number=user_n;
}

int record::get_ISBN() const
{
	return ISBN;
}

void record::set_ISBN(const int isbn)
{
	ISBN=isbn;
}

bool get_if_return() const
{
	return if_return;
}

void return_the_book()
{
	if_return=true;
}

ostream& operator<<(ostream &os,const record &r)
{
	os
	<<borrow_time<<endl
	<<user_number<<endl
	<<ISBN<<endl;
	if(if_return)
		os<<"already returned"<<endl;
	else
		os<<"still not returned"<<endl;
	return os;
}

istream& operator>>(istream &is,record &r)
{
	int b_t,u_n,isbn;//borrow_time,user_number,ISBN
	string reminder;//"already returned" or "still not returned"
	is>>b_t>>user_number>>ISBN>>reminder;
	r.set_borrow_time(b_t);
	r.set_user_number(u_n);
	r.set_ISBN(isbn);
	if(reminder=="already returned")
		r.return_the_book();
	return is;
}
