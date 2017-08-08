#include"user.h"

//define for member functions in the class user
std::string user::get_user_name() const
{
	return user_name;
}

void user::set_user_name(const std::string u_name)
{
	user_name=u_name;
}

int user::get_user_number() const
{
	return user_number;
}

radix62 user::get_keyword() const
{
	return keyword;
}	

void user::set_keyword(const radix62 key)
{
	keyword=key;
}

authority_type user::get_authority() const
{
	return authority;
}

void user::set_authority(const authority_type au)
{
	authority=au;
}

bool user::operator==(const user &u)
{
	if(user_number==u.get_user_number())
		return true;
	else
		return false;
}

std::ostream& user::operator<<(std::ostream &os)
{
	os
	<<user_name<<std::endl
	<<user_number<<std::endl
	<<keyword<<std::endl;
	switch(authority)
	{
	case SystemAdministrator:
		os<<"SystemAdministrator"<<std::endl;
		break;
	case BookAdministrator:
		os<<"BookAdministrator"<<std::endl;
		break;
	case Reader:
		os<<"Reader"<<std::endl;
		break;
	}
	return os;
}
