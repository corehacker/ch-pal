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
 * \date   17-Oct-2013
 *
 * \brief
 *
 ******************************************************************************/

/********************************** INCLUDES **********************************/
#include "ch-pal/exp_pal.h"

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
   e_error = pal_clock_env_deinit ();
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_clock_env_deinit failed: %d", e_error);
   }
   
   
   return e_error;
}

static PAL_RET_E pal_clock_env_init ()
{
   PAL_RET_E         e_error           = ePAL_RET_FAILURE;
   int               i_ret_val   = -1;

   i_ret_val = timeGetTime();
   if (i_ret_val <= 0)
   {
      PAL_LOG_HIGH("timeGetTime failed: %d, Errno: %d", i_ret_val, errno);
      e_error = ePAL_RET_CLOCK_FAILURE;
   }
   else
   {
      gui_pal_init_time_ms = i_ret_val;
      gull_pal_init_time_ns = i_ret_val * 1000; // TODO
      PAL_LOG_FULL("timeGetTime success. Init Time: %u ms, %lu ns",
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
   Sleep (ui_sleep_time_ms);
}

uint32_t pal_get_system_time ()
{
   uint32_t          ui_curr_time_ms      = 0;
   uint32_t          ui_elapsed_time_ms   = 0;
   int               i_ret_val            = -1;

   i_ret_val = timeGetTime();
   if (i_ret_val > 0)
   {
      ui_curr_time_ms = i_ret_val;
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
   uint64_t          ull_curr_time_ns     = 0;

   ull_curr_time_ns = pal_get_system_time_ms ();
   return ull_curr_time_ns * 1000;
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
   errno_t i_ret_val = -1;
   uint8_t *puc_temp = NULL;
   if ((NULL == puc_dest) || (NULL == puc_src) || (0 == ui_dest_len))
   {
      goto LBL_CLEANUP;
   }

   i_ret_val = strncpy_s((char *) puc_dest, ui_dest_len, 
      (const char *) puc_src, _TRUNCATE);
   if (0 != i_ret_val)
   {
      puc_temp = NULL;
   }
   else
   {
      puc_temp = puc_dest;
   }
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
   unsigned int i_rand_number = 0;
   errno_t i_rand_ret = -1;

   i_rand_ret = rand_s(&i_rand_number);
   return (uint32_t) i_rand_number;
}

uint64_t pal_rand64 ()
{
   return (uint64_t) pal_rand32 ();
}

uint32_t pal_get_uint32_max ()
{
   return UINT32_MAX;
}

static PAL_RET_E pal_get_next_key_stroke (
   uint8_t *puc_byte)
{
   return ePAL_RET_NOT_IMPLEMENTED;
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
