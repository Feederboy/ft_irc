#ifndef HEADER_HPP
# define HEADER_HPP

#include <cstring>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <signal.h>

# define VERSION "302"
# define AVAILABLE_CHANNEL_MODE "opsitnmbvkl"
# define MAX_USR_NICK_LENGHT 9
# define MAXCLIENT 7
# define BUFF_LEN 518
# define MAX_CHANNEL_PER_USER 8
# define MAX_USER_PER_CHAN 10

/* Notif send from server to client Format  */

# define NOTIF_NICK(newNickname) ("You're now know as " + newNickname + "\r\n")
# define NOTIF_PRIVMSG(dest, msg) ("PRIVMSG " + dest + " :" + msg + "\r\n")
# define NOTIF_NOTICE(dest, msg) ("NOTICE " + dest + " " + msg + "\r\n")
# define NOTIF_JOIN(channel) ("JOIN " + channel + "\r\n")
# define NOTIF_KICK(channel, usr, reason) ("KICK" + channel + " " + usr + " " + reason + "\r\r")
# define NOTIF_INVITE(channel, nick) ("INVITE " + nick + " :" + channel + "\r\n")
# define NOTIF_PART(channel) ("PART :" + channel + "\r\n")
# define NOTIF_PART_REASON(channel, reason) ("PART :" + channel + " :\"" + reason + "\"")
# define NOTIF_TOPIC(channel, topic) ("TOPIC " + channel + " :" + topic + "\r\n")
# define NOTIF_QUIT(reason) ("QUIT :quit: " + reason) + "\r\n"

/*  Reply send from server to client Format  */

# define REPLY_WELCOME(nick,user,host) ("001 "+ nick+ " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
# define REPLY_YOURHOST(servername, version) (" :Your host is  " + servername + ", running on version " + version + "\r\n")
# define REPLY_CREATED(creationTime) (" :This server was created " + creationTime + "\r\n")
# define REPLY_MYINFO(servername, version, modeChan) (" :" + servername + " " + version + " " + modeChan + "\r\n")
# define REPLY_INVITING(nick, channel) (" :" + nick + " :" + channel + "\r\n")
# define REPLY_CHANNELMODEIS(channel, mode) (channel + " :+" + mode + "\r\n")
# define REPLY_CREATIONTIME(channel, time) (channel + " " + time + "\r\n")
# define REPLY_NAMEREPLY(channel, members) ("= " + channel + " :" + members +"\r\n")
# define REPLY_ENDOFNAMES(channel) (channel + " :End of '/'NAMES list.\r\n")
# define REPLY_TOPIC(channel, topic) (channel + " :" + topic + "\r\n")
# define REPLY_TOPICWHOTIME(channel, nick, time) (channel + " " + nick + " " + time + "\r\n")
# define REPLY_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define ERR_SERVFULL(host) ("Error: (unknow@"+ host +") [No more connection accepted, server FULL]\n")
# define REPLY_BANLIST(channel, banList) (channel + " " + banList + "\r\n")
# define REPLY_ENDOFBANLIST(channel) (channel + " :ENd of channel ban list.\r\n")

/* Numeric replies  */

# define RPL_WELCOME 001
# define RPL_YOURHOST 002
# define RPL_CREATED 003
# define RPL_MYINFO 004
# define RPL_ISUPPORT 005
# define RPL_CHANNELMODEIS 324
# define RPL_CREATIONTIME 329
# define RPL_NOTOPIC    331
# define RPL_TOPIC 332
# define RPL_TOPICWHOTIME 333
# define RPL_INVITELIST 336
# define RPL_ENDOFINVITELIST 337
# define RPL_INVITING 341
# define RPL_NAMREPLY 353
# define RPL_ENDOFNAMES 366
# define RPL_BANLIST 367
# define RPL_ENDOFBANLIST 368

/*  Error numeric replies  */

# define ERR_NOSUCHNICK         401
# define ERR_NOSUCHSRVER        402
# define ERR_NOSHUCHCHANNEL     403
# define ERR_CANNOTSENDTOCHAN   404
# define ERR_TOOMANYCHANNELS    405
# define ERR_TOOMANYTARGETS     407
# define ERR_NORECIPIENT        411
# define ERR_NOTEXTTOSEND       412
# define ERR_UNKNOWCOMMAND      421
# define ERR_NOMOTD             422
# define ERR_NONICKNAMEGIVEN    431
# define ERR_ERRONEUSNICKMANE   432
# define ERR_NICKNAMEINUSER     433
# define ERR_NICKCOLLISION      436
# define ERR_USERNOTINCHANNEL   441
# define ERR_NOTONCHANNEL       442
# define ERR_USERONCHANNEL      443
# define ERR_NOTREGISTERED      451
# define ERR_NEEDMOREPARAMS     461
# define ERR_ALREADYREGISTERED  462
# define ERR_PASSWDMISMATCH     464
# define ERR_KEYSET             467
# define ERR_CHANNELISFULL      471
# define ERR_UNKNOWNMODE        472
# define ERR_INVITEONLYCHAN     473
# define ERR_BANNEDFROMCHAN     474
# define ERR_BADCHANNELKEY      475
# define ERR_BADCHANMASK        476
# define ERR_CHANOPRIVSNEEDED   482
# define ERR_INVALIDKEY         525

#include "User.hpp" 
#include "Server.hpp"
#include "Channel.hpp"
#include "Excep.hpp"
#include "Command.hpp"
#include "parsing.hpp"
#include "utils.hpp"


#endif
