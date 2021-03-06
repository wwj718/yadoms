#include "stdafx.h"
#include "Energy.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CEnergy::CEnergy(boost::shared_ptr<yApi::IYPluginApi> api,
                    const RBUF& rbuf,
                    size_t rbufSize)
      : m_instantPower(boost::make_shared<yApi::historization::CPower>("instant")),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_keywords({ m_instantPower , m_batteryLevel , m_rssi })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeENERGY,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(ENERGY),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.ENERGY.subtype;

      m_id = rbuf.ENERGY.id1 | (rbuf.ENERGY.id2 << 8);

      long instantPower = rbuf.ENERGY.instant1 << 24 | rbuf.ENERGY.instant2 << 16 | rbuf.ENERGY.instant3 << 8 | rbuf.ENERGY.instant4;
      m_instantPower->set(instantPower);

      // No total power on CM180 if count > 0
      if (m_subType != sTypeELEC3 || rbuf.ENERGY.count == 0)
      {
         m_totalPower = boost::make_shared<yApi::historization::CEnergy>("total");
         m_keywords.push_back(m_totalPower);

         Poco::Int64 totalPower = rbuf.ENERGY.total3 << 24 | rbuf.ENERGY.total4 << 16 | rbuf.ENERGY.total5 << 8 | rbuf.ENERGY.total6;
         totalPower += rbuf.ENERGY.total2 * 2 ^ 32;
         totalPower += rbuf.ENERGY.total1 * 2 ^ 40;
         m_totalPower->set(static_cast<Poco::Int64>(totalPower / 223.666));
      }

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.ENERGY.battery_level));
      m_rssi->set(NormalizeRssiLevel(rbuf.ENERGY.rssi));

      Init(api);
   }

   CEnergy::~CEnergy()
   {
   }

   void CEnergy::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeENERGY);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CEnergy::encode(boost::shared_ptr<ISequenceNumberProvider> /*seqNumberProvider*/) const
   {
      throw shared::exception::CInvalidParameter("Energy is a read-only message, can not be encoded");
   }

   void CEnergy::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CEnergy::getDeviceName() const
   {
      return m_deviceName;
   }

   void CEnergy::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CEnergy::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeELEC2: ssModel << "CM119/160";
         break;
      case sTypeELEC3: ssModel << "CM180";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


