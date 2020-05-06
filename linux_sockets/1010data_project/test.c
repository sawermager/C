#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "assertf.h"

#define STR_LIMIT 1024

/**
 * @brief Crude test tool to verify concurrent transactions (either TCP or UDP) 
 *        with multiple clients.
 * 
 * Example Usage: Create 2 client connections to server (first child proc starts the server)
 *                Each client sends 1 valid (root) and 1 invalid (rooot) usernames.
 * ./test 3 61000 TCP
 *      ctrl-c to end test.
 * 
 * TEST OUTPUT:
 * argv_list = ./server
 * argv_list = ./client
 * argv_list = ./client
 * IPv4: 0.0.0.0
 * Successful bind.
 * new child (pid 62002) using descriptor 4
 * new child (pid 62003) using descriptor 4
 * [Server] Received 5 msg bytes from localhost:43692:4: root
 * [Client] Received 1024 bytes: Valid username for root
 * [Server] Received 5 msg bytes from localhost:43694:4: root
 * [Server] Received 6 msg bytes from localhost:43692:4: rooot
 * [Client] Received 1024 bytes: Valid username for root
 * [Server] Received 6 msg bytes from localhost:43694:4: rooot
 * [Client] Received 1024 bytes: Invalid username for rooot
 * [Client] Received 1024 bytes: Invalid username for rooot
 *
 * SYSLOG OUTPUT:
 * May  6 14:36:15 sawermager-System-Product-Name server[62104]: Wed May  6 14:36:15 2020#012: 62104: Valid username
 * May  6 14:36:15 sawermager-System-Product-Name server[62105]: Wed May  6 14:36:15 2020#012: 62105: Valid username
 * May  6 14:36:15 sawermager-System-Product-Name server[62104]: Wed May  6 14:36:15 2020#012: 62104: Invalid username
 * May  6 14:36:15 sawermager-System-Product-Name server[62105]: Wed May  6 14:36:15 2020#012: 62105: Invalid username
 * 
 */
int main (int argc, char **argv) {
        pid_t pid, wpid, child_pid_server;
	int loop_cnt;
        int status, ret=1;
	char tst_str_client_msg[STR_LIMIT] = "root rooot";

        assertf(argc == 4, "[TestExe] Usage: num_procs port socket_type (TCP or UDP)");

	char *argv_list_server[] = {"./server", argv[2], argv[3], NULL};
	char *argv_list_client[] = {"./client", "localhost", argv[2], argv[3], "root", "rooot", NULL};
	char **argv_list = NULL;

	for (loop_cnt=0; loop_cnt < atoi(argv[1]); loop_cnt++) {
           if (loop_cnt == 0) {
		   argv_list = argv_list_server;
	   }
	   else {
		   argv_list = argv_list_client;
	   }
	   pid = fork();
	   assertf(pid != -1, "[Test] failed fork()");
	   if (pid == 0) { // child
                printf ("argv_list = %s\n", argv_list[0]);
	        execv(argv_list[0], argv_list);
                exit(EXIT_SUCCESS); 
	   }
	}
        while ((wpid = wait(&status)) > 0);      
        exit(EXIT_SUCCESS);
}

