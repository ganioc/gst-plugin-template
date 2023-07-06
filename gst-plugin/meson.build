plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-ruff-plugin')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'GStreamer Ruff Plug-ins')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org')
configure_file(output : 'config.h', configuration : cdata)

gstaudio_dep = dependency('gstreamer-audio-1.0',
    fallback: ['gst-plugins-base', 'audio_dep'])

gio_dep = dependency('gio-2.0', fallback: ['glib', 'libgio_dep'])
# Plugin 1
#plugin_sources = [
#  'src/gstplugin.c'
#  ]

#gstpluginexample = library('gstplugin',
#  plugin_sources,
#  c_args: plugin_c_args,
#  dependencies : [gst_dep],
#  install : true,
#  install_dir : plugins_install_dir,
#)

# Plugin 2 (audio filter example)
#audiofilter_sources = [
#  'src/gstaudiofilter.c',
#  ]

#gstaudiofilterexample = library('gstaudiofilterexample',
#  audiofilter_sources,
#  c_args: plugin_c_args,
#  dependencies : [gst_dep, gstaudio_dep],
#  install : true,
#  install_dir : plugins_install_dir,
#)

# # The myfilter Plugin
#  gstmyfilter_sources = [
#   'src/gstmyfilter.c',
#   ]

# gstmyfilter = library('gstmyfilter',
#   gstmyfilter_sources,
#   c_args: plugin_c_args,
#   dependencies : [gst_dep, gstbase_dep],
#   install : true,
#   install_dir : plugins_install_dir,
# )


 gstseifilter_sources = [
  'src/gstseifilter.c',
  ]

gstseifilter = library('gstseifilter',
  gstseifilter_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstbase_dep, gio_dep],
  install : true,
  install_dir : plugins_install_dir,
)


# The Custom Plugin
#  gstcustplugin_sources = [
#   'src/gstcustplugin.c',
#   ]

# gstcustplugin = library('gstcustplugin',
#   gstcustplugin_sources,
#   c_args: plugin_c_args,
#   dependencies : [gst_dep, gstbase_dep],
#   install : true,
#   install_dir : plugins_install_dir,
# )
