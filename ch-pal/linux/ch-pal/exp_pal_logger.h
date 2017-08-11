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
 * \file   exp_logger.h
 *
 * \author sandeepprakash
 *
 * \date   23-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_LOGGER_H__
#define __EXP_LOGGER_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define LOGGER_MAX_LOG_STR_LEN                  (512)

#define LOGGER_FILENAME_PREFIX_LEN              (256)

#ifndef WIN32
#define LOG_LOW(modulename,filename,function,line,format,...)                 \
      LOG_LOW_V2(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_MED(modulename,filename,function,line,format,...)                 \
      LOG_MED_V2(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_HIGH(modulename,filename,function,line,format,...)                \
      LOG_HIGH_V2(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_FULL(modulename,filename,function,line,format,...)                \
      LOG_FULL_V2(modulename,filename,function,line,format,##__VA_ARGS__)
#else
#define LOG_LOW(modulename,filename,function,line,format,...)                 \
      LOG_LOW_V1(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_MED(modulename,filename,function,line,format,...)                 \
      LOG_MED_V1(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_HIGH(modulename,filename,function,line,format,...)                \
      LOG_HIGH_V1(modulename,filename,function,line,format,##__VA_ARGS__)

#define LOG_FULL(modulename,filename,function,line,format,...)                \
      LOG_FULL_V1(modulename,filename,function,line,format,##__VA_ARGS__)
#endif

#define LOG_LOW_V1(modulename,filename,function,line,format,...)              \
do                                                                            \
{                                                                             \
   logger (modulename,filename,function,line,eLOG_LEVEL_LOW,format,           \
      ##__VA_ARGS__);                                                         \
} while (0)

#define LOG_MED_V1(modulename,filename,function,line,format,...)              \
do                                                                            \
{                                                                             \
   logger (modulename,filename,function,line,eLOG_LEVEL_MEDIUM,format,        \
      ##__VA_ARGS__);                                                         \
} while (0)

#define LOG_HIGH_V1(modulename,filename,function,line,format,...)             \
do                                                                            \
{                                                                             \
   logger (modulename,filename,function,line,eLOG_LEVEL_HIGH,format,          \
      ##__VA_ARGS__);                                                         \
} while (0)

#define LOG_FULL_V1(modulename,filename,function,line,format,...)             \
do                                                                            \
{                                                                             \
   logger (modulename,filename,function,line,eLOG_LEVEL_FULL,format,          \
      ##__VA_ARGS__);                                                         \
} while (0)

#ifndef WIN32
#define LOG_LOW_V2(modulename,filename,function,line,format,...)              \
do                                                                            \
{                                                                             \
   logger_v2 (modulename,pthread_self(),filename,function,line,               \
      eLOG_LEVEL_LOW,format, ##__VA_ARGS__);                                  \
} while (0)

#define LOG_MED_V2(modulename,filename,function,line,format,...)              \
do                                                                            \
{                                                                             \
   logger_v2 (modulename,pthread_self(),filename,function,line,               \
      eLOG_LEVEL_MEDIUM,format, ##__VA_ARGS__);                               \
} while (0)

#define LOG_HIGH_V2(modulename,filename,function,line,format,...)             \
do                                                                            \
{                                                                             \
   logger_v2 (modulename,pthread_self(),filename,function,line,               \
      eLOG_LEVEL_HIGH,format, ##__VA_ARGS__);                                 \
} while (0)

#define LOG_FULL_V2(modulename,filename,function,line,format,...)             \
do                                                                            \
{                                                                             \
   logger_v2 (modulename,pthread_self(),filename,function,line,               \
      eLOG_LEVEL_FULL,format, ##__VA_ARGS__);                                 \
} while (0)
#endif

/******************************** ENUMERATIONS ********************************/
typedef enum _LOG_LEVEL_E
{
   eLOG_LEVEL_INVALID            = 0x00000000,

   eLOG_LEVEL_LOW,

   eLOG_LEVEL_MEDIUM,

   eLOG_LEVEL_HIGH,

   eLOG_LEVEL_FULL,

   eLOG_LEVEL_MAX,
} LOG_LEVEL_E;

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct _PAL_LOGGER_INIT_PARAMS_X
{
   bool b_enable_file_logging;

   bool b_enable_console_logging;

   LOG_LEVEL_E e_level;

   uint32_t ui_file_name_suffix;

   uint8_t uca_filename_prefix[LOGGER_FILENAME_PREFIX_LEN];
} PAL_LOGGER_INIT_PARAMS_X;

/***************************** FUNCTION PROTOTYPES ****************************/
void pal_logger_env_init(
   PAL_LOGGER_INIT_PARAMS_X *px_init_params);

void pal_logger_env_deinit ();

void logger (
   const char *puc_modulename_str,
   const char *puc_filename_str,
   const char *puc_function_str,
   const unsigned int ui_line,
   const LOG_LEVEL_E e_level,
   const char *puc_format_str,
   ...);

#ifndef WIN32
void logger_v2 (
   const char *puc_modulename_str,
   const pthread_t ui_thread_id,
   const char *puc_filename_str,
   const char *puc_function_str,
   const unsigned int ui_line,
   const LOG_LEVEL_E e_level,
   const char *puc_format_str,
   ...);
#endif

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_LOGGER_H__ */
