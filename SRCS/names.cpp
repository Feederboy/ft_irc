#include "header.hpp"

/*
**  Command: NAMES
**  Parameters: <channel>{,<channels>}
**
**  The NAMES command is used to view the nicknames joined to a channel and their channel membership prefixes.
**  The param of this command is a list of channel names, delimited by a comma (",") character.
**
**  The channel names are evaluated one-by-one.
**  For each channel that exists and they are able to see the users in,
**  the server returns one of more RPL_NAMREPLY numerics containing the users joined to the channel
**  and a single RPL_ENDOFNAMES numeric.
**  If the channel name is invalid or the channel does not exist,
**  one RPL_ENDOFNAMES numeric containing the given channel name should be returned.
**  If the given channel has the secret channel mode set and the user is not joined to that channel,
**  one RPL_ENDOFNAMES numeric is returned. 
**
**  Numeric Replies:
**      RPL_NAMEREPLY (353) RPL_ENDOFNAMES (366)
*/

void    printChannelNames(const std::string &chanName, User &usr, Server &serv)
{
    Channel *chan;

    chan = serv.getChannelByName(chanName);
    if (!chan || (chan->isSecret() && !chan->userIsOnChan(&usr)))
    {
        send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES(chanName));
        return;
    }
    send_reply(usr, RPL_NAMREPLY, REPLY_NAMEREPLY(chanName, chan->getMembersList()));
    send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES(chanName));
}

void    printAllServerNames(User &usr, Server &serv)
{
    std::vector<Channel *>  chans;
    std::vector<User *>     usersOnServ;
    std::string             listUsersNoChannel;
    std::string             chanName = "*";


    chans = serv.getChannels();
    for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
    {
        if ((*it)->isSecret() && !(*it)->userIsOnChan(&usr))
            continue;
        else
        {
            send_reply(usr, RPL_NAMREPLY, REPLY_NAMEREPLY((*it)->getName(), (*it)->getMembersList()));
            send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES((*it)->getName()));
        }
    }
    usersOnServ = serv.getUsers();
    for (std::vector<User *>::iterator iter = usersOnServ.begin(); iter != usersOnServ.end(); iter++)
    {
        if ((*iter)->getChannels().empty() && (*iter)->getNick() != usr.getNick())
            listUsersNoChannel.append((*iter)->getNick() + " ");
    }
    if (!listUsersNoChannel.empty())
    {
        send_reply(usr, RPL_NAMREPLY, REPLY_NAMEREPLY(chanName, listUsersNoChannel));
    }
    send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES(chanName));
}

void    names(std::vector<std::string> args, User &usr, Server &serv)
{
    std::vector<std::string> chanName;


    if (args.empty())
    {
        printAllServerNames(usr, serv);
        return;
    }
    else
        chanName = ft_split(args[0], ",");
    
    for (size_t i = 0; i < chanName.size(); i++)
        printChannelNames(chanName[i], usr, serv);
}
