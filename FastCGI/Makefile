homepath=./bin/
infotext=infotext

register_object= Register.c user_op.c /usr/lib/libfcgi.a
register_target=register.out
register_lib = 

user_object=./resource/CJsonObject.cpp ./resource/cJSON.c  ./user_op.c ./user.c /usr/lib/libfcgi.a
user_target=user.out
user_lib=

room_object=./resource/CJsonObject.cpp ./resource/cJSON.c ./user_op.c  ./room_op.c ./room.c /usr/lib/libfcgi.a /usr/local/lib/libhiredis.a
room_target=room.out
room_lib=

manage_object = ./container_manage.c  /usr/local/lib/libhiredis.a  /usr/local/lib/libevent.a
manage_target=containerManage.out
manage_lib=


register:
	g++ $(register_object) -o $(homepath)$(register_target) $(register_lib)

user:
	g++ -std=c++11 $(user_object) -o $(homepath)$(user_target) $(user_lib)

room:
	g++ -std=c++11 $(room_object) -o $(homepath)$(room_target) $(room_lib)


containerManage:$(manage_object)
	g++ -std=c++11 $(manage_object) -o $(homepath)$(manage_target) $(manage_lib)

clean:
	rm $(homepath)*.out $(homepath)$ -f
