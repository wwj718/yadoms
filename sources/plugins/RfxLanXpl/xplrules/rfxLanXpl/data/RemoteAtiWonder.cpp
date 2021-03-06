#include "stdafx.h"
#include "RemoteAtiWonder.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(ERemoteAtiWonderCodes,
            ((A))
            ((B))
            ((Power))
            ((TV))
            ((DVD))
            ((InterrogrationPoint))
            ((Guide))
            ((Drag))
            ((VolumeUp))
            ((VolumeDown))
            ((Mute))
            ((ChannelUp))
            ((ChannelDown))
            ((Key1))
            ((Key2))
            ((Key3))
            ((Key4))
            ((Key5))
            ((Key6))
            ((Key7))
            ((Key8))
            ((Key9))
            ((Txt))
            ((Key0))
            ((SnapshotESC))
            ((C))
            ((Up))
            ((D))
            ((TVRADIO))
            ((Left))
            ((OK))
            ((Right))
            ((Down))
            ((E))
            ((V))
            ((F))
            ((Rewind))
            ((Play))
            ((FastForward))
            ((Record))
            ((Stop))
            ((Pause))
            ((TV2))
            ((VCR))
            ((RADIO))
            ((TVPreview))
            ((ChannelList))
            ((VideoDesktop))
            ((Red))
            ((Green))
            ((Yellow))
            ((Blue))
            ((RenameTab))
            ((AcquireImage))
            ((EditImage))
            ((FullScreen))
            ((DVDAudio))
            ((CursorLeft))
            ((CursorRight))
            ((CursorUp))
            ((CursorDown))
            ((CursorUpLeft))
            ((CursorUpRight))
            ((CursorDownRight))
            ((CursorDownLeft))
            ((V2))
            ((V2End))
            ((X))
            ((XEnd))
         );

         const shared::plugin::yPluginApi::CStandardCapacity& RemoteAtiWonderCapacity = shared::plugin::yPluginApi::CStandardCapacity("RemoteAtiWonder",
                                                                                                                                      shared::plugin::yPluginApi::CStandardUnits::NoUnits,
                                                                                                                                      shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CRemoteAtiWonder::CRemoteAtiWonder(const std::string& keywordName)
            : CSingleHistorizableData<ERemoteAtiWonderCodes>(keywordName,
                                                             RemoteAtiWonderCapacity,
                                                             shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
         {
         }

         CRemoteAtiWonder::~CRemoteAtiWonder()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


