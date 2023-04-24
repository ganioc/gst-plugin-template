

## 检查buffer是一个H.264 NAL Unit
In this example, we check if the buffer contains a H.264 NAL unit by checking if it has the GST_BUFFER_FLAG_DELTA_UNIT, GST_BUFFER_FLAG_DECODE_ONLY, and GST_BUFFER_FLAG_DECODED_UNIT flags set. These flags indicate that the buffer contains a NAL unit.

```c
static GstFlowReturn my_sink_chain(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
    // Check if the buffer contains a H.264 NAL unit
    if (GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DELTA_UNIT) &&
        GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DECODE_ONLY) &&
        GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DECODED_UNIT))
    {
        // Get the NAL type from the first byte of the buffer
        guint8 nal_type = GST_BUFFER_DATA(buf)[0] & 0x1F;
        
        // Handle the NAL unit based on its type
        switch (nal_type)
        {
            case 1: // Non-IDR slice
                // Handle the NAL unit
                break;
            case 5: // IDR slice
                // Handle the NAL unit
                break;
            // Handle other NAL types here
            default:
                break;
        }
    }
    
    // Pass the buffer downstream
    return gst_pad_push(my_sinkpad, buf);
}
```

Q: How to check for keyframes?
A: 


## add a SEI message into 
In this example, we first check if the buffer contains a H.264 NAL unit and handle it as before. Then, we check if we need to insert a SEI message using the need_to_insert_sei_message() function.

If we need to insert a SEI message, we create the message using the create_sei_message() function. We then add the NAL unit header to the SEI message by modifying the first three bytes of the buffer. Finally, we push the SEI message downstream using gst_pad_push().

```c
static GstFlowReturn my_sink_chain(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
    // Check if the buffer contains a H.264 NAL unit
    if (GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DELTA_UNIT) &&
        GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DECODE_ONLY) &&
        GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DECODED_UNIT))
    {
        // Get the NAL type from the first byte of the buffer
        guint8 nal_type = GST_BUFFER_DATA(buf)[0] & 0x1F;
        
        // Handle the NAL unit based on its type
        switch (nal_type)
        {
            case 1: // Non-IDR slice
                // Handle the NAL unit
                break;
            case 5: // IDR slice
                // Handle the NAL unit
                break;
            // Handle other NAL types here
            default:
                break;
        }
        
        // Check if we need to insert a SEI message
        if (need_to_insert_sei_message())
        {
            // Create the SEI message
            GstBuffer *sei_buf = create_sei_message();
            
            // Add the NAL unit header to the SEI message
            GstMapInfo map;
            gst_buffer_map(sei_buf, &map, GST_MAP_WRITE);
            map.data[0] = 0x06; // NAL unit type for SEI message
            map.data[1] = 0x01; // SEI message payload type
            map.data[2] = 0x80; // SEI message payload size
            gst_buffer_unmap(sei_buf, &map);
            
            // Push the SEI message downstream
            gst_pad_push(my_sinkpad, sei_buf);
        }
    }
    
    // Pass the buffer downstream
    return gst_pad_push(my_sinkpad, buf);
}
```

Q: 如何生成一个SEI Message呢？
A: 还需要有padding, 32bit, 也就是4字节的倍数,

SEI message payload type,

SEI message payload size,

SEI message payload end byte,



## 暴露set_property,这样应用就可以对参数进行设置了
In this example, we create a pipeline and a plugin using gst_pipeline_new() and gst_element_factory_make(). We then set the property of the plugin using the set_plugin_property() function, which uses g_object_set() to set the property value.

After setting the property, we add the plugin to the pipeline using gst_bin_add() and link the elements together using gst_element_link(). We start the pipeline using gst_element_set_state() and wait until it finishes using gst_bus_timed_pop_filtered().

Finally, we cleanup the pipeline and the bus using gst_element_set_state(), gst_object_unref(), and gst_message_unref().


```c
static GstFlowReturn
gst_my_filter_chain (GstPad    *pad,
             GstObject *parent,
             GstBuffer *buf)
{
  GstMyFilter *filter = GST_MY_FILTER (parent);
  GstBuffer *outbuf;

  outbuf = gst_my_filter_process_data (filter, buf);
  gst_buffer_unref (buf);
  if (!outbuf) {
    /* something went wrong - signal an error */
    GST_ELEMENT_ERROR (GST_ELEMENT (filter), STREAM, FAILED, (NULL), (NULL));
    return GST_FLOW_ERROR;
  }

  return gst_pad_push (filter->srcpad, outbuf);
}

```

## 处理caps

```c
static gboolean my_plugin_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    MyPlugin *plugin = MY_PLUGIN(parent);
    
    switch (GST_EVENT_TYPE(event))
    {
        case GST_EVENT_CAPS:
        {
            GstCaps *caps;
            gst_event_parse_caps(event, &caps);
            
            // Do something with the caps
            GstStructure *structure = gst_caps_get_structure(caps, 0);
            const gchar *mime_type = gst_structure_get_name(structure);
            GST_INFO_OBJECT(plugin, "Received caps event with mime type %s", mime_type);
            
            // Optionally modify the caps and push them downstream
            GstCaps *modified_caps = gst_caps_copy(caps);
            GstStructure *modified_structure = gst_caps_get_structure(modified_caps, 0);
            gst_structure_set(modified_structure, "width", G_TYPE_INT, 640, NULL);
            gst_structure_set(modified_structure, "height", G_TYPE_INT, 480, NULL);
            gst_pad_set_caps(pad, modified_caps);
            gst_caps_unref(modified_caps);
            
            break;
        }
        case GST_EVENT_EOS:
            // Handle EOS event
            break;
        default:
            break;
    }
    
    return gst_pad_event_default(pad, parent, event);
}
```

## How to add a property to plugin?



