

set(D2HackResourcePluginSources
  D2ResArchive.cpp
  D2ResArchive.h

  OffsetDataStream.cpp
  OffsetDataStream.h

  TxrImageCodec.cpp
  TxrImageCodec.h

  ResourceLoader.cpp
  ResourceLoader.h
)

add_library(d2_hack_resource_loader STATIC ${D2HackResourcePluginSources})
