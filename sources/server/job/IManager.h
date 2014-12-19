#pragma once

namespace job
{
   //-----------------------------------------------------
   ///\brief The jobs manager interface
   //-----------------------------------------------------
   class IManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IManager() {}

      //-----------------------------------------------------
      ///\brief               Start all jobs
      //-----------------------------------------------------
      virtual void start() = 0;

      //-----------------------------------------------------
      ///\brief               Start all jobs
      //-----------------------------------------------------
      virtual void stop() = 0;
   };
	
} // namespace job	
	
	