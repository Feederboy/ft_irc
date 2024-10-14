#include "header.hpp"

/*
**  Command : PRIVMSG
**  Parameters: <target>{,<target>} <text to be sent>
**
**  The PRIVMSG command is used to send private messages between users, as well as to send messages to channels.
**  <target> is the nickname of a client or the name of a channel.
**
**  If <target> is a channel name and the client is banned and not covered by a ban exception,
**  the message will not be delivered and the command will silently fail.
**
**  If a message cannot be delivered to a channel, the server SHOULD respond with an ERR_CANNOTSENDTOCHAN (404),
**  to let the user know that this message could not be delivered.
**
**  If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301),
**  and the command will continue.
**
**  Numeric Replies:
**      ERR_NOSUCHNICK (401)    ERR_NOSUCHSERVER (402)   ERR_CANNOTSENDTOCHAN (404)
**      ERR_TOOMANYTARGETS (407)    ERR_NORECIPIENT(411)    ERR_NOTEXTOSEND (412)
**      ERR_NOTOPLEVEL (413)    RPL_AWAY (301)
*/


void    sendToAllChannel(Channel *chan, std::string text, User &usr)
{
    std::vector<User *> chanMembers = chan->getMembers();

    for (size_t i = 0; i < chanMembers.size(); i++)
    {
        if (chanMembers[i]->getNick() != usr.getNick())
        {
            send_notice(usr, *chanMembers[i], NOTIF_PRIVMSG(chan->getName(),text));
        }
    }
}

void    sendPrivMsgToChannel(std::string receiver, std::string text, User &usr, Server &serv)
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
    sendToAllChannel(channel, text, usr);
}

void    sendPrivMsgToUser(std::string receiver, std::string text, User &usr, Server &serv)
{
    User *userToSend;

    userToSend = serv.getUserByNickname(receiver);
    if (!userToSend)
    {
        send_error(usr, ERR_NOSUCHNICK, receiver);
        return;
    }
    send_notice(usr, *userToSend, NOTIF_PRIVMSG(userToSend->getNick(), text));
}


void    sendPrivMsg(std::string &receiver, std::string text, User &usr, Server &serv)
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
        sendPrivMsgToUser(receiver, text, usr, serv);
    else
        sendPrivMsgToChannel(receiver, text, usr, serv);
    return;
}

void    privmsg(std::vector<std::string> args, User &usr, Server &serv)
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
        sendPrivMsg(*it, ft_join(args, " ", 1), usr, serv);
    }
    return;
}
