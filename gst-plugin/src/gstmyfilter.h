/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2020 Niels De Graef <niels.degraef@gmail.com>
 * Copyright (C) 2023  <<user@hostname.org>>
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

#ifndef __GST_MYFILTER_H__
#define __GST_MYFILTER_H__

#include <gst/gst.h>

#define MYFILTER_NALU_SPS 0x67
#define MYFILTER_NALU_PPS 0x68
#define MYFILTER_NALU_DELIMIT 0x09
#define MYFILTER_NALU_SEI 0x66

#define MYFILTER_SEI_MSG_SIZE 0x40
#define MYFILTER_SEI_NALU_TYPE 0x06
#define MYFILTER_SEI_PAYLOAD_TYPE 0x05
#define MYFILTER_SEI_UUID_SIZE 0x10
#define MYFILTER_SEI_TRAILING_BYTE 0x80

G_BEGIN_DECLS

#define GST_TYPE_MYFILTER (gst_myfilter_get_type())
G_DECLARE_FINAL_TYPE(Gstmyfilter, gst_myfilter,
                     GST, MYFILTER, GstElement)

#define MYFILTER_DATA_MAX_LEN       128
#define MYFILTER_DATA_DEFAULT_LEN   0 

struct _Gstmyfilter
{
    GstElement element;

    GstPad *sinkpad, *srcpad;

    gboolean silent;

    guint8 data[MYFILTER_DATA_MAX_LEN];
    guint datalen;
};

G_END_DECLS

#endif /* __GST_MYFILTER_H__ */
