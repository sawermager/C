
#  multi-threaded client to count prime numbers using multiple servers.

## Tested using 1 virtualbox VM and 1 QEMU/KVM VM (bridged networks used for both).  
&nbsp;
&nbsp;
### BUILD:
```make all```
&nbsp;
### Run Format: Server1  
```
./server  
```
### Run Format: Server2  
```
./server  
```
&nbsp;  
### Run Format: Client   
```
./client -n 1000000 -c 100000 guest1-VirtualBox guest2-VirtualBox    
```
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
### Output Server1:  
```
listening on port 61000 ...  
got connection  
range        0 -   100000 : count = 9594  
range   200000 -   300000 : count = 8013  
range   400000 -   500000 : count = 7678  
range   600000 -   700000 : count = 7445  
range   800000 -   900000 : count = 7323  
```
&nbsp;  
### Output Server2:
```
listening on port 61000 ...  
got connection  
range   100000 -   200000 : count = 8392  
range   300000 -   400000 : count = 7863  
range   500000 -   600000 : count = 7560  
range   700000 -   800000 : count = 7408  
range   900000 -  1000000 : count = 7224  
```
&nbsp;  
### Output Client:
```
biggest = 1000000, chunk_size = 100000  
connected to guest1-VirtualBox  
connected to guest2-VirtualBox  
using 2 servers  
found 78500 primes  
```

