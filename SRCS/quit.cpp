#include "header.hpp"

/*
**  Command: QUIT
**  Parameters: [<reason>]
**
**  The QUIT command is used to terminate a client’s connection to the server.
**  When connections are terminated by a client-sent QUIT command,
**  servers SHOULD prepend <reason> with the ASCII string "Quit: "
**  when sending QUIT messages to other clients,
**  to represent that this user terminated the connection themselves.
**  This applies even if <reason> is empty, in which case the reason sent to other clients
**  SHOULD be just this "Quit: " string.
**
**  Numeric Replies:
**      None
*/

void    quit(std::vector<std::string> args, User &usr, Server &serv)
{
    std::string             reason;
    std::vector<Channel *>  usrChannel;
    int                     usrFd;

    if (args.size() > 0)
        reason = ft_join(args, " ", 0);
    usrChannel = usr.getChannels();
    if (!usrChannel.empty())
    {
        for (std::vector<Channel *>::iterator it = usrChannel.begin(); it != usrChannel.end(); it++)
        {
            sendNoticeToAllChannel(*it, NOTIF_QUIT(reason), usr);
            usr.deleteChannel(*it);
            if ((*it)->getNbMembers() == 0)
                serv.deleteChannel(*it);
        }
    }
    usrFd = usr.getFd();
    serv.delete_from_pollFd(usrFd);
    serv.deleteUser(&usr);
    std::cout << "Client #" << usrFd << " has gone away" << std::endl;
}