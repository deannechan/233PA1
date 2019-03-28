#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<arpa/inet.h>

const int16_t StartOfPacketId;
const int16_t ClientId;
const int16_t ACK;
const int16_t REJECT;
const int16_t EndOfPacketId;

//Reject sub codes
const int16_t OutOfSequence;
const int16_t LengthMismatch;
const int16_t EndOfPacketMissing;
const int16_t DuplicatePacket;

char* ackPacket(int segmentNo);
char* rejPacket(int segmentNo, int flag);
