#ifndef SERVER_HPP
# define SERVER_HPP

#include "header.hpp"

class Server
{
    public:
        Server(const char *port, const std::string &passwrd);
        Server(const Server& src);
        ~Server(void);
        Server& operator=(const Server& rhs);
        std::string             getHost(void);
        std::string             getServName(void);
        int                     getStocketFd(void);
        int                     getNbFd(void);
        std::string             getServPasswrd(void);
        std::vector<User *>     getUsers(void);
        std::vector<Channel *>  getChannels(void);
        std::string             getCreationTime(void);
        User *                  getUserByNickname(const std::string &nickname);
        Channel*                getChannelByName(const std::string &chanName);

        void                    initSever(void);
        void                    delete_from_pollFd(int fd);
        bool                    userConnect(User &user);
        void                    addChannel(Channel *chan);
        void                    deleteChannel(Channel *chan);
        void                    deleteUser(User *user);

    private:
        std::string             _host;
        std::string             _serverName;
        const char*             _port;
        struct addrinfo         _hints;
        struct addrinfo         *_servInfo;
        struct pollfd           _pollFd[MAXCLIENT + 1];
        int                     _socketFd;
        int                     _nbFd;
        int                     _newFd;
        std::string             _passwrd;
        std::string             _creationDate;
        std::vector<User *>     _users;
        std::vector<Channel *>  _channel;
        
        Server(void);
        int     getNextCmd(std::string buff, User &usr);
        void    gatherServerInfo(void);
        int     setSocket(struct addrinfo *tmp);
        int     bindPort(struct addrinfo *tmp);
        void    listenHost(void);
        void    pollLoop(void);
        void    acceptNewConnection(void);
        bool    addNewFdToPollFd(int newFd);
        int     receiveData(int i);

};

#endif