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
 * \file   exp_pal_semaphore.h
 *
 * \author sandeepprakash
 *
 * \date   15-Sep-2012
 *
 * \brief
 *
 ******************************************************************************/

#ifndef __EXP_PAL_SEMAPHORE_H__
#define __EXP_PAL_SEMAPHORE_H__

#ifdef  __cplusplus
extern  "C"
{
#endif

/********************************* CONSTANTS **********************************/

/*********************************** MACROS ***********************************/
#define PAL_SEM_NAME_MAX_STR_LEN             (32)

#define PAL_SEM_VALUE_MAX                    (SEM_VALUE_MAX)

/*********************************** HANDLES **********************************/
typedef struct PAL_SEM_CTXT_X         *PAL_SEM_HDL;

/******************************** ENUMERATIONS ********************************/

/*********************** CLASS/STRUCTURE/UNION DATA TYPES *********************/
typedef struct _PAL_SEM_CREATE_PARAM_X
{
   uint8_t uca_sem_name_str [PAL_SEM_NAME_MAX_STR_LEN];

   uint32_t ui_initial_count;

   uint32_t ui_max_count;
} PAL_SEM_CREATE_PARAM_X;

/***************************** FUNCTION PROTOTYPES ****************************/
PAL_RET_E pal_sem_env_init ();

PAL_RET_E pal_sem_env_deinit ();

PAL_RET_E pal_sem_create(
   PAL_SEM_HDL *phl_sem_hdl,
   PAL_SEM_CREATE_PARAM_X *px_create_param);

PAL_RET_E pal_sem_destroy(
   PAL_SEM_HDL hl_sem_hdl);

PAL_RET_E pal_sem_get(
   PAL_SEM_HDL hl_sem_hdl,
   uint32_t ui_timeout);

PAL_RET_E pal_sem_put(
   PAL_SEM_HDL hl_sem_hdl);

#ifdef   __cplusplus
}
#endif

#endif /* __EXP_PAL_SEMAPHORE_H__ */
