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
 * \file   pal_sock_priv.c
 *
 * \author sandeepprakash
 *
 * \date   23-Nov-2012
 *
 * \brief
 *
 ******************************************************************************/

/********************************** INCLUDES **********************************/
#include <ch-pal/exp_pal.h>

#include "pal_sock_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_setsockopt_so_reuseaddress (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_sock_ret   = -1;
   PAL_SOCK_REUSEADDR_DATA_X *px_data = NULL;


   if ((NULL == px_sock_ctxt) || (NULL == p_data) || (0 == ui_data_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_data = (PAL_SOCK_REUSEADDR_DATA_X *) p_data;

   i_sock_ret = setsockopt(px_sock_ctxt->hl_sock_hdl, SOL_SOCKET, SO_REUSEADDR,
      &(px_data->ui_opt_val), sizeof(px_data->ui_opt_val));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_sock_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("setsockopt success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt_so_broadcast (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_sock_ret   = -1;
   PAL_SOCK_BROADCAST_DATA_X *px_data = NULL;


   if ((NULL == px_sock_ctxt) || (NULL == p_data) || (0 == ui_data_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_data = (PAL_SOCK_BROADCAST_DATA_X *) p_data;

   i_sock_ret = setsockopt(px_sock_ctxt->hl_sock_hdl, SOL_SOCKET, SO_BROADCAST,
      &(px_data->ui_opt_val), sizeof(px_data->ui_opt_val));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_sock_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("setsockopt success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt_so_keepalive (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_sock_ret   = -1;
   PAL_SOCK_KEEPALIVE_DATA_X *px_data = NULL;

   if ((NULL == px_sock_ctxt) || (NULL == p_data) || (0 == ui_data_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_data = (PAL_SOCK_KEEPALIVE_DATA_X *) p_data;

   i_sock_ret = setsockopt (px_sock_ctxt->hl_sock_hdl, IPPROTO_TCP,
      TCP_KEEPIDLE, &(px_data->ui_keepalive_time_ms),
      sizeof(px_data->ui_keepalive_time_ms));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_sock_ret, errno);
   }

   i_sock_ret = setsockopt(px_sock_ctxt->hl_sock_hdl, SOL_SOCKET, SO_KEEPALIVE,
      &(px_data->ui_opt_val), sizeof(px_data->ui_opt_val));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_sock_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("setsockopt success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt_enable_blocking (
   PAL_SOCK_CTXT_X *px_sock_ctxt)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_fcntl_ret   = -1;
   int i_flags = 0;

   if (NULL == px_sock_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_fcntl_ret = fcntl (px_sock_ctxt->hl_sock_hdl, F_GETFL, 0);
   if (i_fcntl_ret < 0)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_fcntl_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
      goto LBL_CLEANUP;
   }

   i_flags = i_fcntl_ret;
   i_flags = i_flags & ~O_NONBLOCK;
   i_fcntl_ret = fcntl (px_sock_ctxt->hl_sock_hdl, F_SETFL, i_flags);
   if (0 != i_fcntl_ret)
   {
      PAL_LOG_HIGH("fcntl failed: %d, Errno: %d", i_fcntl_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("fcntl success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt_enable_non_blocking (
   PAL_SOCK_CTXT_X *px_sock_ctxt)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_fcntl_ret   = -1;
   int i_flags = 0;

   if (NULL == px_sock_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_fcntl_ret = fcntl (px_sock_ctxt->hl_sock_hdl, F_GETFL, 0);
   if (i_fcntl_ret < 0)
   {
      PAL_LOG_HIGH("fcntl failed: %d, Errno: %d", i_fcntl_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
      goto LBL_CLEANUP;
   }

   i_flags = i_fcntl_ret;
   i_flags = i_flags | O_NONBLOCK;
   i_fcntl_ret = fcntl (px_sock_ctxt->hl_sock_hdl, F_SETFL, i_flags);
   if (0 != i_fcntl_ret)
   {
      PAL_LOG_HIGH("fcntl failed: %d, Errno: %d", i_fcntl_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("fcntl success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt_tcp_nodelay (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int                i_sock_ret   = -1;
   PAL_SOCK_NODELAY_DATA_X   *px_data = NULL;

   if ((NULL == px_sock_ctxt) || (NULL == p_data) || (0 == ui_data_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_data = (PAL_SOCK_NODELAY_DATA_X *) p_data;

   i_sock_ret = setsockopt(px_sock_ctxt->hl_sock_hdl, IPPROTO_TCP, TCP_NODELAY,
      &(px_data->ui_opt_val), sizeof(px_data->ui_opt_val));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("setsockopt failed: %d, Errno: %d", i_sock_ret, errno);
      e_error = ePAL_RET_SOCK_OPTION_FAILED;
   }
   else
   {
      PAL_LOG_FULL("setsockopt success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

void pal_select_populate_fdset (
   PAL_SOCK_SET_X *px_set,
   fd_set *px_fdset,
   int32_t *pi_nfds)
{
   uint32_t ui_index = 0;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;

   FD_ZERO(px_fdset);
   if ((NULL != px_set) && (NULL != px_fdset) && (NULL != pi_nfds))
   {
      for (ui_index = 0; ui_index < px_set->ui_count; ui_index++)
      {
         if (NULL != px_set->xa_sock[ui_index].hl_sock_hdl)
         {
            px_sock_ctxt =
               (PAL_SOCK_CTXT_X *) px_set->xa_sock[ui_index].hl_sock_hdl;
            if (px_sock_ctxt->hl_sock_hdl >= 0)
            {
               FD_SET(px_sock_ctxt->hl_sock_hdl, px_fdset);
               if (px_sock_ctxt->hl_sock_hdl >= *pi_nfds)
               {
                  *pi_nfds = px_sock_ctxt->hl_sock_hdl + 1;
               }
            }
         }
      }
   }
}

void pal_select_populate_set_fdset (
   PAL_SOCK_SET_X *px_set,
   fd_set *px_fdset)
{
   uint32_t ui_index = 0;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;

   if ((NULL != px_set) && (NULL != px_fdset))
   {
      for (ui_index = 0; ui_index < px_set->ui_count; ui_index++)
      {
         if (NULL != px_set->xa_sock[ui_index].hl_sock_hdl)
         {
            px_sock_ctxt =
               (PAL_SOCK_CTXT_X *) px_set->xa_sock[ui_index].hl_sock_hdl;
            if (px_sock_ctxt->hl_sock_hdl >= 0)
            {
               if (FD_ISSET(px_sock_ctxt->hl_sock_hdl, px_fdset))
               {
                  px_set->xa_sock[ui_index].b_is_set = true;
               }
            }
         }
      }
   }
}

PAL_RET_E pal_sock_connect_check_status (
   PAL_SOCK_CTXT_X   *px_sock_ctxt,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   uint32_t ui_start_time = 0;
   uint32_t ui_curr_time = 0;
   uint32_t ui_elapsed_time = 0;
   struct timeval x_timeout = {0};
   fd_set x_write_set = {{0}};
   fd_set x_except_set = {{0}};
   int32_t i_ret_val = -1;
   int32_t i_sockopt_val = -1;
   int i_opt_val = -1;
   socklen_t ui_opt_len;
   int i_fd_isset = -1;

   if (NULL == px_sock_ctxt)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   ui_start_time = pal_get_system_time();
   do
   {
      x_timeout.tv_sec = (ui_timeout_ms / 1000);
      x_timeout.tv_usec = (ui_timeout_ms % 1000) * 1000;

      FD_ZERO(&x_write_set);
      FD_SET(px_sock_ctxt->hl_sock_hdl, &x_write_set);

      FD_ZERO(&x_except_set);
      FD_SET(px_sock_ctxt->hl_sock_hdl, &x_except_set);

      i_ret_val = select (px_sock_ctxt->hl_sock_hdl + 1, NULL, &x_write_set,
         &x_except_set, &x_timeout);
      if (i_ret_val < 0)
      {
         if (EINTR == errno)
         {
            ui_curr_time = pal_get_system_time ();
            ui_elapsed_time = ui_curr_time - ui_start_time;
            if (ui_elapsed_time >= ui_timeout_ms)
            {
               PAL_LOG_HIGH("select failed: %d, Errno: %d..Probably timed out",
                  i_ret_val, errno);
               e_error = ePAL_RET_SOCK_CONNECT_FAILED;
               break;
            }
            else
            {
               ui_timeout_ms = ui_timeout_ms - ui_elapsed_time;
               continue;
            }
         }
         else
         {
            PAL_LOG_HIGH("select failed: %d, Errno: %d", i_ret_val, errno);
            e_error = ePAL_RET_SOCK_CONNECT_FAILED;
            break;
         }
      }
      else
      {
         PAL_LOG_FULL("select success");
         i_fd_isset = FD_ISSET(px_sock_ctxt->hl_sock_hdl, &x_except_set);
         if (1 == i_fd_isset)
         {
            e_error = ePAL_RET_SOCK_CONNECT_FAILED;
            break;
         }

         i_fd_isset = FD_ISSET(px_sock_ctxt->hl_sock_hdl, &x_write_set);
         if (1 == i_fd_isset)
         {
            PAL_LOG_FULL("select success");
            i_sockopt_val = getsockopt (px_sock_ctxt->hl_sock_hdl, SOL_SOCKET,
               SO_ERROR, &i_opt_val, &ui_opt_len);
            if (i_sockopt_val < 0)
            {
               PAL_LOG_HIGH("getsockopt failed: %d, Errno: %d",
                  i_sockopt_val, errno);
               e_error = ePAL_RET_SOCK_CONNECT_FAILED;
               break;
            }
            if (1 == i_opt_val)
            {
               PAL_LOG_HIGH("getsockopt indicates error on socket: %d",
                  i_opt_val);
               e_error = ePAL_RET_SOCK_CONNECT_FAILED;
            }
            else
            {
               PAL_LOG_FULL("connect success");
               e_error = ePAL_RET_SUCCESS;
            }
            break;
         }
         else
         {
            PAL_LOG_FULL("connect failed");
            e_error = ePAL_RET_SOCK_CONNECT_FAILED;
            break;
         }
      }
   } while (1);

LBL_CLEANUP:
   return e_error;
}
