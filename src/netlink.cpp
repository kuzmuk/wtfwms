/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "netlink.h"

int tcp_netlink(inet_diag_msg * da, unsigned long inode) {
	int fd;
	struct sockaddr_nl nladdr;
	struct {
		struct nlmsghdr nlh;
		struct inet_diag_req r;
	} req;
	struct msghdr msg;
	char	buf[8192];
	struct iovec iov;
	int lnr = 0;
	struct inet_diag_msg *r;
	int rv = 0;

	if ((fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_INET_DIAG)) < 0) {
		perror("can't open");
	  return -1;
	}

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;

	req.nlh.nlmsg_len = sizeof(req);
	req.nlh.nlmsg_type = TCPDIAG_GETSOCK;
	req.nlh.nlmsg_flags = NLM_F_ROOT|NLM_F_MATCH|NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = 123456;
	memset(&req.r, 0, sizeof(req.r));
	req.r.idiag_family = AF_INET;
	req.r.idiag_states = 0xfff & (~(1 << TCP_TIME_WAIT));
	
	//req.r.idiag_states = 0xfff;
	req.r.idiag_ext = 0;

	iov.iov_base = &req;
	iov.iov_len = sizeof(req);

	memset (&msg, 0, sizeof (msg));
	msg.msg_name = (void*)&nladdr;
	msg.msg_namelen = sizeof(nladdr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	if (sendmsg(fd, &msg, 0) < 0)
	{
	  rv = -2;
	  //perror("sendmsg error");
	  goto netlink_done;
	}

	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);

	while (1)  {
		int status;
		struct nlmsghdr *h;

		msg = (struct msghdr) {
			(void*)&nladdr, sizeof(nladdr), &iov,	1,
			NULL,
			0,
			0
		};

		status = recvmsg(fd, &msg, 0);

		if (status < 0) {
			if (errno == EINTR)
				continue;
			rv = -3;
			goto netlink_done;
		}

		if (status == 0) {
			rv = 0;
			goto netlink_done;
		}


		h = (struct nlmsghdr*)buf;
		while (NLMSG_OK(h, status)) {
			if (h->nlmsg_seq == 123456)	{
				if (h->nlmsg_type == NLMSG_DONE) {
					rv = 0;
					goto netlink_done;
				}

				if (h->nlmsg_type == NLMSG_ERROR) {
					rv = -4;
					goto netlink_done;
				}

				r = (inet_diag_msg*) NLMSG_DATA(h);

				if (r->idiag_family == AF_INET) {
					if (r->idiag_inode == inode) {
						(*da) = (*r);
						rv = 1;
						goto netlink_done;
					}
				} else {
						/* ipv6 */
				}
			}
			h = NLMSG_NEXT(h, status);
		}
	}

	netlink_done:
	if (fd >= 0)
		close (fd);
	return rv;
}
