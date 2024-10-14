#include "header.hpp"

/*
**  Command: KICK
**  Parameters: <channel> <user> *("," <user>) [<comment>]
**
**  The KICK command can be used to request the forced removal of a user from a channel.
**  It causes the <user> to be removed from the <channel> by force.
**  If no comment is given, the server SHOULD use a default message instead.
**
**  Numeric replies:
**      ERR_NEEDMOREPARAMS (461)   ERR_NOSUCHCHANNEL (403) ERR_CHNOPRIVSNEEDED (482)
**      ERR_USERNOTINCHANNEL (441) ERR_NOTONCHANNEL (442)
*/

void    kickUsers(Channel *chan, std::vector<std::string> targets, std::string reason, User &usr, Server &serv)
{
    std::vector<User *>     usrsOnChan;
    User    *victim;

    for (size_t i = 0; i < targets.size(); i++)
    {
        victim = serv.getUserByNickname(targets[i]);
        if (!victim)
        {
            send_error(usr, ERR_NOSUCHNICK, targets[i]);
            continue;
        }
        if (!chan->userIsOnChan(victim))
        {
            send_error(usr, ERR_NOTONCHANNEL, chan->getName());
            continue;
        }
        usrsOnChan = chan->getMembers();
        for (size_t j = 0; j < usrsOnChan.size(); j++)
        {
            send_notice(usr, *usrsOnChan[j], NOTIF_KICK(chan->getName(), victim->getNick(), reason));
        }
        victim->deleteChannel(chan);
        if (chan->getNbMembers() == 0)
            serv.deleteChannel(chan);
    }
}


void    kick(std::vector<std::string> args, User &usr, Server &serv)
{
    std::vector<std::string>    chanName;
    std::vector<std::string>    targets;
    Channel                     *chan;
    std::string                 reason;

    if (args.size() < 2)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "KICK");
        return;
    }
    chanName = ft_split(args[0], ",");
    targets = ft_split(args[1], ",");
    reason = ":";

    if (args.size() > 2)
        reason += ft_join(args, " ", 2);
    for (size_t i = 0; i < chanName.size(); i++)
    {
        chan = serv.getChannelByName(chanName[i]);
        if (!chan)
        {
            send_error(usr, ERR_NOSHUCHCHANNEL, chanName[i]);
            continue;
        }
        if (!chan->userIsOnChan(&usr))
        {
            send_error(usr, ERR_NOTONCHANNEL, chanName[i]);
            continue;
        }
        if (!chan->userIsOP(&usr))
        {
            send_error(usr, ERR_CHANOPRIVSNEEDED, chanName[i]);
            continue;
        }
        kickUsers(chan, targets, reason, usr, serv);
    }

    return;
}