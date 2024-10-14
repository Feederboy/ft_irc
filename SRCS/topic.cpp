#include "header.hpp"

/*
**  Command: TOPIC
**  Parameters: <channel> [<topic>]
**
**  The TOPIC command is used to change or view the topic of the given channel.
**  If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned
**  specifying the current channel topic or lack of one.
**  If <topic> is an empty string, the topic for the channel will be cleared.
**
**  If the client sending this command is not joined to the given channel, and tries to view its’ topic,
**  the server MAY return the ERR_NOTONCHANNEL (442) numeric and have the command fail.
**
**  Numeric Replies:
**      ERR_NEEDMOREPARAMS(461)     ERR_NOSUCHCHANNEL(403)  ERR_NOTONCHANNEL(442)
**      ERR_CHANOPRIVSNEEDED (482)
**      RPL_NOTOPIC (331)   RPL_TOPIC (332)     RPL_TOPICWHOTIME (333)
*/

void    sendTopicToMembers(Channel *chan, User &usr, std::string topic)
{
    std::vector<User *> usersOnChan;

    usersOnChan = chan->getMembers();
    for(std::vector<User *>::iterator it = usersOnChan.begin(); it != usersOnChan.end(); it++)
        send_notice(usr, *(*it), topic);
}

void    topic(std::vector<std::string> args, User &usr, Server &serv)
{
    Channel     *chan;
    std::string topic;

    std::cout << "In topic function, printing args" << std::endl;
    for (size_t i = 0; i < args.size(); i++)
        std::cout << "Args : " << args[i] << "\n";

    if (args.size() < 1)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "TOPIC");
        return;
    }
    chan = serv.getChannelByName(args[0]);
    if (!chan)
    {
        send_error(usr, ERR_NOSHUCHCHANNEL, args[0]);
        return;
    }
    if (!chan->userIsOnChan(&usr))
    {
        send_error(usr, ERR_NOTONCHANNEL, args[0]);
        return;
    }
    if (args.size() > 1)
    {
        if (chan->topicProtected() && !chan->userIsOP(&usr))
        {
            send_error(usr, ERR_CHANOPRIVSNEEDED, args[0]);
            return;
        }
        if (args[1].size() == 1 && args[1] == ":")
        {
            chan->unsetTopic();
        }
        else
        {
            topic = ft_join(args, " ", 1);
            if (topic[0] == ':')
                topic.erase(topic.begin());
            chan->setTopic(topic, usr);
        }
        sendTopicToMembers(chan, usr, NOTIF_TOPIC(chan->getName(), topic));
        return;
    }
    if (chan->hasTopicSet())
    {
        send_reply(usr, RPL_TOPIC, REPLY_TOPIC(chan->getName(), chan->getTopic()));
        send_reply(usr, RPL_TOPICWHOTIME, REPLY_TOPICWHOTIME(chan->getName(), chan->getTopicWho(), chan->getTopicWhen()));
    }
    else
        send_reply(usr, RPL_NOTOPIC, REPLY_NOTOPIC(chan->getName()));
}
