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
 * \file   pal_mutex.c
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

#include "pal_mutex_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_mutex_env_init()
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_mutex_env_deinit()
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_mutex_create(
   PAL_MUTEX_HDL *phl_mutex_hdl,
   PAL_MUTEX_CREATE_PARAM_X *px_create_param)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_MUTEX_CTXT_X *px_mut_ctxt = NULL;
   int i_mut_ret_val = -1;

   if ((NULL == phl_mutex_hdl) || (NULL == px_create_param))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *phl_mutex_hdl = NULL;

   px_mut_ctxt = pal_malloc (sizeof(PAL_MUTEX_CTXT_X), NULL );
   if (NULL == px_mut_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_mut_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   i_mut_ret_val = pthread_mutex_init (&(px_mut_ctxt->hl_mut_hdl), 0);
   if (0 != i_mut_ret_val)
   {
      PAL_LOG_HIGH("pthread_mutex_init failed: %d, Errno: %d",
         i_mut_ret_val, errno);
      pal_free (px_mut_ctxt);
      px_mut_ctxt = NULL;
      e_error = ePAL_RET_MUTEX_CREATE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("pthread_mutex_init success");
      *phl_mutex_hdl = (PAL_MUTEX_HDL) px_mut_ctxt;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_mutex_destroy(
   PAL_MUTEX_HDL hl_mutex_hdl)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_MUTEX_CTXT_X *px_mut_ctxt = NULL;
   int i_mut_ret_val = -1;

   if (NULL == hl_mutex_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_mut_ctxt = (PAL_MUTEX_CTXT_X *) hl_mutex_hdl;

   i_mut_ret_val = pthread_mutex_destroy (&(px_mut_ctxt->hl_mut_hdl));
   if (0 != i_mut_ret_val)
   {
      PAL_LOG_HIGH("pthread_mutex_destroy failed: %d, Errno: %d",
         i_mut_ret_val, errno);
      e_error = ePAL_RET_MUTEX_DELETE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("pthread_mutex_destroy success");
      e_error = ePAL_RET_SUCCESS;
   }
   pal_free (px_mut_ctxt);
   px_mut_ctxt = NULL;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_mutex_lock(
   PAL_MUTEX_HDL hl_mutex_hdl)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_MUTEX_CTXT_X *px_mut_ctxt = NULL;
   int i_mut_ret_val = -1;

   if (NULL == hl_mutex_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_mut_ctxt = (PAL_MUTEX_CTXT_X *) hl_mutex_hdl;

   i_mut_ret_val = pthread_mutex_lock (&(px_mut_ctxt->hl_mut_hdl));
   if (0 != i_mut_ret_val)
   {
      PAL_LOG_HIGH("pthread_mutex_lock failed: %d, Errno: %d",
         i_mut_ret_val, errno);
      e_error = ePAL_RET_MUTEX_LOCK_FAILED;
   }
   else
   {
      PAL_LOG_FULL("pthread_mutex_lock success: %p", px_mut_ctxt);
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_mutex_unlock(
   PAL_MUTEX_HDL hl_mutex_hdl)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_MUTEX_CTXT_X *px_mut_ctxt = NULL;
   int i_mut_ret_val = -1;

   if (NULL == hl_mutex_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_mut_ctxt = (PAL_MUTEX_CTXT_X *) hl_mutex_hdl;

   i_mut_ret_val = pthread_mutex_unlock (&(px_mut_ctxt->hl_mut_hdl));
   if (0 != i_mut_ret_val)
   {
      PAL_LOG_HIGH("pthread_mutex_unlock failed: %d, Errno: %d",
         i_mut_ret_val, errno);
      e_error = ePAL_RET_MUTEX_UNLOCK_FAILED;
   }
   else
   {
      PAL_LOG_FULL("pthread_mutex_unlock success: %p", px_mut_ctxt);
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}
