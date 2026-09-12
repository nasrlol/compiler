
void test_nil_checks()
{
    String8 buffer = &nil_str8;
    String8Node node = &nil_str8_node;

    b32 str8_result = is_nil_str8(buffer);
    b32 str8_node_result = is_nil_str8_node(node);


    Assert(str8_result);
}
