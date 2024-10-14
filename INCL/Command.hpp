#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "header.hpp"

/*  Prototype of all the commands in Command directory */

void    invite(std::vector<std::string> args, User &usr, Server &serv);
void    join(std::vector<std::string> args, User &usr, Server &serv);
void    kick(std::vector<std::string> args, User &usr, Server &serv);
void    mode(std::vector<std::string> args, User &usr, Server &serv);
void    names(std::vector<std::string> args, User &usr, Server &serv);
void    nick(std::vector<std::string> args, User &usr, Server &serv);
void    sendNoticeToAllChannel(Channel *chan, std::string text, User &usr);
void    notice(std::vector<std::string> args, User &usr, Server &serv);
void    part(std::vector<std::string> args, User &usr, Server &serv);
bool    checkPasswrd(User &usr, Server &serv);
void    pass(std::vector<std::string> args, User &usr, Server &serv);
void    sendToAllChannel(Channel *chan, std::string text, User &usr);
void    privmsg(std::vector<std::string> args, User &usr, Server &serv);
void    quit(std::vector<std::string> args, User &usr, Server &serv);
void    topic(std::vector<std::string> args, User &usr, Server &serv);
void    user(std::vector<std::string> args, User &usr, Server &serv);

#endif