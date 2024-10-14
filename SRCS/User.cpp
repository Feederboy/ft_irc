#include "header.hpp"

User::User(void)
{}

User::User(int fd): _fd(fd), _nickname(""), _username(""), _hostname(""), _registerStep(0), _currentChannel(NULL)
{}

User::User(const User& src)
{
    *this = src;
}

User::~User(void)
{
    if (!_channels.empty())
    {
        for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            (*it)->deleteChanOp(this);
            (*it)->deleteUser(this);
        }
    }
}

User    &User::operator=(const User &rhs)
{
    if (this != &rhs)
    {
        this->_fd = rhs._fd;
        this->_nickname = rhs._nickname;
        this->_username = rhs._username;
        this->_hostname = rhs._hostname;
        this->_realName = rhs._realName;
        this->_servername = rhs._servername;
        this->_password =rhs._password;
        this->_mode = rhs._mode;
        this->_registerStep =rhs._registerStep;
    }
    return (*this);
}

int             User::getFd(void)
{
    return (_fd);
}

std::string     User::getNick(void)
{
    return (_nickname);
}

std::string     User::getUsername(void)
{
    return (_username);
}

std::string     User::getRealName(void)
{
    return (_realName);
}

std::string     User::getHostname(void)
{
    return (_hostname);
}

std::string     User::getServername(void)
{
    return (_servername);
}

std::string     User::getPasswrd(void)
{
    return (_password);
}

std::string     User::getMode(void)
{
    return (_mode);
}

std::string     User::getBuffer(void)
{
    return (_bufferCmd);
}

bool            User::isRegister(void)
{
    if (_registerStep == 3)
        return (true);
    else
        return (false);
}

std::string     User::getNickmask(void)
{
    std::string     nickMask;

    nickMask = _nickname + "!" + _username + "@" + _hostname;
    return (nickMask);
}

Channel     *User::getCurrentChannel(void)
{
    return (_currentChannel);
}

bool            User::isRegisterToChannel(Channel *chan)
{
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if (chan == *it)
            return true;
    }
    return (false);
}

std::vector<Channel *>  User::getChannels(void)
{
    return (_channels);
}

void            User::addRegisterStep(unsigned int value)
{
    _registerStep = _registerStep + value;
}

void            User::setBufferCmd(std::string &buff)
{
    _bufferCmd = buff;
}

void            User::setFd(int fd)
{
    _fd = fd;
}

void            User::setNickname(std::string nick)
{
    _nickname = nick;
}

void            User::setUsername(std::string user)
{
    _username = user;
}

void            User::setRealName(const std::string &realName)
{
    _realName = realName;
}

void            User::setHostname(std::string host)
{
    _hostname = host;
}

void            User::setServname(std::string serv)
{
    _servername = serv;
}

void            User::setPasswrd(std::string pass)
{
    _password = pass;
}

void            User::setMode(std::string mode)
{
    _mode = mode;
}


void        User::addChannel(Channel *chan)
{
    if (_channels.size() < MAX_CHANNEL_PER_USER)
    {
        std::cout << _nickname << "'s joined channel " << chan->getName() << std::endl;
        _channels.push_back(chan);
    }
    else
    {
        std::cout << _nickname << " max number of channel reached." << std::endl;
    }
}

void        User::deleteChannel(Channel *chan)
{
    chan->deleteChanOp(this);
    chan->deleteUser(this);
    std::cout <<  "In User::deleteChannel after chan delete User\n";
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        std::cout << "During the loop in User::deleteChannel" << std::endl;
        if ((*it)->getName() == chan->getName())
        {
            this->setCurrentChannel(NULL);
            std::cout << _nickname << " left channel " << chan->getName() << std::endl;
            _channels.erase(it);
            return;
        }
    }
}

void    User::setCurrentChannel(Channel *chan)
{
    _currentChannel = chan;
    if (!chan)
        std::cout << _nickname << " isnt on any channel." << std::endl;
    else
        std::cout << _nickname << "'s current channel set to " << chan->getName() << std::endl;
}

std::ostream & operator<<(std::ostream &stream, User &user)
{
    stream << "[User] Nickname: " << user.getNick() << " NickMask: " << user.getNickmask();
    stream << "\nfd: " << user.getFd() << " password: " << user.getPasswrd();
    if (user.isRegister())
        stream << "\nUser is registered";
    else
        stream << "\nUser is unregister";
    return (stream);
}