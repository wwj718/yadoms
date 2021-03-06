#pragma once
#include <toPlugin.pb.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>


namespace plugin_cpp_api
{
   class CDeviceRemoved : public shared::plugin::yPluginApi::IDeviceRemoved
   {
   public:
      explicit CDeviceRemoved(const toPlugin::DeviceRemoved& msg);
      virtual ~CDeviceRemoved();

      // IDeviceRemoved Implementation
      const std::string& device() const override;
      const shared::CDataContainer& details() const override;
      // [END] IDeviceRemoved Implementation

   private:

      const std::string m_device;
      const shared::CDataContainer m_details;
   };
} // namespace plugin_cpp_api	
