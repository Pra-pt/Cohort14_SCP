#include<stdio.h>

#include"task_mgr.h"
#include "client-management.h"

#define CLIENT_MAX_INPUT 100

//void perform_add_task();

int num[CLIENT_MAX_INPUT];

void perform_add_task()
{
        FILE *inputFile;
        int ii=0, len=0, offset=0;
        struct task_mgr* new_task=NULL;
        struct _data_struct_* new_data=NULL;

        inputFile = fopen("Add_input_text.txt", "r");
        printf("\n %s : %d", __FUNCTION__, __LINE__);
        
        new_task = get_new_task();
        new_task->group_id = GROUP_ADD; 
        new_task->data_send = NULL;
        new_task->data_recv = NULL;
        printf("\n %s : %d", __FUNCTION__, __LINE__);

        if (new_task == NULL){
           printf("Task Creation Error \n");
        }

        add_task(new_task);
        printf("\n %s : %d", __FUNCTION__, __LINE__);
        offset=0;
        printf("\n %s : %d", __FUNCTION__, __LINE__);
        while (!feof(inputFile)) {
        printf("\n %s : %d", __FUNCTION__, __LINE__);
            for (ii=0; ((ii < CLIENT_MAX_INPUT) && !feof(inputFile)); ii++) {
                fscanf(inputFile, "%d " ,&num[ii]);
            }
            len = ii;
        printf("\n %s : %d", __FUNCTION__, __LINE__);
            
            new_data = get_new_data();
        printf("\n %s : %d", __FUNCTION__, __LINE__);

            if ( new_data == NULL ) {
                printf("Data Creation Error \n");
            }
            add_data_to_task(new_task,new_data, 1);
            new_data->offset = offset;
            new_data->len    = len;
            new_data->client_id = send_mesg( DATA , new_task->group_id, &num[0], len);
        printf("\n %s : %d Cliend_id %d ", __FUNCTION__, __LINE__, new_data->client_id);

#if 0
            for (ii=0; ii< len; ii++) {
                printf("Index(%d) Value (%d)\n", ii, num[ii]);
            }
#endif

            offset += len;
        printf("\n %s : %d", __FUNCTION__, __LINE__);
        }

        new_data= new_task->data_send;
        int retry = 15;
        printf("\n %s : %d", __FUNCTION__, __LINE__);
        while ( retry ) {

            if (new_task->data_send == NULL)
                break;
            printf("%s: Retry %d\n", __FUNCTION__, retry);
            sleep(1);
            --retry;
        }
        new_data = new_task->data_recv;

        long int x = 0,result = 0;
        struct _data_struct_* del_data=NULL;
        while(new_data != NULL ) {
            x  = *((long int *)&new_data->mesg[0]);
            printf("ntohs(%ld) %ld\n", x, ntohs(x));
            result += ntohs(x);
            del_data = new_data;
            new_data = new_data->next_data;
            free(del_data->mesg);
            free(del_data);
        }
        printf("FINAL RESULT %ld\n", result);
        remove_task(new_task->task_id);

        fclose(inputFile);
}

#if 0
int main(){
    perform_add_task();
}
#endif
