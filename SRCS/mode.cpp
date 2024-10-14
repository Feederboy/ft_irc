#include "header.hpp"

/*
**  Command : MODE
**
**  1. Channel mode:
**     Parameters: <channel> {[+|-] o|p|s|i|t|n|b|v|k|l} [<user>]
**                                          [<limit>] [<ban mask>]
**
**  The MODE command is provided so that channel operators may change the
**  characteristics of `their' channel.  It is also required that servers
**  be able to change channel modes so that channel operators may be created.
**
**      All mode:
**          o - give/take channel operator privileges;
**          p - private channel flag;
**          s - secret channel flag;
**          i - invite-only channel flag;
**          t - topic settable by channel operators only flag;
**          n - no message to channel from client outside;
**          m - moderated channel;
**          b - set/remove a ban mask to kepp User out flag;
**          v - give/take ability to speak on moderated channel;
**          k - set/remove channel key (password);
**          l - set/remove  the user limit to channel;
**
**  Numeric reples:
**      ERR_NEEDMOREPARAMS (461)    ERR_CHANOPRIVSNEEDED (482)  ERR_NOSUCHNICK (401)
**      ERR_NOTONCHANNEL (442)      ERR_NOSUCHCHANNEL (403)     ERR_UNKNOWNMODE (472)
**      ERR_INVALIDKEY (525)
**      RPL_CHANNELMODEIS (324)     RPL_CREATIONTIME (329)      RPL_INVITELIST (336)
**      RPL_ENDOFINVITELIST (337)   RPL_BANLIST (367)           RPL_ENDOFBANLIST (368)
*/



/*
**  The mode o l v k need to have args with them when set.
*/

std::string     addModeToChannel(std::string &modeWanted, std::vector<std::string> args, Channel *chan, User &usr, Server &serv)
{

    std::string     modeWithArgs = "olvk";
    User*           targetedUser;
    int             lim;
    std::string     channelMode;

    channelMode = chan->getMode();
    for (size_t i = 0; i < modeWanted.size(); i++)
    {
        if (modeWithArgs.find(modeWanted[i]) != std::string::npos && args.size() < 3)
        {
            send_error(usr, ERR_NEEDMOREPARAMS, args[0]);
            return ("");
        }
        if (modeWanted[i] == 'o')
        {
            targetedUser = serv.getUserByNickname(args[2]);
            if (!targetedUser)
            {
                send_error(usr, ERR_NOSUCHNICK, args[2]);
                return ("");
            }
            chan->addChanOP(targetedUser);
        }
        else if (modeWanted[i] == 'l')
        {
            lim = atoi(args[2].c_str());
            if (lim > 0)
                chan->setLimit(lim);
        }
        else if (modeWanted[i] == 'b' && args.size() > 2)
        {
            chan->ban(args[2]);
        }
        else if (modeWanted[i] == 'v')
        {
            if (!chan->isModerate())
            {
                return ("");
            }
            targetedUser = serv.getUserByNickname(args[2]);
            if (!targetedUser)
            {
                send_error(usr, ERR_NOSUCHNICK, args[2]);
                return ("");
            }
            chan->addModerater(targetedUser);
        }
        else if (modeWanted[i] == 'k' && args.size() < 4)
        {
            if (chan->hasKey())
            {
                send_error(usr, ERR_KEYSET, args[2]);
                return ("");
            }
            chan->setKey(args[2]);
        }
        if (channelMode.find(modeWanted[i]) == std::string::npos)
            channelMode.append(&modeWanted[i]);
    }
    return (channelMode);
}

/*
**  The mode o b v need to have args with them when unset
*/

std::string     removeModeToChannel(std::string &modeWanted, std::vector<std::string> args, Channel *chan, User &usr, Server &serv)
{
    User    *targetedUser;
    std::string modeWithArgs = "obv";
    std::string channelMode;

    channelMode = chan->getMode();
    for (size_t i = 0; i < modeWanted.size(); i++)
    {
        if (modeWithArgs.find(modeWanted[i]) != std::string::npos && args.size() < 3)
        {
            send_error(usr, ERR_NEEDMOREPARAMS, args[0]);
            return ("");
        }
        size_t  pos =  channelMode.find(modeWanted[i]);
        if (modeWanted[i] == 'o')
        {
            targetedUser = serv.getUserByNickname(args[2]);
            if (!targetedUser)
            {
                send_error(usr, ERR_NOSUCHNICK, args[2]);
                return ("");
            }
            chan->deleteChanOp(targetedUser);
        }
        else if (modeWanted[i] == 'l')
        {
            chan->setLimit(MAX_USER_PER_CHAN);
        }
        else if (modeWanted[i] == 'b')
        {
                chan->unban(args[2]);
        }
        else if (modeWanted[i] == 'v')
        {
            if (!chan->isModerate())
                return ("");
            targetedUser = serv.getUserByNickname(args[2]);
            if (!targetedUser)
            {
                send_error(usr, ERR_NOSUCHNICK, args[2]);
                return ("");
            }
            chan->deleteModerater(targetedUser);
        }
        else if (modeWanted[i] == 'k')
            chan->unsetKey();
        if (pos != std::string::npos)
            channelMode.erase(channelMode.begin() + pos);
    }
    return (channelMode);
}


bool    checkMode(std::string mode, User &usr)
{
    std::string     modeAvailable = AVAILABLE_CHANNEL_MODE;

    for (size_t i = 0; i < mode.size() - 1; i++)
    {
        if (modeAvailable.find(mode[i]) == std::string::npos)
        {
            send_error(usr, ERR_UNKNOWNMODE, &mode[i]);
            return (false);
        }
    }
    return (true);
}

void    channelMode(std::vector<std::string> args, User &usr, Server &serv)
{
    Channel *chan;
    char    modeSet;
    std::string     modeWanted;
    std::string     channelMode;

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
    if (args.size() == 1)
    {
        send_reply(usr, RPL_CHANNELMODEIS, REPLY_CHANNELMODEIS(chan->getName(), chan->getMode()));
        send_reply(usr, RPL_CREATIONTIME, REPLY_CREATIONTIME(chan->getName(), chan->getCreationDate()));
        return;
    }

    if ((args[1] == "b" || args[1] == "+b") && args.size() < 3)
    {
        send_reply(usr, RPL_BANLIST, REPLY_BANLIST(chan->getName(), chan->getBannedList()));
        send_reply(usr, RPL_ENDOFBANLIST, REPLY_ENDOFBANLIST(chan->getName()));
    }
    if (args[1][0] == '+' || args[1][0] == '-')
    {
        modeSet = args[1][0];
        modeWanted = &args[1][1];
    }
    else
    {
        modeSet = '\0';
        modeWanted = args[1];
    }
    if (modeSet && !chan->userIsOP(&usr))
    {
        send_error(usr, ERR_CHANOPRIVSNEEDED, args[0]);
        return;
    }
    if (modeSet && ((modeWanted.find('o') || modeWanted.find('b')) && modeWanted.size() > 3))
    {
        send_error(usr, ERR_CHANOPRIVSNEEDED, args[0]);
        return;
    }
    if (!checkMode(modeWanted, usr))
        return;
    if (modeSet == '+')
    {
        channelMode = addModeToChannel(modeWanted, args, chan, usr, serv);
        if (channelMode.empty())
            return;
    }
    else if (modeSet == '-')
        channelMode = removeModeToChannel(modeWanted, args, chan, usr, serv);
    chan->setMode(channelMode);
}


void    mode(std::vector<std::string> args, User &usr, Server &serv)
{
    if (args.size() < 1)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "MODE");
        return;
    }

    if (args[0].find('#') == std::string::npos)
        return;
    else
        channelMode(args, usr, serv);
    return;
}