// Deanne Chan W1393868

#include "ServerPacket.h"

#define BUFLEN 265

void printMessagePacket(char* message);
void printACKPacket(char* message);

int main(){
    int s;
    struct sockaddr_in server, client;
    int slen, recv_len;
    char buf[BUFLEN];
    
    char segNo;
    const int16_t END = 0xFFFF;
    slen = sizeof(client);

    // Create the socket
	// 1) Internet Domain 2) Datagram 3) Default protocol(UDP)
	s = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Socket was Created.\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind info to socket
	bind(s, (struct sockaddr *) &server, sizeof(server));
    puts("Bind done");

    //keep listening for data
    int count = 1; //count packet order
    while(1){
        printf("----------------------\n");
        printf("Server is Listening!!\n");
        fflush(stdout);
        //clear the buffer
        memset(buf,'\0', BUFLEN);

        //receive data
        recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &client, (socklen_t *) &slen);
        printf("Received Packet From %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        memcpy(&segNo, buf + 5, 1); 
        int segNum = (int)segNo; //segment number

        printf("\nReceived Segment No. : %d\n",segNum);
        printf("Server Segment No. : %d\n", count);

        int int_buf;
        memcpy(&int_buf, buf+recv_len-2, 2);
    
        // int checkEOP = (int)checkEnd1;

        printf("End of packet id: %d\n", ntohs(int_buf));
        printf("END: %d\n", (END & 0XFFFF));

        char payloadSize;
        memcpy(&payloadSize, buf+6,1);
        int payloadLen = (int)payloadSize & 0xFF;
        printf("payloadSize: %d\n", payloadLen);
        printf("recv_len: %d\n", recv_len-9);

        char* rej1 = rejPacket(segNum,1);
        char* rej2 = rejPacket(segNum,2);
        char* rej3 = rejPacket(segNum,3);
        char* rej4 = rejPacket(segNum,4);
        char* ACK = ackPacket(segNum);

        printf("\nThe server message is: ");
        if (segNum != count)
        {
            if(segNum == count-1) //reject 4 DP
            {
                sendto(s,rej4,strlen(rej4),0,(struct sockaddr *) &client, slen);
                printf("Duplicate packet\n");
                printMessagePacket(rej4);
            }
            else //reject 1 OOS
            {
                sendto(s,rej1,strlen(rej1),0,(struct sockaddr *) &client, slen);
                printf("Out of sequence\n");
                printMessagePacket(rej1);
            }
        }
        else if(recv_len - 9 != payloadSize) //reject 2 LM
        {
            sendto(s,rej2,strlen(rej2),0,(struct sockaddr *) &client, slen);
            printf("Length mismatch\n");
            printMessagePacket(rej2);
        }
        else if(ntohs(int_buf) != (END & 0xFFFF)) //reject 3 EOPM
        {
            sendto(s,rej3,strlen(rej3),0,(struct sockaddr *) &client, slen);
            printf("End of the packet missing\n");
            printMessagePacket(rej3);
        }
        else //ACK
        {
            sendto(s,ACK,strlen(ACK),0,(struct sockaddr *) &client, slen);
            printf("ACK\n");
            printACKPacket(ACK);
            count++;
        }
        printf("\n");
    }
    return 0;
}

void printMessagePacket(char* message){
    int msg, len;
    printf("reject packet created:\n");
    memcpy(&msg, &message[0], 2);
    printf("Start of Packet id: %#X \n", msg);
    printf("Client id: %d \n", message[2]);
    memcpy(&msg, &message[3], 2);
    printf("REJECT: %#X \n", msg);
    memcpy(&msg, &message[5], 2);
    printf("Reject Subcode: %#X \n", msg);
    printf("Received Segment No: %d \n", message[7]);
    len = strlen(message);
    memcpy(&msg, &message[len-2], 1); 
    printf("End of Packet id: %#X \n", msg); 
}

void printACKPacket(char* message){
    int msg, len;
    printf("ack packet created:\n");
    memcpy(&msg, &message[0], 2);
    printf("Start of Packet id: %#X \n", msg);
    printf("Client id: %d \n", message[2]);
    memcpy(&msg, &message[3], 2);
    printf("ACK: %#X \n", msg);
    printf("Received Segment No: %d \n", message[5]);
    len = strlen(message);
    memcpy(&msg, &message[len-2], 1); 
    printf("End of Packet id: %#X \n", msg); 
}