
#define JOIN_REQUEST 0                                                        
#define DATA 1                                                                  
#define UPDATE_REQUEST 2                                                        
                               
/*flags
 * CONT : continue                                                              
 * ACK:                                                                         
 * END                                                                          
 * */                                                                           
                                                                                
enum Flags{                                                                     
    CONT,                                                                       
    ACK,                                                                        
    END                                                                         
};                                                                              
struct Mesg_header{                                                             
     unsigned type:4;                                                           
     unsigned group_id:10;                                                      
     unsigned flags:8;                                                          
     unsigned len:16;
     char payload[0];  //check on its memory allocation when data is received.  
};                                                                              

//States that a client can have                                                                                                                                                
enum States{                                                                   
    UNKNOWN,                                                                    
    JOINING,                                                                    
    TASK_HANDLE                                                                 
};                                                                              
                      
