#pragma once
#include "IProperties.h"
#include <database/entities/Entities.h>

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script properties
   //-----------------------------------------------------
   class CProperties : public IProperties
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] ruleData Rule data
      //-----------------------------------------------------
      explicit CProperties(boost::shared_ptr<const database::entities::CRule> ruleData);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CProperties();

   protected:
      // IProperties Implementation
      virtual std::string interpreterName() const;
      virtual bool isModelBased() const;
      virtual std::string scriptPath() const;
      virtual std::string logPath() const;
      virtual const shared::CDataContainer& configuration() const;
      // [END] IProperties Implementation

      //-----------------------------------------------------
      ///\brief               Build the script Path from the rule data
      ///\param[in] ruleData Rule data
      ///\return              The script path
      //-----------------------------------------------------
      std::string buildScriptPath(boost::shared_ptr<const database::entities::CRule> ruleData);
      
      //-----------------------------------------------------
      ///\brief               Build the log Path from the rule data
      ///\return              The script path
      ///\note Script path must be build before to call this method
      //-----------------------------------------------------
      std::string buildLogPath() const;

   private:
      //-----------------------------------------------------
      ///\brief               The interpreter name
      //-----------------------------------------------------
      const std::string m_interpreterName;

      //-----------------------------------------------------
      ///\brief               Model-based or locally defined flag
      //-----------------------------------------------------
      const bool m_modelBased;

      //-----------------------------------------------------
      ///\brief               The script path
      //-----------------------------------------------------
      const std::string m_scriptPath;

      //-----------------------------------------------------
      ///\brief               The log file path
      //-----------------------------------------------------
      const std::string m_logPath;

      //-----------------------------------------------------
      ///\brief               The configuration
      //-----------------------------------------------------
      const shared::CDataContainer& m_configuration;
   };

} } // namespace automation::script
	
	