#include "stdafx.h"
#include "Information.h"
#include "../PluginException.hpp"

namespace pluginSystem
{
   namespace serializers
   {
      CInformation::CInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information)
         : m_information(information)
      {
      }

      CInformation::~CInformation()
      {
      }

      void CInformation::toPb(toPlugin::Information* pb) const
      {
         pb->set_type(m_information->getType());
         pb->set_version(m_information->getVersion());
         switch (m_information->getReleaseType())
         {
         case shared::versioning::EReleaseType::kStableValue: pb->set_releasetype(toPlugin::Information_EReleaseType_kStable);
            break;
         case shared::versioning::EReleaseType::kReleaseCandidateValue: pb->set_releasetype(toPlugin::Information_EReleaseType_kReleaseCandidate);
            break;
         default: pb->set_releasetype(toPlugin::Information_EReleaseType_kBeta);
            break;
         }
         pb->set_author(m_information->getAuthor());
         pb->set_url(m_information->getUrl());
         pb->set_identity(m_information->getIdentity());
         pb->set_tostring(m_information->toString());
         pb->set_supportedonthisplatform(m_information->isSupportedOnThisPlatform());
         pb->set_supportmanuallycreateddevice(m_information->getSupportManuallyCreatedDevice());
         pb->set_supportdeviceremovednotification(m_information->getSupportDeviceRemovedNotification());
         pb->set_packagefilecontent(m_information->getPackage()->serialize());
         pb->set_path(m_information->getPath().string());

         if (!pb->IsInitialized())
            throw CPluginException("unable to serialize plugin information");
      }
   }
} // namespace pluginSystem::serializers
