#include "stdafx.h"
#include "Analog.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{

   CAnalog::CAnalog(const std::string& keywordName,
                   const yApi::EKeywordAccessMode& accessMode,
                   const yApi::historization::EMeasureType& measureType,
                   yApi::historization::typeInfo::ITypeInfo& additionalInfo)
      : CSingleHistorizableData<unsigned int>(keywordName,
                                        yApi::CStandardCapacities::Text,
                                        accessMode,
                                        0,
                                        measureType,
                                        additionalInfo)
   {}

   CAnalog::~CAnalog()
   {}
}