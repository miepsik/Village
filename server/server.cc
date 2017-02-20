#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/sem.h>
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

#define QUEUE_SIZE 5
#define MAX_GAMER 3
#define ARCHER_ATTACK 5
#define ARCHER_DEFF 20
#define SPEAR_ATTACK 20
#define SPEAR_DEFF 5
#define WALL_DEFF 10
#define CAP 15
#define INITIAL_MESSAGE "HELLOe"
#define ARCHER_WOOD 5
#define ARCHER_FOOD 20
#define SPEAR_WOOD 10
#define SPEAR_FOOD 15
#define WOOD_WOOD_COST 9
#define WOOD_FOOD_COST 3
#define FOOD_WOOD_COST 11
#define FOOD_FOOD_COST 1
#define WALL_WOOD_COST 1000
#define WALL_FOOD_COST 250

int nSocket, nClientSocket;
int nBind, nListen;
int nFoo;
int numberOfGamers;
int *wood, *food, *archer, *spear, *players, *woodSpeed, *foodSpeed, *wall, *recrutationSpeed, *points;
int epollDesc;
int endGame;
int resSem, uniSem;
epoll_data_t epd;
epoll_event event;
socklen_t nTmp;
struct sockaddr_in stAddr, stClientAddr;

void InicializeGamer(int gamerSocket) {
    char buff[100];
    for (int i = 0; i < MAX_GAMER; i++){
        if (players[i] == -1){
            printf("Index: %d\n", i);
            players[i] = gamerSocket;
            points[i] = 0;
            wood[i] = 0;
            food[i] = 0;
            archer[i] = 0;
            spear[i] = 0;
            woodSpeed[i] = 100;
            foodSpeed[i] = 100;
            wall[i] = 1;
            //recrutationSpeed[i] = 5000;
            int l = sprintf(buff, "h%d 10 10 %d %d %d %d %d %d %d %d %d %de", i, ARCHER_WOOD, ARCHER_FOOD, SPEAR_WOOD, SPEAR_FOOD, 
                    woodSpeed[i] * WOOD_WOOD_COST, woodSpeed[i] * WOOD_FOOD_COST, foodSpeed[i] * FOOD_WOOD_COST, foodSpeed[i] * FOOD_FOOD_COST,
                    wall[i] * WALL_WOOD_COST, wall[i] * WOOD_FOOD_COST);
            //printf("wiadomość zrobiona\n");
            write(gamerSocket, buff, l+1);
            //printf("i wysłana");
            event.events = EPOLLIN;
            epd.u32 = (i+1)*1000;
            event.data = epd;
            epoll_ctl(epollDesc, EPOLL_CTL_ADD, gamerSocket, &event);
            
            break;
        }
    }
    numberOfGamers++;
}

char* listToAttack(int x) {
    static char result[255];
    char buff[5];
    result[0] = '\0';
    for (int i = 0; i <MAX_GAMER; i++) {
        if (players[i] != -1 && i != x) {
            sprintf(buff, "%d ", i);
            strcat(result, buff);
        }
    }
    strcat(result, "e");
    return result;
}


void *addResources(void *threadID) {
    struct sembuf s;
    s.sem_flg = 0;
    while (1) {
        for (int i = 0; i < MAX_GAMER; i++) {
            if (players[i] >= 0) {
                s.sem_num = i;
                s.sem_op = -1;
                semop(resSem, &s, 1);
                wood[i] += woodSpeed[i];
                food[i] += foodSpeed[i];
                s.sem_op = 1;
                semop(resSem, &s, 1);
                char buff[255];
                int n = sprintf(buff, "x%d %de", wood[i], food[i]);
                write(players[i], buff, n+1);
                //printf("Surowce gracz %d\n", i);
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
                write(players[i], buff, l+1);
        }
        endGame = 2;
    }
}


void *sendAttack(void *args) {
    struct sembuf s;
    s.sem_flg = 0;
    char *str = (char *) args;
    int attacker, target, archers, spears;
    sscanf(str, "%d %d %d %d", &attacker, &target, &archers, &spears);
    if (archer[attacker] < archers || spear[attacker] < spears || archers+spears < 20) {
        char buff[255];
        int l = sprintf(buff, "s-1 -1 -1 -1 -1e");
        write(players[attacker], buff, l+1);
        pthread_exit(NULL);
    }
    s.sem_num = attacker;
    s.sem_op = -1;
    semop(uniSem, &s, 1);
    archer[attacker] -= archers;
    spear[attacker] -= spears;
    s.sem_op = 1;
    semop(uniSem, &s, 1);
    char buff[255];
    int l = sprintf(buff, "s%d %de", archer[attacker], spear[attacker]);
    write(players[attacker], buff, l+1);
    sleep(5);
    int attack, deff;
    attack = archers * ARCHER_ATTACK + spears * SPEAR_ATTACK;
    deff = archer[target] * ARCHER_DEFF + spear[target] * ARCHER_DEFF + wall[target]*WALL_DEFF;
    if (attack > deff) {
        s.sem_num = target;
        s.sem_op = -1;
        semop(uniSem, &s, 1);
        archer[target] = 0;
        spear[target] = 0;
        s.sem_op = 1;
        semop(uniSem, &s, 1);
        archers -= archers * deff/attack;
        spears -= spears * deff/attack;
        int takenWood = 0, takenFood = 0;
        int cap = CAP * (archers+spears);
        s.sem_op = -1;
        semop(resSem, &s, 1);
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
        s.sem_op = 1;
        semop(resSem, &s, 1);
        l = sprintf(buff, "b%d %d %d %d %de", attacker, wood[target], food[target], archer[target], spear[target]);
        write(players[target], buff, l+1);
        sleep(5);
        s.sem_num = attacker;
        s.sem_op = -1;
        semop(uniSem, &s, 1);
        archer[attacker] += archers;
        spear[attacker] += spears;
        s.sem_op = 1;
        semop(uniSem, &s, 1);
        s.sem_op = -1;
        semop(uniSem, &s, 1);
        food[attacker] += takenFood;
        wood[attacker] += takenWood;
        points[attacker]++;
        s.sem_op = 1;
        semop(uniSem, &s, 1);
        if (points[attacker] > MAX_GAMER) {
            theEnd(attacker);
        }
        l = sprintf(buff, "h%d %d %d %d %de", archer[attacker], spear[attacker], points[attacker], wood[attacker], food[attacker]);
        write(players[attacker], buff, l+1);
    } else {
        s.sem_num = target;
        s.sem_op = -1;
        semop(uniSem, &s, 1);
        archer[target] -= archer[target] * deff/attack;
        spear[target] -= spear[target] * deff/attack;
        s.sem_op = 1;
        semop(uniSem, &s, 1);
    }
    pthread_exit(NULL);
}


void *acceptAndInicializeGamer(void *threadID){
    nTmp = sizeof(struct sockaddr);
    while (1) {
        nClientSocket = accept(nSocket, (struct sockaddr*) &stClientAddr, &nTmp);
        //printf("Ktos sioe zglosil\n");
        if (nClientSocket > 0) {
            //printf("if\n");
            sleep(2);
            char buff[30];
            read(nClientSocket, buff, 20);
            if (!strcmp(buff, INITIAL_MESSAGE) && numberOfGamers < MAX_GAMER) {
                printf("%d\n", nClientSocket);
                InicializeGamer(nClientSocket);
            } else 
                write(nClientSocket, "-1\0", 3);
        }
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
    resSem = semget(9999, MAX_GAMER, IPC_CREAT);
    uniSem = semget(8888, MAX_GAMER, IPC_CREAT);
    for (int i = 0; i < MAX_GAMER; i++) {
        semctl(resSem, i, SETVAL, 1);
        semctl(uniSem, i, SETVAL, 1);
    }
}

void clear() {
    for (int i = 0; i < MAX_GAMER; i++) {
        if (players[i] != -1)
            close(players[i]);
    }
    free(wood);
    free(food);
    free(points);
    free(archer);
    free(spear);
    free(players);
    free(woodSpeed);
    free(foodSpeed);
    free(recrutationSpeed);
}

char* upgrade(int pl, char t) {
    char *buff = (char*) malloc(55*sizeof(char));
    sprintf(buff, "-1 -1 -1");
    switch (t) {
        case 'w':
            if (wood[pl] >= woodSpeed[pl]*WOOD_WOOD_COST && food[pl] >= woodSpeed[pl]*WOOD_FOOD_COST) {
                wood[pl] -= woodSpeed[pl]*WOOD_WOOD_COST;
                food[pl] -= woodSpeed[pl]*WOOD_FOOD_COST;
                woodSpeed[pl] *= 1.1;
                sprintf(buff, "%d %d %d", woodSpeed[pl], woodSpeed[pl]*WOOD_WOOD_COST, woodSpeed[pl]*WOOD_FOOD_COST);
            }
            break;
        case 'f':
            if (wood[pl] >= foodSpeed[pl]*FOOD_WOOD_COST && food[pl] >= foodSpeed[pl]*FOOD_FOOD_COST) {
                wood[pl] -= foodSpeed[pl]*FOOD_WOOD_COST;
                food[pl] -= foodSpeed[pl]*FOOD_FOOD_COST;
                foodSpeed[pl] *= 1.1;
                sprintf(buff, "%d %d %d", foodSpeed[pl], foodSpeed[pl]*FOOD_WOOD_COST, foodSpeed[pl]*FOOD_FOOD_COST);
            }
            break;
        case 'd':
            if (wood[pl] >= wall[pl]*wall[pl]*WALL_WOOD_COST && food[pl] >= wall[pl]*WALL_FOOD_COST) {
                wood[pl] -= wall[pl]*wall[pl]*WALL_WOOD_COST;
                food[pl] -= wall[pl]*WALL_FOOD_COST;
                wall[pl] += 1;
                sprintf(buff, "%d %d %d", wall[pl], wall[pl]*WALL_WOOD_COST, wall[pl]*WALL_FOOD_COST);
            }   
            break;
    }
    return buff;
}

void closePlayer(int p) {
    printf("PLAYER %d CLOSE\n", p);
    close(players[p]);
    players[p] = -2;
    
}

void recruit(int pl, char t, int x) {
    struct sembuf s;
    s.sem_flg = 0;
    s.sem_num = pl;
    s.sem_op = -1;
    char *buff = (char*) malloc(55*sizeof(int));
    int l = sprintf(buff, "r%c-1e", t);
    semop(uniSem, &s, 1);
    semop(resSem, &s, 1);
    switch (t) {
        case 'a':
            if (wood[pl] >= x*ARCHER_WOOD && food[pl] >= x*ARCHER_FOOD) {
                archer[pl] += x;
                wood[pl] -= x*ARCHER_WOOD;
                food[pl] -= x*ARCHER_FOOD;
                l = sprintf(buff, "r%c%de", t, archer[pl]);
            }
            break;
        case 's':
  	 	    if (wood[pl] >= x*SPEAR_WOOD && food[pl] >= x*SPEAR_FOOD) {
                spear[pl] += x;
                wood[pl] -= x*SPEAR_WOOD;
                food[pl] -= x*SPEAR_FOOD;
                l = sprintf(buff, "r%c%de", t, spear[pl]);
            }
            break;
    }
    s.sem_op = 1;
    semop(resSem, &s, 1);
    semop(uniSem, &s, 1);
    write(players[pl], buff, l+1);
}

int main(int argc, char* argv[]){
    
    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    FILE *p;
    p = fopen("../config", "r");
    char b[10];
    if (fgets(b, 10, p) == NULL){
        fprintf(stderr, "error while reading config file");
        exit(1);
    }
    unsigned int port = atoi(b);
    printf("%d port\n", port);
    stAddr.sin_port = htons(port);

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
    while (2) {
        inicialize();
        while (1) {
            if (endGame == 2) {
                break;
            }
            epoll_wait(epollDesc, &event, 1, -1);
            for (unsigned int pl = 0; pl < MAX_GAMER; pl++) {
                if (players[pl] != -1 && event.data.u32 == (pl+1) * 1000) {
                    char buff[255], helper[2];
                    memset(buff, 0, 255);
                    buff[0] = '\0';
                    int l;
                    l = read(players[pl], buff, 1);
                    if (l < 1)
                        closePlayer(pl);
                    else {
                        helper[0] = '0';
                        while (helper[0] != 'e') {
                            l = read(players[pl], helper, 1);
                            if (l>0) strcat(buff, helper);
                        }
                        char message[255], *s;
                        int l;
                        printf("otrzymane: %s\n", buff);
                        event.data.u32 = 0;
                        switch (buff[0]) {
                            case 'u': // upgrade
                                l = sprintf(message, "u%c%se", buff[1], upgrade(pl, buff[1]));
                                write(players[pl], message, l+1);
                                break;
                            case 'a': // want list of villages to attack
                                l = sprintf(message, "a%s", listToAttack(pl));
                                write(players[pl], message, l+1);
                                printf("%s\n", message);
                                break;
                            case 's' : // send attack
                                l = strlen(buff);
                                s = (char*)malloc((l-1)*sizeof(char));
                                strncpy(s, buff+1, l-2);
                                s[l-2] = '\0';
                                printf("s: %s\n", s);
                                pthread_t a;
                                pthread_create(&a, NULL, sendAttack, (void *)s);
                                break;
                            case 'r' : // recruit
                                int l = strlen(buff);
                                s = (char*)malloc((l-2)*sizeof(char));
                                strncpy(s, buff+2,l-3);
                                sscanf(s, "%d", &l);
                                recruit(pl, buff[1], l);
                                break;
                        }
                    }
                }
            }
        }
        clear();
    }
    return(0);
}
