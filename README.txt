Deanne Chan (W1393868)

Files:
- Client folder
    - Client.c
    - DataPacket.c
    - DataPacket.h
    - obj folder
- Server folder
    - Server.c
    - ServerPacket.c
    - ServerPacket.h
    - obj folder
- client-run1.png
- client-run2.png
- server-run1.png
- server-run2.png
- server-run3.png
- server-run4.png
- ServerNotResponding.png
- README.txt

Compile and Run Instructions:

FOR SERVER:
1. Open terminal
2. cd to Server folder
3. gcc -c -o obj/ServerPacket.o ServerPacket.c
4. gcc -c -o obj/Server.o Server.c
5. gcc -o obj/server obj/Server.o obj/ServerPacket.o
6. run using the command: ./obj/server

FOR CLIENT:
1. Open terminal
2. cd to Client folder
3. gcc -c -o obj/DataPacket.o DataPacket.c
4. gcc -c -o obj/Client.o Client.c
5. gcc -o obj/client obj/Client.o obj/DataPacket.o
6. run using the command: ./obj/client

Keywords:
- OOS : out of sequence
- LM : length mismatch
- EOPM : end of packet missing
- DP : duplicate packet
- ACK : acknowledge (successful)

Screenshots:
1. client-runs and server-runs
- shows client program running successfully. 
- send 5 packets with:
    - packet out of sequence error
    - length mismatch error
    - duplicate packet error
    - end of packet missing error
    - ACK

2. ServerNotResponding
- no server response withint 3s so client tries to resend packets 3 times
before it displays 'Server does not respond' message