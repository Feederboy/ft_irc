#include "header.hpp"

/*
**  Command: JOIN
**  Parameters: <channel>{,<channel>} [<key>{,<key>}]
**
**  The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it.
**  The server receiving the command checks whether or not the client can join the given channel,
**  and processes the request. Servers MUST process the parameters of this command as lists on incoming commands
**  from clients, with the first <key> being used for the first <channel>, the second <key> being used for the second <channel>
**
**  While a client is joined to a channel, they receive all relevant information about that channel
**  including the JOIN, PART, KICK, and MODE messages affecting the channel.
**  They receive all PRIVMSG and NOTICE messages sent to the channel, and they also receive QUIT messages
**  from other clients joined to the same channel
**
**  Servers MAY restrict the number of channels a client may be joined to at one time.
**  This limit SHOULD be defined in the CHANLIMIT RPL_ISUPPORT parameter.
**  If the client cannot join this channel because they would be over their limit,
**  they will receive an ERR_TOOMANYCHANNELS (405) reply and the command will fail.
**
**  Numeric replies:
**      ERR_NEEDMOREPARAMS (461)    ERR_NOSUCHCHANNEL (403)     ERR_TOOMANYCHANNELS (405)
**      ERR_BADCHANNELKEY (475)     ERR_BANNEDFROMCHAN (474)    ERR_CHANNELISFULL (471)
**      ERR_INVITEONLYCHAN (473)    ERR_BADCHANMASK (476)
**      RPL_TOPIC (332)             RPL_TOPICWHOTIME (333)      RPL_NAMEREPLY (353)
**      RPL_ENDOFNAMES (366)
*/


void    createChannel(std::string chanName, std::string key, User &usr, Server &serv)
{
    Channel *newChannel = new Channel(chanName, key, "", &usr, "");

    serv.addChannel(newChannel);
    usr.addChannel(newChannel);
    usr.setCurrentChannel(newChannel);

    send_reply(usr, RPL_NAMREPLY, REPLY_NAMEREPLY(chanName, newChannel->getMembersList()));
    send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES(chanName));
}

void    joinChannel(Channel *chan, User &usr)
{
    std::vector<User *> userInChannel;

    chan->addUser(&usr);
    usr.addChannel(chan);
    usr.setCurrentChannel(chan);
    userInChannel = chan->getMembers();
    
    send_notice(usr, usr, NOTIF_JOIN(chan->getName()));
    for(size_t i = 0; i < userInChannel.size(); i++)
    {
        send_notice(usr, *userInChannel[i], NOTIF_JOIN(chan->getName()));
    }
    if (chan->hasTopicSet())
    {
        send_reply(usr, RPL_TOPIC, REPLY_TOPIC(chan->getName(), chan->getTopic()));
        send_reply(usr, RPL_TOPICWHOTIME, REPLY_TOPICWHOTIME(chan->getName(), chan->getTopicWho(), chan->getTopicWhen()));
    }
    else
        send_reply(usr, RPL_NOTOPIC, REPLY_NOTOPIC(chan->getName()));
    send_reply(usr, RPL_NAMREPLY, REPLY_NAMEREPLY(chan->getName(), chan->getMembersList()));
    send_reply(usr, RPL_ENDOFNAMES, REPLY_ENDOFNAMES(chan->getName()));
}

void    joinExistingChannel(Channel *chan, std::string key, User &usr)
{
    if (usr.getCurrentChannel())
    {
        if (usr.getCurrentChannel() == chan)
            return;
        else if (usr.isRegisterToChannel(chan) && usr.getCurrentChannel() != chan)
        {
            usr.setCurrentChannel(chan);
            return;
        }
    }
    if (chan->hasKey() && chan->getKey() != key)
    {
        send_error(usr, ERR_BADCHANNELKEY, chan->getName());
        return;
    }
    if (chan->isBanned(usr))
    {
        send_error(usr, ERR_BANNEDFROMCHAN, chan->getName());
        return;
    }
    if (chan->getNbMembers() == chan->getLimit())
    {
        send_error(usr, ERR_CHANNELISFULL, chan->getName());
        return;
    }
    if (chan->isInviteOnly() && !chan->userIsInvited(usr))
    {
        send_error(usr, ERR_INVITEONLYCHAN, chan->getName());
        return;
    }
    joinChannel(chan, usr);
}

void    checkChannel(std::string chanName, std::string key, User &usr, Server &serv)
{
    Channel *chan;

    if (!usr.isRegister())
    {
        send_error(usr, ERR_NOTREGISTERED, "JOIN");
        return;
    }

    if (chanName[0] == '#')
    {
        chan = serv.getChannelByName(chanName);
        if (chan == NULL)
        {
            send_notice(usr, usr, NOTIF_JOIN(chanName));
            createChannel(chanName, key, usr, serv);
            return;
        }
    }
    else
    {
        send_error(usr, ERR_BADCHANMASK, chanName);
        return;
    }
    if (chan->isSecret())
    {
        send_error(usr, ERR_NOSHUCHCHANNEL, chanName);
        return;
    }
    joinExistingChannel(chan, key, usr);
}


void    join(std::vector<std::string> args, User &usr, Server &serv)
{
    std::vector<std::string> channels;
    std::vector<std::string> keys;

    if (args.size() < 1)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "JOIN");
        return;
    }
    channels = ft_split(args[0], ",");
    keys.resize(channels.size());
    if (args.size() > 1)
        keys = ft_split(args[1], ",");
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (i + 1 == MAX_CHANNEL_PER_USER)
        {
            send_error(usr, ERR_TOOMANYCHANNELS, "JOIN");
            return;
        }
        checkChannel(channels[i], keys[i], usr, serv);
    }
    return;
}