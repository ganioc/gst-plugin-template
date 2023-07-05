/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
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

/**
 * SECTION:element-plugin
 *
 * FIXME:Describe plugin here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! plugin ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include <gio/gio.h>

#include "gstseifilter.h"

GST_DEBUG_CATEGORY_STATIC(gst_seifilter_debug);
#define GST_CAT_DEFAULT gst_seifilter_debug

/* add by yango */
static struct timespec old_ts = {0}; 

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

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

#define gst_seifilter_parent_class parent_class
G_DEFINE_TYPE(GstSeiFilter, gst_seifilter, GST_TYPE_ELEMENT);

// GST_ELEMENT_REGISTER_DEFINE (sei_filter, "sei_filter", GST_RANK_NONE,
//     GST_TYPE_SEI_FILTER);

static void gst_seifilter_set_property(GObject *object,
                                       guint prop_id, const GValue *value, GParamSpec *pspec);

static void gst_seifilter_get_property(GObject *object,
                                       guint prop_id, GValue *value, GParamSpec *pspec);

static gboolean gst_seifilter_sink_event(GstPad *pad,
                                         GstObject *parent, GstEvent *event);

static GstFlowReturn gst_seifilter_chain(GstPad *pad,
                                         GstObject *parent, GstBuffer *buf);

/* GObject vmethod implementations */

/* initialize the plugin's class */
static void
gst_seifilter_class_init(GstSeiFilterClass *klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *)klass;
  gstelement_class = (GstElementClass *)klass;

  gobject_class->set_property = gst_seifilter_set_property;
  gobject_class->get_property = gst_seifilter_get_property;

  g_object_class_install_property(gobject_class, PROP_SILENT,
                                  g_param_spec_boolean("silent", "Silent", "Produce verbose output ?",
                                                       FALSE, G_PARAM_READWRITE));

  gst_element_class_set_details_simple(gstelement_class,
                                       "seifilter",
                                       "SEI Filter",
                                       "SEI Filter Element", "yangjun@nanchao.org");

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
gst_seifilter_init(GstSeiFilter *filter)
{
  filter->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
  gst_pad_set_event_function(filter->sinkpad,
                             GST_DEBUG_FUNCPTR(gst_seifilter_sink_event));
  gst_pad_set_chain_function(filter->sinkpad,
                             GST_DEBUG_FUNCPTR(gst_seifilter_chain));
  GST_PAD_SET_PROXY_CAPS(filter->sinkpad);
  gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS(filter->srcpad);
  gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);

  filter->silent = FALSE;
}

static void
gst_seifilter_set_property(GObject *object, guint prop_id,
                           const GValue *value, GParamSpec *pspec)
{
  GstSeiFilter *filter = GST_SEIFILTER(object);

  switch (prop_id)
  {
  case PROP_SILENT:
    filter->silent = g_value_get_boolean(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
gst_seifilter_get_property(GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec)
{
  GstSeiFilter *filter = GST_SEIFILTER(object);

  switch (prop_id)
  {
  case PROP_SILENT:
    g_value_set_boolean(value, filter->silent);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_seifilter_sink_event(GstPad *pad, GstObject *parent,
                         GstEvent *event)
{
  GstSeiFilter *filter;
  gboolean ret;

  filter = GST_SEIFILTER(parent);

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
/*
 * GET from HTTP server,
 * Put result in buffer, length in buffer_len
 */
gboolean read_from_server(gchar *host, guint16 port, gchar *uri, gchar *buffer, guint16 *buffer_len)
{
  // http get, socket
  GSocketClient *client = g_socket_client_new();
  GSocketConnection *connection = NULL;
  GError *error = NULL;
  gchar request[256];
  gchar port_str[12];
  gchar local_buffer[2048];
    // gsize buffer_size = 0;
  gsize rs;

  connection = g_socket_client_connect_to_host(client,
                                               host, 
                                               port, 
                                               NULL, 
                                               &error);
  // g_main_loop_run(loop);
  //g_print("after connection\n");

  if (error == NULL)
  {
    g_print("Connection OK\n");

    // get thre result
    // GAsyncResult *async_result = G_ASYNC_RESULT(result);
    // GSocketConnection *connection = g_socket_client_connect_to_host_finish(client,
    //   async_result, NULL);
    GInputStream *istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream *ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

    // gchar *request = "GET /one HTTP/1.0\r\nHost: 127.0.0.1:5000\r\n\r\n";
    g_sprintf(port_str, "%d", port);
    g_sprintf(request, 
      "GET /%s HTTP/1.0\r\nHost: %s:%s\r\n\r\n",
      uri,
      host,
      port_str);
    

    g_output_stream_write(ostream, 
      request, 
      strlen(request), 
      NULL, 
      &error);

    //g_print("after ostream write\n");

    // read HTTP response,


    do
    {
      rs = g_input_stream_read(istream, local_buffer, 1024, NULL, &error);

      if (error != NULL)
      {
        g_print("read error\n");
        g_io_stream_close(G_IO_STREAM(connection), NULL, &error);
        g_object_unref(istream);
        g_object_unref(client);
        return FALSE;
      }
      else if(rs > 0)
      {
        g_print("read OK :: %d\n",rs);
        for(int i = 0; i < rs; i++){
          g_print("%c", local_buffer[i]);
        }
        // bzero(buffer,1024);
      }
    } while (rs > 0);
    g_print("\n");
    g_print("after read istream\n");


    g_io_stream_close(G_IO_STREAM(connection), NULL, &error);
    g_object_unref(istream);
    g_object_unref(client);

    return TRUE;
  }
  else
  {
    g_print("Connection failed\n");
    g_object_unref(client);

    return FALSE;
  }

}
/*
 * default SPS interval is 15ms, too short for http GET request
 * 592149191 - 577500847 = 14.6 ms
*/
static gboolean is_in_valid_interval(void){
  struct timespec ts;  
  struct timespec temp_ts;

  clock_gettime(CLOCK_REALTIME, &ts);

  // temp_ts.tv_sec = old_ts.tv_sec;
  // temp_ts.tv_nsec = old_ts.tv_nsec;  



  // g_print("time cur: %d %d\n", ts.tv_sec, ts.tv_nsec);
  // g_print("time old: %d %d\n", temp_ts.tv_sec, temp_ts.tv_nsec);


  if(ts.tv_sec - old_ts.tv_sec > 1) {
    g_print("Over 1 s\n");
    old_ts.tv_sec = ts.tv_sec;
    old_ts.tv_nsec = ts.tv_nsec;
    return TRUE;
  }else if (ts.tv_sec - old_ts.tv_sec == 1 && (ts.tv_nsec + NS_IN_S - old_ts.tv_nsec) > MIN_SEI_INTERVAL_TIME_NS) {
    g_print("2nd, %d, %d\n", ts.tv_sec - old_ts.tv_sec,ts.tv_nsec + NS_IN_S - old_ts.tv_nsec );
    old_ts.tv_sec = ts.tv_sec;
    old_ts.tv_nsec = ts.tv_nsec;
    return TRUE;
  }else if (ts.tv_sec == old_ts.tv_sec && (ts.tv_nsec - old_ts.tv_nsec) > MIN_SEI_INTERVAL_TIME_NS){
    g_print("3rd, %d, %d\n",ts.tv_sec - old_ts.tv_sec , ts.tv_nsec - old_ts.tv_nsec);
    old_ts.tv_sec = ts.tv_sec;
    old_ts.tv_nsec = ts.tv_nsec;
    return TRUE;
  }else{
    // g_print("Not over\n");
    return FALSE;
  }
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_seifilter_chain(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
  GstSeiFilter *filter;
  GstMapInfo info;
  guint8 *data;
  guint size;
  gboolean rtn;
  gchar datum[128];
  gsize datum_len = 0;

  filter = GST_SEIFILTER(parent);

  gst_buffer_map(buf, &info, GST_MAP_READ);
  data = info.data;
  size = info.size;

  if (size >= 5 &&
      data[0] == 0x00 &&
      data[1] == 0x00 &&
      data[2] == 0x00 &&
      data[3] == 0x01 &&
      is_in_valid_interval())
  {
    g_print("SSP caught\n");
    rtn = read_from_server("127.0.0.1", 
      5000, 
      "one", 
      datum,
      &datum_len);
  }

  // if (filter->silent == FALSE)
  //   g_print ("I'm plugged, therefore I'm in.\n");

  /* just push out the incoming buffer without touching it */
  return gst_pad_push(filter->srcpad, buf);
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
seifilter_init(GstPlugin *seifilter)
{
  /* debug category for filtering log messages
   *
   * exchange the string 'Template plugin' with your description
   */
  GST_DEBUG_CATEGORY_INIT(gst_seifilter_debug, "seifilter",
                          0, "SEI Filter");

  // return GST_ELEMENT_REGISTER (sei_filter, plugin);
  return gst_element_register(seifilter, "seifilter",
                              GST_RANK_NONE,
                              GST_TYPE_SEIFILTER);
}

/* PACKAGE: this is usually set by meson depending on some _INIT macro
 * in meson.build and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use meson to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "seifilterplugin"
#endif

/* gstreamer looks for this structure to register plugins
 *
 * exchange the string 'Template plugin' with your plugin description
 */
GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  seifilter,
                  "seifilter",
                  seifilter_init,
                  PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
