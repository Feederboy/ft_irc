#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "header.hpp"


class User;
class Server;
/*
**  Key for channel is like having a password to join it
*/

class Channel
{
    public:
        Channel(void);
        Channel(std::string name, std::string key, std::string topic, User *usr, std::string mode);
        Channel(const Channel& src);
        ~Channel(void);
        Channel& operator=(const Channel& rhs);
        void    setName(const std::string &name);
        void    setTopic(const std::string &topic, User &usr);
        void    unsetTopic(void);
        void    setKey(const std::string &key);
        void    unsetKey(void);
        void    setMode(const std::string &mode);
        void    setCreationDate(double time);
        void    addUser(User *usr);
        void    deleteUser(User *usr);
        void    addChanOP(User *usr);
        void    deleteChanOp(User *usr);
        void    setModerate(bool value);
        void    addModerater(User *usr);
        void    deleteModerater(User *usr);
        void    setLimit(int limit);
        void    unsetLimit(void);
        void    ban( std::string &banMask);
        void    unban( std::string &banMask);
        void    invite(User *usr);


        std::string             getName(void) const;
        std::string             getTopic(void) const;
        std::string             getKey(void) const;
        std::string             getMode(void) const;
        bool                    hasKey(void) const;
        std::string             getCreationDate(void) const;
        std::vector<User *>     getMembers(void) const;
        std::vector<User *>     getChanOP(void) const;
        std::string             getMembersList(void);
        int                     getNbMembers(void);
        int                     getLimit(void);
        std::string             getTopicWho(void);
        std::string             getTopicWhen(void);
        std::string             getBannedList(void);

        bool                    hasTopicSet(void);
        bool                    isInviteOnly(void);
        bool                    userIsInvited(User &usr);
        bool                    isSecret(void);
        bool                    userIsOnChan(User *usr);
        bool                    userIsOP(User *usr);
        bool                    isModerate(void);
        bool                    userIsModerater(User &usr);
        bool                    isBanned(User &usr);
        bool                    topicProtected(void);
        void                    sendMessage(User &usr, const std::string &msg);


    private:
        std::string                 _name;
        std::string                 _topic;
        bool                        _hasTopic;
        std::string                 _topicWho;
        std::string                 _topicWhen;
        std::string                 _key;
        bool                        _hasKey;
        std::vector<User *>         _members;
        std::vector<User *>         _chanOP;
        std::string                 _mode;
        bool                        _moderate;
        std::vector<User *>         _moderaters;
        double                      _creation_date;
        int                         _limits;
        std::vector<std::string>    _banMask;
        std::vector<User *>         _invitedUsers;

};

std::ostream & operator<<(std::ostream &stream, Channel &chan);

#endif