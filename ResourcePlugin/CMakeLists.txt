

set(D2HackResourcePluginSources
  D2ResArchive.cpp
  D2ResArchive.h

  D2ResFileInfo.cpp
  D2ResFileInfo.h

  OffsetDataStream.cpp
  OffsetDataStream.h

  TxrImageCodec.cpp
  TxrImageCodec.h

  ResourceLoader.cpp
  ResourceLoader.h

  D2RawLoader.cpp
  D2RawLoader.h
)

add_library(d2_hack_resource_loader STATIC ${D2HackResourcePluginSources})
