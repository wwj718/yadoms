#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IMultiSwitch.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device
{
   //--------------------------------------------------------------
   /// \brief	8 channels adressable switch (Family 29)
   //--------------------------------------------------------------
   class C8ChannelAddressableSwitch : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	api yApi context
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      C8ChannelAddressableSwitch(EOneWireFamily family,
                                 const std::string& id,
                                 boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::shared_ptr<ioInterfaces::IMultiSwitch> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~C8ChannelAddressableSwitch();

   protected:
      // IDevice implementation
      boost::shared_ptr<const IIdentification> ident() const override
      {
         return m_identification;
      }
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override
      {
         return m_keywords;
      }
      void get() const override;
      void set(const std::string& keyword, const std::string& command) override;
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The IO number
      //--------------------------------------------------------------
      enum { NbIo = 8 };

      //--------------------------------------------------------------
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

      //--------------------------------------------------------------
      /// \brief	The yApi context
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::IYPluginApi> m_context;

      //--------------------------------------------------------------
      /// \brief	The I/O access object
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::IMultiSwitch> m_io;

      //--------------------------------------------------------------
      /// \brief	The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_keyword[NbIo];
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device