#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/Field.hpp>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A humidity historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CHumidity : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CHumidity(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CHumidity();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      // [END] IHistorizable implementation

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value
      ///\param[in] humidity        humidity value (%)
      //-----------------------------------------------------
      void set(int humidity);

      //-----------------------------------------------------
      ///\brief                     Get value
      ///\return                    The humidity value (%)
      //-----------------------------------------------------
      int humidity() const;

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the humidity value
      ///\param[in] value           Raw value
      ///\return                    The normalized humidity (0-100)
      //-----------------------------------------------------
      static int Normalize(int value);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The humidity value (%)
      //-----------------------------------------------------
      int m_humidity;

      //-----------------------------------------------------
      ///\brief                     The measure type
      //-----------------------------------------------------
      const EMeasureType m_measureType;
   };



} } } } // namespace shared::plugin::yadomsApi::historization
