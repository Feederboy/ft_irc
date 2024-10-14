#include "header.hpp"


/*
**  Command: USER
**  Parameters: <username> 0 * <realname>
**  
**  The USER command is used at the beginning of a connection to specify the username and realname of a new user.
**  It must be noted that <realname> must be the last parameter because it may contain SPACE (' ', 0x20) characters,
**  and should be prefixed with a colon (:)
**  
**  The maximum length of <username> may be specified by the USERLEN RPL_ISUPPORT parameter.
**  If this length is advertised, the username MUST be silently truncated to the given length before being used.
**  The minimum length of <username> is 1, it MUST NOT be empty.
**  If it is empty, the server SHOULD reject the command with ERR_NEEDMOREPARAMS (even if an empty parameter is provided);
**  otherwise it MUST use a default value instead.
**  
**  The second and third parameters of this command SHOULD be sent as one zero ('0', 0x30) and one asterisk character ('*', 0x2A) by the client
**  
**  If a client tries to send the USER command after they have already completed registration with the server,
**  the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.
**
**  Numeric Replies:
**      ERR_NEEDMOREPARAMES (461)   ERR_ALREADYREGISTERED (462)
**
**  Command example:
**      USER guest 0 * :Victor Vernhes      [User gets registered with username "~guest" and real name "Victor Vernhes"]
*/


//  Do a better parsing for this command, also check if user already give a nick, is so, set itto registered

void    user(std::vector<std::string> args, User &usr, Server &serv)
{
    if (args.size() < 4)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "USER");
        return;
    }
    if (usr.isRegister())
    {
        send_error(usr, ERR_ALREADYREGISTERED, usr.getNick());
        return;
    }

    usr.setUsername(args[0]);
    usr.setHostname(args[1]);
    usr.setServname(args[2]);
    usr.setRealName(ft_join(args, " ", 3));

    if (!usr.isRegister())
    {
        if (serv.getServPasswrd() != "")
        {
            if (!checkPasswrd(usr, serv))
                return;
        }
        usr.addRegisterStep(2);
        std::cout << "User #" << usr.getFd() << " is registred as " << usr.getNick() << std::endl;
        if (usr.isRegister())
        {
            endOfRegistration(usr, serv);
            return;
        }
    }
    return;
}
