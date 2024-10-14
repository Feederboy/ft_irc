#include "header.hpp"

std::map<int, std::string> errReplies;

void    initErrReplies(void)
{
    errReplies[ERR_NOSUCHNICK] = " :No such nick";
    errReplies[ERR_NOSUCHSRVER] = " :No such server";
    errReplies[ERR_NOSHUCHCHANNEL] = " No such channel";
    errReplies[ERR_CANNOTSENDTOCHAN] = " :Cannot send to channel";
    errReplies[ERR_TOOMANYCHANNELS] = " :You have joined too many channels";
    errReplies[ERR_TOOMANYTARGETS] = " :Duplicate recipients. No message delivered";
    errReplies[ERR_NORECIPIENT] = " :No recipient given";
    errReplies[ERR_NOTEXTTOSEND] = " :No text to send";
    errReplies[ERR_UNKNOWCOMMAND] = " :Unknow command";
    errReplies[ERR_NOMOTD] = " :MOTD file is missing";
    errReplies[ERR_NONICKNAMEGIVEN] = " :No nickname given";
    errReplies[ERR_ERRONEUSNICKMANE] = " :Erroneus nickname";
    errReplies[ERR_NICKNAMEINUSER] = " :Nickname is already in use";
    errReplies[ERR_NICKCOLLISION] = " :Nickname collision Kill";
    errReplies[ERR_USERNOTINCHANNEL] = " :They aren't on that channel";
    errReplies[ERR_NOTONCHANNEL] = " :You're not on that channel";
    errReplies[ERR_USERONCHANNEL] = " :is alrealdy on channel";
    errReplies[ERR_NOTREGISTERED] = " :You have not registered";
    errReplies[ERR_NEEDMOREPARAMS] = " :Not enough parameters";
    errReplies[ERR_ALREADYREGISTERED] = " :You may not reregister";
    errReplies[ERR_PASSWDMISMATCH] = " :Password incorrect";
    errReplies[ERR_KEYSET] = " :Channel key already set";
    errReplies[ERR_CHANNELISFULL] = " :Cannot join channel (+l)";
    errReplies[ERR_UNKNOWNMODE] = " :Is unknow mode char to me";
    errReplies[ERR_INVITEONLYCHAN] = " :Cannot join channel (+i)";
    errReplies[ERR_BANNEDFROMCHAN] = " :Cannot join channel (+b)";
    errReplies[ERR_BADCHANNELKEY] = " :Cannot join channel (+k)";
    errReplies[ERR_BADCHANMASK] = " :Bad Channel Mask";
    errReplies[ERR_CHANOPRIVSNEEDED] = " :You're not channel operator";
    errReplies[ERR_INVALIDKEY] = " :Key is not well-formed";
}

void    send_reply(User &usr, int replyValue, std::string text)
{
    std::ostringstream tmp;
    std::string     msg;

    tmp << std::setfill('0') << std::setw(3) << replyValue << " " << usr.getNick() << text;
    msg = tmp.str();

    if (send(usr.getFd(), &msg[0], msg.size(), 0) == -1)
        throw Errfunc(std::strerror(errno));
}

void	send_notice( User &sender, User &receiver, std::string textToSend)
{
	std::ostringstream tmp;

	tmp << ":" << sender.getNickmask() << " " << textToSend;

	std::string msg = tmp.str();

	if ( send(receiver.getFd(), &msg[0], msg.size(), 0) == -1 )
        throw Errfunc(std::strerror(errno));

}

void    sendWelcome(User &user, const std::string &msg)
{
    if (send(user.getFd(), &msg[0], msg.size(), 0) == -1)
        throw Errfunc(std::strerror(errno));
}

void    endOfRegistration(User &usr, Server &serv)
{
    sendWelcome(usr, REPLY_WELCOME(usr.getNick(), usr.getUsername(), usr.getHostname()));
    send_reply(usr, RPL_YOURHOST, REPLY_YOURHOST(serv.getServName(), "302"));
    send_reply(usr, RPL_CREATED, REPLY_CREATED(serv.getCreationTime()));
    send_reply(usr, RPL_MYINFO, REPLY_MYINFO(serv.getServName(), VERSION, AVAILABLE_CHANNEL_MODE));
    send_error(usr, ERR_NOMOTD, "");
}

void    send_error(User &usr, int ErrValue, std::string arg)
{
    std::ostringstream  tmp;
    std::string         msg;

    if (*(arg.end() - 1) == '\n')
        arg.erase(arg.size() -1);
    
    if (arg.empty())
        tmp << ErrValue << " " << usr.getNick() << errReplies[ErrValue] << "\r\n";
    else
        tmp << ErrValue << " " << usr.getNick() << ": " << arg << errReplies[ErrValue] << "\r\n"; 
    msg = tmp.str();

    if (send(usr.getFd(), &msg[0], msg.size(), 0) == -1)
        throw Errfunc(std::strerror(errno));
}

std::string     ft_join(std::vector<std::string> &src, std::string sep, int idx)
{
    std::string     str;

    for (size_t i = idx; i < src.size(); i++)
    {
        str.append(src[i]);
        if (i != src.size() - 1)
            str.append(sep);
    }
    return (str);
}

void        signalHandler(int signal)
{
    (void)signal;
    throw ControlC();
}