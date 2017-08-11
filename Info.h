#ifndef INFO_H
#define INFO_H

#include<string>

class info
{
private:
	std::string BookName;
	std::string writer;
	int ISBN;
public:
	info(std::string bn,std::string w,int isbn)
	{
		BookName=bn;
		writer=w;
		ISBN=isbn;
	}
	info()
	{
		BookName="暂无";
		writer="暂无";
		ISBN=0;
	}

	std::string get_BookName() const;
	void set_BookName(const std::string name);
	std::string get_writer() const;
	void set_writer(const std::string w);
	int get_ISBN() const;
	void set_ISBN(const int isbn);
	
	bool operator==(const info &);//ensure the onlyness of the ISBN;
	friend std::ostream& operator<<(std::ostream &,const info &);
	friend std::istream& operator>>(std::istream &,info &);
};

class record
{
private:
	std::string borrow_time;
	int user_number;
	int ISBN;
	bool if_return;
public:
	record(std::string borrow_t,int user_n,int isbn)
	{
		borrow_time=borrow_t;
		user_number=user_n;
		ISBN=isbn;
		if_return=false;
	}
	record()
	{
		borrow_time="unknown";
		user_number=0;
		ISBN=0;
		if_return=false;
	}
	
	std::string get_borrow_time() const;
	void set_borrow_time(const std::string borrow_t);
	int get_user_number() const;
	void set_user_number(const int user_n);
	int get_ISBN() const;
	void set_ISBN(const int isbn);
	bool get_if_return() const;
	void return_the_book();//set the if_return as true;
	
	friend std::ostream& operator<<(std::ostream &,const record &);
	friend std::istream& operator>>(std::istream &,record &);
};
#endif
