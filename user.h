#ifndef USER_H
#define USER_H

#include<string>
#include"radix62.h"

enum authority_type
{
	SystemAdministrator,
	BookAdministrator,
	Reader
};

class user
{
public:
	user(std::string u_name,int u_number,radix62 key,authority_type au)//user_name,user_number,key_word,authority
	{
		user_name=u_name;
		user_number=u_number;
		keyword=key;
		authority=au;
	}
	
	std::string get_user_name() const;
	void set_user_name(const std::string u_name);
	int get_user_number() const;
	radix62 get_keyword() const;
	void set_keyword(const radix62 key);//not in charge of examining the correctness of the keyword
	authority_type get_authority() const;
	void set_authority(const authority_type au);//this function can only be used by users with higher authorities, but we don't ensure that in this function, so be careful when use this function.
	
	bool operator==(const user &u);//compare the user_number in two instances
	std::ostream& operator<<(std::ostream &);
	//To avoid the case that user_number be modified or something like that happenning, don't offer the overriding for operator '>>'
private:
	std::string user_name;
	int user_number;//user_number can't be modefied after the instance is constructed
	radix62 keyword;
	authority_type authority;	
};

#endif
