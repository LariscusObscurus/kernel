/*
 * stdargs.h
 *
 *  Created on: 25.04.2015
 *      Author: leonhardt
 */

#ifndef STDARGS_H_
#define STDARGS_H_

typedef __builtin_va_list       va_list;
#define va_start(ap, X)         __builtin_va_start(ap, X)
#define va_arg(ap, type)        __builtin_va_arg(ap, type)
#define va_end(ap)              __builtin_va_end(ap)

#endif /* STDARGS_H_ */
