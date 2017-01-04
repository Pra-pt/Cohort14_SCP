
int result = 0;
void perform_add_op(char *mesg, int len){
    int* x;
    x = (int* ) mesg;
    for (int ii=0; ii<len; ii++){
        result += x[ii];
    }
    printf("\nResult %d\n", result);
    recv_mesg( 1 , 0, &result, 1 );

}
