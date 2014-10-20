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
 * \file   exp_pal_general.h
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_GENERAL_H__
#define __EXP_PAL_GENERAL_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define pal_malloc(ui_size,px_malloc_params)                                   \
   pal_malloc_impl_v2(ui_size,px_malloc_params,(uint8_t *)__FILE__,            \
      (uint8_t *)__FUNCTION__,__LINE__)

#define pal_free                          pal_free_impl

#define PAL_UINT32_MAX_VALUE              (pal_get_uint32_max())

#define PAL_TIME_WAIT_INFINITE            (0xffffffff)

#define PAL_LOG_STR                       "PAL"

#define PAL_LOG_LOW(format,...)                                               \
do                                                                            \
{                                                                             \
   LOG_LOW (PAL_LOG_STR,__FILE__,__FUNCTION__,__LINE__,format,                \
      ##__VA_ARGS__);                                                         \
} while (0)

#define PAL_LOG_MED(format,...)                                               \
do                                                                            \
{                                                                             \
   LOG_MED (PAL_LOG_STR,__FILE__,__FUNCTION__,__LINE__,format,                \
      ##__VA_ARGS__);                                                         \
} while (0)

#define PAL_LOG_HIGH(format,...)                                              \
do                                                                            \
{                                                                             \
   LOG_HIGH (PAL_LOG_STR,__FILE__,__FUNCTION__,__LINE__,format,               \
      ##__VA_ARGS__);                                                         \
} while (0)

#define PAL_LOG_FULL(format,...)                                              \
do                                                                            \
{                                                                             \
   LOG_FULL (PAL_LOG_STR,__FILE__,__FUNCTION__,__LINE__,format,               \
      ##__VA_ARGS__);                                                         \
} while (0)

#define PAL_MAX(__A__,__B__)   \
   ((__A__ > __B__) ? (__A__) : (__B__))

#define PAL_GET_INT32_FROM_CONSOLE(pi_int32_var)                              \
   do                                                                         \
   {                                                                          \
      PAL_RET_E e_error = ePAL_RET_FAILURE;                                   \
      do                                                                      \
      {                                                                       \
         e_error = pal_get_int32_from_console (pi_int32_var);                 \
      } while (ePAL_RET_SUCCESS != e_error);                                  \
   } while (0)


#define PAL_GET_STRING_FROM_CONSOLE(puc_str_buf,ui_buf_len)                   \
   do                                                                         \
   {                                                                          \
      PAL_RET_E e_error = ePAL_RET_FAILURE;                                   \
      do                                                                      \
      {                                                                       \
         e_error = pal_get_string_from_console (puc_str_buf,ui_buf_len);      \
      } while (ePAL_RET_SUCCESS != e_error);                                  \
   } while (0)

#define PAL_STR(str) ((uint8_t *) str)

#define SYS_STR(str) ((char *) str)

/******************************** ENUMERATIONS ********************************/
typedef enum _PAL_RET_E
{
   ePAL_RET_SUCCESS                                             = 0x00000000,

   ePAL_RET_FAILURE                                             = 0x00000001,

   ePAL_RET_GENERAL_BASE                                        = 0x00001000,
   /*!< Base error code for general errors. */

   /*
    * General error codes.
    */
   ePAL_RET_INVALID_ARGS                                        = 0x00001001,

   ePAL_RET_INVALID_HANDLE                                      = 0x00001002,

   ePAL_RET_NOT_INITIALIZED                                     = 0x00001003,

   ePAL_RET_OUT_OF_MEMORY                                       = 0x00001004,

   ePAL_RET_CLOCK_FAILURE                                       = 0x00001005,

   ePAL_RET_OPERATION_TIMEDOUT                                  = 0x00001006,

   ePAL_RET_NO_DATA                                             = 0x00001007,

   ePAL_RET_NOT_IMPLEMENTED                                     = 0x00001008,

   ePAL_RET_FORK_FAILURE                                        = 0x00001009,

   ePAL_RET_OPEN_FAILURE                                        = 0x0000100A,

   ePAL_RET_CLOSE_FAILURE                                       = 0x0000100B,

   ePAL_RET_READ_FAILURE                                        = 0x0000100C,

   ePAL_RET_WRITE_FAILURE                                       = 0x0000100D,

   ePAL_RET_SETSID_FAILURE                                      = 0x0000100E,

   ePAL_RET_CHDIR_FAILURE                                       = 0x0000100F,


   /*
    * Base error codes for specific operations.
    */
   ePAL_RET_MUTEX_BASE                                          = 0x00002000,
   /*!< Base error code for mutex operations. */

   ePAL_RET_MUTEX_CREATE_FAILED                 = (ePAL_RET_MUTEX_BASE + 1),

   ePAL_RET_MUTEX_DELETE_FAILED                 = (ePAL_RET_MUTEX_BASE + 2),

   ePAL_RET_MUTEX_LOCK_FAILED                   = (ePAL_RET_MUTEX_BASE + 3),

   ePAL_RET_MUTEX_UNLOCK_FAILED                 = (ePAL_RET_MUTEX_BASE + 4),

   ePAL_RET_SEM_BASE                                            = 0x00003000,
   /*!< Base error code for semaphore operations. */

   ePAL_RET_SEM_CREATE_FAILED                 = (ePAL_RET_SEM_BASE + 1),

   ePAL_RET_SEM_DELETE_FAILED                 = (ePAL_RET_SEM_BASE + 2),

   ePAL_RET_SEM_GET_FAILED                    = (ePAL_RET_SEM_BASE + 3),

   ePAL_RET_SEM_PUT_FAILED                    = (ePAL_RET_SEM_BASE + 4),

   ePAL_RET_SEM_EXCEEDED_LIMIT                = (ePAL_RET_SEM_BASE + 5),

   ePAL_RET_SEM_IN_USE                        = (ePAL_RET_SEM_BASE + 6),

   ePAL_RET_THREAD_BASE                                         = 0x00004000,
   /*!< Base error code for thread operations. */

   ePAL_RET_THREAD_CREATE_FAILED              = (ePAL_RET_THREAD_BASE + 1),

   ePAL_RET_THREAD_DELETE_FAILED              = (ePAL_RET_THREAD_BASE + 2),

   ePAL_RET_THREAD_JOIN_FAILED                = (ePAL_RET_THREAD_BASE + 3),

   ePAL_RET_FILE_BASE                                           = 0x00005000,

   ePAL_RET_FILE_STAT_FAILED                  = (ePAL_RET_FILE_BASE + 1),

   ePAL_RET_FILE_OPEN_FAILED                  = (ePAL_RET_FILE_BASE + 2),

   ePAL_RET_FILE_CLOSE_FAILED                 = (ePAL_RET_FILE_BASE + 3),

   ePAL_RET_FILE_READ_FAILED                  = (ePAL_RET_FILE_BASE + 4),

   ePAL_RET_FILE_WRITE_FAILED                 = (ePAL_RET_FILE_BASE + 5),

   ePAL_RET_FILE_EOF_REACHED                  = (ePAL_RET_FILE_BASE + 6),

   ePAL_RET_FILE_READ_BUF_OVERFLOW            = (ePAL_RET_FILE_BASE + 7),

   ePAL_RET_FILE_SEEK_FAILED                  = (ePAL_RET_FILE_BASE + 8),

   ePAL_RET_FILE_TELL_FAILED                  = (ePAL_RET_FILE_BASE + 9),

   ePAL_RET_SOCK_BASE                                           = 0x00010000,
   /*!< Base error code for socket operations. */

   ePAL_RET_SOCK_CREATE_FAILED                = (ePAL_RET_SOCK_BASE + 1),

   ePAL_RET_SOCK_CLOSE_FAILED                 = (ePAL_RET_SOCK_BASE + 2),

   ePAL_RET_SOCK_BIND_FAILED                  = (ePAL_RET_SOCK_BASE + 3),

   ePAL_RET_SOCK_LISTEN_FAILED                = (ePAL_RET_SOCK_BASE + 4),

   ePAL_RET_SOCK_ACCEPT_FAILED                = (ePAL_RET_SOCK_BASE + 5),

   ePAL_RET_SOCK_CONNECT_FAILED               = (ePAL_RET_SOCK_BASE + 6),

   ePAL_RET_SOCK_OPTION_FAILED                = (ePAL_RET_SOCK_BASE + 7),

   ePAL_RET_SOCK_SELECT_FAILED                = (ePAL_RET_SOCK_BASE + 8),

   ePAL_RET_SOCK_RECV_FAILED                  = (ePAL_RET_SOCK_BASE + 9),

   ePAL_RET_SOCK_SEND_FAILED                  = (ePAL_RET_SOCK_BASE + 10),

   ePAL_RET_SOCK_CLOSED                       = (ePAL_RET_SOCK_BASE + 11),

   ePAL_RET_SOCK_ADDR_RESOLUTION_FAILED       = (ePAL_RET_SOCK_BASE + 12),

   /*!< Base error code for socket operations. */
} PAL_RET_E;

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct _PAL_MALLOC_PARAMS_X
{
   uint32_t ui_reserved;
} PAL_MALLOC_PARAMS_X;

typedef struct _PAL_DAMONIZE_PROCESS_PARAMS_X
{
   uint32_t ui_placeholder;
} PAL_DAMONIZE_PROCESS_PARAMS_X;

/***************************** FUNCTION PROTOTYPES ****************************/

PAL_RET_E pal_env_init();

PAL_RET_E pal_env_deinit ();

void *pal_malloc_impl (
   uint32_t ui_size,
   PAL_MALLOC_PARAMS_X  *px_malloc_params);

void *pal_malloc_impl_v2 (
   uint32_t ui_size,
   PAL_MALLOC_PARAMS_X  *px_malloc_params,
   uint8_t *puc_filename,
   uint8_t *puc_function,
   uint32_t ui_line_no);

void pal_free_impl (
   void *p_ptr);

void *pal_memset(
   void *p_ptr,
   uint32_t ui_value,
   uint32_t ui_ptr_len);

void *pal_memmove (
   void *p_dest,
   const void *p_src,
   uint32_t ui_dest_len);

void pal_sleep (
   uint32_t ui_sleep_time_ms);

uint32_t pal_get_system_time ();

uint32_t pal_get_system_time_ms ();

uint64_t pal_get_system_time_ns ();

PAL_RET_E pal_atoi (
   uint8_t *puc_str,
   int32_t *pi_int_val);

uint8_t *pal_strncpy (
   uint8_t *puc_dest,
   const uint8_t *puc_src,
   uint32_t ui_dest_len);

int32_t pal_strncmp (
   const uint8_t *puc_str1,
   const uint8_t *puc_str2,
   uint32_t ui_max_str_len);

int32_t pal_strcmp (
   const uint8_t *puc_str1,
   const uint8_t *puc_str2);

uint32_t pal_strlen(
   const uint8_t *puc_str);

uint32_t pal_strnlen (
   const uint8_t *puc_str,
   uint32_t ui_str_len);

uint32_t pal_rand32 ();

uint64_t pal_rand64 ();

uint32_t pal_get_uint32_max ();

PAL_RET_E pal_get_int32_from_console (
   int32_t *pi_int_val);

PAL_RET_E pal_get_string_from_console (
   uint8_t *puc_str_buf,
   uint32_t ui_str_buf_len);

PAL_RET_E pal_daemonize_process (
   PAL_DAMONIZE_PROCESS_PARAMS_X *px_params);

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_GENERAL_H__ */
