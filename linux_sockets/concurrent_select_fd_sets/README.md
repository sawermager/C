# linux_sockets_C_python

Quick Start:

TO BUILD:
========
cd {workspace}/concurrnt_num
make all

To run manually:
==================
- launch server:
  ./server
- launch client: From any terminal on same host, launch telnet client and make number guess:
  telnet localhost 61000
- Ctrl-C to quit server. Ctrl-] and 'quit' to stop telnet client.

Example Ouput from starting server and 1 telnet client that makes 3 guesses:

server output:
FD_SETSIZE=1024
server ready
new connection on fd 4
answer = 384
received guess 55 on fd=4
received guess 22 on fd=4
received guess 384 on fd=4
closing fd=4

client input/output:
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
55
guess too large
22
guess too large
384
correct guess!
Connection closed by foreign host.

Description:
====================
Program to start server that uses the select() call with file descriptor sets
to play a number guessing game.
The server picks magic number and waits for multiple concurrent clients to connect and send guesses.


