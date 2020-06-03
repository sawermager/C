# linux_sockets_C

Quick Start:

TO BUILD:
========
cd 1010data_project/  
make all

To run using automatin test tool:
===================================
./test <num_procs> <port> <TCP|UDP>
ctrl-c to quit.

Example: ./test 3 61000 TCP

To run manually:
==================
- launch server:
  ./server <port> <TCP|UDP>
- launch client:
  ./client localhost <port> <TCP|UDP> msg_str ...

Example: Check validity of 1 valid and 1 invalid username from client.
   ./server 61000 TCP

   (in another terminal on same host as sever)
   ./client localhost 61000 TCP root rooot


Project Guidlines:
====================
Requirements:
1) Exist in userspace--please no kernel modules or drivers
2) Consist of dependencies provided by the OS and libc.  Please don't use e.g., libuv or Boost.
3) Work on Linux (it is OK if it also works on Windows)
4) Please implement your solution in an ANSI C standard, such as C99 (or even -stdu=gnu99), but not in K&R or C++.

Design:
1) Server will bind to a TCP or UDP port and listen for client communications
2) Client performs some kind of auth transaction with the server, such as providing a valid username
3) Client may send multiple messages or transactions to the server.  The purpose of this exchange is unspecified, except that these transactions should mutate some global state such as an append-only log.
4) Server may accept multiple client connections.  Whether those connections are serviced in serial or parallel is unspecified.

Analysis:

1) Please provide a test that demonstrates your server's ability to handle 1000 concurrent clients.  The definition of "concurrent" is something we ask you to specify and justify.  Any analysis on transaction throughput is appreciated.



