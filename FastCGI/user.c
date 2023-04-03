#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fcgi_stdio.h>

#include "./resource/CJsonObject.hpp"
#include "user_op.h"

int main()
{
	char* buffer = NULL;
	while (FCGI_Accept() >= 0)
	{
		std::string requetype;
		std::string username;
    	std::string password;
		std::string newpass;
    	std::string sendText;

    	int strlen=atoi(getenv("CONTENT_LENGTH"));
    	buffer = (char*)calloc(1, strlen+1);
    	fread(buffer, 1, strlen, stdin);

    	neb::CJsonObject recvjson(buffer);
    	recvjson.Get("username", username);
    	recvjson.Get("password", password);
		recvjson.Get("requetype", requetype);

		neb::CJsonObject snedjson("");
		do
		{
			// 验证账号密码
			if(1 != CheckUser(username.data(), password.data()))
			{
				snedjson.Add("result", "login fail, user error");
				break;
			}

			// 判断是登录操作
			if(0 == requetype.compare("login"))
			{
				// TODO:返回在线服务列表
				snedjson.Add("result", "Success");
				break;
			}

			// 判断是修改密码操作
			if(0 == requetype.compare("changepass"))
			{
				recvjson.Get("newpassword", newpass);
				if(1 == ChangePssword(username.data(), newpass.data()))
				{
					snedjson.Add("result", "Success");
					break;
				}
				snedjson.Add("result", "change pass fail");
				break;
			}

			// TODO: 其余消息处理
			snedjson.Add("result", "know message type");
		} while(0);

        sendText = snedjson.ToFormattedString();
        printf("Content-Type:application/json\r\n");
        printf("Content_length:%d\r\n\r\n", sendText.size());
        printf("%s\r\n", sendText.data());
    	free(buffer);
	}
}