/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
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

/**
 * SECTION:element-myfilter
 *
 * FIXME:Describe myfilter here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! myfilter ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>

#include "gstmyfilter.h"

GST_DEBUG_CATEGORY_STATIC(gst_myfilter_debug);
#define GST_CAT_DEFAULT gst_myfilter_debug

/* Filter signals and args */
enum
{
    /* FILL ME */
    LAST_SIGNAL
};

enum
{
    PROP_0,
    PROP_SILENT,
    PROP_DATALEN,
    PROP_DATA,
    PROP_DATAOUT,
    PROP_DATAOUTLEN
};

static gboolean JustCaughtDelimiter = FALSE;
// 086f3693-b7b3-4f2c-9653-21492feee5b8
static guint8 uuid[MYFILTER_SEI_UUID_SIZE] = {
    0x08, 0x6f, 0x36, 0x93,
    0xb7, 0xb3,
    0x4f, 0x2c,
    0x96, 0x53,
    0x21, 0x49, 0x2f, 0xee, 0xe5, 0xb8};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE("sink",
                                                                   GST_PAD_SINK,
                                                                   GST_PAD_ALWAYS,
                                                                   GST_STATIC_CAPS("ANY"));

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
                                                                  GST_PAD_SRC,
                                                                  GST_PAD_ALWAYS,
                                                                  GST_STATIC_CAPS("ANY"));

#define gst_myfilter_parent_class parent_class
G_DEFINE_TYPE(Gstmyfilter, gst_myfilter, GST_TYPE_ELEMENT);

GST_ELEMENT_REGISTER_DEFINE(myfilter, "myfilter", GST_RANK_NONE,
                            GST_TYPE_MYFILTER);

static void gst_myfilter_set_property(GObject *object,
                                      guint prop_id, const GValue *value, GParamSpec *pspec);
static void gst_myfilter_get_property(GObject *object,
                                      guint prop_id, GValue *value, GParamSpec *pspec);

static gboolean gst_myfilter_sink_event(GstPad *pad,
                                        GstObject *parent, GstEvent *event);
static GstFlowReturn gst_myfilter_chain(GstPad *pad,
                                        GstObject *parent, GstBuffer *buf);

/* GObject vmethod implementations */

/* initialize the myfilter's class */
static void
gst_myfilter_class_init(GstmyfilterClass *klass)
{
    GObjectClass *gobject_class;
    GstElementClass *gstelement_class;

    gobject_class = (GObjectClass *)klass;
    gstelement_class = (GstElementClass *)klass;

    gobject_class->set_property = gst_myfilter_set_property;
    gobject_class->get_property = gst_myfilter_get_property;

    g_object_class_install_property(gobject_class, PROP_SILENT,
                                    g_param_spec_boolean(
                                        "silent",
                                        "Silent",
                                        "Produce verbose output ?",
                                        FALSE,
                                        G_PARAM_READWRITE));
    // add datlen property to the class
    g_object_class_install_property(gobject_class,
                                    PROP_DATALEN,
                                    g_param_spec_uint(
                                        "datalen",
                                        "DataLen",
                                        "Data Length",
                                        0,
                                        MYFILTER_DATA_MAX_LEN,
                                        0,
                                        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // add data array
    g_object_class_install_property(gobject_class,
                                    PROP_DATA,
                                    g_param_spec_pointer(
                                        "data",
                                        "dataarr",
                                        "A data array",
                                        G_PARAM_READWRITE));

    // add datout len property to the class
    g_object_class_install_property(gobject_class,
                                    PROP_DATAOUTLEN,
                                    g_param_spec_uint(
                                        "dataoutlen",
                                        "DataOutLen",
                                        "DataOut Length",
                                        0,
                                        MYFILTER_DATA_MAX_LEN,
                                        0,
                                        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // add dataout array
    g_object_class_install_property(gobject_class,
                                    PROP_DATAOUT,
                                    g_param_spec_pointer(
                                        "dataout",
                                        "dataoutarr",
                                        "A dataout array",
                                        G_PARAM_READWRITE));

    gst_element_class_set_details_simple(gstelement_class,
                                         "myfilter",
                                         "FIXME:Generic",
                                         "FIXME:Generic Template Element",
                                         " <<user@hostname.org>>");

    gst_element_class_add_pad_template(gstelement_class,
                                       gst_static_pad_template_get(&src_factory));
    gst_element_class_add_pad_template(gstelement_class,
                                       gst_static_pad_template_get(&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad callback functions
 * initialize instance structure
 */
static void
gst_myfilter_init(Gstmyfilter *filter)
{
    filter->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
    gst_pad_set_event_function(filter->sinkpad,
                               GST_DEBUG_FUNCPTR(gst_myfilter_sink_event));
    gst_pad_set_chain_function(filter->sinkpad,
                               GST_DEBUG_FUNCPTR(gst_myfilter_chain));
    GST_PAD_SET_PROXY_CAPS(filter->sinkpad);
    gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);

    filter->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
    GST_PAD_SET_PROXY_CAPS(filter->srcpad);
    gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);

    filter->silent = FALSE;

    filter->datalen = MYFILTER_DATA_DEFAULT_LEN;

    g_mutex_init(&filter->mutex);

    filter->dataoutlen = MYFILTER_DATA_DEFAULT_LEN;
}

static void
gst_myfilter_set_property(GObject *object, guint prop_id,
                          const GValue *value, GParamSpec *pspec)
{
    Gstmyfilter *filter = GST_MYFILTER(object);

    switch (prop_id)
    {
    case PROP_SILENT:
        filter->silent = g_value_get_boolean(value);
        break;
    case PROP_DATALEN:
        g_mutex_lock(&filter->mutex);
        filter->datalen = g_value_get_uint(value);
        g_mutex_unlock(&filter->mutex);
        break;
    case PROP_DATAOUTLEN:
        filter->dataoutlen = g_value_get_uint(value);
        break;
    case PROP_DATA:
        filter->data = g_value_get_pointer(value);
        break;
    case PROP_DATAOUT:
        filter->dataout = g_value_get_pointer(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
gst_myfilter_get_property(GObject *object, guint prop_id,
                          GValue *value, GParamSpec *pspec)
{
    Gstmyfilter *filter = GST_MYFILTER(object);

    switch (prop_id)
    {
    case PROP_SILENT:
        g_value_set_boolean(value, filter->silent);
        break;
    case PROP_DATALEN:
        g_mutex_lock(&filter->mutex);
        g_value_set_uint64(value, filter->datalen);
        g_mutex_unlock(&filter->mutex);
        break;
    case PROP_DATAOUTLEN:
        g_mutex_lock(&filter->mutex);
        g_value_set_uint(value, filter->dataoutlen);
        g_mutex_unlock(&filter->mutex);
        break;
    case PROP_DATA:
        g_value_set_pointer(value, filter->data);
        break;
    case PROP_DATAOUT:
        g_value_set_pointer(value, filter->dataout);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_myfilter_sink_event(GstPad *pad, GstObject *parent,
                        GstEvent *event)
{
    Gstmyfilter *filter;
    gboolean ret;

    filter = GST_MYFILTER(parent);

    GST_LOG_OBJECT(filter, "Received %s event: %" GST_PTR_FORMAT,
                   GST_EVENT_TYPE_NAME(event), event);

    switch (GST_EVENT_TYPE(event))
    {
    case GST_EVENT_CAPS:
    {
        GstCaps *caps;

        gst_event_parse_caps(event, &caps);
        /* do something with the caps */

        /* and forward */
        ret = gst_pad_event_default(pad, parent, event);
        break;
    }
    default:
        ret = gst_pad_event_default(pad, parent, event);
        break;
    }
    return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_myfilter_chain(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
    Gstmyfilter *filter;
    GstMapInfo info;
    guint8 *data;
    guint size;
    // guint16 i = 0;

    filter = GST_MYFILTER(parent);

    gst_buffer_map(buf, &info, GST_MAP_READ);
    data = info.data;
    size = info.size;

    if (size >= 5 &&
        data[0] == 0x00 &&
        data[1] == 0x00 &&
        data[2] == 0x00 &&
        data[3] == 0x01)
    {
        if (JustCaughtDelimiter == TRUE &&
            data[4] == MYFILTER_NALU_SPS)
        {
            // Send SEI message out
            g_print("In chain() , Send SEI()\n");
            g_print("datalen: %d\n", filter->datalen);

            JustCaughtDelimiter = FALSE;

            g_mutex_lock(&filter->mutex);
            guint8 sei_index = 0, sei_data_size = filter->datalen, i = 0;
            ;
            guint16 gbuffer_size = 4 + 3 + 16 + sei_data_size + 1;

            // Add the NAL unit header to the SEI message
            GstBuffer *sei_buf = gst_buffer_new_and_alloc(gbuffer_size);
            GstMapInfo map;

            gst_buffer_map(sei_buf, &map, GST_MAP_WRITE);
            map.data[0] = 0x00;
            map.data[1] = 0x00;
            map.data[2] = 0x00;
            map.data[3] = 0x01;
            map.data[4] = MYFILTER_SEI_NALU_TYPE;    // NAL unit type for SEI message
            map.data[5] = MYFILTER_SEI_PAYLOAD_TYPE; // SEI message payload type
            map.data[6] = 16 + sei_data_size;        // SEI message payload size

            sei_index = 7;
            // Copy uuid,
            for (i = 0; i < MYFILTER_SEI_UUID_SIZE; i++)
            {
                map.data[sei_index++] = uuid[i];
            }

            // Copy data;
            for (i = 0; i < sei_data_size; i++)
            {
                map.data[sei_index++] = filter->data[i];
                filter->dataout[i] = filter->data[i];
                g_print("%02x ", filter->data[i]);
            }
            filter->dataout[i] = 0;
            g_print("\n");
            filter->datalen = 0;

            // set output to textoverlay
            filter->dataoutlen = sei_data_size;

            g_mutex_unlock(&filter->mutex);

            // End byte is 0x80;
            map.data[gbuffer_size - 1] = MYFILTER_SEI_TRAILING_BYTE;

            // Unmap the buffer
            gst_buffer_unmap(sei_buf, &map);

            // Push the SEI message downstream
            gst_pad_push(filter->srcpad, sei_buf);
        }
        else if (data[4] == MYFILTER_NALU_DELIMIT)
        {
            JustCaughtDelimiter = TRUE;
        }
        else
        {
            JustCaughtDelimiter = FALSE;
        }
    }

    /* just push out the incoming buffer without touching it */
    return gst_pad_push(filter->srcpad, buf);
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
myfilter_init(GstPlugin *myfilter)
{
    /* debug category for filtering log messages
     *
     * exchange the string 'Template myfilter' with your description
     */
    GST_DEBUG_CATEGORY_INIT(gst_myfilter_debug, "myfilter",
                            0, "Template myfilter");

    return GST_ELEMENT_REGISTER(myfilter, myfilter);
}

/* PACKAGE: this is usually set by meson depending on some _INIT macro
 * in meson.build and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use meson to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstmyfilter"
#endif

/* gstreamer looks for this structure to register myfilters
 *
 * exchange the string 'Template myfilter' with your myfilter description
 */
GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  myfilter,
                  "myfilter",
                  myfilter_init,
                  PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
