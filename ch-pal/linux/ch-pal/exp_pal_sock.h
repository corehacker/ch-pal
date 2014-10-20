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
 * \file   exp_pal_sock.h
 *
 * \author sandeepprakash
 *
 * \date   14-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_SOCK_H__
#define __EXP_PAL_SOCK_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define PAL_SOCK_IN_ADDR_ANY        (0x00000000)

/******************************** ENUMERATIONS ********************************/
typedef enum _PAL_SOCK_DOMAIN_E
{
   ePAL_SOCK_DOMAIN_INVALID,

   ePAL_SOCK_DOMAIN_AF_UNIX,

   ePAL_SOCK_DOMAIN_AF_INET,

   ePAL_SOCK_DOMAIN_AF_INET6,

   ePAL_SOCK_DOMAIN_AF_IPX,

   ePAL_SOCK_DOMAIN_AF_NETLINK,

   ePAL_SOCK_DOMAIN_AF_X25,

   ePAL_SOCK_DOMAIN_AF_AX25,

   ePAL_SOCK_DOMAIN_AF_ATMPVC,

   ePAL_SOCK_DOMAIN_AF_APPLETALK,

   ePAL_SOCK_DOMAIN_AF_PACKET,

   ePAL_SOCK_DOMAIN_MAX,
} PAL_SOCK_DOMAIN_E;

typedef enum _PAL_SOCK_TYPE_E
{
   ePAL_SOCK_TYPE_INVALID,

   ePAL_SOCK_TYPE_SOCK_STREAM,

   ePAL_SOCK_TYPE_SOCK_DGRAM,

   ePAL_SOCK_TYPE_SOCK_SEQPACKET,

   ePAL_SOCK_TYPE_SOCK_RAW,

   ePAL_SOCK_TYPE_SOCK_RDM,

   ePAL_SOCK_TYPE_SOCK_PACKET,

   ePAL_SOCK_TYPE_MAX,
} PAL_SOCK_TYPE_E;

typedef enum _PAL_SOCK_PROTOCOL_E
{
   ePAL_SOCK_PROTOCOL_INVALID,

   ePAL_SOCK_PROTOCOL_DEFAULT,

   ePAL_SOCK_PROTOCOL_MAX,
} PAL_SOCK_PROTOCOL_E;

typedef enum _PAL_SOCK_FAMILY_E
{
   ePAL_SOCK_FAMILY_INVALID,

   ePAL_SOCK_FAMILY_MAX,
} PAL_SOCK_FAMILY_E;

typedef enum _PAL_SOCK_CONN_MODE_E
{
   ePAL_SOCK_CONN_MODE_INVALID,

   ePAL_SOCK_CONN_MODE_BLOCKING,

   ePAL_SOCK_CONN_MODE_NON_BLOCKING,

   ePAL_SOCK_CONN_MODE_MAX
} PAL_SOCK_CONN_MODE_E;

typedef enum _PAL_SOCK_OPT_E
{
   ePAL_SOCK_OPT_INVALID                     = 0x00000000,

   /*
    * General Socket Options.
    */
   ePAL_SOCK_OPT_SO_REUSEADDR,
   /*!< PAL_SOCK_REUSEADDR_DATA_X */

   ePAL_SOCK_OPT_SO_BROADCAST,
   /*!< PAL_SOCK_BROADCAST_DATA_X */

   ePAL_SOCK_OPT_SO_KEEPALIVE,
   /*!< PAL_SOCK_KEEPALIVE_DATA_X */

   ePAL_SOCK_OPT_ENABLE_BLOCKING,

   ePAL_SOCK_OPT_ENABLE_NON_BLOCKING,

   /*
    * TCP Socket Options.
    */
   ePAL_SOCK_OPT_TCP_INVALID                 = 0x00001000,

   ePAL_SOCK_OPT_TCP_NODELAY,
   /*!< PAL_SOCK_NODELAY_DATA_X */

   /*
    * UDP Socket Options.
    */
   ePAL_SOCK_OPT_UDP_INVALID                 = 0x00002000,

   ePAL_SOCK_OPT_MAX
} PAL_SOCK_OPT_E;

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct PAL_SOCK_CTXT_X   *PAL_SOCK_HDL;

typedef struct _PAL_SOCK_IN_ADDR_X
{
   uint32_t ui_ip_addr_no;
} PAL_SOCK_IN_ADDR_X;

typedef struct _PAL_SOCK_ADDR_LIST_X
{
   uint32_t ui_count;

   PAL_SOCK_IN_ADDR_X **ppx_addr_list;
} PAL_SOCK_ADDR_LIST_X;

typedef struct _PAL_SOCK_ADDR_IN_X
{
   uint16_t us_sa_family;

   uint16_t us_sin_port_no;

   PAL_SOCK_IN_ADDR_X x_sin_addr;

   uint8_t uca_sin_zero[8];
} PAL_SOCK_ADDR_IN_X;

typedef struct _PAL_SOCK_REUSEADDR_DATA_X
{
   uint32_t ui_opt_val;
} PAL_SOCK_REUSEADDR_DATA_X;

typedef struct _PAL_SOCK_BROADCAST_DATA_X
{
   uint32_t ui_opt_val;
} PAL_SOCK_BROADCAST_DATA_X;

typedef struct _PAL_SOCK_KEEPALIVE_DATA_X
{
   uint32_t ui_opt_val;

   uint32_t ui_keepalive_time_ms;
} PAL_SOCK_KEEPALIVE_DATA_X;

typedef struct _PAL_SOCK_NODELAY_DATA_X
{
   uint32_t ui_opt_val;
} PAL_SOCK_NODELAY_DATA_X;

#define PAL_SOCK_SET_MAX   (16)

typedef struct _PAL_SOCK_SET_CTXT_X
{
   PAL_SOCK_HDL hl_sock_hdl;
   bool b_is_set;
} PAL_SOCK_SET_CTXT_X;

typedef struct _PAL_SOCK_SET_X
{
   uint32_t ui_count;
   PAL_SOCK_SET_CTXT_X xa_sock[PAL_SOCK_SET_MAX];
} PAL_SOCK_SET_X;

#define PAL_SOCK_SET_COUNT(sock_set) \
   (((PAL_SOCK_SET_X *)(sock_set))->ui_count)

#define PAL_SOCK_SET_SET(sock_hdl, sock_set) \
do \
{ \
   if (PAL_SOCK_SET_COUNT(sock_set) < PAL_SOCK_SET_MAX) \
   {\
      ((PAL_SOCK_SET_X *)(sock_set))->xa_sock[(PAL_SOCK_SET_COUNT(sock_set))++].hl_sock_hdl = \
            (sock_hdl);\
   }\
} while(0)

#define PAL_SOCK_SET_ZERO(sock_set) \
do \
{ \
   (void) pal_memset ((sock_set), 0x00, sizeof(*(sock_set))); \
   (PAL_SOCK_SET_COUNT(sock_set) = 0);\
} while (0)

#define PAL_SOCK_SET_ISSET(sock_hdl, sock_set) \
   pal_is_sock_set(sock_hdl, sock_set)

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_sock_env_init ();

PAL_RET_E pal_sock_env_deinit ();

PAL_RET_E pal_sock_create (
   PAL_SOCK_HDL *phl_sock_hdl,
   PAL_SOCK_DOMAIN_E e_domain,
   PAL_SOCK_TYPE_E e_type,
   PAL_SOCK_PROTOCOL_E e_protocol);

PAL_RET_E pal_sock_close (
   PAL_SOCK_HDL hl_sock_hdl);

PAL_RET_E pal_sock_bind (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_sock_addr_in);

PAL_RET_E pal_sock_listen (
   PAL_SOCK_HDL hl_sock_hdl,
   uint32_t ui_backlog);

PAL_RET_E pal_sock_accept (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_src_sock_addr_in,
   PAL_SOCK_HDL *phl_accept_sock_hdl);

PAL_RET_E pal_sock_connect (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_ADDR_IN_X *px_dest_sock_addr_in,
   PAL_SOCK_CONN_MODE_E e_mode,
   uint32_t ui_timeout_ms);

PAL_RET_E pal_setsockopt (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_OPT_E e_sock_opt,
   void *p_opt_data,
   uint32_t ui_opt_data_len);

uint32_t pal_htonl(
   uint32_t ui_hostlong_ho);

uint16_t pal_htons(
   uint16_t us_hostshort_ho);

uint32_t pal_ntohl(
   uint32_t ui_netlong_no);

uint16_t pal_ntohs(
   uint16_t us_netshort_no);

PAL_RET_E pal_inet_aton(
   uint8_t *puc_ip_addr_str,
   PAL_SOCK_IN_ADDR_X *px_in_addr);

PAL_RET_E pal_select (
   PAL_SOCK_SET_X *px_read_set,
   PAL_SOCK_SET_X *px_write_set,
   PAL_SOCK_SET_X *px_except_set,
   uint32_t ui_timeout_ms,
   uint32_t *pui_no_active_socks);

bool pal_is_sock_set (
   PAL_SOCK_HDL hl_sock_hdl,
   PAL_SOCK_SET_X *px_set);

PAL_RET_E pal_gethostbyname (
   uint8_t *puc_hostname_str,
   PAL_SOCK_IN_ADDR_X *px_sock_in_addr);

PAL_RET_E pal_sock_recv(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags);

PAL_RET_E pal_sock_recv_fixed(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   uint32_t ui_timeout_ms);

PAL_RET_E pal_sock_send (
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags);

PAL_RET_E pal_sock_send_fixed (
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   uint32_t ui_timeout_ms);

PAL_RET_E pal_gethostname(
   uint8_t *puc_hostname,
   uint32_t ui_hostname_len);

PAL_RET_E pal_sendto(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   PAL_SOCK_ADDR_IN_X *px_dest_sock_addr_in);

PAL_RET_E pal_recvfrom(
   PAL_SOCK_HDL hl_sock_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len,
   uint32_t ui_flags,
   PAL_SOCK_ADDR_IN_X *px_src_sock_addr_in);

PAL_RET_E pal_get_ip_addr_str (
   PAL_SOCK_IN_ADDR_X *px_addr_in,
   uint8_t *puc_ip_addr_str,
   uint32_t ui_ip_addr_str_len);

PAL_RET_E pal_getaddrinfo (
   uint8_t *puc_hostname_str,
   PAL_SOCK_ADDR_LIST_X *px_addr_list);

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_SOCK_H__ */
