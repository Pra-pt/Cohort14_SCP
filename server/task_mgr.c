#include<stdio.h>
#include<stdlib.h>
#include"task_mgr.h"

#define PASS 0;

static unsigned int task_id = 1;

struct task_mgr *task_handle = NULL;

int add_task(struct task_mgr* new_task){
    if ( task_handle == NULL ){
        task_handle = new_task;
    } else {
        new_task->next_task = task_handle;
        task_handle = new_task;
    }    
    return PASS;
}

struct task_mgr* get_new_task(){
    struct task_mgr* new_task=NULL;
    new_task = (struct task_mgr *) malloc (sizeof(struct task_mgr));
    if (new_task == NULL){
         return NULL;
    }
    new_task->task_id = task_id;
    new_task->data_recv = NULL;
    new_task->data_send = NULL;
    new_task->next_task = NULL;
    task_id++;
    return new_task;
}

void remove_task( unsigned int task_id){
    struct task_mgr* prev = NULL;
    struct task_mgr* temp_task = task_handle;
    while (temp_task->task_id != task_id) {
        prev = temp_task;
        temp_task = temp_task->next_task;
        if ( temp_task == NULL ) {
            break;
        }
    }
    if (temp_task != NULL) {
        if ( prev == NULL){
            task_handle = temp_task->next_task;
        } else {
            prev->next_task = temp_task->next_task;
        }
        free(temp_task);
    }
}

struct task_mgr* find_task(unsigned int task_id){
    struct task_mgr* temp_task = task_handle;

    while (temp_task->task_id != task_id) {
        temp_task = temp_task->next_task;
        if ( temp_task == NULL ) {
            break;
        }
    }
    return temp_task;
}

struct task_mgr* find_task_by_group(int group_id){
    struct task_mgr* temp_task = task_handle;

    while (temp_task->group_id != group_id) {
        temp_task = temp_task->next_task;
        if ( temp_task == NULL ) {
            break;
        }
    }
    return temp_task;
}

struct _data_struct_* get_new_data(){
    struct _data_struct_* new_data=NULL;
    new_data = (struct _data_struct_ *) malloc (sizeof(struct _data_struct_));
    if (new_data == NULL){
         return NULL;
    }
    new_data->next_data = NULL;
    return new_data;
}

void add_data_to_task(struct task_mgr* new_task, struct _data_struct_* data , int send){
    if ( send ){
    if (new_task->data_send == NULL) { 
        new_task->data_send = data;
    } else {
        data->next_data = new_task->data_send;
        new_task->data_send = data;
    } 
    } else {
    if (new_task->data_recv == NULL) { 
        new_task->data_recv = data;
    } else {
        data->next_data = new_task->data_recv;
        new_task->data_recv = data;
    }
    }
}

struct _data_struct_* remove_data_to_task
             (struct task_mgr* new_task, int client_id){
    struct _data_struct_ *prev = NULL, *current = new_task->data_send;
    while (current != NULL){
        if (current->client_id == client_id) {
            printf("In %s, Client_id match\n",__FUNCTION__);
            if (prev == NULL){
                new_task->data_send = NULL;
                return current;
            } else {
                prev->next_data = current->next_data;
                return current;
            }
        }
        printf("In %s, Client_id mis-match %d %d\n",__FUNCTION__, current->client_id, client_id);
        prev = current;
        current = current->next_data;
    }
    return current;
}
