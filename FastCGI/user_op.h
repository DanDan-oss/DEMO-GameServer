#ifndef USEROP_H
#define USEROP_H

unsigned char CheckUser(const char* _name, const char* _password_dgst);
void AddUser(const char* _name, const char* _password);
unsigned char DelUser(const char* _name);
unsigned char ChangePssword(const char* _name, const char* _password);


#endif