/*******************************************************************************
 *  Repository for C modules.
 *  Copyright (C) 2012 Sandeep Prakash
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
 * \file   pal_file_private.h
 *
 * \author sandeepprakash
 *
 * \date   17-Oct-2013
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __PAL_FILE_PRIVATE_H__
#define __PAL_FILE_PRIVATE_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define PAL_FILE_NAME_STR_MAX_LEN                  (512)

/******************************** ENUMERATIONS ********************************/

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef FILE *LINUX_FILE_HANDLE;

typedef struct _PAL_FILE_CXT_X
{
   LINUX_FILE_HANDLE hl_file_hdl;

   uint8_t uca_filename[PAL_FILE_NAME_STR_MAX_LEN];
} PAL_FILE_CXT_X;

/***************************** FUNCTION PROTOTYPES ****************************/

#ifdef   __cplusplus
}
#endif

#endif /* __PAL_FILE_PRIVATE_H__ */
