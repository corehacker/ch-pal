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
 * \file   pal_semaphore.c
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

#include "pal_semaphore_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/
static PAL_RET_E pal_sem_get_infinite (
   PAL_SEM_CTXT_X *px_sem_ctxt);

static PAL_RET_E pal_sem_get_timed (
   PAL_SEM_CTXT_X *px_sem_ctxt,
   uint32_t ui_timeout);

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_sem_env_init ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_sem_env_deinit ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_sem_create(
   PAL_SEM_HDL *phl_sem_hdl,
   PAL_SEM_CREATE_PARAM_X *px_create_param)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   PAL_SEM_CTXT_X *px_sem_ctxt      = NULL;
   int             i_sem_ret_val    = -1;

   if ((NULL == phl_sem_hdl) || (NULL == px_create_param))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (px_create_param->ui_max_count > PAL_SEM_VALUE_MAX)
   {
      PAL_LOG_HIGH("Invalid Args. ui_max_count cannot be greater than %d"
         " (PAL_SEM_VALUE_MAX)", PAL_SEM_VALUE_MAX);
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *phl_sem_hdl = NULL;

   px_sem_ctxt = (PAL_SEM_CTXT_X *) pal_malloc (sizeof(PAL_SEM_CTXT_X), NULL);
   if(NULL == px_sem_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_sem_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   (void) pal_memmove (&(px_sem_ctxt->x_create_param), px_create_param,
      sizeof(px_sem_ctxt->x_create_param));

   px_sem_ctxt->hl_sem_hdl = CreateSemaphore(NULL, px_create_param->ui_initial_count, 
      px_create_param->ui_max_count, NULL);
   if (NULL == px_sem_ctxt->hl_sem_hdl)
   {
      perror ("sem_init");
      PAL_LOG_HIGH("CreateSemaphore failed: %d, Errno: %d", i_sem_ret_val, errno);
      pal_free (px_sem_ctxt);
      px_sem_ctxt = NULL;
      e_error = ePAL_RET_SEM_CREATE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("CreateSemaphore success. Initial: %d, Max: %d",
         px_create_param->ui_initial_count, px_create_param->ui_max_count);
      *phl_sem_hdl = (PAL_SEM_HDL) px_sem_ctxt;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sem_destroy(
   PAL_SEM_HDL hl_sem_hdl)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   PAL_SEM_CTXT_X *px_sem_ctxt      = NULL;
   BOOL b_mut_ret_val = false;

   if (NULL == hl_sem_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sem_ctxt = (PAL_SEM_CTXT_X *) hl_sem_hdl;

   b_mut_ret_val = CloseHandle(px_sem_ctxt->hl_sem_hdl);
   if (false == b_mut_ret_val)
   {
      PAL_LOG_HIGH("CloseHandle failed: %d, Errno: %d", b_mut_ret_val, errno);
      e_error = ePAL_RET_SEM_DELETE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("sem_destroy success");
      e_error = ePAL_RET_SUCCESS;
   }
   pal_memset (&(px_sem_ctxt->hl_sem_hdl), 0x00,
      sizeof(px_sem_ctxt->hl_sem_hdl));
   pal_free (px_sem_ctxt);
   px_sem_ctxt = NULL;
LBL_CLEANUP:
   return e_error;
}

static PAL_RET_E pal_sem_get_infinite (
   PAL_SEM_CTXT_X *px_sem_ctxt)
{
   return pal_sem_get_timed (px_sem_ctxt, INFINITE);
}

static PAL_RET_E pal_sem_get_timed (
   PAL_SEM_CTXT_X *px_sem_ctxt,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   DWORD i_sem_ret_val = 0;

   if (NULL == px_sem_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   i_sem_ret_val = WaitForSingleObject(px_sem_ctxt->hl_sem_hdl, 
      ui_timeout_ms);
   switch (i_sem_ret_val)
   {
   case WAIT_ABANDONED:
      {
         PAL_LOG_HIGH("WaitForSingleObject failed: %d, Errno: %d",
            i_sem_ret_val, GetLastError ());
         e_error = ePAL_RET_SEM_GET_FAILED;
         break;
      }
   case WAIT_OBJECT_0:
      {
         PAL_LOG_FULL("WaitForSingleObject success");
         e_error = ePAL_RET_SUCCESS;
         break;
      }
   case WAIT_TIMEOUT:
      {
         PAL_LOG_FULL("WaitForSingleObject timedout: %d, Errno: %d",
            i_sem_ret_val, GetLastError ());
         e_error = ePAL_RET_SEM_IN_USE;
         break;
      }
   case WAIT_FAILED:
      {
         PAL_LOG_HIGH("WaitForSingleObject failed: %d, Errno: %d",
            i_sem_ret_val, GetLastError ());
         e_error = ePAL_RET_SEM_GET_FAILED;
         break;
      }
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sem_get(
   PAL_SEM_HDL hl_sem_hdl,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   PAL_SEM_CTXT_X *px_sem_ctxt      = NULL;


   if (NULL == hl_sem_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sem_ctxt = (PAL_SEM_CTXT_X *) hl_sem_hdl;
   if (PAL_TIME_WAIT_INFINITE == ui_timeout_ms)
   {
      e_error = pal_sem_get_infinite (px_sem_ctxt);
      if (ePAL_RET_SUCCESS != e_error)
      {
         PAL_LOG_FULL("pal_sem_get_infinite failed: %d", e_error);
      }
   }
   else
   {
      e_error = pal_sem_get_timed (px_sem_ctxt, ui_timeout_ms);
      if (ePAL_RET_SUCCESS != e_error)
      {
         PAL_LOG_FULL("pal_sem_get_timed failed: %d", e_error);
      }
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sem_put(
   PAL_SEM_HDL hl_sem_hdl)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   PAL_SEM_CTXT_X *px_sem_ctxt      = NULL;
   BOOL b_sem_ret_val = false;
   LONG ui_prev_value = 0;

   if (NULL == hl_sem_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sem_ctxt = (PAL_SEM_CTXT_X *) hl_sem_hdl;

   b_sem_ret_val = ReleaseSemaphore(px_sem_ctxt->hl_sem_hdl, 1, 
      &ui_prev_value); // TODO
   if (false == b_sem_ret_val)
   {
      PAL_LOG_LOW("ReleaseSemaphore failed: %d, Errno: %d", b_sem_ret_val, 
         GetLastError ());
      if (EOVERFLOW == errno)
      {
         e_error = ePAL_RET_SEM_EXCEEDED_LIMIT;
      }
      else
      {
         e_error = ePAL_RET_SEM_PUT_FAILED;
      }
   }
   else
   {
      PAL_LOG_FULL("ReleaseSemaphore success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}
