#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define MAX_GAMER 2

int nSocket, nClientSocket;
int nBind, nListen;
int nFoo;
int numberOfGamers;
int *wood, *food, *archer, *spear, *players, *woodSpeed, *foodSpeed, *wall, *recrutationSpeed;
socklen_t nTmp;
struct sockaddr_in stAddr, stClientAddr;

void InicializeGamer(int gamerSocket) {
    char buff[4];
    for (int i = 0; i < MAX_GAMER; i++){
        if (players[i] != -1){
            sprintf(buff, "%d\n", i);
            players[i] = gamerSocket;
            wood[i] = 0;
            food[i] = 0;
            archer[i] = 0;
            spear[i] = 0;
            woodSpeed[i] = 10;
            foodSpeed[i] = 10;
            wall[i] = 0;
            recrutationSpeed[i] = 5000;
            write(gamerSocket, buff, 4);
            break;
        }
    }
    numberOfGamers++;
}


void *acceptAndInicializeGamer(void *threadID){
    nTmp = sizeof(struct sockaddr);
    while (numberOfGamers < MAX_GAMER) {
        nClientSocket = accept(nSocket, (struct sockaddr*) &stClientAddr, &nTmp);
        printf("%d/n", nClientSocket);
        InicializeGamer(nClientSocket);
    }
    pthread_exit(NULL);
}


void inicialize(){
    nFoo = 1;
    numberOfGamers = 0;
    wood = (int*)malloc(MAX_GAMER*sizeof(int));
    food = (int*)malloc(MAX_GAMER*sizeof(int));
    archer = (int*)malloc(MAX_GAMER*sizeof(int));
    players = (int*)malloc(MAX_GAMER*sizeof(int));
    for (int i = 0; i < MAX_GAMER; i++)
        players[i] = -1;
    spear = (int*)malloc(MAX_GAMER*sizeof(int));
    woodSpeed = (int*)malloc(MAX_GAMER*sizeof(int));
    foodSpeed = (int*)malloc(MAX_GAMER*sizeof(int));
    wall = (int*)malloc(MAX_GAMER*sizeof(int));
    recrutationSpeed = (int*)malloc(MAX_GAMER*sizeof(int));
}

int main(int argc, char* argv[]){

   inicialize();
   /* address structure */
   memset(&stAddr, 0, sizeof(struct sockaddr));
   stAddr.sin_family = AF_INET;
   stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   stAddr.sin_port = htons(SERVER_PORT);

   /* create a socket */
   nSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (nSocket < 0) {
       fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
       exit(1);
   }
   setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

   /* bind a name to a socket */
   nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
   if (nBind < 0){
       fprintf(stderr, "%s: Can't bind a name to a socket.\n", argv[0]);
       exit(1);
   }
   /* specify queue size */
   nListen = listen(nSocket, QUEUE_SIZE);
   if (nListen < 0){
       fprintf(stderr, "%s: Can't set queue size.\n", argv[0]);
   }

   pthread_t thread;
   long t = 1;
   if (pthread_create(&thread, NULL, acceptAndInicializeGamer, (void *)t) < 0)
       printf("ERROR \n");
   while (1);
   //while(1)
   //{
   //    /* block for connection request */
   //    nTmp = sizeof(struct sockaddr);
   //    nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
   //    if (nClientSocket < 0)
   //    {
   //        fprintf(stderr, "%s: Can't create a connection's socket.\n", argv[0]);
   //        exit(1);
   //    }
//
  //         printf("%s: [connection from %s]\n",
   //               argv[0], inet_ntoa((struct in_addr)stClientAddr.sin_addr));
  //         time_t now;
  //         struct tm *local;
  //         time (&now);
  //         local = localtime(&now);
  //         char buffer[50];
  //         int n;
   //        n = sprintf(buffer, "%s\n", asctime(local));
  //         write(nClientSocket, buffer, n);
  //         close(nClientSocket);
  //     }

   close(nSocket);
   return(0);
}
