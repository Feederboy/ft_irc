#include "header.hpp"

/*
**  Command: PASS
**  Parameters: <password>
**
**  The PASS command is used to set a ‘connection password’.
**  If set, the password must be set before any attempt to register the connection is made.
**  This requires that clients send a PASS command before sending the NICK / USER combination.
**
**  The password supplied must match the one defined in the server configuration.
**  It is possible to send multiple PASS commands before registering but
**  only the last one sent is used for verification and it may not be changed once the client has been registered.
**
**  If the password supplied does not match the password expected by the server, then the server SHOULD send ERR_PASSWDMISMATCH (464)
**  and MAY then close the connection with ERROR. Servers MUST send at least one of these two messages.
**
**  Numeric replies:
**  ERR_NEEDMOREPARAMS (461)  ERR_ALREADYREGISTERED (462)  ERR_PASSWDMISMATCH (464)
**
**  Example:
**      PASS secretpasswordhere
*/

bool    checkPasswrd(User &usr, Server &serv)
{
    /*
    **  Pssword dismatch because the user password keep the \n at the end
    */
    //std::cout << "In check PAssword" << std::endl;
   // std::cout << "User password : " << usr.getPasswrd() << " Server password : " << serv.getServPasswrd() <<std::endl;

    if (usr.getPasswrd() == serv.getServPasswrd())
    {
        return (true);
    }
    else
    {
        std::string         err;
        std::ostringstream  tmp;
        std::string         msg;

        if (usr.getPasswrd() == "")
            err = "Password required to log in into the server";
        else
            err = "Wrong password";
        tmp << "ERROR : Close link: (" << usr.getUsername() << "@" << serv.getHost() << ") [" << err << "]\r\n";
        msg = tmp.str();
        if (send(usr.getFd(), &msg[0], msg.size(), 0) == -1)
            return false;
        std::cout << "client #" << usr.getFd() << " gone away" << std::endl;
        serv.delete_from_pollFd(usr.getFd());
        serv.deleteUser(&usr);
        return (false);
    }
}

void    pass(std::vector<std::string> args, User &usr, Server &serv)
{
    if (args.empty())
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "PASS");
        return;
    }
    if (usr.isRegister())
    {
        send_error(usr, ERR_ALREADYREGISTERED, usr.getNick());
        return;
    }
    if (args.size() != 1)
        usr.setPasswrd(args[args.size() -1]);
    else
        usr.setPasswrd(args[0]);
    checkPasswrd(usr, serv);
}
