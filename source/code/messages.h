#ifndef MESSAGES_H
#define MESSAGES_H

global_variable String8List *errors;
global_variable String8List *warnings;

internal void WARN(String8List *warning_list);
internal void ERROR(String8List *error_list);

#define PUSH_WARNING(message) _PUSH_WARNING(global_arena, warnings, message)
#define PUSH_ERROR(message)   _PUSH_ERROR(global_arena, errors, message)

#if DEBUG // DEBUG

#define PANIC(panic_message)				      \
    do { 						                 \
        Log(panic_message);         \
        OSExit();					             \
    } while(0);						           \

#else

#define PANIC(panic_message)				      \
    do { 						                 \
        Log(panic_message);	\
        breakpoint();                             \
    } while(0);		                           \



#endif //-

#define _PUSH_ERROR(global_arena, error_list, message)			\
    do {								                          \
        if(error_list != NULL) {					              \
            errors_list = PushStruct(global_arena, String8List)   \
        }							                             \
        str8_list_push_node(error_list, str8_node(message));	  \
        \
    } while(0);



#define _PUSH_WARNING(global_arena, error_list, message)		  \
    do {								                          \
        if(error_list != NULL) {					              \
            errors_list = PushStruct(global_arena, String8List)   \
        }							\
        str8_list_push_node(error_list, str8_node(message));  	\
        \
    } while(0);

internal void
warn(MemArena *arena, String8List *warning_list)
{
    u64 total_size = 0;

    FOR_STR8_NODE(warning_list, current) {
        total_size += current->value.size;
    }

    String8 big_buffer = PushString8(arena, total_size);
    FOR_STR8_NODE(warning_list, current) {
        str8_append(&big_buffer, str8("Warning: "));
        str8_append(&big_buffer, current->value);
        str8_append(&big_buffer, str8("\n"));
    }
    LogStr8(big_buffer);
}


internal void
error(MemArena *arena, String8List *error_list)
{
    u64 total_size = 0;
    FOR_STR8_NODE(error_list, current) {
        total_size += current->value.size;
    }

    String8 big_buffer = PushString8(arena, total_size);
    FOR_STR8_NODE(error_list, current) {
        str8_append(&big_buffer, str8("Error: "));
        str8_append(&big_buffer, current->value);
        str8_append(&big_buffer, str8("\n"));
    }
    LogStr8(big_buffer);
}



#endif // MESSAGES_H
