// Deanne Chan W1393868 

#include "DataPacket.h"

#define BUFLEN 512

int main_thread(int s, struct sockaddr_in server, char **error);
char *read_input(char *buffer, int bufSize, char segmentNo);
void printMessagePacket(char* message, char* buffer);

int main(void){
    struct sockaddr_in server, client;
    struct hostent *hp;
    int s, slen = sizeof(client);
    char* packet;
    char* error;
    struct timeval tv;

    //build socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    //setup address
    memset((char *) &client, 0, sizeof(client));
    //client address struct
	client.sin_family = AF_INET;
	client.sin_port = htons(22345);
	hp = gethostbyname("localhost");
    bcopy( hp->h_addr, &(client.sin_addr.s_addr), hp->h_length);

	//bind info to socket
	bind(s, (struct sockaddr *) &client, sizeof(client));

    //server address struct
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	bcopy( hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);

    tv.tv_usec = 0;
	tv.tv_sec = 2;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

    printf("\nClient started, waiting for input: ");
    if (main_thread(s, server, &error) != 0)
    {
        fprintf(stderr, "main_thread: %s\n", error);
        return 1;
    }
    
    printf("Transmission Complete \n");

    return 0;
}

int main_thread(int s, struct sockaddr_in server, char **error){
    char input_buffer[256], currentSegmentNo = 1, *message, *buf, *response;
    int tries, type_id, reject_code;

    while (1)
    {
        // read input
        message = read_input(input_buffer, 256, currentSegmentNo);
        if (message == NULL)
            continue;
        // send packet and get response
        buf = sendPacket(s, message, server); 
        if (buf != NULL){
            response = checkResponse(buf);
            if(strcmp(response, "ACK") == 0) {
                currentSegmentNo++;
                printf("\nincrementing segment number: %d \n", currentSegmentNo);
            }
            printf("Response from server is: %s\n", response);
            printf("-----------------------------\n\n");
            printf("Client started, waiting for input: ");
        }else{
            printf("Server does not respond.\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

char *read_input(char *buffer, int bufSize, char segmentNo){
    char *pos;
    char *message;
    int len;
    int16_t endId;

    // read input
    if (fgets(buffer, bufSize, stdin) == NULL)
        return NULL;
    if ((pos = strchr(buffer, '\n')) != NULL)
        *pos = '\0';
    if (strlen(buffer) == 0)
        return NULL;

    // create corresponding packet
    if (strcmp(buffer, "OOS") == 0){   
        printf("creating OOS packet\n");
        message = buildDataPacket(buffer, segmentNo + 1);
        printMessagePacket(message, buffer);
        return message;
    }
    else if (strcmp(buffer, "DP") == 0){
        printf("creating DP packet\n");
        message = buildDataPacket(buffer, segmentNo - 1);
        printMessagePacket(message, buffer);
        return message;
    }
    message = buildDataPacket(buffer, segmentNo);
    if (strcmp(buffer, "LM") == 0){
        printf("creating LM packet\n");
        len = message[6] & 0xFF;
        message[6] = len + 10;
    }
    else if (strcmp(buffer, "EOPM") == 0){
        printf("creating EOPM packet\n");
        len = strlen(message);
        endId = 0xFFF0;
        memcpy(message + len - 2, &endId, 2);
    } else {
        printf("creating ACK packet\n");
    }
    printMessagePacket(message, buffer);
    return message;
}

void printMessagePacket(char* message, char* buffer){
    int msg, len;
    printf("packet created:\n");
    memcpy(&msg, &message[0], 2);
    printf("Start of Packet id: %#X \n", msg);
    printf("Client id: %d \n", message[2]);
    memcpy(&msg, &message[3], 2);
    printf("Data: %#X \n", msg);
    printf("Segment No: %d \n", message[5]);
    printf("Length: %d \n", message[6]);
    printf("Payload: %s\n", buffer);
    len = strlen(message);
    memcpy(&msg, &message[len-2], 1); 
    printf("End of Packet id: %#X \n", msg); 
}