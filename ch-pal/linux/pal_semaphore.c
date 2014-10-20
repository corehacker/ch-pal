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
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

/********************************** INCLUDES **********************************/
#include <ch-pal/exp_pal.h>

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

   /*
    * The second argument '0' indicates that this semaphore is not a process
    * shared semaphore.
    */
   i_sem_ret_val = sem_init (&(px_sem_ctxt->hl_sem_hdl), 0,
      px_create_param->ui_initial_count);
   if (0 != i_sem_ret_val)
   {
      perror ("sem_init");
      PAL_LOG_HIGH("sem_init failed: %d, Errno: %d", i_sem_ret_val, errno);
      pal_free (px_sem_ctxt);
      px_sem_ctxt = NULL;
      e_error = ePAL_RET_SEM_CREATE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("sem_init success");
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
   int             i_sem_ret_val    = -1;

   if (NULL == hl_sem_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sem_ctxt = (PAL_SEM_CTXT_X *) hl_sem_hdl;

   i_sem_ret_val = sem_destroy(&(px_sem_ctxt->hl_sem_hdl));
   if (0 != i_sem_ret_val)
   {
      PAL_LOG_HIGH("sem_destroy failed: %d, Errno: %d", i_sem_ret_val, errno);
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
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   int             i_sem_ret_val    = -1;

   if (NULL == px_sem_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   /*
    * sem_wait()  decrements  (locks) the semaphore pointed to by sem.  If the
    * semaphore's value is greater than zero, then the decrement proceeds, and
    * the function returns, immediately.  If the semaphore currently has the
    * value zero, then the call blocks until either it becomes possible to
    * perform  the  decrement  (i.e., the semaphore value rises above zero),
    * or a signal handler interrupts the call.
    *
    * ERRORS
    *  EINTR  The call was interrupted by a signal handler; see signal(7).
    *  EINVAL sem is not a valid semaphore.
    */
   do
   {
      i_sem_ret_val = sem_wait (&(px_sem_ctxt->hl_sem_hdl));
      if (0 != i_sem_ret_val)
      {
         if (EINTR == errno)
         {
            continue;
         }
         else
         {
            PAL_LOG_HIGH("sem_wait failed: %d, Errno: %d",
               i_sem_ret_val, errno);
            e_error = ePAL_RET_SEM_GET_FAILED;
            break;
         }
      }
      else
      {
         PAL_LOG_FULL("sem_wait success");
         e_error = ePAL_RET_SUCCESS;
         break;
      }
   } while (1);

LBL_CLEANUP:
   return e_error;
}

static PAL_RET_E pal_sem_get_timed (
   PAL_SEM_CTXT_X *px_sem_ctxt,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E       e_error          = ePAL_RET_FAILURE;
   int             i_sem_ret_val    = -1;
   int             i_clock_ret_val  = -1;
   struct timespec x_timespec       = {0};

   if (NULL == px_sem_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_clock_ret_val = clock_gettime (CLOCK_REALTIME, &x_timespec);
   if (0 != i_clock_ret_val)
   {
      PAL_LOG_HIGH("clock_gettime failed: %d, Errno: %d",
         i_clock_ret_val, errno);
      e_error = ePAL_RET_CLOCK_FAILURE;
      goto LBL_CLEANUP;
   }

   /*
    * 1. Convert timeout in milliseconds to seconds.
    * 2. Take the remaining time and convert it into nano seconds.
    */
   x_timespec.tv_sec    += ui_timeout_ms / 1000;
   ui_timeout_ms        %= 1000;
   x_timespec.tv_nsec   += ui_timeout_ms * 1000000;

   /*
    * 3. Adjust the time. After the above operation of addition, the nanoseconds
    *    field might have a value greater than 1 second (1000000000)
    */
   x_timespec.tv_sec    += x_timespec.tv_nsec / 1000000000;
   x_timespec.tv_nsec   %= 1000000000;

   /*
    * sem_timedwait() is the same as sem_wait(), except that abs_timeout
    * specifies a limit on the amount of time that the call should block if
    * the decrement  cannot be  immediately  performed.   The  abs_timeout
    * argument  points to a structure that specifies an absolute timeout in
    * seconds and nanoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
    * This structure is defined as follows:
    *
    *     struct timespec {
    *         time_t tv_sec;      // Seconds
    *         long   tv_nsec;     // Nanoseconds [0 .. 999999999]
    *     };
    *
    * If the timeout has already expired by the time of the call, and the
    * semaphore could not be locked immediately, then sem_timedwait() fails
    * with a timeout error (errno set to ETIMEDOUT).
    *
    * If  the  operation  can be performed immediately, then sem_timedwait()
    * never fails with a timeout error, regardless of the value of abs_timeout.
    * Furthermore, the validity of abs_timeout is not checked in this case.
    *
    * ERRORS
    *  EINTR  The call was interrupted by a signal handler; see signal(7).
    *  EINVAL sem is not a valid semaphore.
    *  EINVAL The value of abs_timeout.tv_nsecs is less than 0, or greater than
    *         or equal to 1000 million.
    *  ETIMEDOUT
    *         The call timed out before the semaphore could be locked.
    */
   do
   {
      i_sem_ret_val = sem_timedwait (&(px_sem_ctxt->hl_sem_hdl),
         &x_timespec);
      if (0 != i_sem_ret_val)
      {
         if (EINTR == errno)
         {
            continue;
         }
         else if ((EAGAIN != errno) && (ETIMEDOUT != errno))
         {
            PAL_LOG_HIGH("sem_timedwait failed: %d, Errno: %d",
               i_sem_ret_val, errno);
            e_error = ePAL_RET_SEM_GET_FAILED;
            break;
         }
         else
         {
            PAL_LOG_FULL("sem_timedwait failed: %d, Errno: %d. "
               "Probably Timedout...", i_sem_ret_val, errno);
            e_error = ePAL_RET_SEM_IN_USE;
            break;
         }
      }
      else
      {
         PAL_LOG_FULL("sem_timedwait success");
         e_error = ePAL_RET_SUCCESS;
         break;
      }
   } while (1);

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
   int             i_sem_ret_val    = -1;

   if (NULL == hl_sem_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sem_ctxt = (PAL_SEM_CTXT_X *) hl_sem_hdl;

   i_sem_ret_val = sem_post(&(px_sem_ctxt->hl_sem_hdl));
   if (0 != i_sem_ret_val)
   {
      PAL_LOG_HIGH("sem_post failed: %d, Errno: %d", i_sem_ret_val, errno);
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
      PAL_LOG_FULL("sem_post success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}
