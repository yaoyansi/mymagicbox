import sys

g_indent     = 0;
g_indentChar = '    ';

'''
usage:

import flog

@flog.trace
def f1():
    ...
'''
def trace(func):
    def wrapper(*args):
        global g_indent; 
        global g_indentChar;

        g_indent += 1;
        print g_indentChar*g_indent, func.__name__, args, 
        print

        ret = func(*args);

        g_indent -= 1;
        return ret;

    return wrapper
