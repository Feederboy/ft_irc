#include "header.hpp"

// Need totest ut, maybe change it to template

bool            checkPort(const std::string &src)
{
    int tmp;

    if (isDigit(src))
    {
        tmp = atoi(src.c_str());
        if (tmp <= 0 || tmp > 65535)
        {
            std::cerr << "Error : Available port must be between 1 and 65 535\n";
            return (false);
        }
        return (true);
    }
    std::cerr << "Error: port argument is not only digit" << std::endl;
    return (false);
}

bool            isDigit(const std::string &src)
{
    for (size_t i = 0; i < src.size(); i++)
    {
        if (src[i] < '0' || src[i] > '9')
            return (false);
    }
    return (true);
}

std::vector<std::string>    ft_split(std::string &src, std::string sep)
{
    std::vector<std::string> args;
    size_t      start;
    size_t      end;


    start = 0;
    end =  src.find(sep);
    while (end != std::string::npos)
    {
        args.push_back(src.substr(start, end - start));
        start = end + sep.size();
        end = src.find(sep, start);
    }
    args.push_back(src.substr(start, end - start));
    return (args);
}


bool        isValidStrForNick(const std::string &src)
{
    if (src.find(' ') != std::string::npos || src.find(',') != std::string::npos 
        || src.find('*') != std::string::npos || src.find('?') != std::string::npos 
        || src.find('!') != std::string::npos || src.find('@') != std::string::npos
        || src.find('.') != std::string::npos)
    {
        return (false);
    }
    else if (src[0] == '#' || src[0] == '&' || src[0] == '$' || src[0] == ':')
    {
        return (false);
    }
    return (true);
}

std::string     parseNickname(std::string &src)
{
    std::string nickName;

    if (src.empty() || !isValidStrForNick(src) || (src[0] == '#' || src[0] == '&'))
        return (nickName);
    if (src.size() > MAX_USR_NICK_LENGHT)
        nickName = src.substr(0, MAX_USR_NICK_LENGHT);
    else
        nickName = src;
    return (nickName);
}


std::map<std::string, cmdFunction>  initCmdMap(void)
{
    static std::map<std::string, cmdFunction>   ptrCmd;

    if (ptrCmd.empty())
    {
        ptrCmd["INVITE"] = invite;
        ptrCmd["JOIN"] = join;
        ptrCmd["KICK"] = kick;
        ptrCmd["MODE"] = mode;
        ptrCmd["NAMES"] = names;
        ptrCmd["NICK"] = nick;
        ptrCmd["NOTICE"] = notice;
        ptrCmd["PART"] = part;
        ptrCmd["PASS"] = pass;
        ptrCmd["PRIVMSG"] = privmsg;
        ptrCmd["QUIT"] = quit;
        ptrCmd["TOPIC"] = topic;
        ptrCmd["USER"] = user;
    }
    return (ptrCmd);
}

/*
**  Send the command without separating [ex: NICK vnr]
**  Need to separate the command from the parameters
**  Create function pointer on functions command
*/

int     parse_cmd(std::string &cmd, User &user, Server &serv)
{
    std::vector<std::string>            args;
    std::string                         cmdToExec;
    std::map<std::string, cmdFunction>  ptrFunction;
    std::map<std::string, cmdFunction>::iterator it;

    if (cmd.empty())
        return (1);
    args = ft_split(cmd, " ");
    cmdToExec = args[0];
    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i][args[i].size() - 1] == '\r')
            args[i].erase(args[i].size() - 1);
        if (args[i].empty())
            args.erase(args.begin() + i);
    }
    ptrFunction = initCmdMap();
    it =  ptrFunction.find(cmdToExec);
    if (it == ptrFunction.end())
    {
        if (user.getCurrentChannel() != NULL)
        {
            sendToAllChannel(user.getCurrentChannel(), cmd, user);
            return (0);
        }
        send_error(user, ERR_UNKNOWCOMMAND, cmdToExec);
        return (1);
    }

    /* Delete args[0] (command name) and call the function */
    args.erase(args.begin());
    it->second(args, user, serv);
    return (0);
}

bool    checkEndCommand(const std::string &src)
{
    if (!src.empty() && src[src.size() -1] == '\n' && src[src.size() - 2] == '\r')
        return (true);
    else
        return (false);
}

void    getNbCmd(std::string &src, User &usr, Server &serv)
{
    int     nbCmd;
    std::string     cmd;

    nbCmd = std::count(src.begin(), src.end(), '\n');
    while (nbCmd > 0)
    {
        size_t pos;
        pos = src.find('\n');
        if (src[pos - 1] == '\r')
        {
            cmd = src.substr(0, pos -   1);
        }
        else
            cmd =  src.substr(0, pos);
        src = src.substr(pos + 1);
        parse_cmd(cmd, usr, serv);
        cmd.clear();
        nbCmd--;
    }
}