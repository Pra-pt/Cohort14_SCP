#include "connection_mgr.h"


int send_mesg(int mesg_type, int group_id, char *mesg, int len){

    switch (mesg_type) {
        case TASK;
            if ( group_id == GROUP_ADD ) {
#if 0
                recv_mesg(mesg_type, group_id, mesg, len);
#else
                perform_add_op(mesg, len);
#endif
                return 1;
            }
    }
    return 1;
}

int result = 0;
int recv_mesg(int mesg_type, int group_id, char* mesg, int len){
    switch (mesg_type) {
        case TASK;
            if ( group_id == GROUP_ADD ) {
                struct task_mgr* task;
                struct _data_struct_* new_data=NULL;
                task = find_task(1);
                if (remove_data_to_task(task, 1)) {
                    new_data = get_new_data();
                    if ( new_data == NULL ) {
                         printf("Data Creation Error \n");
                    }
                    new_data->offset = 0;
                    new_data->len    = len;
                    new_data->cliend_id = 1;

                    add_data_to_task(new_task,new_data, 0);
                }
#if 0
                perform_add_op();
#endif
            }
    }

}


