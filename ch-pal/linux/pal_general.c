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
 * \file   pal_general.c
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

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/****************************** STATIC GLOBALS ********************************/
static uint32_t   gui_pal_init_time_ms    = 0;

static uint64_t gull_pal_init_time_ns = 0;

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/
static PAL_RET_E pal_clock_env_init ();

static PAL_RET_E pal_clock_env_deinit ();

static PAL_RET_E pal_get_next_key_stroke (
   uint8_t *puc_byte);

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_env_init()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = pal_clock_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_clock_env_init failed: %d", e_error);
      goto LBL_CLEANUP;
   }

   e_error = pal_mutex_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_mutex_env_init failed: %d", e_error);
      goto LBL_CLEANUP;
   }
   e_error = pal_sem_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_sem_env_init failed: %d", e_error);
      goto LBL_CLEANUP;
   }
   e_error = pal_sock_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_sock_env_init failed: %d", e_error);
      goto LBL_CLEANUP;
   }
   e_error = pal_thread_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_thread_env_init failed: %d", e_error);
      goto LBL_CLEANUP;
   }
   e_error = pal_file_env_init ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_file_env_init failed: %d", e_error);
   }
LBL_CLEANUP:
   if (ePAL_RET_SUCCESS != e_error)
   {
      pal_env_deinit ();
   }
   return e_error;
}

PAL_RET_E pal_env_deinit ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   e_error = pal_file_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_file_env_deinit failed: %d", e_error);
   }
   e_error = pal_clock_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_clock_env_deinit failed: %d", e_error);
   }
   e_error = pal_thread_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_thread_env_deinit failed: %d", e_error);
   }
   e_error = pal_sock_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_sock_env_deinit failed: %d", e_error);
   }
   e_error = pal_sem_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_sem_env_deinit failed: %d", e_error);
   }
   e_error = pal_mutex_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_mutex_env_deinit failed: %d", e_error);
   }
   return e_error;
}

static PAL_RET_E pal_clock_env_init ()
{
   PAL_RET_E         e_error           = ePAL_RET_FAILURE;
   struct timespec   x_curr_time       = {0};
   int               i_ret_val   = -1;

   i_ret_val = clock_gettime (CLOCK_REALTIME, &x_curr_time);
   if (0 != i_ret_val)
   {
      PAL_LOG_HIGH("clock_gettime failed: %d, Errno: %d", i_ret_val, errno);
      e_error = ePAL_RET_CLOCK_FAILURE;
   }
   else
   {
      gui_pal_init_time_ms = ((x_curr_time.tv_sec * 1000)
         + (x_curr_time.tv_nsec / 1000000));
      gull_pal_init_time_ns = ((x_curr_time.tv_sec * 1000000000)
         + (x_curr_time.tv_nsec));
      PAL_LOG_FULL("clock_gettime success. Init Time: %u ms, %lu ns",
         gui_pal_init_time_ms, gull_pal_init_time_ns);
      e_error = ePAL_RET_SUCCESS;
   }
   return e_error;
}

static PAL_RET_E pal_clock_env_deinit ()
{
   PAL_RET_E    e_error     = ePAL_RET_FAILURE;

   gui_pal_init_time_ms = 0;
   gull_pal_init_time_ns = 0;
   e_error              = ePAL_RET_SUCCESS;

   return e_error;
}

void *pal_malloc_impl (
   uint32_t ui_size,
   PAL_MALLOC_PARAMS_X  *px_malloc_params)
{
   void *p_temp = NULL;
   if (0 != ui_size)
   {
      p_temp = malloc (ui_size);
      if (NULL != p_temp)
      {
         PAL_LOG_FULL("malloc success");
         (void) pal_memset(p_temp, 0x00, ui_size);
      }
      else
      {
         PAL_LOG_HIGH("malloc failed: %p, Errno: %d", p_temp, errno);
      }
   }
   return p_temp;
}

void *pal_malloc_impl_v2 (
   uint32_t ui_size,
   PAL_MALLOC_PARAMS_X  *px_malloc_params,
   uint8_t *puc_filename,
   uint8_t *puc_function,
   uint32_t ui_line_no)
{
   void *p_temp = NULL;
   if (0 != ui_size)
   {
      p_temp = malloc (ui_size);
      if (NULL != p_temp)
      {
         PAL_LOG_FULL("malloc success: %d bytes.From %s:%s:%d", ui_size,
            puc_filename, puc_function, ui_line_no);
         (void) pal_memset(p_temp, 0x00, ui_size);
      }
      else
      {
         PAL_LOG_HIGH("malloc failed: %p, Errno: %d", p_temp, errno);
      }
   }
   return p_temp;
}

void pal_free_impl (
   void *p_ptr)
{
   if (NULL != p_ptr)
   {
      free (p_ptr);
   }
}

void *pal_memset(
   void *p_ptr,
   uint32_t ui_value,
   uint32_t ui_ptr_len)
{
   void *p_temp = NULL;
   if ((NULL != p_ptr) && (0 != ui_ptr_len))
   {
      p_temp = memset(p_ptr, (int) ui_value, (size_t) ui_ptr_len);
   }
   return p_temp;
}

void *pal_memmove(
   void *p_dest,
   const void *p_src,
   uint32_t ui_dest_len)
{
   void *p_temp = NULL;
   if ((NULL != p_dest) && (NULL != p_src) && (0 != ui_dest_len))
   {
      p_temp = memmove(p_dest, p_src, (size_t) ui_dest_len);
   }
   return p_temp;
}

void pal_sleep(
   uint32_t ui_sleep_time_ms)
{
   int               i_ret_val   = -1;
   struct timespec   x_sleep_requested = { 0 };

   i_ret_val = clock_gettime (CLOCK_REALTIME, &x_sleep_requested);
   if (0 != i_ret_val)
   {
      PAL_LOG_HIGH("clock_gettime failed: %d, Errno: %d", i_ret_val, errno);
      goto LBL_CLEANUP;
   }
   PAL_LOG_FULL("clock_gettime success");

   /*
    * 1. Convert timeout in milliseconds to seconds.
    * 2. Take the remaining time and convert it into nano seconds.
    */
   x_sleep_requested.tv_sec   += ui_sleep_time_ms / 1000;
   ui_sleep_time_ms           %= 1000;
   x_sleep_requested.tv_nsec  += ui_sleep_time_ms * 1000000;

   /*
    * 3. Adjust the time. After the above operation of addition, the nanoseconds
    *    field might have a value greater than 1 second (1000000000)
    */
   x_sleep_requested.tv_sec   += x_sleep_requested.tv_nsec / 1000000000;
   x_sleep_requested.tv_nsec  %= 1000000000;

   do
   {
      i_ret_val = clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME,
         &x_sleep_requested, NULL);
      if (0 != i_ret_val)
      {
         if (EINTR == errno)
         {
            continue;
         }
         else
         {
            // Ignoring.
            PAL_LOG_HIGH("clock_nanosleep failed: %d, Errno: %d",
               i_ret_val, errno);
            break;
         }
      }
      else
      {
         PAL_LOG_FULL("clock_nanosleep success");
         break;
      }
   } while (1);
LBL_CLEANUP:
   return;
}

uint32_t pal_get_system_time ()
{
   struct timespec   x_curr_time          = { 0 };
   uint32_t          ui_curr_time_ms      = 0;
   uint32_t          ui_elapsed_time_ms   = 0;
   int               i_ret_val            = -1;

   i_ret_val = clock_gettime (CLOCK_REALTIME, &x_curr_time);
   if (0 == i_ret_val)
   {
      ui_curr_time_ms = ((x_curr_time.tv_sec * 1000)
         + (x_curr_time.tv_nsec / 1000000));
      ui_elapsed_time_ms = ui_curr_time_ms - gui_pal_init_time_ms;
   }
   return ui_elapsed_time_ms;
}

uint32_t pal_get_system_time_ms ()
{
   return pal_get_system_time ();
}

uint64_t pal_get_system_time_ns ()
{
   struct timespec   x_curr_time          = { 0 };
   uint64_t          ull_curr_time_ns     = 0;
   uint64_t          ull_elapsed_time_ns  = 0;
   int               i_ret_val            = -1;

   i_ret_val = clock_gettime (CLOCK_REALTIME, &x_curr_time);
   if (0 == i_ret_val)
   {
      ull_curr_time_ns = ((x_curr_time.tv_sec * 1000000000)
         + (x_curr_time.tv_nsec));
      ull_elapsed_time_ns = ull_curr_time_ns - gull_pal_init_time_ns;
   }
   return ull_elapsed_time_ns;
}

PAL_RET_E pal_atoi(
   uint8_t *puc_str,
   int32_t *pi_int_val)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int32_t i_val = -1;

   if ((NULL == puc_str) || (NULL == pi_int_val))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   errno = 0;
   i_val = strtol((const char *) puc_str, NULL, 10);
   if (((ERANGE == errno) && ((LONG_MAX == i_val) || (LONG_MIN == i_val)))
      || ((0 != errno) && (0 == i_val)))
   {
      PAL_LOG_HIGH("strtol failed: %d, Errno: %d", i_val, errno);
      e_error = ePAL_RET_INVALID_ARGS;
   }
   else
   {
      *pi_int_val = i_val;
      e_error = ePAL_RET_SUCCESS;
   }

LBL_CLEANUP:
   return e_error;
}

uint8_t *pal_strncpy(
   uint8_t *puc_dest,
   const uint8_t *puc_src,
   uint32_t ui_dest_len)
{
   uint8_t *puc_temp = NULL;
   if ((NULL == puc_dest) || (NULL == puc_src) || (0 == ui_dest_len))
   {
      goto LBL_CLEANUP;
   }
   puc_temp = (uint8_t *) strncpy ((char *) puc_dest, (const char *) puc_src,
      (size_t) ui_dest_len);
LBL_CLEANUP:
   return puc_temp;
}

int32_t pal_strncmp (
   const uint8_t *puc_str1,
   const uint8_t *puc_str2,
   uint32_t ui_max_str_len)
{
   int32_t i_ret = -1;

   if ((NULL == puc_str1) || (NULL == puc_str2) || (0 == ui_max_str_len))
   {
      goto LBL_CLEANUP;
   }
   i_ret = (int32_t) strncmp ((const char *) puc_str1, (const char *) puc_str2,
      (size_t) ui_max_str_len);
LBL_CLEANUP:
   return i_ret;
}

int32_t pal_strcmp (
   const uint8_t *puc_str1,
   const uint8_t *puc_str2)
{
   int32_t i_ret = -1;

   if ((NULL == puc_str1) || (NULL == puc_str2))
   {
      goto LBL_CLEANUP;
   }
   i_ret = (int32_t) strcmp ((const char *) puc_str1, (const char *) puc_str2);
LBL_CLEANUP:
   return i_ret;
}

uint32_t pal_strlen(
   const uint8_t *puc_str)
{
   uint32_t ui_temp = 0;

   if (NULL != puc_str)
   {
      ui_temp = (size_t) strlen ((const char *) puc_str);
   }
   return ui_temp;
}

uint32_t pal_strnlen(
   const uint8_t *puc_str,
   uint32_t ui_str_len)
{
   uint32_t ui_temp = 0;

   if ((NULL != puc_str) && (0 != ui_str_len))
   {
      ui_temp = (size_t) strnlen ((const char *) puc_str, (size_t) ui_str_len);
   }
   return ui_temp;
}

uint32_t pal_rand32 ()
{
   int i_rand_file_fd = -1;
   int i_rand_number = -1;
   int i_read_ret = -1;

   i_rand_file_fd = open ("/dev/urandom", (O_NONBLOCK | O_RDONLY));
   if (i_rand_file_fd < 0)
   {
      PAL_LOG_HIGH("open failed: %d, Errno: %d", i_rand_file_fd, errno);
      goto LBL_CLEANUP;
   }
   i_read_ret = read (i_rand_file_fd, &i_rand_number, sizeof(i_rand_number));
   if (i_read_ret < 0)
   {
      PAL_LOG_HIGH("read failed: %d, Errno: %d", i_read_ret, errno);
   }
   close(i_rand_file_fd);
LBL_CLEANUP:
   return (uint32_t) i_rand_number;
}

uint64_t pal_rand64 ()
{
   int i_rand_file_fd = -1;
   long long int lli_rand_number = -1;
   int i_read_ret = -1;

   i_rand_file_fd = open ("/dev/urandom", (O_NONBLOCK | O_RDONLY));
   if (i_rand_file_fd < 0)
   {
      PAL_LOG_HIGH("open failed: %d, Errno: %d", i_rand_file_fd, errno);
      goto LBL_CLEANUP;
   }

   i_read_ret = read (i_rand_file_fd, &lli_rand_number,
      sizeof(lli_rand_number));
   if (i_read_ret < 0)
   {
      PAL_LOG_HIGH("read failed: %d, Errno: %d", i_read_ret, errno);
   }
   close(i_rand_file_fd);
LBL_CLEANUP:
   return (uint64_t) lli_rand_number;
}

uint32_t pal_get_uint32_max ()
{
   return UINT32_MAX;
}

static PAL_RET_E pal_get_next_key_stroke (
   uint8_t *puc_byte)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int32_t i_nfds = -1;
   int32_t i_ret = -1;
   fd_set x_rd_fdset = {{0}};

   if (NULL == puc_byte)
   {
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   while (1)
   {
      FD_ZERO(&x_rd_fdset);
      FD_SET(0, &x_rd_fdset);
      i_nfds = 0 + 1;

      errno = 0;
      i_ret = select (i_nfds, &x_rd_fdset, NULL, NULL, NULL );
      if (i_ret > 0)
      {
         if (FD_ISSET(0, &x_rd_fdset))
         {
            i_ret = read (0, puc_byte, 1);
            if (1 == i_ret)
            {
               e_error = ePAL_RET_SUCCESS;
               break;
            }
            else
            {
               e_error = ePAL_RET_NO_DATA;
               break;
            }
         }
         else
         {
            e_error = ePAL_RET_NO_DATA;
            break;
         }
      }
      else
      {
         if (EINTR == errno)
         {
            continue;
         }
         else
         {
            e_error = ePAL_RET_NO_DATA;
            break;
         }
      }
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_get_int32_from_console (
   int32_t *pi_int_val)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   uint8_t uca_rd_buf[16] = {0};
   uint32_t ui_rd_buf_len = 0;
   int32_t i_isdigit_ret = -1;
   bool b_ignore_char = false;
   uint8_t uc_key_stroke = 0;

   if (NULL == pi_int_val)
   {
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   (void) memset (uca_rd_buf, 0x00, sizeof(uca_rd_buf));
   ui_rd_buf_len = 0;
   b_ignore_char = false;
   while (1)
   {
      if (ui_rd_buf_len >= sizeof(uca_rd_buf))
      {
         uca_rd_buf[sizeof(uca_rd_buf) - 1] = '\0';
         ui_rd_buf_len = sizeof(uca_rd_buf) - 1;
         b_ignore_char = true;
      }

      e_error = pal_get_next_key_stroke (&uc_key_stroke);
      if (ePAL_RET_SUCCESS != e_error)
      {
         continue;
      }

      if (true == b_ignore_char)
      {
         if ('\n' == uc_key_stroke)
         {
            break;
         }
         else
         {
            continue;
         }
      }
      else
      {
         i_isdigit_ret = isdigit(uc_key_stroke);
         if ('\n' == uc_key_stroke)
         {
            uca_rd_buf [ui_rd_buf_len] = '\0';
            ui_rd_buf_len++;
            break;
         }
         else if (0 == i_isdigit_ret)
         {
            if ((0 == ui_rd_buf_len) && ('-' == uc_key_stroke))
            {
               uca_rd_buf [ui_rd_buf_len] = uc_key_stroke;
               ui_rd_buf_len++;
            }
            else
            {
               continue;
            }
         }
         else
         {
            uca_rd_buf [ui_rd_buf_len] = uc_key_stroke;
            ui_rd_buf_len++;
         }
      }
   }

   if (0 == ui_rd_buf_len)
   {
      e_error = ePAL_RET_NO_DATA;
   }
   else if ('\0' == uca_rd_buf[0])
   {
      e_error = ePAL_RET_NO_DATA;
   }
   else
   {
      *pi_int_val = atoi ((const char *) uca_rd_buf);
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_get_string_from_console (
   uint8_t *puc_str_buf,
   uint32_t ui_str_buf_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   uint32_t ui_rd_buf_len = 0;
   bool b_ignore_char = false;
   uint8_t uc_key_stroke = 0;

   if ((NULL == puc_str_buf) || (ui_str_buf_len < 2))
   {
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   (void) memset (puc_str_buf, 0x00, ui_str_buf_len);
   ui_rd_buf_len = 0;
   b_ignore_char = false;
   while (1)
   {
      if (ui_rd_buf_len >= ui_str_buf_len)
      {
         puc_str_buf[ui_str_buf_len - 1] = '\0';
         ui_rd_buf_len = ui_str_buf_len - 1;
         b_ignore_char = true;
      }

      e_error = pal_get_next_key_stroke (&uc_key_stroke);
      if (ePAL_RET_SUCCESS != e_error)
      {
         continue;
      }

      if (true == b_ignore_char)
      {
         if ('\n' == uc_key_stroke)
         {
            break;
         }
         else
         {
            continue;
         }
      }
      else
      {
         if ('\n' == uc_key_stroke)
         {
            puc_str_buf [ui_rd_buf_len] = '\0';
            ui_rd_buf_len++;
            break;
         }
         else
         {
            puc_str_buf [ui_rd_buf_len] = uc_key_stroke;
            ui_rd_buf_len++;
         }
      }
   }

   if (0 == ui_rd_buf_len)
   {
      e_error = ePAL_RET_NO_DATA;
   }
   else if ('\0' == puc_str_buf[0])
   {
      e_error = ePAL_RET_NO_DATA;
   }
   else
   {
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_daemonize_process (
   PAL_DAMONIZE_PROCESS_PARAMS_X *px_params)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int32_t i_retval = -1;
   pid_t i_new_pid = -1;
   pid_t i_sessionid = -1;

   if (NULL == px_params)
   {
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_new_pid = fork ();
   if (i_new_pid < 0)
   {
      PAL_LOG_HIGH("fork failed: %d, Errno: %d", i_new_pid, errno);
      e_error = ePAL_RET_FORK_FAILURE;
      goto LBL_CLEANUP;
   }

   if (i_new_pid > 0)
   {
      exit (0);
   }

   (void) umask (0);

   i_sessionid = setsid ();
   if (i_sessionid < 0)
   {
      PAL_LOG_HIGH("setsid failed: %d, Errno: %d", i_sessionid, errno);
      e_error = ePAL_RET_SETSID_FAILURE;
      goto LBL_CLEANUP;
   }

   i_retval = chdir ("/");
   if (0 != i_retval)
   {
      PAL_LOG_HIGH("chdir failed: %d, Errno: %d", i_retval, errno);
      e_error = ePAL_RET_CHDIR_FAILURE;
      goto LBL_CLEANUP;
   }

   i_retval = close (STDIN_FILENO);
   if (0 != i_retval)
   {
      PAL_LOG_HIGH("close failed: %d, Errno: %d", i_retval, errno);
      e_error = ePAL_RET_CLOSE_FAILURE;
      goto LBL_CLEANUP;
   }

   i_retval = close (STDOUT_FILENO);
   if (0 != i_retval)
   {
      PAL_LOG_HIGH("close failed: %d, Errno: %d", i_retval, errno);
      e_error = ePAL_RET_CLOSE_FAILURE;
      goto LBL_CLEANUP;
   }

   i_retval = close (STDERR_FILENO);
   if (0 != i_retval)
   {
      PAL_LOG_HIGH("close failed: %d, Errno: %d", i_retval, errno);
      e_error = ePAL_RET_CLOSE_FAILURE;
   }
   else
   {
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}
