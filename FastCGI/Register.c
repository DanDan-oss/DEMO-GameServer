#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcgi_stdio.h>
#include "user_op.h"

int main()
{
	while (0 <= FCGI_Accept())
	{
		char buff[1024] = {0};
		fread(buff, 1, sizeof(buff), stdin);
		std::string orig_string(buff);
		int add_pos = orig_string.find("&");
		std::string username = orig_string.substr(9, add_pos-9);
		std::string password = orig_string.substr(add_pos+10, orig_string.size()-add_pos - 10);
		
		printf("Content-Type=text\r\n\r\n");
		if(0 < CheckUser(username.c_str(), NULL))
		{
			printf("create fail: user Exist!\r\n");
			continue;
		}
		AddUser(username.c_str(), password.c_str());

		printf("Success\r\n");
		printf("username:%s\n", username.data());
		printf("password:%s\n", password.data());
		continue;
	}
}
