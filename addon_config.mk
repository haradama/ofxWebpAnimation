meta:
    ADDON_NAME = ofxWebpAnimation
    ADDON_DESCRIPTION = Animated WebP loader / renderer
    ADDON_AUTHOR = Your Name
    ADDON_TAGS = "image" "animation" "webp"
    ADDON_URL = https://github.com/yourname/ofxWebpAnimation

common:
	ADDON_PKG_CONFIG_LIBRARIES += libwebp libwebpdemux
    ADDON_SOURCES_EXCLUDE =
    ADDON_LIBS       += webp webpdemux
    ADDON_CFLAGS     +=
    ADDON_CPPFLAGS   +=
    ADDON_LDFLAGS    +=
    ADDON_INCLUDES   +=
