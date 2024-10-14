#include "header.hpp"

Server::Server(void){ }

Server::Server(const char* port,const std::string &passwrd) : _port(port), _passwrd(passwrd)
{
    _host = "127.0.0.1";
    _serverName = "IrcServer";
    time_t now = time(0);
    _creationDate = ctime(&now);
    _creationDate.erase(_creationDate.size() -1);
    std::cout << "Creation date: " << _creationDate << std::endl;
}

Server::Server(const Server& src)
{
    *this = src;
}

Server::~Server(void)
{
    close(_socketFd);
    if (!_users.empty())
    {
        for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
        {
            if (_users.empty())
                break;
            delete (*it);
            _users.erase(it);
        }
    }
    if (!_channel.empty())
    {
        for (std::vector<Channel *>::iterator iter = _channel.begin(); iter != _channel.end(); iter++)
        {
            if (_channel.empty())
                break;
            delete (*iter);
            _channel.erase(iter);
        }
    }
}

Server    &Server::operator=(const Server &rhs)
{

   if (this != &rhs)
    {
        this->_host = rhs._host;
        this->_serverName = rhs._serverName;
        this->_port = rhs._port;
        this->_hints = rhs._hints;
        this->_servInfo = rhs._servInfo;
        this->_socketFd = rhs._socketFd;
        this->_nbFd = rhs._nbFd;
        this->_newFd = rhs._newFd;
        this->_passwrd = rhs._passwrd;
    }
   return (*this);
}

std::string     Server::getHost(void)
{
    return (_host);
}

std::string     Server::getServName(void)
{
    return (_serverName);
}

int         Server::getStocketFd(void)
{
    return (_socketFd);
}

int         Server::getNbFd(void)
{
    return (_nbFd);
}

std::string     Server::getServPasswrd(void)
{
    return (_passwrd);
}

std::vector<User *>     Server::getUsers(void)
{
    return (_users);
}

std::vector<Channel *>  Server::getChannels(void)
{
    return (_channel);
}

std::string     Server::getCreationTime(void)
{
    return (_creationDate);
}

User        *Server::getUserByNickname(const std::string &nickname)
{
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->getNick() == nickname)
        {
            User *user = *it;
            return (user);
        }
    }
    return (NULL);
}

Channel         *Server::getChannelByName(const std::string &chanName)
{
    for (std::vector<Channel *>::iterator it =_channel.begin(); it != _channel.end(); it++)
    {
        if ((*it)->getName() == chanName)
        {
            Channel *chan = *it;
            return (chan);
        }
    }
    return (NULL);
}

bool        Server::userConnect(User &user)
{
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->getFd() == user.getFd())
        {
            return (true);
        }
    }
    return (false);
}



    /*
    **  Gathering Server information throught getaddrinfo
    */

void    Server::gatherServerInfo(void)
{
   int      status;

   std::cout << "Gathering Server informations .... ";
   memset(&_hints, 0, sizeof(_hints));
   _hints.ai_family = AF_INET;
   _hints.ai_socktype = SOCK_STREAM;
   _hints.ai_flags = AI_PASSIVE;
   if ((status =  getaddrinfo(_host.c_str(), _port, &_hints, &_servInfo)) != 0)
   {
        std::cerr << " KO" << std::endl;
        errno = status;
        throw Errfunc(std::strerror(errno));
   }
   std::cout << " DONE" << std::endl;
}

   /*
   ** Creating socket and setting it up to be bind
   ** SOL_SOCKET = set option at the socket Level
   ** SO_REUSEADDR = Reuse of the local address is supported
   */

int    Server::setSocket(struct addrinfo *tmp)
{
    int     yo = 1;

    _socketFd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
    if (_socketFd == -1)
        return (1);
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yo, sizeof(_socketFd)) == -1)
        throw ErrSocket(std::strerror(errno));
    if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
        throw ErrFcntl();
    return (0);
}

    /*
    **  Bind port with socket   
    */

int    Server::bindPort(struct addrinfo *tmp)
{
    std::cout << "Binding port " << _port << " ...";
    if (bind(_socketFd, tmp->ai_addr, tmp->ai_addrlen) == -1)
    {
        std::cerr << " KO\n";
        close(_socketFd);
        return (1);
    }
    std::cout << " DONE" << std::endl;
    return (0);
}

void    Server::listenHost(void)
{
    std::cout << "Start listening ... ";
    if (listen(_socketFd, MAXCLIENT) == -1)
    {
        std::cerr << " KO" << std::endl;
        close(_socketFd);
        throw ErrListen();
    }
    std::cout << " DONE" << std::endl;
}

void    Server::initSever(void)
{
    struct addrinfo *tmp;

    std::cout << "Init Server ....\n";
    this->gatherServerInfo();
    for (tmp = _servInfo; tmp != NULL; tmp = tmp->ai_next)
    {
        if (this->setSocket(tmp))
            continue;
        if (this->bindPort(tmp))
            continue;
        break;
    }
    freeaddrinfo(_servInfo);
    if (!tmp)
        throw ErrBind();
    this->listenHost();
    std::cout << "Server initialisation complete" << std::endl;
    this->pollLoop();
}

void    Server::acceptNewConnection(void)
{
    struct sockaddr_in hostAddr;
    socklen_t addrLen = sizeof(hostAddr);

    _newFd = accept(_socketFd, (struct sockaddr *)&hostAddr, &addrLen);
    if (_newFd == -1)
        throw ErrAccept(std::strerror(errno));
    std::cout << "New client #" << _newFd << " from " << inet_ntoa(hostAddr.sin_addr)
                << ":" << ntohs(hostAddr.sin_port) << std::endl;
}

    /*
    **  Verify if max number of client isn't reach
    **  -> Use send if max number of client reach to inform client
    **  -> Add the new FD on the pollFd struct
    **     Increment the nbFd available
    */

bool    Server::addNewFdToPollFd(int newFd)
{
    if (_nbFd == MAXCLIENT + 1)
    {
        std::cerr << "Max number of client reach\n";
        std::string errMsg = ERR_SERVFULL(_host);
        send(newFd, &errMsg[0],errMsg.size(), 0);
        return false;
    }
    _pollFd[_nbFd].fd = newFd;
    _pollFd[_nbFd].events = POLLIN;
    _nbFd++;
    return true;
}

int     Server::receiveData(int i)
{
    char    buffer[BUFF_LEN];
    int     bytes;

    memset(buffer, 0, BUFF_LEN);
    bytes = recv(_pollFd[i].fd, buffer, BUFF_LEN - 1, 0);
    if (bytes <= 0)
    {
        if (!bytes)
        {
            std::cout << "Clent #" << _pollFd[i].fd << " gone away" << std::endl;
            delete_from_pollFd(_pollFd[i].fd);
            deleteUser(_users[i - 1]);
            return (0);
        }
        if (bytes == -1)
        {
            throw ErrRecv(std::strerror(errno));
        }
    }
    getNextCmd(buffer, *_users[i - 1]);
    return (1);
}

void    Server::pollLoop(void)
{
    _pollFd[0].fd = _socketFd;
    _pollFd[0].events = POLLIN;
    _nbFd = 1;

    while (1)
    {
        int poll_count = poll(_pollFd, _nbFd, -1);
        if (poll_count < 0)
            throw ErrPoll(std::strerror(errno));
        for (int i = 0; i < _nbFd; i++)
        {
            if (_pollFd[i].revents & POLLIN)
            {
                // THere is a new connection
                if (_pollFd[i].fd == _socketFd)
                {
                    // Need to accept the new connection
                    this->acceptNewConnection();
                    if (this->addNewFdToPollFd(_newFd))
                    {
                        // create new User and add it to User* linked list 
                        _users.push_back(new User(_newFd));
                        break;
                    }
                }
                else
                    this->receiveData(i);
            }
        }
    }
}

void        Server::delete_from_pollFd(int fd)
{
    int i;

    i = 0;
    while (i < _nbFd)
    {
        if (_pollFd[i].fd == fd)
            break;
        i++;
    }
    if (i == _nbFd)
        return;
    _pollFd[i] = _pollFd[_nbFd - 1];
    _pollFd[i].events = POLLIN;
    close(fd);
    _nbFd--;
}

int     Server::getNextCmd(std::string buff, User &usr)
{
    std::string     tmp;

    //std::cout << "Buff receive in getNexCmd:[" << buff << "]\nSize: " << buff.size() << std::endl;
    if (!usr.getBuffer().empty())
        tmp = usr.getBuffer();
    tmp.append(buff);
    if (!checkEndCommand(buff))
    {
        //std::cout << "In checkEndCommand false condition" << std::endl;
        usr.setBufferCmd(tmp);
        return (1);
    }
    else 
    {
        std::string     empty;

        //std::cout << "In else condition\n";
        usr.setBufferCmd(empty);
        getNbCmd(tmp, usr, *this);
        return (0);
    }
}

void    Server::addChannel(Channel *chan)
{
    _channel.push_back(chan);
}

void    Server::deleteChannel(Channel *chan)
{

    for (std::vector<Channel *>::iterator it = _channel.begin(); it != _channel.end(); it++)
    {
        if ((*it)->getName() == chan->getName())
        {
            delete *it;
            _channel.erase(it);
            return;
        }
    }
}

void    Server::deleteUser(User *user)
{
    std::vector<Channel *>  usersChan;

    usersChan = user->getChannels();
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->getNick() == user->getNick())
        {
            delete (*it);
            _users.erase(it);
            break;
        }
    }
    if (usersChan.size() < 1)
    {
        for (std::vector<Channel *>::iterator iter = usersChan.begin(); iter != usersChan.end(); iter++)
        {
            if ((*iter)->getMembers().empty())
                deleteChannel(*iter);
        }
    }
}
