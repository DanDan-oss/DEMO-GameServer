#ifndef ROOMOP_H
#define ROOMOP_H

#include <string>

std::string CreateRoom();
std::string create_room(unsigned int _room_index);
unsigned char CheckRoom(unsigned int _roomId);
unsigned char DelRoom(unsigned int _roomId);

#endif