
struct _data_struct_ {
    int client_id;
    int offset;
    int len;
    char* mesg;
    struct _data_struct_* next_data;
} ;

struct task_mgr {
    unsigned int task_id;
    int group_id;
    struct _data_struct_ *data_send;
    struct _data_struct_ *data_recv;
    struct task_mgr* next_task;
};

extern struct task_mgr *task_handle;

struct task_mgr* get_new_task();
struct _data_struct_* get_new_data();
struct task_mgr* find_task_by_group(int group_id);
struct _data_struct_* remove_data_to_task\
        (struct task_mgr* new_task, int client_id);
