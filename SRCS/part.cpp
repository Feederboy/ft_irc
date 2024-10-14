#include "header.hpp"

/*
**  Command; PART
**  Parameters: <channel>{,<channel>} [<reason>]
**
**  The PART command removes the client from the given channel(s).
**  On sending a successful PART command, the user will receive a PART message from the server for each channel
**  they have been removed from. <reason> is the reason that the client has left the channel(s).
**
**  For each channel in the parameter of this command, if the channel exists and the client is not joined to it,
**  they will receive an ERR_NOTONCHANNEL (442) reply and that channel will be ignored.
**  If the channel does not exist, the client will receive an ERR_NOSUCHCHANNEL (403) reply
**  and that channel will be ignored.
**
**  Numeric replies:
**      ERR_NEEDMOREPARAMS(461)     ERR_NOSUCHCHANNEL(403)  ERR_NOTONCHANNEL (442)
*/

void    part(std::vector<std::string> args, User &usr, Server &serv)
{
    Channel                     *chan;
    std::vector<User *>         usersOnChan;
    std::vector<std::string>    chanList;
    std::string                 reason;

    if (args.size() < 1)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "PART");
        return;
    }

    chanList = ft_split(args[0], ",");
    std::cout << "After split of chanlist and chanlist size : " << chanList.size() << std::endl; 
    if (args.size() > 1)
    {
        reason = ft_join(args, " ", 1);
        std::cout << "After reason split, and reason: " << reason << std::endl;
    }
    for (size_t i = 0; i < chanList.size(); i++)
    {
        chan = serv.getChannelByName(chanList[i]);
        std::cout << "After server getChannelByName\n";
        if (!chan)
        {
            send_error(usr, ERR_NOSHUCHCHANNEL, chanList[i]);
            continue;
        }
        if (!chan->userIsOnChan(&usr))
        {
            send_error(usr, ERR_NOTONCHANNEL, chanList[i]);
            continue;
        }
        std::cout << "After the 2 condition in part for loop\n";

        usersOnChan = chan->getMembers();
        for (std::vector<User *>::iterator it = usersOnChan.begin(); it != usersOnChan.end(); it++)
        {
            if (reason.empty())
            {
                std::cout << "Reason empty\n";
                send_notice(usr, *(*it), NOTIF_PART(chan->getName()));
            }
            else
            {
                std::cout << "Reason not empty\n";
                send_notice(usr, *(*it), NOTIF_PART_REASON(chan->getName(), reason));
            }
        }
        /*for (size_t j = 0; j < usersOnChan.size(); j++)
        {
            std::cout <<  "In for loop for user On chan\n";
            if (reason.empty())
                send_notice(usr, *usersOnChan[j], NOTIF_PART(chan->getName()));
            else
                send_notice(usr, *usersOnChan[j], NOTIF_PART_REASON(chan->getName(), reason));
        }*/
        std::cout << "After second Loop\n";
        usr.deleteChannel(chan);
        std::cout << "After user delete chan\n";
        if (chan->getNbMembers() == 0)
        {
            serv.deleteChannel(chan);
            std::cout << "After server delete channel\n";
        }
    }
}