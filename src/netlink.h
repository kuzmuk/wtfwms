/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef NETLINK_H
#define	NETLINK_H


#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <netinet/tcp.h>

  
# define TIME_WAIT 6
# define TCP_STATE_LISTEN 10
int tcp_netlink(inet_diag_msg * da, unsigned long inode);

#endif	/* NETLINK_H */

