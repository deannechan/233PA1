#include "ServerPacket.h"

const int16_t StartOfPacketId = 0xFFFF;
const int16_t ClientId = 1;//0xFF;
const int16_t ACK = 0xFFF2;
const int16_t REJECT = 0xFFF3;
const int16_t EndOfPacketId = 0xFFFF;

//Reject sub codes
const int16_t OutOfSequence = 0xFFF4;
const int16_t LengthMismatch = 0xFFF5;
const int16_t EndOfPacketMissing = 0xFFF6;
const int16_t DuplicatePacket = 0xFFF7;

// ACK packet
char* ackPacket(int segmentNo){
    char* onePacket = malloc(9);
    memset(onePacket,'\0',9);
    memcpy(onePacket, &StartOfPacketId, 2);
    memcpy(onePacket+2, &ClientId, 1);
    memcpy(onePacket+3, &ACK, 2);
    memcpy(onePacket+5, &segmentNo,1);
    memcpy(onePacket+6, &EndOfPacketId, 2);
   
    return onePacket;
}

// reject packets with corresponding error code
char* rejPacket(int segmentNo, int flag){
    char* onePacket= malloc(11);
    memset(onePacket,'\0',11);
    memcpy(onePacket, &StartOfPacketId, 2);
    memcpy(onePacket+2, &ClientId, 1);
    memcpy(onePacket+3, &REJECT, 2);
    
    // reject cases
    if(flag == 1){
        memcpy(onePacket+5, &OutOfSequence,2);
    }
    if(flag == 2){
        memcpy(onePacket+5, &LengthMismatch,2);
    }
    if(flag == 3){
        memcpy(onePacket+5, &EndOfPacketMissing,2);
    }
    if(flag == 4){
        memcpy(onePacket+5, &DuplicatePacket,2);
    }
    memcpy(onePacket+7, &segmentNo, 1);
    memcpy(onePacket+8, &EndOfPacketId,2);

    return onePacket;
}
