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
 * \file   exp_pal_mutex.h
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_MUTEX_H__
#define __EXP_PAL_MUTEX_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define PAL_MUTEX_NAME_MAX_STR_LEN             (32)

/*********************************** HANDLES **********************************/
typedef struct PAL_MUTEX_CTXT_X         *PAL_MUTEX_HDL;

/******************************** ENUMERATIONS ********************************/

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct _PAL_MUTEX_CREATE_PARAM_X
{
   uint8_t uca_mut_name_str [PAL_MUTEX_NAME_MAX_STR_LEN];
} PAL_MUTEX_CREATE_PARAM_X;

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_mutex_env_init ();

PAL_RET_E pal_mutex_env_deinit ();

PAL_RET_E pal_mutex_create (
   PAL_MUTEX_HDL  *phl_mutex_hdl,
   PAL_MUTEX_CREATE_PARAM_X   *px_create_param);

PAL_RET_E pal_mutex_destroy (
   PAL_MUTEX_HDL  hl_mutex_hdl);

PAL_RET_E pal_mutex_lock (
   PAL_MUTEX_HDL  hl_mutex_hdl);

PAL_RET_E pal_mutex_unlock (
   PAL_MUTEX_HDL  hl_mutex_hdl);

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_MUTEX_H__ */
