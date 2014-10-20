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
 * \file   pal_sock_private.h
 *
 * \author sandeepprakash
 *
 * \date   16-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __PAL_SOCK_PRIVATE_H__
#define __PAL_SOCK_PRIVATE_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef int LINUX_SOCK_HDL;

typedef struct _PAL_SOCK_CTXT_X
{
   LINUX_SOCK_HDL hl_sock_hdl;
} PAL_SOCK_CTXT_X;

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_setsockopt_so_reuseaddress (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len);

PAL_RET_E pal_setsockopt_so_broadcast (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len);

PAL_RET_E pal_setsockopt_so_keepalive (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len);

PAL_RET_E pal_setsockopt_tcp_nodelay (
   PAL_SOCK_CTXT_X *px_sock_ctxt,
   void *p_data,
   uint32_t ui_data_len);

PAL_RET_E pal_setsockopt_enable_blocking (
   PAL_SOCK_CTXT_X *px_sock_ctxt);

PAL_RET_E pal_setsockopt_enable_non_blocking (
   PAL_SOCK_CTXT_X *px_sock_ctxt);

void pal_select_populate_fdset (
   PAL_SOCK_SET_X *px_set,
   fd_set *px_fdset,
   int32_t *pi_nfds);

void pal_select_populate_set_fdset (
   PAL_SOCK_SET_X *px_set,
   fd_set *px_fdset);

PAL_RET_E pal_sock_connect_check_status (
   PAL_SOCK_CTXT_X   *px_sock_ctxt,
   uint32_t ui_timeout_ms);

#ifdef   __cplusplus
}
#endif

#endif /* __PAL_SOCK_PRIVATE_H__ */
