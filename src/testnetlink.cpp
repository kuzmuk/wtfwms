#include "netlink.h"

int main(int argc, char* argv[]) {
	 if (argc == 2) {
		unsigned long in = atol(argv[1]);
		printf("data: %d\r\n", in);
		inet_diag_msg data;
		int res = tcp_netlink(&data, in);
		if (res > 0) {
		printf ( "%08X:%04X %08X:%04X %d\r\n",
						  data.id.idiag_src[0],
						  ntohs(data.id.idiag_sport),
						  data.id.idiag_dst[0],
						  ntohs(data.id.idiag_dport),
						  data.idiag_inode);
		} else {
			printf("not found, res = %d \r\n" , res);
		}
		return 0;
	 } else {
		printf("provide inode\r\n");
	 }
 }


