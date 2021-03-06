/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "message_handling.h"
#define PORT "4020" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

#define MAXDATALENGTH 5000

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    size_t n;
    if (size) {
        --size;
        for ( n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = malloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

/*
 *1. send group join messages & send end message.
 *2. wait for ack message from server.
 *3. wait for server mesg for next instruction. 
 */
void client_manage(int sockfd){
    char buf[MAXDATASIZE];
    int count =100;
    char *x;

    memset(buf,'\0',MAXDATASIZE);
    buf[0] = 1;
    //printf("%s\n",argv[2]);
    //sprintf(buf,"%s", argv[2]);

    //if ((send(sockfd,buf, strlen(buf),0))== -1) {
    //        fprintf(stderr, "Failure Sending Message\n");
    //        close(sockfd);
    //        exit(1);
    //}
    //else {
        //printf("Client:Message being sent: %s\n",buf);


/*Taking list of groups which the client want to join.
*/
  int numOfGroups = 0,j = 0;                                                   
  int groups[1024];                                                             
  char temp;                                                                    
  printf("\nEnter groups to join:"); 
  do {                                                                          
      scanf("%d%c", &groups[numOfGroups], &temp);                              
      numOfGroups++;                                                           
  } while(temp != '\n');                                                        
                                                                                
  for(j=0; j<numOfGroups; j++) {                                              
    printf("%d ", groups[j]);                                                   
  }        

    for(j=0;j<numOfGroups;j++)
    {
        struct Mesg_header *mesg = (struct Mesg_header*)malloc(sizeof\
        (struct Mesg_header));
        mesg->type=JOIN_REQUEST;
        if(j==(numOfGroups-1))
        {
            mesg->flags=END;;
        }
        else
        {    
            mesg->flags=CONT;
        }
        mesg->group_id=groups[j];
        if ((send(sockfd,mesg, sizeof(mesg),0))== -1) {                       
            fprintf(stderr, "Failure Sending Message\n");               
            close(sockfd);                                              
            exit(1);                                                    
        }                                                                   
        else {                                                              
            printf("Client:Message being sent: \n");              
        }      

        recv(sockfd, mesg,sizeof(mesg), 0);
        printf("\nreceived mesg type:%d, group_id:%d , flag:%d  ",mesg->type\
                ,mesg->group_id,mesg->flags);
    }

    int *payload = NULL, ii = 0;
    struct Mesg_header *mesg = (struct Mesg_header*)malloc(/*sizeof \
                (struct Mesg_header)*/ MAXDATALENGTH );
    while (1) {
        //struct Mesg_header *mesg = (struct Mesg_header*)malloc(/*sizeof \
        //        (struct Mesg_header)*/ MAXDATALENGTH );
        //while (recv(sockfd, mesg, sizeof(mesg), 0) == 0);
        if ( recv(sockfd, mesg, MAXDATALENGTH, 0) != 0 ) { 
        printf("\n Received mesg type=%d , group_id =%d, len=%d, flags=%d\n",\
                mesg->type, mesg->group_id, mesg->len, mesg->flags);
        switch ( mesg->type ) {
            case DATA : 
                //int *payload = NULL;
                //payload = (int*)malloc(((mesg->len) * sizeof(int)));
                payload = (int *)&mesg->payload[0];
                //recv(sockfd, payload, (mesg->len) * sizeof(int) , 0);
                long int result = 0, *res ;
                for (ii = 0; ii < mesg->len; ii++){
            printf("Payload[%d] htons(%d) %d\n", ii, payload[ii], \
                    ntohs(payload[ii]));
                    result += ntohs(payload[ii]);
                }
                printf("\n Result = %ld\n", result);
                printf(" Result = %ld", result);
                mesg->len = 1;
                res = (long int*)&mesg->payload[0];
                *res = htons(result);
                printf("Final Result htons(%ld) %ld\n", result, *res);
                sleep(1);
                if ((send(sockfd,mesg, (sizeof(struct Mesg_header) + \
                                        sizeof(long int)),0))== -1) {
                    fprintf(stderr, "Failure Sending Message\n");               
                    close(sockfd);                                              
                    exit(1);                                                    
                }
        }
        }
    }


#if 0
    while(1 && count !=5) {
            memset(buf,'\0',MAXDATASIZE);

            sleep(2);
            /* snprintf(buf, MAXDATASIZE,"Client of group %s : Message: %s", \
                 argv[2], rand_string_alloc(10)); */

            if ((send(sockfd,buf, strlen(buf),0))== -1) {
                    fprintf(stderr, "Failure Sending Message\n");
                    close(sockfd);
                    exit(1);
            }
            else {
                    printf("Client:Message being sent: %s\n",buf);
            }
            count --;
            sleep(10);
        }
//#else
        int ii =1, jj = 0;
        printf("\n");
        recv(sockfd, buf, MAXDATASIZE, 0);
        printf(" DATA : \n");
        for (ii = 1 ; ii < buf[0]; ii++) {
            printf("%d:%#x ",buf[ii],buf[ii]);
        }
        printf("--> Recvd\n");
        x = (char *)buf;  
        int len = buf[0], sum = 0;
        for (jj = 1 ; jj <= len; jj++){
            //printf(" index %d : Value %d\n ", jj, buf[jj]);
            sum += buf[jj];
        }

        printf("SUM calculated %d:%#x : htons(sum): %#x \n", sum, sum, htons(sum));
        memset(buf,'\0',MAXDATASIZE);
        len = sizeof(int);
        buf[0] = (char) len;
        sum = htons(sum);
        x = (char *)&sum;
        buf[0] = (char) len;
        for ( jj = 0 ; jj < len; jj++){
           buf[jj+1] = x[jj];
        }

        if ((send(sockfd,buf, len+1,0))== -1) {
                fprintf(stderr, "Failure Sending Message\n");
                close(sockfd);
                exit(1);
        }
    //}

#endif
  //  }
    close(sockfd);
}

int main(int argc, char *argv[]){
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    int count =100;
    if (argc < 3) {
        fprintf(stderr,"usage: client hostname group\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char *x;
    if ((rv = getaddrinfo(argv[1], argv[2] /*PORT*/, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    /*    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
 
    printf("client: received '%s'\n",buf);
    */
    /*
     * Send group id first
     */

    client_manage(sockfd);
#if 0
    memset(buf,'\0',MAXDATASIZE);
    printf("%s\n",argv[2]);
    sprintf(buf,"%s", argv[2]);

    if ((send(sockfd,buf, strlen(buf),0))== -1) {
            fprintf(stderr, "Failure Sending Message\n");
            close(sockfd);
            exit(1);
    }
    else {
        printf("Client:Message being sent: %s\n",buf);
        while(1 && count !=5) {
            memset(buf,'\0',MAXDATASIZE);
            sleep(2);
            snprintf(buf, MAXDATASIZE,"Client of group %s : Message: %s", argv[2], rand_string_alloc(10));

            if ((send(sockfd,buf, strlen(buf),0))== -1) {
                    fprintf(stderr, "Failure Sending Message\n");
                    close(sockfd);
                    exit(1);
            }
            else {
                    printf("Client:Message being sent: %s\n",buf);
            }
            count --;
            sleep(10);
        }
		}
    close(sockfd);
#endif
    return 0;
}
