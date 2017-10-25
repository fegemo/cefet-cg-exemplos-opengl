#ifndef _CHECKERROR_H_
#define _CHECKERROR_H_

void check(const char *file, int line);

///
/// Uso:
/// [... algumas chamadas opengl]
/// check_gl_error();
///
#define check_gl_error() check(__FILE__,__LINE__)

#endif // _CHECKERROR_H_
