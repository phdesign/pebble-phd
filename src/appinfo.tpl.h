#ifndef APPINFO_H
#define APPINFO_H

#define LONG_NAME "<%= config.info.longName %>"
#define VERSION_LABEL "<%= config.info.versionLabel %>"
#define UUID "<%= config.info.uuid %>"

#define PERSIST_KEY_CONFIG 0
#define PERSIST_KEY_STORAGE_VERSION 1

<% for (prop in config.info.appKeys) { 
  %>#define <%= prop %> <%= config.info.appKeys[prop] %>
<% } %>

#endif
