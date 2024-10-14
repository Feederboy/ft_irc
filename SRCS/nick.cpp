#include "header.hpp"

/*
**  Command: NICK
**  Parameters: <nickname>
**
**  The NICK command is used to give the client a nickname or change the previous one.
**  If the server receives a NICK command from a client where the desired nickname is already in use on the network,
**  it should issue an ERR_NICKNAMEINUSE numeric and ignore the NICK command.
**  
**  If the server does not accept the new nickname supplied by the client as valid (containing invalid characters),
**  it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command.
**  Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames,
**  and MAY disallow digits as the first character.
**  
**  If the server does not receive the <nickname> parameter with the NICK command,
**  it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.
**  
**  Numeric Replies:
**      ERR_NONICKNAMEGIVEN (431)  ERR_ERRONEUSNICKNAME (432)
**      ERR_NICKNAMEINUSE (433)  ERR_NICKCOLLISION (436)
**  
**  Command examples:
**      NICK Wiz            [Requestiong the new nick "Wiz"]
**  Message Examples:
**      :Wiz NICK killroy                   [Wiz changed his nickname to killroy]
**      :dan-!d@localhost NICK Mamo         [dan- changed his nickname to Mamo]
*/

void    sendNickMessage(User &usr, const std::string &msg)
{
    if (send(usr.getFd(), &msg[0], msg.size(), 0) == -1)
    {
        std::cerr << "Error on send function" << std::endl;
        return;
    }
}

void    checkNick(std::string arg, User &usr, Server &serv)
{
    std::vector<User *>     users = serv.getUsers();

    for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getNick() == arg)
        {
            send_error(usr, ERR_NICKNAMEINUSER, arg);
            return;
        }
    }
    
    if (serv.userConnect(usr))
    {
        if (usr.getNick().empty())
            std::cout << "User #" << usr.getFd() << " nick set to " << arg << std::endl;
        else
            std::cout << usr.getNick() << ": Nick change to " << arg << std::endl;
        if (!usr.isRegister() && usr.getNick().empty())
        {
            if (serv.getServPasswrd() != "")
            {
                if (!checkPasswrd(usr, serv))
                    return;
            }
            std::cout << "User #" << usr.getFd() << " is registered has " << arg << std::endl;
            usr.addRegisterStep(1);
            usr.setNickname(arg);
            sendNickMessage(usr, NOTIF_NICK(arg));
            if (usr.isRegister())
            {
                endOfRegistration(usr, serv);
            }
        }
        else
        {
            sendNickMessage(usr, NOTIF_NICK(arg));
            usr.setNickname(arg);
        }
    }
}

void    nick(std::vector<std::string> args, User &usr, Server &serv)
{
    std::string nickName;
    
    if (args.size() == 0)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "NICK");
        return;
    }
    else if (args[0].empty() || args.size() > 1)
    {
        send_error(usr, ERR_ERRONEUSNICKMANE, nickName);
        return;
    }
    nickName = parseNickname(args[0]);
    if (nickName.empty())
    {
        send_error(usr, ERR_ERRONEUSNICKMANE, args[0]);
        return;
    }
    if (serv.userConnect(usr) && usr.getNick() == args[0])
        return;
    checkNick(nickName, usr, serv);
}
