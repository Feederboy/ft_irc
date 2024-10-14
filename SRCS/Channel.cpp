#include "../INCL/header.hpp"

Channel::Channel(void)
{ }

Channel::Channel(std::string name, std::string key, std::string topic, User *usr, std::string mode): _name(name), _topic(topic), _mode(mode), _moderate(false), _limits(MAX_USER_PER_CHAN)
{
    if (!key.empty())
        this->setKey(key);
    else
        this->unsetKey();
    if (topic.empty())
        unsetTopic();
    else
        this->setTopic(topic, *usr);
    addUser(usr);
    addChanOP(usr);
}

Channel::Channel(const Channel& src)
{
    *this = src;
}

Channel::~Channel(void)
{ }

Channel    &Channel::operator=(const Channel &rhs)
{
    if (this != &rhs)
    {
        this->_name = rhs._name;
        this->_topic = rhs._topic;
        this->_hasTopic = rhs._hasTopic;
        this->_topicWho = rhs._topicWho;
        this->_topicWhen = rhs._topicWhen;
        this->_key = rhs._key;
        this->_hasKey = rhs._hasKey;
        this->_members = rhs._members;
        this->_chanOP = rhs._chanOP;
        this->_mode = rhs._mode;
        this->_moderate = rhs._moderate;
        this->_moderaters = rhs._moderaters;
        this->_creation_date = rhs._creation_date;
        this->_limits = rhs._limits;
        this->_banMask =rhs._banMask;
        this->_invitedUsers = rhs._invitedUsers;
    }
    return (*this);
}

void    Channel::setName(const std::string &name)
{
    _name = name;
}

void    Channel::setTopic(const std::string &topic, User &usr)
{
    _topic = topic;
    _topicWho = usr.getNick();
    _topicWhen = time(0);
    _hasTopic = true;
}

void    Channel::unsetTopic(void)
{
    _topic = "";
    _hasTopic = false;
    _topicWho = "";
}

void    Channel::setKey(const std::string &key)
{
    _key = key;
    _hasKey = true;
}

void    Channel::unsetKey(void)
{
    _key = "";
    _hasKey = false;
}

void    Channel::setMode(const std::string &mode)
{
    _mode = mode;
    if (mode.find('k') != std::string::npos && !_key.empty())
        _hasKey = true;
    else
        _hasKey = false;
    if (mode.find('m') != std::string::npos)
        _moderate = true;
    else
        _moderate = false;
    if (!_topic.empty())
        _hasTopic = true;
    else
        _hasTopic = false;
}

void    Channel::setCreationDate(double time)
{
    _creation_date = time;
}

void    Channel::addUser(User *usr)
{
    _members.push_back(usr);
}

void    Channel::deleteUser(User *usr)
{
    for (std::vector<User *>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if ((*it) == usr)
        {
            _members.erase(it);
            return;
        }
    }
}

void    Channel::addChanOP(User *usr)
{
    _chanOP.push_back(usr);
}

void    Channel::deleteChanOp(User *usr)
{
    for (std::vector<User *>::iterator it = _chanOP.begin(); it != _chanOP.end(); it++)
    {
        if ((*it) == usr)
        {
            _chanOP.erase(it);
            std::cout << "In channel::deleteChanOP after erase ChanOP" << std::endl;
            return;
        }
    }
}

void        Channel::setModerate(bool value)
{
    _moderate = value;
}

void        Channel::addModerater(User *usr)
{
    _moderaters.push_back(usr);
}

void        Channel::deleteModerater(User *usr)
{
    for (std::vector<User *>::iterator it = _moderaters.begin(); it != _moderaters.end(); it++)
    {
        if ((*it) == usr)
        {
            _moderaters.erase(it);
            return;
        }
    }
}

void    Channel::setLimit(int limit)
{
    if (limit > 0 && limit < MAX_USER_PER_CHAN)
        _limits = limit;
    else
        _limits = MAX_USER_PER_CHAN;
}

void    Channel::unsetLimit(void)
{
    _limits = -1;
}

void    Channel::ban( std::string &banMask)
{
    _banMask.push_back(banMask);
}

void    Channel::unban( std::string &banMask)
{
    for (std::vector<std::string>::iterator it = _banMask.begin(); it != _banMask.end(); it++)
    {
        if ((*it) == banMask)
        {
            _banMask.erase(it);
            return;
        }
    }
}

void    Channel::invite(User *usr)
{
    _invitedUsers.push_back(usr);
}

std::string Channel::getName(void) const
{
    return (_name);
}

std::string Channel::getTopic(void) const
{
    return (_topic);
}

std::string Channel::getKey(void) const
{
    return (_key);
}

std::string Channel::getMode(void) const
{
    return (_mode);
}

bool        Channel::hasKey(void) const
{
    return (_hasKey);
}

// AHAH  je bug
std::string      Channel::getCreationDate(void) const
{
    std::ostringstream  tmp;
    std::string         ret;

    tmp << std::fixed << _creation_date;
    ret = tmp.str();
    return (ret);
}

std::vector<User *>     Channel::getMembers(void) const
{
    return (_members);
}

std::vector<User *>     Channel::getChanOP(void) const
{
    return (_chanOP);
}

std::string     Channel::getMembersList(void)
{
    std::ostringstream  tmp;
    std::string         ret;

    for (std::vector<User *>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if (this->userIsOP(*it))
            tmp << "@" << (*it)->getNick() << " ";
        else
            tmp << (*it)->getNick() << " ";
    }
    ret = tmp.str();
    return (ret);
}

int         Channel::getNbMembers(void)
{
    return (_members.size());
}

int         Channel::getLimit(void)
{
    if (_limits > 0)
        return (_limits);
    else 
        return (MAX_USER_PER_CHAN);
}

std::string Channel::getTopicWho(void)
{
    return (_topicWho);
}

std::string Channel::getTopicWhen(void)
{
    std::ostringstream  tmp;
    std::string         ret;

    tmp << std::fixed << _topicWhen;
    ret = tmp.str();
    return (ret);
}

std::string Channel::getBannedList(void)
{
    std::string banList;

    for (size_t i = 0; i < _banMask.size(); i++)
    {
        banList.append(_banMask[i]);
        if (i != _banMask.size() - 1)
            banList.append(", ");
    }
    return (banList);
}

bool        Channel::hasTopicSet(void)
{
    return (_hasTopic);
}

bool        Channel::isInviteOnly(void)
{
    if (_mode.find('i') != std::string::npos)
        return (true);
    else
        return (false);
}

bool        Channel::userIsInvited(User &usr)
{
    if (find(_invitedUsers.begin(), _invitedUsers.end(), &usr) != _invitedUsers.end())
        return (true);
    else
        return (false);
}

bool        Channel::isSecret(void)
{
    if (_mode.find("s") == std::string::npos)
        return (false);
    return (true);
}

bool        Channel::userIsOnChan(User *usr)
{
    for (std::vector<User *>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if ((*it)->getNick() == usr->getNick())
            return (true);
    }
    return (false);
}

bool        Channel::userIsOP(User *usr)
{
    for (std::vector<User *>::iterator it = _chanOP.begin(); it != _chanOP.end(); it++)
    {
        if ((*it)->getNick() == usr->getNick())
            return (true);
    }
    return (false);
}

bool        Channel::isModerate(void)
{
    return (_moderate);
}

bool        Channel::userIsModerater(User &usr)
{
    for (std::vector<User *>::iterator it = _moderaters.begin(); it != _moderaters.end(); it++)
    {
        if ((*it)->getNick() == usr.getNick())
            return (true);
    }
    return (false);
}

bool        Channel::isBanned(User &usr)
{
    std::string nickmask;

    nickmask = usr.getNickmask();
    for (size_t i = 0; i < _banMask.size(); i++)
    {
        if (nickmask == _banMask[i])
            return (true);
    }
    return (false);
}

bool    Channel::topicProtected(void)
{
        std::cout << "[Topic protected]Mode for the channel " << _name << " : " << _mode << std::endl;
    if (_mode.find('t') != std::string::npos)
    {
        std::cout << "Channel mode t is set" << std::endl;
        return (true);
    }
    return (false);
}

void        Channel::sendMessage(User &usr, const std::string &msg)
{
    std::ostringstream tmp;
    std::string         formatedMsg;

    tmp << _name << ":";
    if (userIsOP(&usr))
        tmp << "@";
    tmp << usr.getNick() << ": " << msg << "\r\n";
    formatedMsg = tmp.str();
    for (std::vector<User *>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if ((*it)->getNick() != usr.getNick())
        {
            if (send ((*it)->getFd(), &formatedMsg[0], formatedMsg.size(), 0) == -1)
            {
                std::cout << "Error sending message in Channel::sendMessage()" << std::endl;
                return;
                // Throw Error here
            }
        }
    }
}

std::ostream & operator<<(std::ostream &stream, Channel &chan)
{
    stream << "Channel name: " << chan.getName();
    stream << " created by " << chan.getChanOP()[0]->getNick();
    stream << "\n Channel mode: " << chan.getMode() << "\n";
    if (chan.hasKey())
        stream << " Channel is private with key: " << chan.getKey();
    else
        stream << "Channel is public";
    return (stream);
    
}