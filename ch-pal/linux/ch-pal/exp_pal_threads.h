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
 * \file   exp_pal_threads.h
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_THREADS_H__
#define __EXP_PAL_THREADS_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define PAL_THREAD_MAX_THREADS_PER_PROCESS         (32)

#define PAL_THREAD_NAME_MAX_STR_LEN                (32)

/******************************** ENUMERATIONS ********************************/

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct PAL_THREAD_CTXT_X *PAL_THREAD_HDL;

typedef void * (*pfn_pal_thread_start_routine) (
      void *p_thread_args);

typedef struct _PAL_THREAD_CREATE_PARAMS_X
{
   uint8_t                       uca_thread_name[PAL_THREAD_NAME_MAX_STR_LEN];

   uint32_t                      ui_stack_size_bytes;

   pfn_pal_thread_start_routine  fn_start_routine;

   void                          *p_app_data;
} PAL_THREAD_CREATE_PARAMS_X;

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_thread_env_init ();

PAL_RET_E pal_thread_env_deinit ();

PAL_RET_E pal_thread_create (
   PAL_THREAD_HDL *phl_thread_hdl,
   PAL_THREAD_CREATE_PARAMS_X *px_create_params);

PAL_RET_E pal_thread_delete (
   PAL_THREAD_HDL hl_thread_hdl);

PAL_RET_E pal_thread_get_id (
   PAL_THREAD_HDL hl_thread_hdl,
   uint32_t       *pui_thread_id);

uint32_t pal_thread_get_current_thread_id ();

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_THREADS_H__ */
