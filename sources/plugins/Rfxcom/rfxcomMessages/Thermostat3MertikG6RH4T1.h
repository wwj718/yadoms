#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat Mertik G6RH4T1 keyword
   //--------------------------------------------------------------
   class CThermostat3MertikG6RH4T1 : public IThermostat3Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat3MertikG6RH4T1();

      // ILighting2Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& keyword, const shared::CDataContainer& yadomsCommand);
      virtual void reset();
      virtual void setFromProtocolState(unsigned char cmd);
      virtual void toProtocolState(unsigned char& cmd) const;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      yApi::historization::CSwitch m_onOff;
      yApi::historization::CUpDownStop m_UpDown;
      yApi::historization::CUpDownStop m_RunUpDown;

      //--------------------------------------------------------------
      /// \brief	                        Status byte buffering
      //--------------------------------------------------------------
      unsigned char m_statusByte;
   };

} // namespace rfxcomMessages