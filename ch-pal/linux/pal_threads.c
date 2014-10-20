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
 * \file   pal_threads.c
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

/********************************** INCLUDES **********************************/
#include <ch-pal/exp_pal.h>

#include "pal_threads_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_thread_env_init ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_thread_env_deinit ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_thread_create (
   PAL_THREAD_HDL *phl_thread_hdl,
   PAL_THREAD_CREATE_PARAMS_X *px_create_params)
{
   PAL_RET_E          e_error             = ePAL_RET_FAILURE;
   PAL_THREAD_CTXT_X *px_thread_ctxt      = NULL;
   int                i_pthread_ret       = -1;
   void              *p_stack_ptr         = NULL;
   pthread_attr_t    *px_thread_attr      = NULL;
   size_t             ui_cur_stack_size   = 0;

   if ((NULL == phl_thread_hdl) || (NULL == px_create_params))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *phl_thread_hdl = NULL;

   px_thread_ctxt = pal_malloc (sizeof(PAL_THREAD_CTXT_X), NULL);
   if (NULL == px_thread_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_thread_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   i_pthread_ret = pthread_attr_init(&(px_thread_ctxt->x_thread_attr));
   if (0 != i_pthread_ret)
   {
      PAL_LOG_HIGH("pthread_attr_init failed: %d, Errno: %d",
         i_pthread_ret, errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
      goto LBL_CLEANUP;
   }

   px_thread_attr = &(px_thread_ctxt->x_thread_attr);

   i_pthread_ret = pthread_attr_setdetachstate (
      px_thread_attr, PTHREAD_CREATE_DETACHED);
   if (0 != i_pthread_ret)
   {
      PAL_LOG_HIGH("pthread_attr_setdetachstate failed: %d, Errno: %d",
         i_pthread_ret, errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
      goto LBL_CLEANUP;
   }

   i_pthread_ret = pthread_attr_setinheritsched (
      px_thread_attr, PTHREAD_EXPLICIT_SCHED);
   if (0 != i_pthread_ret)
   {
      PAL_LOG_HIGH("pthread_attr_setinheritsched failed: %d, Errno: %d",
         i_pthread_ret, errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
      goto LBL_CLEANUP;
   }

   i_pthread_ret = pthread_attr_getstacksize (px_thread_attr,
      &ui_cur_stack_size);
   if ((0 != i_pthread_ret) || (0 == ui_cur_stack_size))
   {
      PAL_LOG_HIGH("pthread_attr_getstacksize failed: %d, Errno: %d",
         i_pthread_ret, errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
      goto LBL_CLEANUP;
   }

   if (px_create_params->ui_stack_size_bytes > ui_cur_stack_size)
   {
      i_pthread_ret = posix_memalign (&p_stack_ptr, sysconf (_SC_PAGESIZE),
         px_create_params->ui_stack_size_bytes);
      if (0 != i_pthread_ret)
      {
         PAL_LOG_HIGH("posix_memalign failed: %d, Errno: %d",
            i_pthread_ret, errno);
         e_error = ePAL_RET_THREAD_CREATE_FAILED;
         goto LBL_CLEANUP;
      }

      i_pthread_ret = pthread_attr_setstack (px_thread_attr, p_stack_ptr,
         px_create_params->ui_stack_size_bytes);
      if (0 != i_pthread_ret)
      {
         PAL_LOG_HIGH("pthread_attr_setstack failed: %d, Errno: %d",
            i_pthread_ret, errno);
         e_error = ePAL_RET_THREAD_CREATE_FAILED;
         goto LBL_CLEANUP;
      }
   }

   i_pthread_ret = pthread_create (&(px_thread_ctxt->hl_thread_hdl),
      px_thread_attr, px_create_params->fn_start_routine,
      px_create_params->p_app_data);
   if (0 != i_pthread_ret)
   {
      PAL_LOG_HIGH("pthread_create failed: %d, Errno: %d",
         i_pthread_ret, errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
   }
   else
   {
      *phl_thread_hdl = (PAL_THREAD_HDL) px_thread_ctxt;
      PAL_LOG_FULL("pthread_create success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   if (ePAL_RET_SUCCESS != e_error)
   {
      if (NULL != px_thread_attr)
      {
         i_pthread_ret = pthread_attr_destroy (px_thread_attr);
         if (0 != i_pthread_ret)
         {
            PAL_LOG_HIGH("pthread_attr_destroy failed: %d, Errno: %d",
               i_pthread_ret, errno);
         }
      }
      if (NULL != px_thread_ctxt)
      {
         pal_free (px_thread_ctxt);
         px_thread_ctxt = NULL;
      }
   }
   return e_error;
}

PAL_RET_E pal_thread_delete (
   PAL_THREAD_HDL hl_thread_hdl)
{
   PAL_RET_E          e_error          = ePAL_RET_FAILURE;
   PAL_THREAD_CTXT_X *px_thread_ctxt   = NULL;

   if (NULL == hl_thread_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_thread_ctxt = (PAL_THREAD_CTXT_X *) hl_thread_hdl;
   px_thread_ctxt->hl_thread_hdl = -1;
   pal_free (px_thread_ctxt);
   px_thread_ctxt = NULL;
   e_error = ePAL_RET_SUCCESS;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_thread_get_id (
   PAL_THREAD_HDL hl_thread_hdl,
   uint32_t       *pui_thread_id)
{
   PAL_RET_E          e_error          = ePAL_RET_FAILURE;
   PAL_THREAD_CTXT_X *px_thread_ctxt   = NULL;

   if ((NULL == hl_thread_hdl) || (NULL == pui_thread_id))
   {
      PAL_LOG_HIGH("Invalid Handle/Args");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_thread_ctxt = (PAL_THREAD_CTXT_X *) hl_thread_hdl;
   *pui_thread_id = px_thread_ctxt->hl_thread_hdl;
   e_error = ePAL_RET_SUCCESS;
LBL_CLEANUP:
   return e_error;
}

uint32_t pal_thread_get_current_thread_id ()
{
   return (uint32_t) pthread_self ();
}
