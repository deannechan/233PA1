#include "DataPacket.h"

const int16_t StartOfPacketId = 0xFFFF;
const int16_t EndOfPacketId = 0xFFFF;
const int16_t ClientId = 1;//0xFF;
const int16_t Length = 0xFF;
const int16_t DATA = 0xFFF1;

const int16_t REJECT = 0xFFF3; //REJECT
const int16_t OOS = 0xFFF4;   //out of sequence
const int16_t LM = 0xFFF5;  //length mismatch
const int16_t EOPM = 0xFFF6; //end of the packet missing
const int16_t DP = 0xFFF7;  //duplicate packet

// Data packet format
char* buildDataPacket(char* payload,int segNo){
    char* onePacket= malloc(265);
    int len = strlen(payload);
    if(len >255) len = 255;
    memset(onePacket,'\0',265);
    memcpy(onePacket, &StartOfPacketId, 2);
    memcpy(onePacket+2, &ClientId, 1);
    memcpy(onePacket+3, &DATA, 2);
    memcpy(onePacket+5, &segNo,1);
    memcpy(onePacket+6, &len, 1);
    memcpy(onePacket+7, payload, len);
    memcpy(onePacket+7+len, &EndOfPacketId,2);

    return onePacket;
}

// send packet to server
char* sendPacket(int s, char* packet, struct sockaddr_in server){
	int size = 10;
	char* buf = malloc(265);

	for(int i = 0; i < 4; i++){
		sendto(s, packet, strlen(packet), 0, (struct sockaddr *) &server, sizeof(server) );
        memset(buf,'\0', 265);
		int rc = recv(s, buf, 265, 0);
		
		if(rc > 0){
			return buf;
		}else if(i != 0 && rc <= 0){
			printf("Resend packet to server %d\n", i);	
            sleep(3);
		}
	}
	return NULL;
}

// check message response
char* checkResponse(char* packet){
    char errorCode;
    char message;

    memcpy(&message, packet + 3, 1);
    memcpy(&errorCode, packet + 5, 1);

    if(message == REJECT){
        if (errorCode == DP){
            return "Duplicate packet!";
        }
        if(errorCode == OOS){
            return "Packet out of sequence!";
        }
        if(errorCode == LM){
            return "Length mismatch!";
        }
        if(errorCode == EOPM){
            return "End of the packet missing!";
        }
    }
    return "ACK";
}
