#pragma once

#include "../../IPluginRequester.h"
#include "server/database/IDatabaseRequester.h"

namespace database { namespace common { namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Plugin requester for SQLite database
      //--------------------------------------------------------------
      class CPlugin : public IPluginRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         explicit CPlugin(boost::shared_ptr<IDatabaseRequester> databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CPlugin();

         // IPluginRequester implementation
         virtual int addInstance(const entities::CPlugin& newPlugin);
         virtual void removeInstance(int pluginId);
         virtual boost::shared_ptr<entities::CPlugin> getInstance(int pluginId);
         virtual boost::shared_ptr<entities::CPlugin> getSystemInstance();
         virtual std::vector<boost::shared_ptr<entities::CPlugin> > getInstances();
         virtual void updateInstance(const entities::CPlugin& newPlugin);
         virtual void disableAutoStartForAllPluginInstances(const std::string& pluginName);
         // [END] IPluginRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to IDatabaseRequester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
      };

} //namespace requesters
} //namespace common
} //namespace database 

