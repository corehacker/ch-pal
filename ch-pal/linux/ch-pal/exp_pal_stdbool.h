/*******************************************************************************
 *  Repository for C modules.
 *  Copyright (C) 2013 Sandeep Prakash
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/

/*******************************************************************************
 * Copyright (c) 2012, Sandeep Prakash <123sandy@gmail.com>
 *
 * \file   exp_pal_stdbool.h
 *
 * \author sandeepprakash
 *
 * \date   18-Oct-2013
 *
 * \brief
 *
 ******************************************************************************/

#ifndef _STDBOOL_H                                                               
#define _STDBOOL_H                                                               
                                                                                 
#ifndef __cplusplus   

typedef char _Bool;
                                                                                 
#define bool   _Bool                                                             
#define true   1                                                                 
#define false  0                                                                 
                                                                                 
#else /* __cplusplus */                                                          
                                                                                 
/* Supporting <stdbool.h> in C++ is a GCC extension.  */                         
#define _Bool  bool                                                              
#define bool   bool                                                              
#define false  false                                                             
#define true   true                                                              
                                                                                 
#endif /* __cplusplus */                                                         
                                                                                 
/* Signal that all the definitions are present.  */                              
#define __bool_true_false_are_defined  1                                         
                                                                                 
#endif   /* stdbool.h */




#ifndef _STDBOOL_H                                                               
#define _STDBOOL_H                                                               
                                                                                 
#ifndef __cplusplus                                                              
                                                                                 
#define bool   _Bool                                                             
#define true   1                                                                 
#define false  0                                                                 
                                                                                 
#else /* __cplusplus */                                                          
                                                                                 
/* Supporting <stdbool.h> in C++ is a GCC extension.  */                         
#define _Bool  bool                                                              
#define bool   bool                                                              
#define false  false                                                             
#define true   true                                                              
                                                                                 
#endif /* __cplusplus */                                                         
                                                                                 
/* Signal that all the definitions are present.  */                              
#define __bool_true_false_are_defined  1                                         
                                                                                 
#endif   /* stdbool.h */
