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
 * \file   pal_file.c
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

#include "pal_file_private.h"

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/

/******************************** ENUMERATIONS ********************************/

/************************* STRUCTURE/UNION DATA TYPES *************************/

/************************ STATIC FUNCTION PROTOTYPES **************************/

/****************************** LOCAL FUNCTIONS *******************************/
PAL_RET_E pal_file_env_init ()
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_file_env_deinit ()
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   e_error = ePAL_RET_SUCCESS;

   return e_error;
}

PAL_RET_E pal_fopen (
   PAL_FILE_HDL *phl_file_hdl,
   const uint8_t *puc_file_path,
   const uint8_t *puc_open_mode)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   errno_t i_fopen_ret = -1;

   if ((NULL == phl_file_hdl) || (NULL == puc_file_path)
      || (NULL == puc_open_mode))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_f_ctxt = pal_malloc(sizeof(PAL_FILE_CXT_X), NULL);
   if (NULL == px_f_ctxt)
   {
      PAL_LOG_HIGH("pal_malloc failed: %p", px_f_ctxt);
      e_error = ePAL_RET_OUT_OF_MEMORY;
      goto LBL_CLEANUP;
   }

   (void) pal_strncpy(px_f_ctxt->uca_filename, puc_file_path,
      sizeof(px_f_ctxt->uca_filename));

   i_fopen_ret = fopen_s (&(px_f_ctxt->hl_file_hdl), 
      (const char *) puc_file_path,
      (const char *) puc_open_mode);
   if ((0 != i_fopen_ret) || (NULL == px_f_ctxt->hl_file_hdl))
   {
      PAL_LOG_HIGH("fopen failed: %d, Errno: %d",
         px_f_ctxt->hl_file_hdl, errno);
      e_error = ePAL_RET_FILE_OPEN_FAILED;
   }
   else
   {
      *phl_file_hdl = (PAL_FILE_HDL) px_f_ctxt;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_fclose (
   PAL_FILE_HDL hl_file_hdl)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   int i_fclose_ret = -1;

   if (NULL == hl_file_hdl)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;

   if (NULL != px_f_ctxt->hl_file_hdl)
   {
      i_fclose_ret = fclose (px_f_ctxt->hl_file_hdl);
      if (EOF == i_fclose_ret)
      {
         PAL_LOG_HIGH("fclose failed: %d, Errno: %d", i_fclose_ret, errno);
         e_error = ePAL_RET_FILE_CLOSE_FAILED;
      }
      else
      {
         e_error = ePAL_RET_SUCCESS;
      }
   }
   else
   {
      e_error = ePAL_RET_SUCCESS;
   }
   pal_free (px_f_ctxt);
   px_f_ctxt = NULL;
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_freadline (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t ui_buf_len,
   uint32_t *pui_line_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   uint32_t ui_count = 0;
   uint32_t ui_fread_ret = 0;
   uint8_t *puc_temp = 0;
   int i_feof_ret = -1;

   if ((NULL == hl_file_hdl) || (NULL == puc_buf) || (0 == ui_buf_len)
      || (NULL == pui_line_len))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;
   do
   {
      puc_temp = &(puc_buf [ui_count]);
      ui_fread_ret = fread (puc_temp, 1, 1, px_f_ctxt->hl_file_hdl);
      if (1 == ui_fread_ret)
      {
         ui_count++;
         if ('\n' == (char) *puc_temp)
         {
            e_error = ePAL_RET_SUCCESS;
            break;
         }
      }
      else
      {
         PAL_LOG_HIGH("fread failed: %d, Errno: %d", ui_fread_ret, errno);
         i_feof_ret = feof (px_f_ctxt->hl_file_hdl);
         if (0 != i_feof_ret)
         {
            PAL_LOG_HIGH("feof failed: %d, Errno: %d. EOF Reached",
               i_feof_ret, errno);
            if (0 != ui_count)
            {
               e_error = ePAL_RET_SUCCESS;
            }
            else
            {
               e_error = ePAL_RET_FILE_EOF_REACHED;
            }
         }
         else
         {
            PAL_LOG_HIGH("EOF not reached. Other error: %d, Errno: %d",
               ui_fread_ret, errno);
            e_error = ePAL_RET_FILE_READ_FAILED;
         }
         break;
      }
   } while (ui_count < ui_buf_len);

   if (ePAL_RET_SUCCESS != e_error)
   {
      if (ui_count > 0)
      {
         puc_buf [ui_count - 1] = '\0';
      }
      *pui_line_len = ui_count;
      goto LBL_CLEANUP;
   }

   if (ui_count == ui_buf_len)
   {
      PAL_LOG_HIGH("Buffer overflow occured. Supplied %d bytes, but the "
         "line is more than %d bytes. Truncating!", ui_buf_len, ui_count);
      puc_buf [ui_count - 1] = '\0';
      *pui_line_len = ui_count;
      e_error = ePAL_RET_FILE_READ_BUF_OVERFLOW;
   }
   else
   {
      puc_buf [ui_count - 1] = '\0';
      *pui_line_len = ui_count;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_freadline_v2 (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t ui_buf_len,
   uint32_t *pui_line_len,
   uint32_t *pui_actual_line_len)
{
   PAL_RET_E e_error = ePAL_RET_NOT_IMPLEMENTED;

   return e_error;
}

PAL_RET_E pal_get_file_size (
   uint8_t *puc_file_path,
   uint64_t  *pul_file_size)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   struct stat x_stat = {0};
   int i_stat_ret = -1;

   if ((NULL == puc_file_path) || (NULL == pul_file_size))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *pul_file_size = 0;

   i_stat_ret = stat((const char *)puc_file_path, &x_stat);
   if (0 == i_stat_ret)
   {
      PAL_LOG_FULL("stat success. Size of \"%s\": %llu",
         puc_file_path, x_stat.st_size);
      *pul_file_size = x_stat.st_size;
      e_error = ePAL_RET_SUCCESS;
   }
   else
   {
      PAL_LOG_HIGH("stat failed: %d, Errno: %d", i_stat_ret, errno);
      e_error = ePAL_RET_FILE_STAT_FAILED;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_fseek (
   PAL_FILE_HDL hl_file_hdl,
   uint32_t ui_offset,
   PAL_FILE_WHENCE_E e_whence)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   int i_whence = -1;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   int i_fseek_ret = -1;

   if (NULL == hl_file_hdl)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;

   switch (e_whence)
   {
      case ePAL_FILE_WHENCE_SEEK_SET:
      {
         i_whence = SEEK_SET;
         break;
      }
      case ePAL_FILE_WHENCE_SEEK_END:
      {
         i_whence = SEEK_END;
         break;
      }
      case ePAL_FILE_WHENCE_SEEK_CUR:
      {
         i_whence = SEEK_CUR;
         break;
      }
      default:
      {
         break;
      }
   }

   if (-1 == i_whence)
   {
      PAL_LOG_HIGH("Invalid Args. Invalid Whence value: %d", e_whence);
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   i_fseek_ret = fseek (px_f_ctxt->hl_file_hdl, ui_offset, i_whence);
   if (ePAL_RET_FILE_SEEK_FAILED == i_fseek_ret)
   {
      PAL_LOG_HIGH("fseek failed: %d, Errno: %d", i_fseek_ret, errno);
      e_error = ePAL_RET_FILE_SEEK_FAILED;
   }
   else
   {
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_fread (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   size_t ui_fread_ret = 0;
   uint32_t ui_buf_len = 0;

   if ((NULL == hl_file_hdl) || (NULL == puc_buf) || (NULL == pui_buf_len))
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

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;
   ui_buf_len = *pui_buf_len;

   ui_fread_ret = fread (puc_buf, 1, ui_buf_len, px_f_ctxt->hl_file_hdl);
   if (ui_buf_len != ui_fread_ret)
   {
      PAL_LOG_HIGH("fread failed: %d, Errno: %d. Reqd: %d, Read: %d",
         ui_fread_ret, errno, ui_buf_len, ui_fread_ret);
      *pui_buf_len = ui_fread_ret;
      e_error = ePAL_RET_FILE_READ_FAILED;
   }
   else
   {
      *pui_buf_len = ui_fread_ret;
      e_error = ePAL_RET_SUCCESS;
   }
   LBL_CLEANUP: return e_error;
}

PAL_RET_E pal_fwrite (
   PAL_FILE_HDL hl_file_hdl,
   uint8_t *puc_buf,
   uint32_t *pui_buf_len)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   size_t ui_fwrite_ret = 0;
   uint32_t ui_buf_len = 0;

   if ((NULL == hl_file_hdl) || (NULL == puc_buf) || (NULL == pui_buf_len))
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

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;
   ui_buf_len = *pui_buf_len;

   ui_fwrite_ret = fwrite (puc_buf, 1, ui_buf_len, px_f_ctxt->hl_file_hdl);
   if (ui_buf_len != ui_fwrite_ret)
   {
      PAL_LOG_HIGH("fwrite failed: %d, Errno: %d. Reqd: %d, Read: %d",
         ui_fwrite_ret, errno, ui_buf_len, ui_fwrite_ret);
      *pui_buf_len = ui_fwrite_ret;
      e_error = ePAL_RET_FILE_WRITE_FAILED;
   }
   else
   {
      (void) fflush(px_f_ctxt->hl_file_hdl);
      *pui_buf_len = ui_fwrite_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_ftell (
   PAL_FILE_HDL hl_file_hdl,
   uint32_t *pui_cur_pos)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;
   PAL_FILE_CXT_X *px_f_ctxt = NULL;
   long i_ftell_ret = -1;

   if ((NULL == hl_file_hdl) || (NULL == pui_cur_pos))
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   *pui_cur_pos = 0;

   px_f_ctxt = (PAL_FILE_CXT_X *) hl_file_hdl;

   i_ftell_ret = ftell (px_f_ctxt->hl_file_hdl);
   if (i_ftell_ret < 0)
   {
      e_error = ePAL_RET_FILE_TELL_FAILED;
   }
   else
   {
      *pui_cur_pos = i_ftell_ret;
      e_error = ePAL_RET_SUCCESS;
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_frewind (
   PAL_FILE_HDL hl_file_hdl)
{
   PAL_RET_E e_error = ePAL_RET_FAILURE;

   if (NULL == hl_file_hdl)
   {
      PAL_LOG_HIGH("Invalid Args");
      e_error = ePAL_RET_INVALID_ARGS;
      goto LBL_CLEANUP;
   }

   e_error = pal_fseek (hl_file_hdl, 0L, ePAL_FILE_WHENCE_SEEK_SET);
   if (ePAL_RET_SUCCESS != e_error)
   {
      PAL_LOG_HIGH("pal_fseek failed: %d", e_error);
   }
LBL_CLEANUP:
   return e_error;
}

PAL_RET_E pal_fget_line_count(
   PAL_FILE_HDL hl_file_hdl,
   uint32_t *pui_line_count)
{
   PAL_RET_E e_error = ePAL_RET_NOT_IMPLEMENTED;

   return e_error;
}
