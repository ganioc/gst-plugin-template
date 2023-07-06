/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2020 Niels De Graef <niels.degraef@gmail.com>
 * Copyright (C) YEAR AUTHOR_NAME AUTHOR_EMAIL
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_SEI_FILTER_H__
#define __GST_SEI_FILTER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

// 100 ms
#define MIN_SEI_INTERVAL_TIME_NS (500000000)
#define NS_IN_S (1000000000)

#define SEIFILTER_NALU_SPS 0x67
#define SEIFILTER_NALU_PPS 0x68
#define SEIFILTER_NALU_DELIMIT 0x09
#define SEIFILTER_NALU_SEI 0x66

#define SEIFILTER_SEI_MSG_SIZE 0x40
#define SEIFILTER_SEI_NALU_TYPE 0x06
#define SEIFILTER_SEI_PAYLOAD_TYPE 0x05
#define SEIFILTER_SEI_UUID_SIZE 0x10
#define SEIFILTER_SEI_TRAILING_BYTE 0x80

#define SEIFILTER_HOST_LEN 64
#define SEIFILTER_URI_LEN 256

#define GST_TYPE_SEIFILTER (gst_seifilter_get_type())
G_DECLARE_FINAL_TYPE(GstSeiFilter, gst_seifilter,
                     GST, SEIFILTER, GstElement)

struct _GstSeiFilter
{
  GstElement element;

  GstPad *sinkpad, *srcpad;

  gboolean silent;
  gchar *host;
  guint16 host_len;
  guint16 port;
  gchar *uri;
  guint16 uri_len;
  guint16 interval;
};

G_END_DECLS

#endif /* __GST_SEI_FILTER_H__ */
