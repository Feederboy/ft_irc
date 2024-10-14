#ifndef USER_HPP
# define USER_HPP

#include "header.hpp"
#include "Channel.hpp"

class Channel;

class User
{
    public:
        User(void);
        User(int fd);
        User(const User& src);
        ~User(void);
        User& operator=(const User& rhs);

        int                     getFd(void);
        std::string             getNick(void);
        std::string             getUsername(void);
        std::string             getRealName(void);
        std::string             getHostname(void);
        std::string             getServername(void);
        std::string             getPasswrd(void);
        std::string             getMode(void);
        std::string             getBuffer(void);
        bool                    isRegister(void);
        std::string             getNickmask(void);
        Channel                 *getCurrentChannel(void);
        bool                    isRegisterToChannel(Channel *chan);
        std::vector<Channel *>  getChannels(void);

        void            addRegisterStep(unsigned int value);
        void            setBufferCmd(std::string &buff);
        void            setFd(int fd);
        void            setNickname(std::string nick);
        void            setUsername(std::string user);
        void            setRealName(const std::string &realName);
        void            setHostname(std::string host);
        void            setServname(std::string serv);
        void            setPasswrd(std::string pass);
        void            setMode(std::string mode);
        void            addChannel(Channel *chan);
        void            deleteChannel(Channel *chan);
        void            setCurrentChannel(Channel *chan);


    private:
        std::string             _bufferCmd;
        int                     _fd;
        std::string             _nickname;
        std::string             _username;
        std::string             _hostname;
        std::string             _realName;
        std::string             _servername;
        std::string             _password;
        std::string             _mode;
        unsigned int            _registerStep;
        Channel*                _currentChannel;
        std::vector<Channel *>  _channels;
};

std::ostream & operator<<(std::ostream &stream, User &user);

#endif