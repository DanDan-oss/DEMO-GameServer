
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fcgi_stdio.h>
#include <hiredis/hiredis.h>

#include "room_op.h"
#include "user_op.h"
#include "./resource/CJsonObject.hpp"


#define REIDS_IP "192.168.1.1"

int main()
{
    printf("make 0");
    
    char* buffer = NULL;

	while (FCGI_Accept() >= 0)
    {
        std::string requetype;
        std::string username;
    	std::string password;
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
            // 验证对服务器的账户密码
            if(1 != CheckUser(username.data(), password.data()))
			{
				snedjson.Add("result", "login fail, user error");
				break;
			}

            // 判断是不是服务器列表查询
            if(0 == requetype.compare("room_check"))
            {
                snedjson.Add("result", "check server room fail, no function");
                break;
            }

            // 判断是不是创建服务器容器
            if(0 == requetype.compare("room_crate"))
            {
                static unsigned int roomIndex=0;
                std::string room_info =  create_room(roomIndex);
                if(0 == room_info.size())
                {
                    snedjson.Add("result", "crate server room fail");
                    break;
                }
                
                // 将服务器容器 IP和端口返回给客户端
                int strlen = room_info.find(':');
                snedjson.Add("result", "Success");
                snedjson.Add("server_address", room_info.substr(0, strlen));
                snedjson.Add("server_port", room_info.substr(strlen+1, room_info.length()-(strlen+1)));
                roomIndex++
                break;
            }

            // 判断是不是删除服务器容器
            if(0 == requetype.compare("room_delete"))
            {
                snedjson.Add("result", "delete server room fail, no function");
                break;
            }

            // 其余消息
            snedjson.Add("result", "unknow server room operation");
            
        }while(0);

        sendText = snedjson.ToFormattedString();
        printf("Content-Type:application/json\r\n");
        printf("Content_length:%ld\r\n\r\n", sendText.size());
        printf("%s\r\n", sendText.data());
        free(buffer);
    }
}