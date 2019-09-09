# Client_Server
A Server and a Client application written in C.
# Server
The Server application makes use of fork, execvp methods to replicate a bash shell on machine.
Server application reads bash commands sent by the client from scoket and makes use of pipes and dup2 to write STDOUT and STDERR to socket.
# Client
The Client application creates a child process to send bash commands to the server and the parent process reads the messages sent by the server.

# How to run
On your server machine:
```
gcc -o server server.c
./server < port number > 
```
On your client machine:
```
gcc -o client client.c
./client < ip address of server > < port number >
```
Run linux commands:
```
date && ls
```



