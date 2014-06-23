#pragma once
#include "../xplcore/XplMessageSchemaIdentifier.h"

namespace xplrules {

   //------------------------------------
   ///\brief Class container for a device identifier.
   /// It could provide also the commercial name
   //------------------------------------
   class CDeviceIdentifier
   {
   public:
      //------------------------------------
      ///\brief Constructor
      ///\param [in] id The device id
      ///\param [in] commercialName The device commercial name
      ///\param [in] readingProtocol   The xpl protocol used to read data
      ///\param [in] writingProtocol   The xpl protocol used to send command
      //------------------------------------
      CDeviceIdentifier(const std::string & id, const std::string & commercialName, const xplcore::CXplMessageSchemaIdentifier & readingProtocol, const xplcore::CXplMessageSchemaIdentifier & writingProtocol);

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CDeviceIdentifier();

      //------------------------------------
      ///\brief Get the device identifier
      ///\return the device identifier
      //------------------------------------
      const std::string & getId();

      //------------------------------------
      ///\brief Get the device commercial name
      /// If the commercial name is not defined, it retuens the device identifier
      ///\return the commercial name, or device identifier if commercial one is not defined
      //------------------------------------
      const std::string & getCommercialName();

      //------------------------------------
      ///\brief Get the xpl protocol used for reading data
      ///\return the xpl protocol
      //------------------------------------
      const xplcore::CXplMessageSchemaIdentifier & getReadingXplProtocol();

      //------------------------------------
      ///\brief Get the xpl protocol used for sending command
      ///\return the xpl protocol
      //------------------------------------
      const xplcore::CXplMessageSchemaIdentifier & getWritingXplProtocol();
   private:
      //------------------------------------
      ///\brief The device identifier
      //------------------------------------
      std::string m_id;

      //------------------------------------
      ///\brief The commercial name
      //------------------------------------
      std::string m_commercialName;

      //------------------------------------
      ///\brief the xpl protocol used for reading data
      //------------------------------------
      xplcore::CXplMessageSchemaIdentifier m_xplProtocolReading;

      //------------------------------------
      ///\brief the xpl protocol used for sending command
      //------------------------------------
      xplcore::CXplMessageSchemaIdentifier m_xplProtocolWriting;
   };


} //namespace xplrules

