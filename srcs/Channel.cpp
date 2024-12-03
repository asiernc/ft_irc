/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anovio-c <anovio-c@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:58:33 by anovio-c          #+#    #+#             */
/*   Updated: 2024/11/30 12:22:37 by anovio-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &channelName, const std::string &key, Client *creator)
	: _name(channelName),_operatorClient(creator) {
	if (channelName.empty() || (channelName[0] != '&' && channelName[0] != '#'))
		throw std::invalid_argument("Invalid channel name. Must start with '&' or '#'.");
	_clients.insert(creator);
	// invitation mode, by default is false
	_modes['i'] = false;
	if (key.empty())
		_key = "";
	else
		_key = key;
	_limit = 999; // ??
	std::cout << "Channel '" << channelName << "' created by " << creator->getUsername() << ".\n";
}

Channel::~Channel() { }

std::string	Channel::getName() { return this->_name ; }

bool		Channel::getMode(char key) {
	if (_modes.find(key) != _modes.end()) {
        return _modes[key];
    }
    return false;
}

void	Channel::setMode(char mode, bool status, int value) {

// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// l: Set/remove the user limit to channel
	_modes[mode] = status;
	std::cout << "Channel " << _name << " mode " << mode << " set to " << (status ? "ON" : "OFF") << ".\n";
}

bool	Channel::isEmtpy() { return this->_clients.empty() ; }

void	Channel::leave(Client *client) {
	std::unordered_set<Client *>::iterator it;
	it = _clients.find(client);
	if (it != _clients.end()) {
		// send message all of clients before erase.
		_clients.erase(it);
	}
	
	if (_clients.empty())
		delete this;
}
