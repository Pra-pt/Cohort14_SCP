#include<stdio.h>
#include"client-management.h"
/*                                                                             
 *Process message and message structure                                        
 */                                                                            
 
#define JOIN_REQUEST 0
#define DATA 1
#define UPDATE_REQUEST 2




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
                                    if(header->flags == CONT)
                                    {
                                     addClient_to_group(client_id,\
                                             header->group_id,client_id);
                                     client->status = TASK_HANDLE;
                                    }
                                    else
                                    {
                                        client->status = JOINING;
                                    }
                                }
                                printf("Message with type = Join Request \n ");
                                break;
        case DATA:              printf("Message with type =data is received\n");
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
