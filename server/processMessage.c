#include<stdio.h>
/*                                                                             
 *Process message and message structure                                        
 */                                                                            
 
#define JOIN_REQUEST 0
#define DATA 1
#define UPDATE_REQUEST 2

struct Mesg_header{                                                            
    unsigned type:4;                                                            
    unsigned group_id:10;                                                       
    unsigned flags:8;                                                           
};                                                                             


void processMesgHeader(struct Mesg_header *message)                             
{                                                                             
    struct Mesg_header *header = (struct Mesg_header *) message;
    printf("\nType=%d, group_id=%d, flags=%d \n", header->type,\
            header->group_id, header->flags);    
    switch(header->type)
    {
        case JOIN_REQUEST:      printf("Message with type = Join Request \n ");
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
int main()
{
    struct Mesg_header H1={JOIN_REQUEST, 10, 2};
    processMesgHeader(&H1);
    return 0;
}

