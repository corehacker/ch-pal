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
 * \date   17-Oct-2013
 *
 * \brief
 *
 ******************************************************************************/

/********************************** INCLUDES **********************************/
#include "ch-pal/exp_pal.h"

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
   PAL_THREAD_CTXT_X *px_thread_ctxt = NULL;

   px_thread_ctxt = pal_malloc (sizeof(PAL_THREAD_CTXT_X), NULL);
   if (NULL == px_thread_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_thread_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   px_thread_ctxt->hl_thread_hdl = CreateThread (NULL, 
      px_create_params->ui_stack_size_bytes, 
      (LPTHREAD_START_ROUTINE) px_create_params->fn_start_routine, 
      px_create_params->p_app_data, 0, px_thread_ctxt->ul_thread_id);
   if (NULL == px_thread_ctxt->hl_thread_hdl)
   {
      PAL_LOG_HIGH("CreateThread failed: %d, Errno: %d",
         *(px_thread_ctxt->ul_thread_id), errno);
      e_error = ePAL_RET_THREAD_CREATE_FAILED;
   }
   else
   {
      *phl_thread_hdl = (PAL_THREAD_HDL) px_thread_ctxt;
      PAL_LOG_FULL("CreateThread success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   if (ePAL_RET_SUCCESS != e_error)
   {
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
   return ePAL_RET_NOT_IMPLEMENTED;
}

PAL_RET_E pal_thread_get_id (
   PAL_THREAD_HDL hl_thread_hdl,
   uint32_t       *pui_thread_id)
{
   return ePAL_RET_NOT_IMPLEMENTED;
}

uint32_t pal_thread_get_current_thread_id ()
{
   return 0;
}
