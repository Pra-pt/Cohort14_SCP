/*
 *      An implementation in C of search and insertion operations for a hash 
 *      table with linear probing.
 *      compile with: gcc -lm client-management.c
 */

#include "client-management.h"
#include <pthread.h>

pthread_mutex_t clientList_mutex;
pthread_mutex_t groupList_mutex;

clientInfo* clientList[SIZE_HASH_MAP];           // hash table
groupNode *groupList[NUMOFGROUPS];

/* HASH FUNCTIONS */
int hash(int value){
    int hashIndex = value % SIZE_HASH_MAP;
    return hashIndex;
    //return (SIZE_HASH_MAP * fmod((value * HASH_CONSTANT),1));
}

/*
 *Client Api's implementation
 */
void addClient(int cid,int gid,int fd){
    //hashing based on the fd 
    int key = hash(fd);                                  
    // stores the hash value of the number to be inserted
    pthread_mutex_lock(&clientList_mutex);
    if(clientList[key] == NULL){                    
        // if the list in clientList[key] is empty
       // printf("\nwhen clientList[key] is NULL ");
        clientList[key] = malloc(sizeof(clientInfo));    // then creates a new list
        clientList[key]->cid = cid;
        clientList[key]->gid = gid;
        clientList[key]->fd = fd;
        clientList[key]->status=UNKNOWN;
        clientList[key]->next = NULL;
    }else{                                            
        // if the list in clientList[key] is not empty
        clientInfo *hashTableNode = clientList[key];
        while(hashTableNode->next!=NULL){                         
            // traverse the list
            hashTableNode = hashTableNode->next;
        }
        hashTableNode->next = malloc(sizeof(clientInfo));    
        // insert the value as the last element of the list
        hashTableNode->next->cid = cid;
        hashTableNode->next->gid = gid;
        hashTableNode->next->fd = fd;
        hashTableNode->next->status=UNKNOWN;
        hashTableNode->next->next = NULL;
    }
    pthread_mutex_unlock(&clientList_mutex);
}

clientInfo* searchClient_info(int cid,int gid,int fd){
    pthread_mutex_lock(&clientList_mutex);
    clientInfo *hashTableNode = clientList[hash(fd)]; 
    // pointer to the list stored in clientList[hash(value)]
    while(hashTableNode!=NULL){                                
        // scrolls the list
        if(hashTableNode->fd==fd){
            pthread_mutex_unlock(&clientList_mutex);
            return hashTableNode;         

        }
        hashTableNode = hashTableNode->next;
    }
    pthread_mutex_unlock(&clientList_mutex);
    return NULL;                                                  
}

void deleteClient(int cid,int gid,int fd)
{
    int key = hash(fd);
    pthread_mutex_lock(&clientList_mutex);
    if(clientList[key] == NULL) 
    {
        //client not present
        printf("client not found in the list\n");
        pthread_mutex_unlock(&clientList_mutex);
        return;
    }
    else
    {
        //clientInfo *head = clientList[key];
        if(clientList[key]->fd == fd)
        {
            clientList[key]=NULL;
        }
        else
        {
            clientInfo *head = clientList[key];
            while(head->next!=NULL && head->next->fd !=fd)
            {
                head = head->next;
            }
            if(head->next->fd == fd)
            {
                clientInfo *temp = head->next;
                head->next=head->next->next;
                free(temp);
                temp=NULL;
                printf("client with fd: %d  deleted successfully !!!\n",fd);
                pthread_mutex_unlock(&clientList_mutex);
                return;
            }
            if(head->next == NULL)
            {
                printf("client not found in the list\n");
                
            }
        }
        pthread_mutex_unlock(&clientList_mutex);
    }
}

/*
 *Group management Api's implementation
 */
void addClient_to_group(int cid,int gid,int fd){                                     
    // stores the hash value of the number to be inserted                       
    pthread_mutex_lock(&groupList_mutex);
    if(groupList[gid] == NULL){                                                
        // if the list in clientList[key] is empty                              
       // printf("\nwhen clientList[key] is NULL ");                            
        groupList[gid] = malloc(sizeof(groupNode));   // then creates a new list
        groupList[gid]->clientId = cid;
        groupList[gid]->sockfd=fd;                                             
        groupList[gid]->next = NULL;                                           
    }else{                                                                      
                                  
        groupNode *hashTableNode = groupList[gid];                            
        while(hashTableNode->next!=NULL){                                       
            // traverse the list                                                
            hashTableNode = hashTableNode->next;                                
        }                                                                       
        hashTableNode->next = malloc(sizeof(groupNode));                       
        // insert the value as the last element of the list                     
        hashTableNode->next->clientId = cid;
        hashTableNode->next->sockfd = fd;                                      
        hashTableNode->next->next = NULL;                                       
    }                                                                           
    pthread_mutex_unlock(&groupList_mutex);
} 

                                     
void deleteClient_from_group(int cid,int gid,int fd)                            
{                   
    pthread_mutex_lock(&groupList_mutex);
    if(groupList[gid] == NULL)                                                 
    {                                                                           
        //client not present                                                    
        printf("client not found in the grouplist\n");  
        pthread_mutex_unlock(&groupList_mutex);
        return;                                                                 
    }                                                                           
    else                                                                        
    {                                                                           
        //clientInfo *head = clientList[key];                                   
        if(groupList[gid]->clientId == cid)                                  
        {                                                                       
            groupList[gid]=NULL;                                               
        }                                                                       
        else                                                                    
        {                                                                       
            groupNode *head = groupList[gid];                                 
            while(head->next!=NULL && head->next->clientId !=cid)               
            {                                                                   
                head = head->next;                                              
            }                                                                   
            if(head->next->clientId == cid)                                     
            {                                                                   
                groupNode *temp = head->next;                                  
                head->next=head->next->next;                                    
                free(temp);                                                     
                temp=NULL;                                                      
                printf("client with cid: %d  deleted successfully from group:\
                        %d !!!\n",cid,gid);         
               pthread_mutex_unlock(&groupList_mutex);
               return;                                                         
            }                                                                   
            if(head->next == NULL)                                              
            {                                                                   
                printf("client not found in the grouplist\n");                
                                                                         
            }                                                                   
        }
        pthread_mutex_unlock(&groupList_mutex);
    }                                                                           
}        
void displayClients_within_group(int gid)
{
 pthread_mutex_lock(&groupList_mutex);   
 groupNode *hashMapNode = groupList[gid];
 printf("\nGroup %d :\n",gid);                              
 while(hashMapNode!=NULL){                                                      
    printf("\t client %d \n",hashMapNode->clientId);                            
    hashMapNode = hashMapNode->next;                                           
  }       
 printf("\n");
pthread_mutex_unlock(&groupList_mutex);
}


groupNode* getClientList_within_group(int gid)
{
    pthread_mutex_lock(&groupList_mutex);
    return groupList[gid];
    pthread_mutex_unlock(&groupList_mutex);
}

void perform_task (void);

/* Task Manager in Initial Stage */
#include<unistd.h>
#define MAXDATASIZE 1000
int result[5];
int total_res=0;

void perform_task ( void ) {
    char buf[MAXDATASIZE];
    int count =100;
    char *x;
    int ii =1, jj = 0;

    memset(buf,'\0',MAXDATASIZE);
    buf[0]=10;

    groupNode *hashMapNode = groupList[1];
    while(hashMapNode!=NULL){
   printf("\nSend client %d,fd %d \n",hashMapNode->clientId, hashMapNode->sockfd);
        memset(buf,'\0',MAXDATASIZE);
        buf[0]=5;
        for ( ii = 1 ; ii <= 5; ii++ ) {
            buf[ii] = ((char)rand() & 0x7f);
        }

        for (ii = 0; ii <= 5; ii++){
            printf(" %#x:%d ", buf[ii], buf[ii]);
        }
        printf(" Sent \n");

        if ((send(hashMapNode->sockfd,buf, 5 + 1/*strlen(buf)*/,0))== -1) {
            fprintf(stderr, "Failure Sending Message\n");
            close(hashMapNode->sockfd);
            exit(1);
        }
        hashMapNode = hashMapNode->next;

    }
        while ( total_res != 5 ) {
             printf ("!");
             usleep(500);
        }
        printf("\n\n\n SUMUP THE RESULTS %d %d %d %d %d ", \
                   result[0], result[1], result[2], \
                              result[3], result[4]);

        int final_result = result[0] + result[1] + result[2] \
                           + result[3] + result[4];
        printf(" = %d\n", final_result);

}


void displayUserInterface()       
{ //TODO
    int ch;
    while(1)
    {
        printf("******* Menu *******\n");
        printf("1. Display list of clients\n");
        printf("2. Display groups and client_info\n");
        printf("3. Perform Add Task\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1: printf("1");
                    break;
            case 2: printf("2");
                    displayClients_within_group(1);
                    break;
            case 3: printf("3");
                    perform_add_task(); //perform_task();
                    break;
            case 4: printf("4");
                    exit(0);
                    break;
            default: printf("wrong choice !! \n");

        }
    }
}

/* AUXILIARY FUNCTIONS */
void populateHashMap(int numbersToBeGenerated){                     // generates random numbers
    int k = 1;
    int randomNumber;
    for(k=1;k<=numbersToBeGenerated;k++){
        randomNumber = rand() % 100 + 1; 
        addClient(randomNumber,1,randomNumber);                            // inserts them in the hash map
    }
}

void printList(int hashMapRow){
    clientInfo *hashMapNode = clientList[hashMapRow];      
    while(hashMapNode!=NULL){
        printf("%d ",hashMapNode->cid);
        printf("%d ",hashMapNode->gid);
        printf("%d ",hashMapNode->fd);
        hashMapNode = hashMapNode->next;
    }
}

void printHashMap(){                                          
    int i;
    for(i=0;i<SIZE_HASH_MAP;i++){           // for every row of the hash map
        printf("clientList[%d]:\t",i);  
        printList(i);                       
        printf("\n");
    }
}

void searchNumbers(int numbersToBeSearched){
    int k;
    int randomNumber;
    for(k=1;k<=numbersToBeSearched;k++){            
        // searches a random number (from 1 to 100) numbersToBeSearched times
        randomNumber = rand() % 100 + 1; 
        printf("Is the value %d present? %d\n",randomNumber,searchClient_info\
                (randomNumber,1,randomNumber));
    }
}

#if 0
int main (int argc, char const *argv[]){
    srand(time(NULL));
    populateHashMap(25);                                                        
    printf("\nsituation after insertion of random integers:\n");                
    printHashMap();                                                             
    printf("\nsearch of some random integers:\n");                              
  //  searchNumbers(10); 
   addClient(10,2,10);
   printHashMap();
   deleteClient(10,2,10);
   printHashMap();
   addClient_to_group(1,2,1);
   addClient_to_group(2,3,2);
   addClient_to_group(10,2,10);
   displayClients_within_group(2);
   displayClients_within_group(3);
}
#endif
