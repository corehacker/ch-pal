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
 * \file   logger.c
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
#define PAL_LOGGER_FILENAME_LEN           (512)

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/
static FILE *gpfp_logger_file_fp = NULL;
static bool gb_enable_file_logging = false;
static bool gb_enable_console_logging = false;
static LOG_LEVEL_E ge_level = eLOG_LEVEL_LOW;

/****************************** LOCAL FUNCTIONS *******************************/
void pal_logger_env_init(
   PAL_LOGGER_INIT_PARAMS_X *px_init_params)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   char ca_filename[PAL_LOGGER_FILENAME_LEN] = {0};
   errno_t i_fopen_ret = -1;
   if (NULL != px_init_params)
   {
      if (true == px_init_params->b_enable_file_logging)
      {
         // TODO: Dangerous
         (void) sprintf_s(ca_filename, sizeof(ca_filename), "%s%02d.txt",
            px_init_params->uca_filename_prefix,
            px_init_params->ui_file_name_suffix);
         printf ("Opening File: %s for logging\n", ca_filename);

         i_fopen_ret = fopen_s (&gpfp_logger_file_fp,
            (const uint8_t *)ca_filename, (const uint8_t *)"w+");
         if ((0 == i_fopen_ret) || (NULL != gpfp_logger_file_fp))
         {
            gb_enable_file_logging = true;
         }
      }
      if (true == px_init_params->b_enable_console_logging)
      {
         gb_enable_console_logging = true;
      }
      ge_level = px_init_params->e_level;
   }
   return;
}

void pal_logger_env_deinit ()
{
   if (true == gb_enable_file_logging && NULL != gpfp_logger_file_fp)
   {
      (void) fclose(gpfp_logger_file_fp);
      gpfp_logger_file_fp = NULL;
   }
   return;
}

void logger (
   const char *puc_modulename_str,
   const char *puc_filename_str,
   const char *puc_function_str,
   const unsigned int ui_line,
   const LOG_LEVEL_E e_level,
   const char *puc_format_str,
   ...)
{
   char    ca_actual_log[LOGGER_MAX_LOG_STR_LEN] = {0};
   va_list x_va_list;

   if ((NULL != puc_format_str)
      && ((true == gb_enable_file_logging)
         || (true == gb_enable_console_logging)))
   {
      va_start (x_va_list, puc_format_str);
      (void) vsnprintf_s (ca_actual_log, sizeof(ca_actual_log), 
         _TRUNCATE, puc_format_str, x_va_list);
      va_end (x_va_list);

      if (true == gb_enable_file_logging && NULL != gpfp_logger_file_fp)
      {
         if (e_level <= ge_level)
         {
            fprintf (gpfp_logger_file_fp, "%10d:%s:%4s:%32s:%48s:%4d: %s\n",
               pal_get_system_time (),
               ((eLOG_LEVEL_LOW == e_level) ?
                     ("L") :
                     ((eLOG_LEVEL_MEDIUM == e_level) ?
                           ("M") :
                           ((eLOG_LEVEL_HIGH == e_level) ?
                                 ("H") :
                                 ((eLOG_LEVEL_FULL == e_level) ? ("F") : (" "))))),
               ((NULL != puc_modulename_str) ? (puc_modulename_str) : ("NA")),
               ((NULL != puc_filename_str) ? (puc_filename_str) : ("NA")),
               ((NULL != puc_function_str) ? (puc_function_str) : ("NA")),
               ui_line, ca_actual_log);
            fflush (gpfp_logger_file_fp);
         }
      }

      if (true == gb_enable_console_logging)
      {
         if (e_level <= ge_level)
#if 1
            printf ("%10d:%s:%4s:%40s:%4d: %s\n", pal_get_system_time (),
               ((eLOG_LEVEL_LOW == e_level) ?
                     ("L") :
                     ((eLOG_LEVEL_MEDIUM == e_level) ?
                           ("M") :
                           ((eLOG_LEVEL_HIGH == e_level) ?
                                 ("H") :
                                 ((eLOG_LEVEL_FULL == e_level) ? ("F") : (" "))))),
               ((NULL != puc_modulename_str) ? (puc_modulename_str) : ("NA")),
               ((NULL != puc_function_str) ? (puc_function_str) : ("NA")),
               ui_line, ca_actual_log);
#else
         printf ("%4s:%40s:%4d: %s\n",
               ((NULL != puc_modulename_str) ? (puc_modulename_str) : ("NA")),
               ((NULL != puc_function_str) ? (puc_function_str) : ("NA")), ui_line,
               ca_actual_log);
#endif
      }
   }
}
