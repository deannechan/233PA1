#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>

const int16_t StartOfPacketId;
const int16_t EndOfPacketId;
const int16_t ClientId;
const int16_t Length;
const int16_t DATA;

const int16_t REJECT; //REJECT
const int16_t OOS;   //out of sequence
const int16_t LM;  //length mismatch
const int16_t EOPM; //end of the packet missing
const int16_t DP;  //duplicate packet

char* buildDataPacket(char* payload,int segNo);
char* sendPacket(int s, char* packet, struct sockaddr_in server);
char* checkResponse(char* packet);