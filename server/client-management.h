
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "message_handling.h"
//#include<mpi.h>
#define TRUE 1
#define FALSE !TRUE
#define SIZE_HASH_MAP 10
#define HASH_CONSTANT ((sqrt(5) - 1) / 2)
#define NUMOFGROUPS 5

/*
 * Client data structure
 */
typedef struct node{
   // int value;
    int cid;
    int gid;
    int fd;
    int status;
    struct node *next;
}clientInfo;

/* HASH FUNCTIONS */
int hash(int value);                                         /* hash function */

extern clientInfo* clientList[SIZE_HASH_MAP];

/*
 * Api's for client management
 */
void addClient(int cid, int gid, int fd);
clientInfo* searchClient_info(int cid, int gid,int fd);
void deleteClient(int cid, int gid, int fd);


/*Group structure contains list of clients within each group
 */
typedef struct groupNode{
    int clientId;
    int sockfd;
    struct groupNode *next;
}groupNode;

/*
 *Api's for group management
 */
void addClient_to_group(int cid,int gid,int fd);                               
void deleteClient_from_group(int cid,int gid,int fd);                           
void displayClients_within_group(int gid);
groupNode* getClientList_within_group(int gid);

void perform_task (void);

void displayUserInterface();       

