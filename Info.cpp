#include<iostream>
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

string info::get_writer() const
{
	return writer;
}

void info::set_writer(const string w)
{
	writer=w;
}

int info::get_ISBN() const
{
	return ISBN;
}

void info::set_ISBN(const int isbn)
{
	ISBN=isbn;
}

bool info::operator==(const info &i2)//return true if the ISBN of two info's are equal
{
	if(ISBN==i2.get_ISBN())
		return true;
	else
		return false;
}

ostream& operator<<(ostream &os,const info &i)
{
	os<<i.get_BookName();
	os<<std::endl;
	os<<i.get_writer();
	os<<std::endl;
	os<<i.get_ISBN();
	os<<std::endl;
	
	return os;
}

istream& operator>>(istream &is,info &i)
{
	string bn,w;//BookName,writer
	int isbn;//ISBN
	is>>bn;
	is>>w;
	is>>isbn;
	i.set_BookName(bn);
	i.set_writer(w);
	i.set_ISBN(isbn);
	return is;
}

//member function for class record
string record::get_borrow_time() const
{
	return borrow_time;
}

void record::set_borrow_time(const string borrow_t)
{
	borrow_time=borrow_t;
}

int record::get_user_number() const
{
	return user_number;
}

void record::set_user_number(const int user_n)
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

bool record::get_if_return() const
{
	return if_return;
}

void record::return_the_book()
{
	if_return=true;
}

ostream& operator<<(ostream &os,const record &r)
{
	std::string s1="already returned",s2="still not returned";
	os<<r.get_borrow_time();
	os<<std::endl;
	os<<r.get_user_number();
	os<<std::endl;
	os<<r.get_ISBN();
	os<<std::endl;
	if(r.get_if_return())	
		os<<s1<<std::endl;
	else
		os<<s2<<std::endl;
	return os;
}

istream& operator>>(istream &is,record &r)
{
	string b_t;
	int u_n,isbn;//borrow_time,user_number,ISBN
	string reminder;//"already returned" or "still not returned"
	is>>b_t;
	is>>u_n;
	is>>isbn;
	is>>reminder;
	r.set_borrow_time(b_t);
	r.set_user_number(u_n);
	r.set_ISBN(isbn);
	if(reminder=="already returned")
		r.return_the_book();
	return is;
}
