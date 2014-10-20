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
 * \file   pal_sock.c
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

#include "pal_sock_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_sock_env_init ()
{
   PAL_RET_E    e_error    = ePAL_RET_FAILURE;
   WSADATA      x_wsa_data = {0};                                                               
   int          i_wsa_ret = -1;

   i_wsa_ret = WSAStartup(MAKEWORD(2,2), &x_wsa_data);
   if (NO_ERROR == i_wsa_ret)
   {
      e_error = ePAL_RET_SUCCESS;
   }
   else
   {
      e_error = ePAL_RET_FAILURE;
   }
   return e_error;
}

PAL_RET_E pal_sock_env_deinit ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;
   int          i_wsa_ret = -1;

   i_wsa_ret = WSACleanup ();
   if (NO_ERROR == i_wsa_ret)
   {
      e_error = ePAL_RET_SUCCESS;
   }
   else
   {
      e_error = ePAL_RET_FAILURE;
   }
   return e_error;
}

PAL_RET_E pal_sock_create (
   PAL_SOCK_HDL *phl_sock_hdl,
   PAL_SOCK_DOMAIN_E e_domain,
   PAL_SOCK_TYPE_E e_type,
   PAL_SOCK_PROTOCOL_E e_protocol)
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;
   int i_domain = -1;
   int i_type = -1;
   int i_protocol = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;

   if (NULL == phl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *phl_sock_hdl = NULL;

   px_sock_ctxt = pal_malloc (sizeof(PAL_SOCK_CTXT_X), NULL);
   if(NULL == px_sock_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_sock_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   switch (e_domain)
   {
      case ePAL_SOCK_DOMAIN_AF_UNIX:
      {
         i_domain = AF_UNIX;
         break;
      }
      case ePAL_SOCK_DOMAIN_AF_INET:
      {
         i_domain = AF_INET;
         break;
      }
      case ePAL_SOCK_DOMAIN_AF_INET6:
      {
         break;
      }
      case ePAL_SOCK_DOMAIN_AF_PACKET:
      {
         break;
      }
      default:
      {
         break;
      }
   }
   if (-1 == i_domain)
   {
      PAL_LOG_HIGH("Invalid socket domain: %d", e_domain);
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   switch (e_type)
   {
      case ePAL_SOCK_TYPE_SOCK_STREAM:
      {
         i_type = SOCK_STREAM;
         break;
      }
      case ePAL_SOCK_TYPE_SOCK_DGRAM:
      {
         i_type = SOCK_DGRAM;
         break;
      }
      default:
      {
         break;
      }
   }

   if (-1 == i_type)
   {
      PAL_LOG_HIGH("Invalid socket type: %d", e_type);
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_protocol = 0;
   px_sock_ctxt->hl_sock_hdl = socket (i_domain, i_type, i_protocol);
   if (px_sock_ctxt->hl_sock_hdl < 0)
   {
      PAL_LOG_HIGH("socket failed: %d, Errno: %d",
         px_sock_ctxt->hl_sock_hdl, WSAGetLastError());
      pal_free (px_sock_ctxt);
      px_sock_ctxt = NULL;
      e_error = ePAL_RET_SOCK_CREATE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("socket success");
      *phl_sock_hdl = (PAL_SOCK_HDL) px_sock_ctxt;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   if (ePAL_RET_SUCCESS != e_error)
   {
      if (NULL != px_sock_ctxt)
      {
         pal_free (px_sock_ctxt);
         px_sock_ctxt = NULL;
      }
   }
   return e_error;
}

PAL_RET_E pal_sock_close (
   PAL_SOCK_HDL hl_sock_hdl)
{
   PAL_RET_E          e_error      = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X   *px_sock_ctxt = NULL;
   int                i_sock_ret   = -1;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   i_sock_ret = closesocket (px_sock_ctxt->hl_sock_hdl);
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("close failed: %d, Errno: %d", i_sock_ret, WSAGetLastError());
      e_error = ePAL_RET_SOCK_CLOSE_FAILED;
   }
   else
   {
      PAL_LOG_FULL("close success");
      e_error = ePAL_RET_SUCCESS;
   }
   px_sock_ctxt->hl_sock_hdl = -1;
   pal_free (px_sock_ctxt);
   px_sock_ctxt = NULL;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_bind (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_sock_addr_in)
{
   PAL_RET_E          e_error          = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X   *px_sock_ctxt     = NULL;
   int                i_sock_ret       = -1;
   struct sockaddr_in x_sock_addr_in   = {0};

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   x_sock_addr_in.sin_family = AF_INET;
   x_sock_addr_in.sin_port = px_sock_addr_in->us_sin_port_no;
   x_sock_addr_in.sin_addr.s_addr = px_sock_addr_in->x_sin_addr.ui_ip_addr_no;
   i_sock_ret = bind (px_sock_ctxt->hl_sock_hdl,
      (const struct sockaddr *) &x_sock_addr_in, sizeof(x_sock_addr_in));
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("bind failed: %d, Errno: %d", i_sock_ret, WSAGetLastError());
      e_error = ePAL_RET_SOCK_BIND_FAILED;
   }
   else
   {
      PAL_LOG_FULL("bind success");
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_listen (
   PAL_SOCK_HDL hl_sock_hdl,
   uint32_t ui_backlog)
{
   PAL_RET_E          e_error      = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X   *px_sock_ctxt = NULL;
   int                i_sock_ret   = -1;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   i_sock_ret = listen (px_sock_ctxt->hl_sock_hdl, (int) ui_backlog);
   if (0 != i_sock_ret)
   {
      PAL_LOG_HIGH("listen failed: %d, Errno: %d", i_sock_ret, WSAGetLastError());
      e_error = ePAL_RET_SOCK_LISTEN_FAILED;
   }
   else
   {
      PAL_LOG_FULL("listen success");
      e_error = ePAL_RET_SUCCESS;
   }

   e_error = ePAL_RET_SUCCESS;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_accept (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_src_sock_addr_in,
   PAL_SOCK_HDL *phl_accept_sock_hdl)
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;
   PAL_SOCK_CTXT_X *px_accept_sock_ctxt = NULL;
   struct sockaddr_in x_sock_addr_in   = {0};
   int i_sock_addr_len = -1;

   if ((NULL == hl_sock_hdl) || (NULL == px_src_sock_addr_in)
      || (NULL == phl_accept_sock_hdl))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }
   *phl_accept_sock_hdl = NULL;

   px_accept_sock_ctxt = pal_malloc (sizeof(PAL_SOCK_CTXT_X), NULL);
   if(NULL == px_accept_sock_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_sock_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   i_sock_addr_len = sizeof(x_sock_addr_in);
   px_accept_sock_ctxt->hl_sock_hdl = accept (px_sock_ctxt->hl_sock_hdl,
      (struct sockaddr *) &x_sock_addr_in, &i_sock_addr_len);
   if (px_accept_sock_ctxt->hl_sock_hdl < 0)
   {
      PAL_LOG_HIGH("accept failed: %d, Errno: %d",
         px_accept_sock_ctxt->hl_sock_hdl, WSAGetLastError());
      pal_free (px_accept_sock_ctxt);
      px_accept_sock_ctxt = NULL;
      e_error = ePAL_RET_SOCK_ACCEPT_FAILED;
   }
   else
   {
      PAL_LOG_FULL("accept success");
      *phl_accept_sock_hdl = (PAL_SOCK_HDL) px_accept_sock_ctxt;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_connect (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_dest_sock_addr_in,
   PAL_SOCK_CONN_MODE_E e_mode,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;
   PAL_RET_E    e_error_pvt     = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X   *px_sock_ctxt = NULL;
   int                i_sock_ret   = -1;
   struct sockaddr_in x_sock_addr_in   = {0};

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   if ((NULL == px_dest_sock_addr_in) || (e_mode <= ePAL_SOCK_CONN_MODE_INVALID)
      || (e_mode >= ePAL_SOCK_CONN_MODE_MAX)
      || ((ePAL_SOCK_CONN_MODE_NON_BLOCKING == e_mode) && (0 == ui_timeout_ms)))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if ((0 == px_dest_sock_addr_in->us_sin_port_no)
      || (0 == px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   if (ePAL_SOCK_CONN_MODE_NON_BLOCKING == e_mode)
   {
      PAL_LOG_HIGH("Connect to %d:%d in non-blocking mode",
         px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no,
         px_dest_sock_addr_in->us_sin_port_no);
      e_error = pal_setsockopt_enable_non_blocking (px_sock_ctxt);
      if (ePAL_RET_SUCCESS != e_error)
      {
         PAL_LOG_HIGH("pal_setsockopt_enable_non_blocking failed: %d", e_error);
         goto LBL_CLEANUP;
      }
   }
   else
   {
      PAL_LOG_HIGH("Connect to %d:%d in blocking mode",
         px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no,
         px_dest_sock_addr_in->us_sin_port_no);
   }

   x_sock_addr_in.sin_family = AF_INET;
   x_sock_addr_in.sin_port = px_dest_sock_addr_in->us_sin_port_no;
   x_sock_addr_in.sin_addr.s_addr =
      px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no;
   i_sock_ret = connect(px_sock_ctxt->hl_sock_hdl,
      (const struct sockaddr *) &x_sock_addr_in,
      sizeof(x_sock_addr_in));
   if (0 != i_sock_ret)
   {
      if (ePAL_SOCK_CONN_MODE_NON_BLOCKING == e_mode)
      {
         if (EINPROGRESS == errno)
         {
            e_error = pal_sock_connect_check_status (px_sock_ctxt,
               ui_timeout_ms);
            if (ePAL_RET_SUCCESS != e_error)
            {
               PAL_LOG_HIGH("%d:%d pal_sock_connect_check_status failed: %d",
                  px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no,
                  px_dest_sock_addr_in->us_sin_port_no, e_error);
            }
         }
         else
         {
            PAL_LOG_HIGH("connect to %d:%d failed: %d, Errno: %d",
               px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no,
               px_dest_sock_addr_in->us_sin_port_no, i_sock_ret, WSAGetLastError());
            e_error = ePAL_RET_SOCK_CONNECT_FAILED;
         }
      }
      else
      {
         PAL_LOG_HIGH("connect to %d:%d failed: %d, Errno: %d",
            px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no,
            px_dest_sock_addr_in->us_sin_port_no, i_sock_ret, WSAGetLastError());
         e_error = ePAL_RET_SOCK_CONNECT_FAILED;
      }
   }
   else
   {
      PAL_LOG_FULL("connect success");
      e_error = ePAL_RET_SUCCESS;
   }

   if (ePAL_SOCK_CONN_MODE_NON_BLOCKING == e_mode)
   {
      e_error_pvt = pal_setsockopt_enable_blocking (px_sock_ctxt);
      if (ePAL_RET_SUCCESS != e_error_pvt)
      {
         PAL_LOG_HIGH("pal_setsockopt_enable_blocking failed: %d", e_error_pvt);
      }
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_setsockopt (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_OPT_E e_sock_opt,
   void *p_opt_data,
   uint32_t ui_opt_data_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_SOCK_CTXT_X   *px_sock_ctxt = NULL;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   switch (e_sock_opt)
   {
      case ePAL_SOCK_OPT_SO_REUSEADDR:
      {
         e_error = pal_setsockopt_so_reuseaddress (px_sock_ctxt, p_opt_data,
            ui_opt_data_len);
         break;
      }
      case ePAL_SOCK_OPT_SO_BROADCAST:
      {
         e_error = pal_setsockopt_so_broadcast (px_sock_ctxt, p_opt_data,
            ui_opt_data_len);
         break;
      }
      case ePAL_SOCK_OPT_SO_KEEPALIVE:
      {
         e_error = pal_setsockopt_so_keepalive (px_sock_ctxt, p_opt_data,
            ui_opt_data_len);
         break;
      }
      case ePAL_SOCK_OPT_ENABLE_BLOCKING:
      {
         e_error = pal_setsockopt_enable_blocking (px_sock_ctxt);
         break;
      }
      case ePAL_SOCK_OPT_ENABLE_NON_BLOCKING:
      {
         e_error = pal_setsockopt_enable_non_blocking (px_sock_ctxt);
         break;
      }
      case ePAL_SOCK_OPT_TCP_NODELAY:
      {
         e_error = pal_setsockopt_tcp_nodelay (px_sock_ctxt, p_opt_data,
            ui_opt_data_len);
         break;
      }
      default:
      {
         e_error = ePAL_RET_INVALID_ARGS;
         break;
      }
   }

LBL_CLEANUP:
   return e_error;
}


uint32_t pal_htonl(
   uint32_t ui_hostlong_ho)
{
   return htonl (ui_hostlong_ho);
}

uint16_t pal_htons(
   uint16_t us_hostshort_ho)
{
   return htons (us_hostshort_ho);
}

uint32_t pal_ntohl(
   uint32_t ui_netlong_no)
{
   return ntohl (ui_netlong_no);
}

uint16_t pal_ntohs(
   uint16_t us_netshort_no)
{
   return ntohs (us_netshort_no);
}

PAL_RET_E pal_inet_aton(
   uint8_t *puc_ip_addr_str,
   PAL_SOCK_IN_ADDR_X *px_in_addr)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   px_in_addr->ui_ip_addr_no = inet_addr((const char *) puc_ip_addr_str);
   if (0 == px_in_addr->ui_ip_addr_no)
   {
      perror ("InetPton: ");
      PAL_LOG_HIGH("InetPton failed: %d, errno: %d", 
         px_in_addr->ui_ip_addr_no, WSAGetLastError ());
      e_error = ePAL_RET_FAILURE;
   }
   else
   {
      PAL_LOG_FULL("inet_aton success, IP (Net Order): 0x%x",
         px_in_addr->ui_ip_addr_no);
      e_error = ePAL_RET_SUCCESS;
   }
   return e_error;
}

PAL_RET_E pal_select (
   PAL_SOCK_SET_X *px_read_set,
   PAL_SOCK_SET_X *px_write_set,
   PAL_SOCK_SET_X *px_except_set,
   uint32_t ui_timeout_ms,
   uint32_t *pui_no_active_socks)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   uint32_t ui_start_time = 0;
   uint32_t ui_curr_time = 0;
   uint32_t ui_elapsed_time = 0;
   struct timeval x_timeout = {0};
   fd_set x_read_set = {{0}};
   fd_set x_write_set = {{0}};
   fd_set x_except_set = {{0}};
   int i_nfds = -1;
   int32_t i_ret_val = -1;

   pal_select_populate_fdset (px_read_set, &x_read_set, &i_nfds);
   pal_select_populate_fdset (px_write_set, &x_write_set, &i_nfds);
   pal_select_populate_fdset (px_except_set, &x_except_set, &i_nfds);

   ui_start_time = pal_get_system_time();
   do
   {
      x_timeout.tv_sec = (ui_timeout_ms / 1000);
      x_timeout.tv_usec = (ui_timeout_ms % 1000) * 1000;

      if (NULL != px_read_set)
      {
         PAL_LOG_FULL ("select on %d read sockets", px_read_set->ui_count);
      }

      if (NULL != px_write_set)
      {
         PAL_LOG_FULL ("select on %d read sockets", px_write_set->ui_count);
      }

      if (NULL != px_except_set)
      {
         PAL_LOG_FULL ("select on %d read sockets", px_except_set->ui_count);
      }

      i_ret_val = select (i_nfds, &x_read_set, &x_write_set,
         &x_except_set, &x_timeout);
      if (i_ret_val < 0)
      {
         if (EINTR == errno)
         {
            ui_curr_time = pal_get_system_time ();
            ui_elapsed_time = ui_curr_time - ui_start_time;
            if (ui_elapsed_time >= ui_timeout_ms)
            {
               PAL_LOG_HIGH("select failed: %d, Errno: %d...Timed Out",
                  i_ret_val, WSAGetLastError());
               e_error = ePAL_RET_OPERATION_TIMEDOUT;
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
            PAL_LOG_HIGH("select failed: %d, Errno: %d", i_ret_val, WSAGetLastError());
            e_error = ePAL_RET_SOCK_SELECT_FAILED;
            break;
         }
      }
      else
      {
         PAL_LOG_FULL("select success");
         e_error = ePAL_RET_SUCCESS;
         break;
      }
   } while (1);

   if (ePAL_RET_SUCCESS == e_error)
   {
      if (0 != i_ret_val)
      {
         PAL_LOG_FULL("select success: %d", i_ret_val);
      }

      pal_select_populate_set_fdset (px_read_set, &x_read_set);
      pal_select_populate_set_fdset (px_write_set, &x_write_set);
      pal_select_populate_set_fdset (px_except_set, &x_except_set);

      if (NULL != pui_no_active_socks)
      {
         *pui_no_active_socks = i_ret_val;

      }
   }
   return e_error;
}

bool pal_is_sock_set (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_SET_X *px_set)
{
   uint32_t ui_index = 0;
   bool b_is_set = false;

   if (NULL != px_set)
   {
      for (ui_index = 0; ui_index < px_set->ui_count; ui_index++)
      {
         if (hl_sock_hdl == px_set->xa_sock[ui_index].hl_sock_hdl)
         {
            break;
         }
      }
      if (ui_index < px_set->ui_count)
      {
         b_is_set = px_set->xa_sock[ui_index].b_is_set;
      }
   }
   else
   {
      PAL_LOG_HIGH("Invalid Args");
   }

   return b_is_set;
}

PAL_RET_E pal_gethostbyname (
   uint8_t *puc_hostname_str,
   PAL_SOCK_IN_ADDR_X *px_sock_in_addr)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   struct hostent *px_host_ent = NULL;

   if((puc_hostname_str == NULL) || (NULL == px_sock_in_addr))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_host_ent = gethostbyname ((const char *) puc_hostname_str);
   if (NULL == px_host_ent)
   {
      PAL_LOG_HIGH("gethostbyname failed: %p, Errno: %d", px_host_ent, WSAGetLastError());
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == px_host_ent->h_length)
   {
      PAL_LOG_HIGH("gethostbyname h_length: %d, Errno: %d",
         px_host_ent->h_length, WSAGetLastError());
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }


   px_sock_in_addr->ui_ip_addr_no = *((uint32_t *) px_host_ent->h_addr);
   e_error = ePAL_RET_SUCCESS;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_recv(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_sock_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   if ((NULL == puc_buf) || (NULL == pui_buf_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   errno = 0;
   i_sock_ret = recv (px_sock_ctxt->hl_sock_hdl, (char *) puc_buf, 
      *pui_buf_len, ui_flags);
   if (i_sock_ret < 0)
   {
      e_error = ePAL_RET_SOCK_RECV_FAILED;
   }
   else if (0 == i_sock_ret)
   {
      e_error = ePAL_RET_SOCK_CLOSED;
   }
   else
   {
      *pui_buf_len = i_sock_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_recv_fixed(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_sock_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;
   PAL_SOCK_SET_X x_rd_set = {0};
   uint32_t ui_no_active_socks = 0;
   uint32_t ui_start_time_ms = 0;
   uint32_t ui_elapsed_time_ms = 0;
   uint32_t ui_curr_time_ms = 0;
   uint32_t ui_rem_timeout_ms = 0;
   bool b_is_set = false;
   uint32_t ui_recv_bytes = 0;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   if ((NULL == puc_buf) || (NULL == pui_buf_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   ui_rem_timeout_ms = ui_timeout_ms;
   ui_start_time_ms = pal_get_system_time ();

   do
   {
      PAL_SOCK_SET_ZERO(&x_rd_set);
      PAL_SOCK_SET_SET(hl_sock_hdl, &x_rd_set);

      e_error = pal_select (&x_rd_set, NULL, NULL, ui_rem_timeout_ms,
         &ui_no_active_socks);
      if (ePAL_RET_SUCCESS != e_error)
      {
         PAL_LOG_HIGH ("pal_select failed: %d", e_error);
      }
      else
      {
         if (ui_no_active_socks > 0)
         {
            b_is_set = PAL_SOCK_SET_ISSET(hl_sock_hdl, &x_rd_set);
            if (true == b_is_set)
            {
               errno = 0;
               i_sock_ret = recv (px_sock_ctxt->hl_sock_hdl,
                  (char *)  (puc_buf + ui_recv_bytes), 
                  (*pui_buf_len - ui_recv_bytes), ui_flags);
               if (i_sock_ret < 0)
               {
                  PAL_LOG_LOW ("recv failed: %d", i_sock_ret);
                  e_error = ePAL_RET_SOCK_RECV_FAILED;
                  break;
               }
               else if (0 == i_sock_ret)
               {
                  e_error = ePAL_RET_SOCK_CLOSED;
                  break;
               }
               else
               {
                  ui_recv_bytes += i_sock_ret;
                  if ((uint32_t) i_sock_ret < (*pui_buf_len - ui_recv_bytes))
                  {
                     ui_curr_time_ms = pal_get_system_time();
                     ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
                     if (ui_elapsed_time_ms < ui_timeout_ms)
                     {
                        ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
                        continue;
                     }
                     else
                     {
                        *pui_buf_len = ui_recv_bytes;
                        e_error = ePAL_RET_OPERATION_TIMEDOUT;
                        break;
                     }
                  }
                  else
                  {
                     *pui_buf_len = ui_recv_bytes;
                     e_error = ePAL_RET_SUCCESS;
                     break;
                  }
               }
            }
            else
            {
               ui_curr_time_ms = pal_get_system_time();
               ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
               if (ui_elapsed_time_ms < ui_timeout_ms)
               {
                  ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
                  continue;
               }
               else
               {
                  *pui_buf_len = ui_recv_bytes;
                  e_error = ePAL_RET_OPERATION_TIMEDOUT;
                  break;
               }
            }
         }
         else
         {
            PAL_LOG_HIGH("Select success...but no active sockets.");
            ui_curr_time_ms = pal_get_system_time();
            ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
            if (ui_elapsed_time_ms < ui_timeout_ms)
            {
               ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
               continue;
            }
            else
            {
               *pui_buf_len = ui_recv_bytes;
               e_error = ePAL_RET_OPERATION_TIMEDOUT;
               break;
            }
         }
      }
   } while (1);
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_send (
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_sock_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   if ((NULL == puc_buf) || (NULL == pui_buf_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   errno = 0;
   i_sock_ret = send (px_sock_ctxt->hl_sock_hdl, (const char *) puc_buf, 
      *pui_buf_len, ui_flags);
   if (i_sock_ret < 0)
   {
      e_error = ePAL_RET_SOCK_SEND_FAILED;
   }
   else if (0 == i_sock_ret)
   {
      e_error = ePAL_RET_SOCK_CLOSED;
   }
   else
   {
      *pui_buf_len = i_sock_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sock_send_fixed (
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   uint32_t ui_timeout_ms)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_sock_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;
   PAL_SOCK_SET_X x_wr_set = {0};
   uint32_t ui_no_active_socks = 0;
   uint32_t ui_start_time_ms = 0;
   uint32_t ui_elapsed_time_ms = 0;
   uint32_t ui_curr_time_ms = 0;
   uint32_t ui_rem_timeout_ms = 0;
   bool b_is_set = false;
   uint32_t ui_sent_bytes = 0;

   if (NULL == hl_sock_hdl)
   {
      PAL_LOG_HIGH("Invalid Handle");
      e_error = ePAL_RET_INVALID_HANDLE;
      goto LBL_CLEANUP;
   }

   if ((NULL == puc_buf) || (NULL == pui_buf_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   ui_rem_timeout_ms = ui_timeout_ms;
   ui_start_time_ms = pal_get_system_time ();

   do
   {
      PAL_SOCK_SET_ZERO(&x_wr_set);
      PAL_SOCK_SET_SET(hl_sock_hdl, &x_wr_set);

      e_error = pal_select (NULL, &x_wr_set, NULL, ui_rem_timeout_ms,
         &ui_no_active_socks);
      if (ePAL_RET_SUCCESS != e_error)
      {
         PAL_LOG_HIGH ("pal_select failed: %d", e_error);
      }
      else
      {
         if (ui_no_active_socks > 0)
         {
            b_is_set = PAL_SOCK_SET_ISSET(hl_sock_hdl, &x_wr_set);
            if (true == b_is_set)
            {
               errno = 0;
               i_sock_ret = send (px_sock_ctxt->hl_sock_hdl,
                  (const char *) (puc_buf + ui_sent_bytes), 
                  (*pui_buf_len - ui_sent_bytes), ui_flags);
               if (i_sock_ret < 0)
               {
                  e_error = ePAL_RET_SOCK_SEND_FAILED;
                  break;
               }
               else if (0 == i_sock_ret)
               {
                  e_error = ePAL_RET_SOCK_CLOSED;
                  break;
               }
               else
               {
                  ui_sent_bytes += i_sock_ret;
                  if ((uint32_t) i_sock_ret < (*pui_buf_len - ui_sent_bytes))
                  {
                     ui_curr_time_ms = pal_get_system_time();
                     ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
                     if (ui_elapsed_time_ms < ui_timeout_ms)
                     {
                        ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
                        continue;
                     }
                     else
                     {
                        *pui_buf_len = ui_sent_bytes;
                        e_error = ePAL_RET_OPERATION_TIMEDOUT;
                        break;
                     }
                  }
                  else
                  {
                     *pui_buf_len = ui_sent_bytes;
                     e_error = ePAL_RET_SUCCESS;
                     break;
                  }
               }
            }
            else
            {
               ui_curr_time_ms = pal_get_system_time();
               ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
               if (ui_elapsed_time_ms < ui_timeout_ms)
               {
                  ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
                  continue;
               }
               else
               {
                  *pui_buf_len = ui_sent_bytes;
                  e_error = ePAL_RET_OPERATION_TIMEDOUT;
                  break;
               }
            }
         }
         else
         {
            PAL_LOG_HIGH("Select success...but no active sockets.");
            ui_curr_time_ms = pal_get_system_time();
            ui_elapsed_time_ms = ui_curr_time_ms - ui_start_time_ms;
            if (ui_elapsed_time_ms < ui_timeout_ms)
            {
               ui_rem_timeout_ms = ui_timeout_ms - ui_elapsed_time_ms;
               continue;
            }
            else
            {
               *pui_buf_len = ui_sent_bytes;
               e_error = ePAL_RET_OPERATION_TIMEDOUT;
               break;
            }
         }
      }
   } while (1);
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_gethostname(
   uint8_t *puc_hostname,
   uint32_t ui_hostname_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_ret_val = -1;

   if ((NULL == puc_hostname) || (0 == ui_hostname_len))
   {
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_ret_val = gethostname((char *) puc_hostname, (size_t) ui_hostname_len);
   if (i_ret_val < 0)
   {
      PAL_LOG_HIGH("gethostname failed: %d, Errno: %d", i_ret_val, WSAGetLastError());
      e_error = ePAL_RET_SOCK_ADDR_RESOLUTION_FAILED;
   }
   else
   {
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_sendto(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   PAL_SOCK_ADDR_IN_X *px_dest_sock_addr_in)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_sendto_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;
   uint32_t ui_buf_len = 0;
   struct sockaddr_in x_sock_addr_in   = {0};

   if ((NULL == hl_sock_hdl) || (NULL == puc_buf) || (NULL == pui_buf_len)
      || (NULL == px_dest_sock_addr_in))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args. Buffer length 0");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;

   x_sock_addr_in.sin_family = AF_INET;
   x_sock_addr_in.sin_port = px_dest_sock_addr_in->us_sin_port_no;
   x_sock_addr_in.sin_addr.s_addr =
      px_dest_sock_addr_in->x_sin_addr.ui_ip_addr_no;

   errno = 0;
   ui_buf_len = *pui_buf_len;
   i_sendto_ret = sendto (px_sock_ctxt->hl_sock_hdl, 
      (const char *) puc_buf, ui_buf_len, ui_flags, 
      (const struct sockaddr *) &x_sock_addr_in,
      sizeof(x_sock_addr_in));
   if (i_sendto_ret <= 0)
   {
      PAL_LOG_HIGH("sendto failed: %d, Errno: %d", i_sendto_ret, WSAGetLastError());
      *pui_buf_len = 0;
      e_error = ePAL_RET_SOCK_SEND_FAILED;
   }
   else
   {
      *pui_buf_len = i_sendto_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_recvfrom(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   PAL_SOCK_ADDR_IN_X *px_src_sock_addr_in)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_recvfrom_ret = -1;
   PAL_SOCK_CTXT_X *px_sock_ctxt = NULL;
   uint32_t ui_buf_len = 0;
   struct sockaddr_in x_sock_addr_in   = {0};
   int i_addr_len = -1;

   if ((NULL == hl_sock_hdl) || (NULL == puc_buf) || (NULL == pui_buf_len)
      || (NULL == px_src_sock_addr_in))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   if (0 == *pui_buf_len)
   {
      PAL_LOG_HIGH("Invalid Args. Buffer length 0");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_sock_ctxt = (PAL_SOCK_CTXT_X *) hl_sock_hdl;
   i_addr_len = sizeof(x_sock_addr_in);
   ui_buf_len = *pui_buf_len;
   i_recvfrom_ret = recvfrom (px_sock_ctxt->hl_sock_hdl, (char *) puc_buf, 
      ui_buf_len, ui_flags, (struct sockaddr *) &x_sock_addr_in, &i_addr_len);
   if ((i_recvfrom_ret <= 0) || (0 == i_addr_len))
   {
      PAL_LOG_HIGH("recvfrom failed: %d, Errno: %d", i_recvfrom_ret, WSAGetLastError());
      *pui_buf_len = 0;
      e_error = ePAL_RET_SOCK_RECV_FAILED;
   }
   else
   {
      px_src_sock_addr_in->us_sin_port_no = x_sock_addr_in.sin_port;
      px_src_sock_addr_in->x_sin_addr.ui_ip_addr_no =
         x_sock_addr_in.sin_addr.s_addr;
      *pui_buf_len = i_recvfrom_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_get_ip_addr_str (
   PAL_SOCK_IN_ADDR_X *px_addr_in,
   uint8_t *puc_ip_addr_str,
   uint32_t ui_ip_addr_str_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   char *pc_ip_addr_str = NULL;
   struct in_addr x_addr_in = {0};

   if ((NULL == px_addr_in) || (NULL == puc_ip_addr_str)
      || (ui_ip_addr_str_len < 16))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   x_addr_in.s_addr = px_addr_in->ui_ip_addr_no;
   pc_ip_addr_str = inet_ntoa (x_addr_in);
   if (NULL != pc_ip_addr_str)
   {
      (void) pal_strncpy(puc_ip_addr_str, (const uint8_t *) pc_ip_addr_str,
         ui_ip_addr_str_len);
      e_error = ePAL_RET_SUCCESS;
   }
   else
   {
      e_error = ePAL_RET_OUT_OF_MEMORY;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_getaddrinfo (
   uint8_t *puc_hostname_str,
   PAL_SOCK_ADDR_LIST_X *px_addr_list)
{
   PAL_RET_E e_error = ePAL_RET_NOT_IMPLEMENTED;
   return e_error;
}
