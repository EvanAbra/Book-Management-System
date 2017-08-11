#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"time.h"
#include"radix62.h"
#include"Info.h"
#include"user.h"
using namespace std; 

const radix62 encrypt_code("bauxhemsnk");
vector<user> User;
vector<info> Book;
vector<record> Record;

void sysdba(user*);//以系统管理员身份进入后的界面
void bookdba(user*);//以图书管理员身份进入后的界面
void reader(user*);//以读者身份进入后的界面
void query_book(const user*);//查询书籍界面
void query_record(const user*);//查询借书记录界面
void return_book(const user*);//还书界面

//以下三个函数均不执行exit指令以及help指令，exit指令所对应的退出当前账号的登录状态的功能由所对应的用户界面函数实现,bool型变量find又来提示是否在该函数寻找到匹配到合适的指令
void reader_instructions(const string command,user* u,bool &find);//读者身份所对应的相关的指令判断与执行模块
void bookdba_instructions(const string command,user* u,bool &find);//图书管理员相关指令的判断及执行模块
void sysdba_instructions(const string command,user* u,bool &find);//系统管理员相关指令的判断与执行模块

void help_main();//打印主界面的帮助信息
void help_sysdba();//以系统管理员身份登录后的帮助信息
void help_bookdba();//以图书管理员身份登陆后的帮助信息
void help_reader();//以普通读者身份登陆后的帮助信息
void help_query_book();//打印书籍查询界面的帮助信息
void help_query_record();//打印借书记录查询界面的帮助信息

void load();//用文件中的数据对三个vector进行刷新
void save();//将当前做出的更改保存到文件中

radix62 encryption(const radix62 &r);
radix62 decryption(const radix62 &r);

void add_Sys_dba();//当不存在系统管理员时调用此函数重新创建系统管理员，初始密码为sysdba1234
void add_Book_dba(const string &name,const int &number,const radix62 &keyword);//添加图书管理员,添加时对number合法性进行检查
void add_reader(const string &name,const int &number,const radix62 &keyword);//添加读者，添加时对number合法性进行检查
void delete_user(const int user_number);
void display_all_user();//展示所有用户信息

void add_book(const string &BookName,const string &writer,const int &ISBN);
void delete_book(const int &ISBN);
void display_all_book();//展示所有书籍信息
void find_book_with_bookname(const string &BookName);//打印书名为BookName的书的所有信息
void find_book_with_writername(const string &writer);//打印作者名为writer的书的所有信息

int main()
{
	//从文件读入所有用户的账号密码及对应权限
	//读入用户的借书记录
	//读入书籍相关信息
	load();
	//检查现在有无系统管理员，若没有的话创建默认系统管理员
	bool flag=false;
	for(user item:User)
		if(item.get_authority()==SystemAdministrator)
		{
			flag=true;
			break;
		}
	if(!flag)//当前程序中没有系统管理员的信息
		add_Sys_dba();
	string command;
	while(cin>>command)
	{
		if(command=="help")
			help_main();
		else if(command=="save")
			save();
		else if(command=="sign_in")
		{
			cout<<"Enter your user number."<<endl;
			int user_number;
			user* current_user;
			cin>>user_number;
			bool exist_flag=false;
			for(user &item:User)
				if(user_number==item.get_user_number())
				{
					current_user=&item;
					exist_flag=true;
					break;
				}
			if(!exist_flag)
			{
				cout<<"User number doesn't exist!"<<endl;
				continue;//跳过剩余所有，进入下一次while循环
			}
			cout<<"Enter your password."<<endl;
			string key;
			cin>>key;
			radix62 password(key);
			if(encryption(password)==current_user->get_keyword())
				cout<<"Successfully log in!"<<endl;
			else
			{
				cout<<"Wrong password!"<<endl;
				continue;
			}
			if(current_user->get_authority()==SystemAdministrator)
				sysdba(current_user);//转入系统管理员界面
			else if(current_user->get_authority()==BookAdministrator)
				bookdba(current_user);//转入图书管理员界面
			else if(current_user->get_authority()==Reader)
				reader(current_user);//转入读者界面
		}
	}
	return 0;
}

void query_record(const user* u)
{
	bool find=false;
	for(record r:Record)
		if(r.get_user_number()==u->get_user_number()&&r.get_if_return()==true)
		{
			find=true;
			cout<<r;
		}
	for(record r:Record)
		if(r.get_user_number()==u->get_user_number()&&r.get_if_return()==false)
		{
			find=true;
			cout<<r;
		}
	if(!find)
	cout<<"You don't have any borrow-records."<<endl;
}

void return_book(const user* u)
{
	cout<<"Enter the ISBN."<<endl;
	int ISBN;
	bool find=false;
	for(record &r:Record)
		if(r.get_ISBN()==ISBN&&r.get_if_return()==false)
		{
			find=true;
			r.return_the_book();
			cout<<"Successfully return the book!"<<endl;
			break;
		}
	if(!find)
		cout<<"You didn't borrow this book."<<endl;
}

void help_main()
{
	cout<<"instructions:\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\t\t\t\t"<<"functions:"<<endl;
	cout<<"save\b\b\b\b"<<"\t\t\t\t"<<"Save the data to the file."<<endl;
	cout<<"sign_in\b\b\b\b\b"<<"\b\b\t\t\t\t"<<"Use an existed account to sign in."<<endl;
	/*
	cout<<"add_account\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\t\t\t\t"<<"Add a new user.(Only System Administrator has the authority)"<<endl;
	cout<<"add_book\b\b\b\b\b"<<"\b\b\b\t\t\t\t"<<"Add a new Book.(Reader can't do this)"<<endl;
	cout<<"query_record\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t\t"<<"转入查询界面"<<endl;
	*/
}

void reader_instructions(string command,user* u,bool &find)
{
	if(command=="save")
	{
		save();
		find=true;
	}
	else if(command=="change_the_password")		
	{
		find=true;
		cout<<"Enter your current password. It can only contain the English letters and the numbers\t\t";
		string password;
		cin>>password;
		radix62 key(password);
		if(encryption(key)==u->get_keyword())//密码经验证相等
		{
			cout<<"Enter your new password.\t\t";
			string password1;
			string password2;
			cin>>password1;
			cout<<"Enter the password again.\t\t";
			cin>>password2;
			if(password1!=password2)
			{
				cout<<"You entered two different passwords!"<<endl;
				return;
			}
			//检验密码中有无非法字符
			bool flag=false;
			for(char item:password1)
			{
				bool flag2=false;
				if((int)item>=(int)'0'&&(int)item<=(int)'9')
					flag2=true;
				else if((int)item>='a'&&(int)item<=(int)'z')
					flag2=true;
				else if((int)item>='A'&&(int)item<=(int)'Z')
					flag2=true;
				if(!flag2)
				{
					flag=true;
					break;
				}
			}
			if(!flag)
			{
				cout<<"Illegal password! Password can only contain English letters and the numbers."<<endl;
				return;
			}
			else
			{
				radix62 new_key(password1);
				u->set_keyword(encryption(new_key));
				cout<<"Successfully changed the password!"<<endl;
			}
		}//end of if(encryption(key)==u.get_keyword())//密码经验证相等
	}//end of else if(command=="change_the_password")
	else if(command=="change_the_name")
	{
		find=true;
		cout<<"Enter your new name.\t\t\t\t";
		string name;
		cin>>name;
		u->set_user_name(name);
		cout<<"Successfully changed your name!"<<endl;
	}
	else if(command=="query_book")
	{	
		find=true;
		query_book(u);
	}
	else if(command=="query_record")
	{
		find=true;
		query_record(u);
	}
	else if(command=="return_book")
	{
		find=true;
		return_book(u);
	}
}

void reader(user* u)
{
	cout<<"Welcome, "<<u->get_user_name()<<endl;
	cout<<"Your user number is "<<u->get_user_name()
	<<", and your authority is Reader."<<endl;
	cout<<"For more information, please enter \"help\""<<endl;
	string command;
	while(cin>>command)
	{
		bool if_find=false;
		reader_instructions(command,u,if_find);
		if(if_find)
			continue;
		else if(command=="help")
		{
			if_find=true;
			help_reader();
		}
		else if(command=="exit")
		{
			if_find=true;
			save();
			return;
		}
		if(!if_find)
			cout<<"Illegal instructions! Enter \"help\" for available instructions."<<endl;
	}
}

void help_reader()
{
	cout<<"instructions:\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\t\t\t\t"<<"functions:"<<endl;
	cout<<"save\b\b\b\b"<<"\t\t\t\t"<<"Save the data to the file."<<endl;
	cout<<"change_the_password"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\t\t\t\t"
	<<"Modify your password."<<endl;
	cout<<"change_the_name"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t\t"
	<<"Modify your user_name."<<endl;
	cout<<"query_book"<<"\b\b\b\b\b"<<"\b\b\b\b\b\t\t\t\t"
	<<"Find a book by its writer, ISBN or name"<<endl;
	cout<<"query_record"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t\t"
	<<"Display your borrow records."<<endl;
	cout<<"return_book"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\t\t\t\t"
	<<"Return books."<<endl;
	cout<<"exit"<<"\b\b\b\b\t\t\t\t"<<"Sign out.(Automatically save changes)"<<endl;
}

void bookdba_instructions(const string command,user* u,bool &find)
{
	reader_instructions(command,u,find);
	if(find==true)
		return;
	else if(command=="add_book")
	{
		find=true;
		cout<<"Please enter the book's name, the writer's name and the ISBN in order."<<endl;
		string BookName;
		int ISBN;
		string writer;
		cin>>BookName>>writer>>ISBN;
		add_book(BookName,writer,ISBN);
	}
	else if(command=="delete_book")
	{
		find=true;
		cout<<"Please enter the book's ISBN"<<endl;;
		int ISBN;
		cin>>ISBN;
		delete_book(ISBN);
	}

}

void bookdba(user* u)
{
	cout<<"Welcome, "<<u->get_user_name()<<endl;
	cout<<"Your user number is "<<u->get_user_name()
	<<", and your authority is Book Administrator."<<endl;
	cout<<"For more information, please enter \"help\""<<endl;
	string command;
	while(cin>>command)
	{
		bool if_find=false;
		bookdba_instructions(command,u,if_find);
		if(if_find)
			continue;
		else if(command=="help")
		{
			if_find=true;
			help_bookdba();
		}
		else if(command=="exit")
		{
			if_find=true;
			save();
			return;
		}
		if(!if_find)
			cout<<"Illegal instructions! Enter \"help\" for available instructions.\n";
	}

}

void help_bookdba()
{
	help_reader();
	cout<<"add_book"<<"\b\b\b\b\b"<<"\b\b\b\t\t\t\t"
	<<"Add a book into the database."<<endl;
	cout<<"delete_book"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\t\t\t\t"
	<<"Delete a book from the database."<<endl;
}

void sysdba_instructions(const string command,user* u,bool &find)
{
	bookdba_instructions(command,u,find);
	if(find==true)
		return;
	else if(command=="add_bookdba")
	{
		find=true;
		cout<<"Please enter the user_name, user_number and the password in order."<<endl;
		string user_name;
		int user_number;
		string key;
		cin>>user_name>>user_number>>key;
		radix62 password(key);
		add_Book_dba(user_name,user_number,key);
	}
	else if(command=="add_reader")
	{
		find=true;
		cout<<"Please enter the user_name, user_number and the password in order."<<endl;
		string user_name;
		int user_number;
		string key;
		cin>>user_name>>user_number>>key;
		radix62 password(key);
		add_reader(user_name,user_number,key);
	}

}

void sysdba(user* u)
{
	cout<<"Welcome, "<<u->get_user_name()<<endl;
	cout<<"Your user number is "<<u->get_user_name()
	<<", and your authority is System Administrator."<<endl;
	cout<<"For more information, please enter \"help\""<<endl;
	string command;
	while(cin>>command)
	{
		bool if_find=false;
		sysdba_instructions(command,u,if_find);
		if(if_find)
			continue;
		else if(command=="help")
		{
			if_find=true;
			help_sysdba();
		}
		else if(command=="exit")
		{
			if_find=true;
			save();
			return;
		}
		if(!if_find)
			cout<<"Illegal instructions! Enter \"help\" for available instructions.\n";
	}
}

void help_sysdba()
{
	help_bookdba();
	cout<<"add_bookdba"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\t\t\t\t"
	<<"Add a new user whose authority is Book Administrator."<<endl;
	cout<<"add_reader"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t\t"
	<<"Add a new user whose authority is Reader."<<endl;
	cout<<"delete_user"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\t\t\t\t"
	<<"Delete a user."<<endl;
}

void query_book(const user* u)
{
	cout<<"Welcome, this is the frame for book-query. For more information, please enter \"help\""<<endl;
	string command;
	while(cin>>command)
	{
		if(command=="help")
			help_query_book();
		else if(command=="query_with_writer")
		{
			cout<<"Please enter the writer's name\t\t\t";
			string writer;
			cin>>writer;
			find_book_with_writername(writer);
		}
		else if(command=="query_with_bookname")
		{
			cout<<"Please enter the book's name\t\t\t";
			string bookname;
			cin>>bookname;
			find_book_with_bookname(bookname);
		}
		else if(command=="borrow")
		{
			cout<<"Please enter the book's ISBN\t\t\t";
			int ISBN;
			cin>>ISBN;
			bool find=false;
			for(auto item:Book)
				if(ISBN==item.get_ISBN())
				{
					find=true;
					time_t now_time;
					time(&now_time);
					string time=ctime(&now_time);
					record r(time,u->get_user_number(),ISBN);
					continue;
				}
			if(!find)
				cout<<"Wrong ISBN!"<<endl;
		}
		else if(command=="display")
			display_all_book();
		else if(command=="exit")
		{
			save();
			return;
		}
		else
			cout<<"Illegal instruction! For more information, please enter \"help\""<<endl;
	}
}
void help_query_book()
{
	cout<<"instructions:\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\t\t\t\t"<<"functions:"<<endl;
	cout<<"query_with_writer"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t\t"
	<<"Find some book with writer's name."<<endl;
	cout<<"query_with_bookname"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\b\b\t\t\t\t"
	<<"Find some book with book's name."<<endl;
	cout<<"borrow"<<"\b\b\b\b\b\b\t\t\t\t"
	<<"Borrow a book.(You should know its ISBN.)"<<endl;
	cout<<"display"<<"\b\b\b\b\b"<<"\b\b\t\t\t\t"
	<<"Print all books' information on the screen."<<endl;
	cout<<"exit"<<"\b\b\b\b\t\t\t\t"
	<<"Quit the query_book's frame."<<endl;
}
void load()
{
	ifstream in;
	in.open("Program_data.txt");
	int counter;
	//读取User表的数据
	in>>counter;
	for(int i=0;i<counter;i++)
	{
		string u_name;
		int u_number;
		string password;
		string sau;//"SystemAdministrator", "BookAdministrator" or "Reader"
		authority_type au;
		in>>u_name>>u_number>>password>>sau;
	
		//对账号进行检查
		if(u_number<=0)
			continue;
		bool flag=false;
		for(user item:User)
			if(u_number==item.get_user_number())
			{
				flag=true;
				break;
			}	
		if(flag)
			continue;//账号已存在，跳过该账号

		radix62 key(password);
		if(sau=="SystemAdministrator")
			au=SystemAdministrator;
		else if(sau=="BookAdministrator")
			au=BookAdministrator;
		else
			au=Reader;
		user u(u_name,u_number,key,au);
		User.push_back(u);
	}

	//读取Book表的数据
	in>>counter;
	for(int i=0;i<counter;i++)
	{
		info information;
		in>>information;
		Book.push_back(information);
	}

	//读取Record表的数据
	in>>counter;
	for(int i=0;i<counter;i++)
	{
		record Re;
		in>>Re;
		Record.push_back(Re);
	}

	in.close();
	cout<<"Successfully load the file_data!"<<endl;
}

void save()
{
	ofstream out;
	out.open("Program_data.txt");
	//将当前的User表写入
	out<<User.size()<<endl;//写入User表中的成员数量
	for(user item:User)
		out<<item<<endl;
	//写入Book表
	out<<Book.size()<<endl;
	for(info item:Book)
		out<<item<<endl;
	//写入Record表
	out<<Record.size()<<endl;
	for(record item:Record)
		out<<item<<endl;
	out.close();
	cout<<"Successfully save the change!"<<endl;
}

radix62 encryption(const radix62 &r)
{
	radix62 r2=r+encrypt_code;
	return r2;
}

radix62 decryption(const radix62 &r)
{
	radix62 r2=r-encrypt_code;
	return r2;
}

void add_Sys_dba()
{
	radix62 default_keyword("sysdba1234");
	radix62 encrypted_keyword=encryption(default_keyword);
	user Sys_dba("sys_dba",1234,encrypted_keyword,SystemAdministrator);
	User.push_back(Sys_dba);
	cout<<"Successfully create the System Administrator!"<<endl;
	cout<<"name: "<<Sys_dba.get_user_name()<<endl
	<<"number: "<<Sys_dba.get_user_number()<<endl
	<<"password: "<<decryption(Sys_dba.get_keyword())<<endl
	<<"authority: "<<Sys_dba.get_authority()<<endl;
	cout<<"Enter \"help\"for more information."<<endl;
}

void add_Book_dba(const string &name,const int &number,const radix62 &keyword)
{
	//对number范围进行检查
	if(number<=0)
	{
		cout<<"Failed to add the Book_dba."<<endl;
		cout<<"Illegal user_number, user_number is out of range."<<endl;
		return;
	}
	//对number的唯一性进行检验
	bool number_exist=false;
	for(auto item:User)
	{
		if(number==item.get_user_number())
		{
			number_exist=true;
			break;
		}
	}
	if(number_exist)
	{
		cout<<"Failed to add the Book_dba."<<endl
		<<"This user number has already used by others."<<endl;
		return;
	}
	//添加图书管理员
	user Book_dba(name,number,encryption(keyword),BookAdministrator);
	User.push_back(Book_dba);
	cout<<"Successfully add a Book Administrator!"<<endl;
}

void add_reader(const string &name,const int &number,const radix62 &keyword)
{
	//对number范围进行检查
	if(number<=0)
	{
		cout<<"Failed to add a Reader."<<endl;
		cout<<"Illegal user_number, user_number is out of range."<<endl;
		return;
	}
	//对number的唯一性进行检验
	bool number_exist=false;
	for(auto item:User)
	{
		if(number==item.get_user_number())
		{
			number_exist=true;
			break;
		}
	}
	if(number_exist)
	{
		cout<<"Failed to add a Reader."<<endl
		<<"This user number has already used by others."<<endl;
		return;
	}
	//添加读者
	user reader(name,number,encryption(keyword),Reader);
	User.push_back(reader);
	cout<<"Successfully add a reader!"<<endl;
}

void display_all_user()
{
	cout<<"User Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t"
	<<"User Number:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t"
	<<"Authority:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<endl;
	for(user item:User)
	{
		cout<<item.get_user_name();
		for(int i=item.get_user_name().size();i>0;i--)
			cout<<'\b';
		cout<<"\t\t\t";
		cout<<item.get_user_number();
		int num=item.get_user_number();
		int digit=1;
		while(digit*10<num)
			digit++;
		for(int i=digit;i>0;i--)
			cout<<'\b';
		cout<<"\t\t\t";
		switch(item.get_authority())
		{
		case 0:
			cout<<"System Administrator"<<endl;
			break;
		case 1:
			cout<<"Book Administrator"<<endl;
			break;
		case 2:
			cout<<"Reader"<<endl;
			break;
		default:
			cout<<"Unknown"<<endl;
			break;
		}
	}
}

void add_book(const string &BookName,const string &writer,const int &ISBN)
{
	//对ISBN范围进行检查
	if(ISBN<=0)
	{
		cout<<"Failed to add a book."<<endl;
		cout<<"Illegal ISBN, ISBN is out of range."<<endl;
		return;
	}
	//对ISBN的唯一性进行检验
	bool isbn_exist=false;
	for(auto item:Book)
	{
		if(ISBN==item.get_ISBN())
		{
			isbn_exist=true;
			break;
		}
	}
	if(isbn_exist)
	{
		cout<<"Failed to add a book."<<endl
		<<"The ISBN should be only, the book with this ISBN already exists."<<endl;
		return;
	}
	//添加书籍
	info book(BookName,writer,ISBN);
	Book.push_back(book);
	cout<<"Successfully add the book!"<<endl;
}

void delete_book(const int &ISBN)
{
	for(auto current_position=Book.begin();current_position<Book.end();current_position++)
		if(ISBN==current_position->get_ISBN())
		{
			Book.erase(current_position);
			break;
		}
}

void display_all_book()
{
	cout<<"Book Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t"
	<<"Writer Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t"
	<<"ISBN:"<<endl;
	bool find=false;
	for(auto item:Book)
	{
		cout<<item.get_BookName();
		int size=item.get_BookName().size();
		for(int i=0;i<size;i++)
			cout<<'\b';
		cout<<"\t\t\t";
		cout<<item.get_writer();
		size=item.get_writer().size();
		for(int i=0;i<size;i++)
			cout<<'\b';
		cout<<"\t\t\t";
		cout<<item.get_ISBN()<<endl;
		find=true;
	}
	if(!find)
		cout<<"We don't have any book now."<<endl;
}

void find_book_with_bookname(const string &BookName)
{
	cout<<"Book Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t"
	<<"Writer Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t"
	<<"ISBN:"<<endl;
	bool find=false;
	for(auto item:Book)
	{
		if(BookName==item.get_BookName())
		{
			cout<<item.get_BookName();
			int size=item.get_BookName().size();
			for(int i=0;i<size;i++)
				cout<<'\b';
			cout<<"\t\t\t";
			cout<<item.get_writer();
			size=item.get_writer().size();
			for(int i=0;i<size;i++)
				cout<<'\b';
			cout<<"\t\t\t";
			cout<<item.get_ISBN()<<endl;
		}
	}
	if(!find)
		cout<<"No such book whose name is "<<BookName<<endl;
}

void find_book_with_writername(const string &writer)
{
	cout<<"Book Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\t\t\t"
	<<"Writer Name:"<<"\b\b\b\b\b"<<"\b\b\b\b\b"<<"\b\b\t\t\t"
	<<"ISBN:"<<endl;
	bool find=false;
	for(auto item:Book)
	{
		if(writer==item.get_writer())
		{
			find=true;
			cout<<item.get_BookName();
			int size=item.get_BookName().size();
			for(int i=0;i<size;i++)
				cout<<'\b';
			cout<<"\t\t\t";
			cout<<item.get_writer();
			size=item.get_writer().size();
			for(int i=0;i<size;i++)
				cout<<'\b';
			cout<<"\t\t\t";
			cout<<item.get_ISBN()<<endl;
		}
	}
	if(!find)
		cout<<"No such book whose writer is "<<writer<<endl;
}
