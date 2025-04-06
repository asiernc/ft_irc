<div align="center">
  <h1 align="center">
    <br>ft_irc - IRC Server in C++98</br>
  </h1>
  <p align="center">
    <img src="https://img.shields.io/badge/42-FT_IRC-blue?style=flat-square" alt="42 ft_irc"/>
  </p>
</div>

## Table of Contents
- [🧠 Summary](#-summary)
- [💬 Features](#-features)
- [🚀 Usage](#-usage)
- [🖼 Screenshots](#-screenshots)
- [🛠 Installation](#-installation)

---

## 🧠 Summary

**ft_irc** is an IRC server fully developed in C++98 as part of the 42 program. The project aims to create a functional server from scratch, capable of handling multiple clients simultaneously using non-blocking I/O operations and a single `poll()` call (or equivalent).

---

## 💬 Features

### 🔐 Authentication & Connection
- Connect via specified port/password using command-line arguments
- TCP/IP communication (IPv4/IPv6 compatible)

### 👥 User Management
- Set nicknames and usernames
- Private messaging between users

### 📺 Channels
- Create/join channels
- Broadcast messages to all channel members

### ⚙️ Channel Operator Commands
- `KICK`: Remove user from channel
- `INVITE`: Invite user to channel
- `TOPIC`: View/change channel topic
- `MODE`: Advanced channel configuration:
  - `i`: Invite-only channel
  - `t`: Only operators can change topic
  - `k`: Set channel password
  - `o`: Grant/remove operator privileges
  - `l`: Set user limit

### 🛠 Technical Requirements
- No `fork()` usage allowed
- Mandatory non-blocking I/O
- Single `poll()` call
- C++98 standard functions only
- Allowed syscalls only

---

## 🚀 Usage

### ✅ Execution
```sh
./ircserv <port> <password>
<port>: Server listening port
<password>: Client connection password
```

