/*
 * Copyright 2020 Advanced Micro Devices, Inc.
 * Copyright 2020 Valve Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef AC_SQTT_H
#define AC_SQTT_H

#include <stdint.h>

struct ac_thread_trace_data {
   struct radeon_cmdbuf *start_cs[2];
   struct radeon_cmdbuf *stop_cs[2];
   /* struct radeon_winsys_bo or struct pb_buffer */
   void *bo;
   void *ptr;
   uint32_t buffer_size;
   int start_frame;
   char *trigger_file;
};

#define SQTT_BUFFER_ALIGN_SHIFT 12

struct ac_thread_trace_info {
   uint32_t cur_offset;
   uint32_t trace_status;
   union {
      uint32_t gfx9_write_counter;
      uint32_t gfx10_dropped_cntr;
   };
};

struct ac_thread_trace_se {
   struct ac_thread_trace_info info;
   void *data_ptr;
   uint32_t shader_engine;
   uint32_t compute_unit;
};

struct ac_thread_trace {
   uint32_t num_traces;
   struct ac_thread_trace_se traces[4];
};

uint64_t
ac_thread_trace_get_info_offset(unsigned se);

uint64_t
ac_thread_trace_get_data_offset(struct ac_thread_trace_data *data, unsigned se);
uint64_t
ac_thread_trace_get_info_va(uint64_t va, unsigned se);

uint64_t
ac_thread_trace_get_data_va(struct ac_thread_trace_data *data, uint64_t va, unsigned se);

#endif