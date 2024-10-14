#ifndef PARSING_HPP
# define PARSING_HPP

# include "header.hpp"

/*  Typedef for function pointer to map it later in parse command  */
typedef void (*cmdFunction) (std::vector<std::string>, User&, Server&);

bool                                checkPort(const std::string &src);
bool                                isDigit(const std::string &src);
std::vector<std::string>            ft_split(std::string &src, std::string sep);
bool                                isValidStrForNick(const std::string &src);
std::string                         parseNickname(std::string &src);
std::map<std::string, cmdFunction>  initCmdMap(void);
int                                 parse_cmd(std::string &cmd, User &user, Server &serv);
bool                                checkEndCommand(const std::string &src);
void                                getNbCmd(std::string &src, User &usr, Server &serv);

#endif