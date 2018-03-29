#pragma once
#include <iostream>
#include <GL\glew.h>

void _check_gl_error(const char *file, int line, int level = 0);
void _toggle_gl_debug();
#define check_gl_error_full() _check_gl_error(__FILE__,__LINE__,0)
#define check_gl_error() _check_gl_error(__FILE__,__LINE__,1)
