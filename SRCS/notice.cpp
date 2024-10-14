#include "header.hpp"

/*
**  Command: NOTICE
**  Parameters: <target>{,target>} <text to be send>
**
**  The NOTICE command is used to send notices between users, as well as to send notices to channels.
**  <target> is interpreted the same way as it is for the PRIVMSG command.
**
**  The NOTICE message is used similarly to PRIVMSG.
**  The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message.
**  This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command. 
*/



void    sendNoticeToAllChannel(Channel *chan, std::string text, User &usr)
{
    std::vector<User *> chanMembers = chan->getMembers();

    for (size_t i = 0; i < chanMembers.size(); i++)
    {
        if (chanMembers[i]->getNick() != usr.getNick())
        {
            send_notice(usr, *chanMembers[i], NOTIF_NOTICE(chan->getName(),text));
        }
    }
}

void    sendNoticeToChannel(std::string receiver, std::string text, User &usr, Server &serv)
{
    Channel *channel;

    channel = serv.getChannelByName(receiver);
    if (!channel)
    {
        send_error(usr, ERR_NOSHUCHCHANNEL, receiver);
        return;
    }
    if (!(channel->userIsOnChan(&usr) || channel->userIsOP(&usr)))
    {
        send_error(usr, ERR_CANNOTSENDTOCHAN, receiver);
        return;
    }
    if (channel->isModerate() && !channel->userIsModerater(usr))
    {
        send_error(usr, ERR_CANNOTSENDTOCHAN, receiver);
        return;
    }
    sendNoticeToAllChannel(channel, text, usr);
}

void    sendNoticeToUser(std::string receiver, std::string text, User &usr, Server &serv)
{
    User *userToSend;

    userToSend = serv.getUserByNickname(receiver);
    if (!userToSend)
    {
        send_error(usr, ERR_NOSUCHNICK, receiver);
        return;
    }
    send_notice(usr, *userToSend, NOTIF_NOTICE(userToSend->getNick(), text));
}

void    sendNotice(std::string &receiver, std::string text, User &usr, Server &serv)
{
    if (text.length() == 0)
    {
        send_error(usr, ERR_NOTEXTTOSEND, receiver);
        return;
    }
    if (receiver.length() == 0)
    {
        send_error(usr, ERR_NORECIPIENT, receiver);
        return;
    }
    if (receiver.find('#', 0) == std::string::npos)
        sendNoticeToUser(receiver, text, usr, serv);
    else
        sendNoticeToChannel(receiver, text, usr, serv);
}

void    notice(std::vector<std::string> args, User &usr, Server &serv)
{
    if (args.size() < 1)
    {
        send_error(usr, ERR_NORECIPIENT, "");
        return;
    }
    else if (args.size() < 2)
    {
        send_error(usr, ERR_NOTEXTTOSEND, "");
        return;
    }

    std::vector<std::string> receivers = ft_split(args[0], ",");
    if (hasDuplicates(receivers))
    {
        send_error(usr, ERR_TOOMANYTARGETS, findDuplicates(receivers));
        return;
    }
    for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); it++)
    {
        sendNotice(*it, ft_join(args, " ", 1), usr, serv);
    }
    return;
}