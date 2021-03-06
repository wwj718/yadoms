#include "stdafx.h"
#include "FrameFactory.h"
#include "AcquisitionFilterFrame.h"
#include "NewDeviceFrame.h"
#include <shared/Log.h>

namespace web { namespace ws {

   boost::shared_ptr<CFrameBase> CFrameFactory::tryParse(const std::string & frameAsString)
   {
      try
      {
         shared::CDataContainer obj(frameAsString);
         if (obj.containsValue(CFrameBase::getTypeFieldName()))
         {
            switch (obj.get<CFrameBase::EFrameType>(CFrameBase::getTypeFieldName()))
            {
            case CFrameBase::EFrameType::kAcquisitionFilterValue:
               return boost::shared_ptr<CFrameBase>(new CAcquisitionFilterFrame(frameAsString));

            default:
               throw shared::exception::COutOfRange("Unknown type");
            }
         }
         else
         {
            YADOMS_LOG(debug) << "Cannot find frame type : " << frameAsString;
         }
      }
      catch (shared::exception::CInvalidParameter&)
      {
         YADOMS_LOG(debug) << "Fail to parse JSON : " << frameAsString;
      }
      catch (shared::exception::COutOfRange&)
      {
         YADOMS_LOG(debug) << "Fail to parse frame. Unknown type : " << frameAsString;
      }
      catch (std::exception&)
      {
         YADOMS_LOG(debug) << "Fail to parse frame. Unknown error : " << frameAsString;
      }

      //if something fail return null
      return boost::shared_ptr<CFrameBase>();
   }




} //namespace ws
} //namespace web