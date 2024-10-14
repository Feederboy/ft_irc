#include "header.hpp"
/*
**  Command: INVITE
**  Parameters: <nickname> <channel>
**
**  The INVITE command is used to invite a user to a channel. 
**  The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.
**  The target channel SHOULD exist (at least one user is on it).
**  Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.
**  Only members of the channel are allowed to invite other users.
**  Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.
**  Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
**  In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.
**  If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.
**  When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user.
**  Other channel members SHOULD NOT be notified.
**
**  NUMERIC REPLIES:
**      RPL_INVITING (341)  ERR_NEEDMOREPARAMS (461)    ERR_NOSUCHCHANNEL (403)
**      ERR_NOTONCHANNEL (442)  ERR_CHANOPRIVSNEEDED (482)  ERR_USERONCHANNEL(443)
**
**  Command example:
**      INVITE Wiz #foo_bar         [Invite Wiz to #foo_bar]
*/


void    invite(std::vector<std::string> args, User &usr, Server &serv)
{
    Channel*    chan;
    User*       target;

    if (args.size() < 2)
    {
        send_error(usr, ERR_NEEDMOREPARAMS, "INVITE");
        return;
    }

    chan = serv.getChannelByName(args[1]);
    if (!chan)
        return;
    if (!chan->userIsOnChan(&usr))
    {
        send_error(usr, ERR_NOTONCHANNEL, args[1]);
        return;
    }
    if (!chan->userIsOP(&usr) && chan->isInviteOnly())
    {
        send_error(usr, ERR_CHANOPRIVSNEEDED, args[1]);
        return;
    }

    target = serv.getUserByNickname(args[0]);
    if (!target)
    {
        send_error(usr, ERR_NOSUCHNICK, args[0]);
        return;
    }
    if (chan->userIsOnChan(target))
    {
        send_error(usr, ERR_USERONCHANNEL, args[0] + " " + args[1]);
        return;
    }
    send_notice(usr, *target, NOTIF_INVITE(chan->getName(), target->getNick()));
    send_reply(usr, RPL_INVITING, REPLY_INVITING(target->getNick(), chan->getName()));
    chan->invite(target);
    return;
}