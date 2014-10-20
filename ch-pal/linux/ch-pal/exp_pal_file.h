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
 * \file   exp_pal_file.h
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_FILE_H__
#define __EXP_PAL_FILE_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/*********************************** HANDLES **********************************/
typedef struct PAL_FILE_CXT_X         *PAL_FILE_HDL;

/******************************** ENUMERATIONS ********************************/
typedef enum _PAL_FILE_WHENCE_E
{
   ePAL_FILE_WHENCE_INVALID         = 0x00000000,

   ePAL_FILE_WHENCE_SEEK_SET,

   ePAL_FILE_WHENCE_SEEK_END,

   ePAL_FILE_WHENCE_SEEK_CUR,

   ePAL_FILE_WHENCE_MAX
} PAL_FILE_WHENCE_E;

typedef enum _PAL_DIR_ENTRY_TYPE_E
{
   ePAL_DIR_ENTRY_TYPE_INVALID         = 0x00000000,

   ePAL_DIR_ENTRY_TYPE_FILE,

   ePAL_DIR_ENTRY_TYPE_DIR,

   ePAL_DIR_ENTRY_TYPE_MAX
} PAL_DIR_ENTRY_TYPE_E;

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct _PAL_DIR_ENTRY_X
{
   PAL_DIR_ENTRY_TYPE_E e_type;

   uint8_t puc_name;

   uint32_t ui_name_len;
} PAL_DIR_ENTRY_X;

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_file_env_init ();

PAL_RET_E pal_file_env_deinit ();

PAL_RET_E pal_fopen (
   PAL_FILE_HDL *phl_file_hdl,
   const uint8_t *puc_file_path,
   const uint8_t *puc_open_mode);

PAL_RET_E pal_fclose (
   PAL_FILE_HDL hl_file_hdl);

PAL_RET_E pal_freadline (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t ui_buf_len,
   uint32_t *pui_line_len);

PAL_RET_E pal_freadline_v2 (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t ui_buf_len,
   uint32_t *pui_line_len,
   uint32_t *pui_actual_line_len);

PAL_RET_E pal_get_file_size (
   uint8_t *puc_file_path,
   uint64_t  *pul_file_size);

PAL_RET_E pal_fseek (
   PAL_FILE_HDL hl_file_hdl,
   uint32_t ui_offset,
   PAL_FILE_WHENCE_E e_whence);

PAL_RET_E pal_fread (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len);

PAL_RET_E pal_fwrite (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len);

PAL_RET_E pal_ftell (
   PAL_FILE_HDL hl_file_hdl,
   uint32_t *pui_cur_pos);

PAL_RET_E pal_frewind (
   PAL_FILE_HDL hl_file_hdl);

PAL_RET_E pal_get_dir_listing (
   uint8_t *puc_dir_pathname,
   PAL_DIR_ENTRY_X **ppx_listing,
   uint32_t *pui_listing_count);

PAL_RET_E pal_fget_line_count(
   PAL_FILE_HDL hl_file_hdl,
   uint32_t *pui_line_count);

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_FILE_H__ */
