#include "ruby.h"

static VALUE get_hello(VALUE self) 
{ 
    VALUE result = rb_str_new2("hello"); 
    return result; 
} 

static VALUE get_world(VALUE self) 
{
    VALUE result = rb_str_new2("world"); 
    return result; 
} 

VALUE cHelloWorld; 
void Init_hello_ruby_world()
{ 
    cHelloWorld = rb_define_class("HelloWorld", rb_cObject); 
    rb_define_method(cHelloWorld, "get_hello", get_hello, 0);
    rb_define_method(cHelloWorld, "get_world", get_world, 0);
} 
