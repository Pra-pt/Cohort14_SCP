#include<stdio.h>
#include"client-management.h"
#include "task_mgr.h"
/*                                                                             
 *Process message and message structure                                        
 */                                                                            
 
#define JOIN_REQUEST 0
#define DATA 1
#define UPDATE_REQUEST 2


extern groupNode *groupList[];

/*struct Mesg_header{                                                            
    unsigned type:4;                                                            
    unsigned group_id:10;                                                       
    unsigned flags:8;  
    char payload[0];    //check on its memory allocation when data is received.
};*/                                                                             

/*handling server side
 * Message Receive
 * file name : server_connection_manager.c
 * here client_id is same as fd.
 */
void process_msg_at_server(struct Mesg_header *message, int client_id)              
{                                                                             
    struct Mesg_header *header = (struct Mesg_header *) message;
    printf("\nType=%d, group_id=%d, flags=%d \n", header->type,\
            header->group_id, header->flags);    
    clientInfo *client = clientList[hash(client_id)];
    switch(header->type)
    {
        case JOIN_REQUEST:     /*check for group id and if CONT flag is added,
                                 then  add to that group structure,
                                 and change client status to TASK_HANDLE
                                 if CONT FLAG not added, then do not update 
                                 client structure and change client state to 
                                 JOINING.
                                 */
                                
                                if(header->group_id)
                                {
                                     addClient_to_group(client_id,\
                                             header->group_id,client_id);
                                    if(header->flags == CONT)
                                    {
                                     client->status = TASK_HANDLE;
                                    }
                                    else
                                    {
                                        client->status = JOINING;
                                    }
                                }
                                printf("Message with type = Join Request \n ");
                                header->flags=ACK;
                                
                                if ((send(client_id,header, sizeof(header),0))== -1) {                         
                                 fprintf(stderr, "Failure Sending Message\n");                       
                                 close(client_id);                                               
                                 exit(1);                                                            
                                }                                                                       
                                else {                                                                  
                                 printf("Client:Message being sent: \n");                            
                                }  
                                break;

        case DATA:              printf("Message with type =data is received\n");
                      if ( header->group_id == GROUP_ADD) {
                      long int *x = (long int *)malloc (sizeof(long int));
                      *x= 0;
                      struct task_mgr* task_temp = 
                             find_task_by_group(header->group_id);
                      struct _data_struct_* new_data= task_temp->data_send;
                               
#if 1
                      groupNode *hashMapNode = groupList[GROUP_ADD];
                      while ( hashMapNode != NULL) {
                          if ((hashMapNode->status == BUSY) && hashMapNode->clientId == client_id) {
                  printf("\n %s : %d Found Hash Match", __FUNCTION__, __LINE__);

                              break;
                          }
                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                          hashMapNode = hashMapNode->next;
                      }

#endif

                      new_data = remove_data_to_task(task_temp, client_id);
                      if ( new_data == NULL ){
                          printf("Error in Data\n");
                      } else {
                          *x = *((long int *)(&header->payload[0]));
                          printf(" Payload Received %d %d\n", *x, ntohs(*x));
                          new_data->mesg = (char *)x;
                          add_data_to_task(task_temp, new_data, 0 );
                          if (hashMapNode != NULL)
                              hashMapNode->status = DONE;
                      }
                      }

                      break;
        case UPDATE_REQUEST:    printf("Message with type= Update Request\
                                        is received");
                                break;
        default:                printf("Wrong type received !!!");
    }

} 

/*auxillary function used for testing
 */
#if 0
int main()
{
    struct Mesg_header H1={JOIN_REQUEST, 10, 2};
    processMesgHeader(&H1);
    return 0;
}
#endif

int send_mesg (int state, int group_id, char* messag, int len) {
    struct Mesg_header *mesg = (struct Mesg_header*)malloc( \
           (sizeof(struct Mesg_header) + (len * (sizeof(int)))));
                  printf("\n %s : %d : Length %d", __FUNCTION__, __LINE__, len);
    switch (state) {
        case DATA:
            switch ( group_id ) {
                case GROUP_ADD :
                  mesg->type = DATA;
                  mesg->group_id = group_id;
                  mesg->flags = 0;
                  mesg->len = len; 

                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                  int *data_in = (int* )messag;
                  int *data_ptr = (int* )mesg->payload;
                  int ii = 0;
                  for( ii = 0; ii < len; ii++) {
                      data_ptr[ii] = htons(data_in[ii]);
                      printf("\n Payload[%d] htons(%d) %d", ii, data_in[ii], data_ptr[ii] );
                      //data_in++;
                  }
                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                  
                  groupNode *hashMapNode = groupList[GROUP_ADD];
                  int retry  = 5; 
                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                  while (retry) {
                      hashMapNode = groupList[GROUP_ADD];
                      while ( hashMapNode != NULL) {
                          if (hashMapNode->status != BUSY) {
                              if ((send(hashMapNode->sockfd, (char* ) mesg, \
                   (sizeof(struct Mesg_header) + (len * (sizeof(int)))) ,0))== -1)
                              {
                                  fprintf(stderr, "Failure Sending Message\n");
                                  close(hashMapNode->sockfd);
                                  exit(1);
                              } else {
                                  hashMapNode->status = BUSY;
                                  return (hashMapNode->sockfd);
                              }
                          }
                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                          hashMapNode = hashMapNode->next;
                      }
                  printf("\n %s : %d Retry %d", __FUNCTION__, __LINE__, retry);

                      sleep(1);
                      --retry;
                  }
                  printf("\n %s : %d", __FUNCTION__, __LINE__);
                  return -1;
            }
    }
    return -1;
}
