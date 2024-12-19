/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anovio-c <anovio-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:44:07 by anovio-c          #+#    #+#             */
/*   Updated: 2024/12/19 11:55:45 by anovio-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

// 403: Canal no existe.
// 441: Usuario no está en el canal.
// 461: Parámetros insuficientes.
// 482: Cliente no es administrador.

/*   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_NOTONCHANNEL*/

/* static void	reviewChannels(std::vector<std::string> &old) {
	std::vector<std::string> filtered;

	for (size_t i = 0; i < old.size(); ++i) {
		if (std::find(filtered.begin(), filtered.end(), old[i]) == filtered.end())
            filtered.push_back(old[i]);
    }
	old = filtered;
	//for (size_t i = 0; i < filtered.size(); ++i)
	//	std::cout << "STR ==> " << filtered[i] << std::endl;
} */

static bool	checkInput(std::vector<std::string> &tokens) {
	size_t	count = 0;
	
	for (size_t i = 0; i < tokens.size(); ++i ) {
		if (tokens[i][0] == '#' || tokens[i][0] == '&')
			count++;
	}
	if (count > 1)
		return false;
	return true;
}

static std::vector<std::string> extractChannels(std::string &channels) {
	std::vector<std::string>	parsed;
	bool						isInvalid = false;

	parsed = split(channels, ',');
	for (size_t i = 0; i < parsed.size(); ++i) {
		if (parsed[i][0] != '#' && parsed[i][0] != '&') {
			isInvalid = true;
			break ;
		}
	}
	return ((isInvalid) ? std::vector<std::string>() : parsed);
}

static void extractReason(std::string &args, std::string &reason) {

	size_t reasonIndex = args.find(':');
	if (reasonIndex == std::string::npos) {
		reason = "No reason specified.";
		return ;
	}
	reason = args.substr(reasonIndex + 1);
	args = args.substr(0, reasonIndex);
}

static std::string	makeBroadcastMessage(Client *client, const std::string &channelName, std::string &userToKick, std::string &reason) {
	
	std::ostringstream oss;
	
	// :<nickname>!<username>@<hostname> JOIN :<channelName>\r\n
    oss << ":" << client->getNickname() << "!" << client->getUsername()
        << "@" << "127.0.0.1" << " KICK " << channelName << " "
        << userToKick << " :" << reason << "\r\n";

	return (oss.str());
}

// KICK <channel>{,<channel>} ??<channel>{,<channel>}??  <user>{,<user>} [<comment>]
// KICK #canal1 asier2 :hola y adios
// ERR_NOSUCHNICK (401)

void	kick(Client *client, std::string &args) {
	if (args.empty()) {
		sendError(client, 461, "ERR_NEEDMOREPARAMS");
		return ;
	}
	
	std::string	reason;
	extractReason(args, reason);
	
	std::vector<std::string>	tokens = split(args, ' ');
	if (tokens.size() != 2 || !checkInput(tokens)) {
		sendError(client, 461, "ERR_NEEDMOREPARAMS");
		return ;
	}
	
	std::vector<std::string>	channels = extractChannels(tokens[0]);
	if (channels.empty()) {
        sendError(client, 403, "ERR_NOSUCHCHANNEL - Invalid channel(s)");
        return;
    }
	std::vector<std::string>	usersList = split(tokens[1], ',');

	for (size_t i = 0; i < channels.size(); ++i) {
		const std::string &channelName = channels[i];
		
		Channel *channel = Server::getInstance().getCheckChannel(channelName);

		if (!channel) {
			sendError(client, 403, "ERR_NOSUCHCHANNEL");
			continue ;
		}
		
		if (!channel->checkOperatorClient(client)) {
			sendError(client, 482, "ERR_CHANOPRIVSNEEDED");
			continue ;
		}

		for (size_t j = 0; j < usersList.size(); ++j) {
			
			std::string userToKick = usersList[j];
			
			if (userToKick.empty()) {
				sendError(client, 482, "ERR_NEEDMOREPARAMS - No user specified for channel " + channelName);
				continue ;
			}

            if (userToKick[0] == ':')
                userToKick = userToKick.substr(1);

			Client *toKick = channel->checkClient(userToKick);
			
			if (!toKick) {
				sendError(client, 442, "ERR_NOTONCHANNEL");
				continue ;
			}

			std::string msg = makeBroadcastMessage(client, channelName, userToKick, reason);

			channel->broadcast(client, msg);
			channel->rmClient(toKick);
		}
	}
}
