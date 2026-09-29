// TODO: handle directives like @import etc

internal Token *consume_token_from_buffer(MemArena *arena, String8 *buffer)
{
    Assert(buffer);
    // TODO: Break out of function if no pound sign is found
    u64 offset_to_first_pound = 0;

    b32 found = str8_find(buffer, 0, str8("#"), &offset_to_first_pound);
    if (!found){
        Log("\nNo pound token found.\n");
        return &nil_token;
    }
    str8_chop_left_ptr(buffer, offset_to_first_pound + 1); // NOTE: do +1 to remove the pound sign and enable us to start parsing the directive immediatly

    //- NOTE: find the start_idx.
    // iterate the buffer and find the whitespaces
    // FIX: WRONG the pound sign should never be followed by a space
    // NOTE: I don't know. I know casey prefers using for loops for everything but arent while loops
    // easier to debug?
    u64 idx           = 0;
    b32 is_whitespace = 0;
    u64 end_offset    = 0;
    String8 substr    = {};

    while (!is_whitespace && idx < buffer->size) {
        is_whitespace = cstring_is_whitespace(buffer->data[idx]);
        end_offset    = idx;
        ++idx;
    }


    if (end_offset != 0) {
        substr = str8_create_substr(buffer, 0, end_offset);
        str8_chop_left_ptr(buffer, end_offset);
    }

    //- handle directives

    if (str8_is_match(substr, str8("import"))) {

        substr = str8_chop_left(*buffer, substr.size);

        while (cstring_is_whitespace(*buffer->data)) {
            str8_chop_left_ptr(buffer, 1);
        }

        u64 end_pos = 0;
        b32 found_first_newline_or_whitespace = false;
        for(u64 substr_index_to_find_newline_or_whitespace = 0; substr_index_to_find_newline_or_whitespace < substr.size; ++substr_index_to_find_newline_or_whitespace) {

            if(cstring_is_whitespace(buffer->data[substr_index_to_find_newline_or_whitespace]) ||
               cstring_is_newline(buffer->data[substr_index_to_find_newline_or_whitespace])) {
                end_pos = substr_index_to_find_newline_or_whitespace;
                found_first_newline_or_whitespace = true;
                continue;
            }

            if(found_first_newline_or_whitespace) {

                break;
            }

        }

        // TODO: we count one to many for some reason
        end_pos-= 1;

        substr = str8_create_substr(buffer, 0, end_pos);

    } else {
        if (str8_is_empty(&substr)) {
            PANIC("directive not defined yet");
            return &nil_token;
        }
        PANIC("Invalid import");
    }

    // NOTE: before there was a pice of code here that said that we should handle module code the
    // code here but this should stay a generic function. do the matrching on the returned struct
    // instead

    Token *new_token = PushStruct(arena, Token);
    new_token->type         = IdentToken;
    new_token->line         = 0;
    new_token->value.as_str = substr;
    return new_token;

    // NOTE: We return an invalid token if we can't find a one to consume.
    // see above.
}

// TODO: I will derefernce this for now. but think about it if you really want it to derefernce it?
// first initial pass to handle the includes and create on single large buffer
// but is this the approach we want?
//- rewrite the buffer to handle module imports
internal ModuleList *scan_buffer_and_handle_modules(MemArena *arena, String8 *buffer)
{

ModuleList *module_list = PushStruct(arena, ModuleList);

u64 insertion_position = 0;

    String8 dest_buffer = PushString8(arena, buffer->size);
    str8_copy(&dest_buffer, buffer);

    Token *token = consume_token_from_buffer(arena, buffer);

    // TODO: auto per directory include module feature
    // compile time arguments

    // TODO: remove one of these in the future when we have a proper seperation between the directory and per file loading
    String8List *files = {};

    String8 included_module_buffer = {};
    if (!is_token_nil(token)) {
#if 0
        files = DirLoad(arena, &token->value.as_str);
        if (files->count == 0) {
            PANIC("include not found in current directory.");
        }
#else

        Module *module = PushStruct(arena, Module);
included_module_buffer = FileLoad(arena, token->value.as_str);
        if(included_module_buffer.size == 0) {
            PANIC("include not found in current directory.");
        }

module->name = token->value.as_str;
#endif


    } else {
        // NOTE: if it's a nil token we should be able to assume that there is no pound token in the
        // file or that the directive used isnt known. or some other edge case. this will be logged
        // from inside the consume_token_from_buffer function
        Log("preprocessor early exit, no file specified");
goto end;
    }

    // TODO: this is the seperation between the per directoryu and per file handling
    // disabled for now
    // TODO: also needs a bit of refactoring
#if 0
    String8 temp = {};

    FOR_STR8_NODE(files, current)
    {
        if (str8_is_match(current->value, token->value.as_str)) {

            String8 current_value_file_path = current->value;

            if (!temp.size) {
                temp= FileLoad(arena, current_value_file_path);
            } else {
                str8_append(&temp, FileLoad(arena, current_value_file_path));
            }
            temp= FileLoad(arena, current->value);
            b32 result = str8_insert(arena, temp, &buffer, 0);
            if(result) {
                Log("preprocessor handled " Str8Fmt, current_value_file_path);

            } else {
                break;
            }
        }

// create module
{
Module *current_module = &nil_token_tree;
module->name = token.value->str_v;
if(module_list->current != &nil_modul)
module_list->curret
}



    }

#else

     insertion_position = 0;
     str8_insert(arena, buffer, &included_module_buffer, insertion_position);

#endif


#if DEBUG

if(preproc_insertion_result) {
b32 debug_output_result = FileWrite(buffer, "output.jr.ps");
Assert(debug_output_result);
}

#endif

end:
    Log("Preprocessor exit\n");
    return module_list;

    // TODO: ...
}