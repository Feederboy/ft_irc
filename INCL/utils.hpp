#ifndef UTILS_HPP
# define UTILS_HPP

#include "header.hpp"

void            initErrReplies(void);
void            send_reply(User &usr, int replyValue, std::string text);
void            send_notice(User &sender, User &receiver, std::string textToSend);
void            endOfRegistration(User &usr, Server &serv);
void            send_error(User &usr, int ErrValue, std::string arg);
std::string     ft_join(std::vector<std::string> &src, std::string sep, int idx);
void            signalHandler(int signal);

template<typename T>
bool    hasDuplicates(std::vector<T> src)
{
    std::vector<T> tmp = src;
    typename std::vector<T>::iterator it;
    sort(tmp.begin(), tmp.end());

    it = adjacent_find(tmp.begin(), tmp.end());
    if (it == tmp.end())
        return (false);
    return (true);
}

template<typename T>
T       findDuplicates(std::vector<T> src)
{
    std::vector<T> tmp = src;
    typename std::vector<T>::iterator it;

    sort(tmp.begin(), tmp.end());
    it = adjacent_find(tmp.begin(), tmp.end());
    if (it == tmp.end())
        return (NULL);
    return (*it);
}


#endif