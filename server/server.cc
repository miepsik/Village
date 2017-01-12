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
#include <sys/epoll.h>
#include <poll.h>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define MAX_GAMER 2
#define ARCHER_ATTACK 5
#define ARCHER_DEFF 20
#define SPEAR_ATTACK 20
#define SPEAR_DEFF 5
#define WALL_DEFF 10
#define CAP 15
#define INITIAL_MESSAGE "HELLOe"

int nSocket, nClientSocket;
int nBind, nListen;
int nFoo;
int numberOfGamers;
int *wood, *food, *archer, *spear, *players, *woodSpeed, *foodSpeed, *wall, *recrutationSpeed, *points;
int epollDesc;
int endGame;
epoll_data_t epd;
epoll_event event;
socklen_t nTmp;
struct sockaddr_in stAddr, stClientAddr;

void InicializeGamer(int gamerSocket) {
    char buff[4];
    for (int i = 0; i < MAX_GAMER; i++){
        if (players[i] == -1){
            sprintf(buff, "%d", i);
            printf("Index: %d\n", i);
            players[i] = gamerSocket;
            points[i] = 0;
            wood[i] = 0;
            food[i] = 0;
            archer[i] = 0;
            spear[i] = 0;
            woodSpeed[i] = 10;
            foodSpeed[i] = 10;
            wall[i] = 0;
            recrutationSpeed[i] = 5000;
            write(gamerSocket, buff, 4);
            event.events = EPOLLIN;
            epd.u32 = (i+1)*1000;
            event.data = epd;
            epoll_ctl(epollDesc, EPOLL_CTL_ADD, gamerSocket, &event);
            break;
        }
    }
    numberOfGamers++;
}

char* listToAttack() {
    static char result[255];
    char buff[5];
    result[0] = '\0';
    for (int i = 0; i <MAX_GAMER; i++) {
        if (players[i] != -1) {
            sprintf(buff, "%d ", i);
            strcat(result, buff);
        }
    }
    strcat(result, "e");
    return result;
}


void *addResources(void *threadID) {
    while (1) {
        for (int i = 0; i < MAX_GAMER; i++) {
            if (players[i] >= 0) {
                wood[i] += woodSpeed[i];
                food[i] += foodSpeed[i];
                char buff[255];
                int n = sprintf(buff, "x%d %de", wood[i], food[i]);
                write(players[i], buff, n+1);
                printf("Surowce gracz %d\n", i);
            }
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

void theEnd(int pl) {
    if (endGame == 0) {
        endGame = 1;
        char buff[255];
        int l = sprintf(buff, "WINNER%de", pl);
        for (int i = 0; i < MAX_GAMER; i++) {
            if (players[i] >= 0)
                write(players[i], buff, l-1);
        }
        endGame = 2;
    }
}


void *sendAttack(void *args) {
    char *str = (char *) args;
    int attacker, target, archers, spears;
    sscanf(str, "%d %d %d %d", &attacker, &target, &archers, &spears);
    if (archer[attacker] < archers || spear[attacker] < spears || archers+spears < 20) {
        char buff[255];
        int l = sprintf(buff, "s-1 -1 -1 -1 -1e");
        write(players[attacker], buff, l-1);
        pthread_exit(NULL);
    }
    archer[attacker] -= archers;
    spear[attacker] -= spears;
    char buff[255];
    int l = sprintf(buff, "s%d %de", archer[attacker], spear[attacker]);
    write(players[attacker], buff, l-1);
    sleep(5);
    int attack, deff;
    attack = archers * ARCHER_ATTACK + spears * SPEAR_ATTACK;
    deff = archer[target] * ARCHER_DEFF + spear[target] * ARCHER_DEFF + wall[target]*WALL_DEFF;
    if (attack > deff) {
        archer[target] = 0;
        spear[target] = 0;
        archers -= archers * deff/attack;
        spears -= spears * deff/attack;
        int takenWood = 0, takenFood = 0;
        int cap = CAP * (archers+spears);
        if (cap < wood[target]){
            takenWood = cap;
            wood[target] -= takenWood;
        } else {
            takenWood = wood[target];
            wood[target] = 0;
        }
        if (cap < food[target]) {
            takenFood = cap;
            food[target] -= cap;
        } else {
            takenFood = food[target];
            food[target] = 0;
        }
        sleep(5);
        archer[attacker] += archers;
        spear[attacker] += spears;
        food[attacker] += takenFood;
        wood[attacker] += takenWood;
        points[attacker]++;
        if (points[attacker] > MAX_GAMER) {
            theEnd(attacker);
        }
        l = sprintf(buff, "h%d %d %d %d %de", archer[attacker], spear[attacker], points[attacker], wood[attacker], food[attacker]);
        write(players[attacker], buff, l-1);
    } else {
        archer[target] -= archer[target] * deff/attack;
        spear[target] -= spear[target] * deff/attack;
    }
    pthread_exit(NULL);
}


void *acceptAndInicializeGamer(void *threadID){
    nTmp = sizeof(struct sockaddr);
    while (1) {
        nClientSocket = accept(nSocket, (struct sockaddr*) &stClientAddr, &nTmp);
        sleep(2);
        char buff[30];
        read(nClientSocket, buff, 20);
        if (!strcmp(buff, INITIAL_MESSAGE) && numberOfGamers < MAX_GAMER) {
            printf("%d\n", nClientSocket);
            InicializeGamer(nClientSocket);
        } else 
            write(nClientSocket, "-1\0", 3);
    }
    pthread_exit(NULL);
}


void inicialize(){
    nFoo = 1;
    endGame = 0;
    numberOfGamers = 0;
    epollDesc = epoll_create1(0);
    wood = (int*)malloc(MAX_GAMER*sizeof(int));
    food = (int*)malloc(MAX_GAMER*sizeof(int));
    points = (int*)malloc(MAX_GAMER*sizeof(int));
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

void clear() {
    for (int i = 0; i < MAX_GAMER; i++) {
        if (players[i] != -1)
            close(players[i]);
    }
    close(nSocket);
    
}

int upgrade(int pl, char t) {
    switch (t) {
        case 'w':
            if (wood[pl] >= woodSpeed[pl]*10 && food[pl] >= woodSpeed[pl]*2) {
                woodSpeed[pl] *= 1.1;
                return woodSpeed[pl];
            }
            return -1;
            break;
        case 'f':
            if (wood[pl] >= foodSpeed[pl]*10 && food[pl] >= foodSpeed[pl]*2) {
                foodSpeed[pl] *= 1.1;
                return foodSpeed[pl];
            }
            return -1;
        case 'd':
            if (wood[pl] >= wall[pl]*wall[pl]*1000 && food[pl] >= wall[pl]*250) {
                wall[pl] += 1;
                return wall[pl];
            }
            return -1;
    }
    return -1;
}

void closePlayer(int p) {
    printf("PLAYER %d CLOSE\n", p);
    close(players[p]);
    players[p] = -2;
    
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

   pthread_t thread[2];
   long t = 1;
   printf("%d\n", nSocket);
   if (pthread_create(&thread[0], NULL, acceptAndInicializeGamer, (void *)t) < 0)
       printf("ERROR \n");
   t++;
   if (pthread_create(&thread[1], NULL, addResources, (void *)t) < 0)
       printf("ERROR \n");
   while (1) {
       if (endGame == 2) {
           endGame = 0;
           break;
       }
       epoll_wait(epollDesc, &event, 1, -1);
       for (unsigned int pl = 0; pl < MAX_GAMER; pl++) {
           if (players[pl] != -1 && event.data.u32 == (pl+1) * 1000) {
                printf("button: %d\n", pl);
                char buff[255], helper[2];
                int l;
                l = read(players[pl], buff, 1);
                if (l < 1)
                    closePlayer(pl);
                else {
                    helper[0] = '0';
                    while (helper[0] != 'e') {
                        l = read(players[pl], helper, 1);
                        if (l) strcat(buff, helper);
                    }
                    char message[255];
                    int l;
                    printf("%s\n", buff);
                    event.data.u32 = 0;
                    switch (buff[0]) {
                        case 'u': // upgrade
                            l = sprintf(message, "u%c%de", buff[1], upgrade(pl, buff[1]));
                            write(players[pl], message, l+1);
                            break;
                        case 'a': // want list of villages to attack
                            l = sprintf(message, "l %s", listToAttack());
                            write(players[pl], message, l+1);
                            break;
                        case 's' : // send attack
                            int l = strlen(buff);
                            char s[l];
                            strncpy(s, buff+1, l-2);
                            s[l-1] = '\0';
                            pthread_t a;
                            pthread_create(&a, NULL, sendAttack, (void *)s);
                            break;

                    }
                }
           }
       }
   }
   clear();
   return(0);
}
