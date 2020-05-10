
#  multi-threaded client to count prime numbers using multiple servers.

## Tested using 1 virtualbox VM and 1 QEMU/KVM VM (bridged networks used for both).  
&nbsp;

### BUILD:
'''make all'''

### Run Format: Server
'''
./server
'''

### Run Format: Client
'''
./client -n 1000000 -c 100000 guest1-VirtualBox guest2-VirtualBox

client -n 1000000 -c 100000 guest1-VirtualBox guest2-VirtualBox








