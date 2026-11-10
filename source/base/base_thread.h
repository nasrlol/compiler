#ifndef BASE_THREAD_H

thread_local Arena *tls_scratch_arena = 0;
#define ScratchGet() scratch_start(tls_scratch_arena)

#define BASE_THREAD_H

typedef void ThreadProc(void *param);


typedef struct Thread Thread;
struct Thread {
    ScratchArena *memory;
    u64 data[1];
};   

typedef struct ThreadStart ThreadStart;
struct ThreadStart {
    ThreadProc *proc;
    void       *param;
};


internal Thread thread_launch(ThreadProc *proc, void *param);
internal b32 thread_join(Thread thread);     
internal void thread_detach(Thread thread);  
internal void thread_set_name(String8 name);   
internal u64  thread_current_id(void);        


#endif // BASE_THREAD_H
