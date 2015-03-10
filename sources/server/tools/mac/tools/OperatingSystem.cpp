#include "stdafx.h"
#include "OperatingSystem.h"
#include <shared/Log.h>
#include <CoreServices/CoreServices.h>
#include <Carbon/Carbon.h>
#include <Poco/Environment.h>

namespace tools {

   const std::string & COperatingSystem::getName()
   {
      return Poco::Environment::osName();
   }

   OSStatus SendAppleEventToSystemProcess(AEEventID EventToSend)
   {
      AEAddressDesc targetDesc;
      static const ProcessSerialNumber kPSNOfSystemProcess = { 0, kSystemProcess };
      AppleEvent eventReply = {typeNull, NULL};
      AppleEvent appleEventToSend = {typeNull, NULL};

      OSStatus error ;

      error = AECreateDesc(typeProcessSerialNumber, &kPSNOfSystemProcess, 
         sizeof(kPSNOfSystemProcess), &targetDesc);

      if (error != noErr)
         return(error);

      error = AECreateAppleEvent(kCoreEventClass, EventToSend, &targetDesc, 
         kAutoGenerateReturnID, kAnyTransactionID, &appleEventToSend);

      AEDisposeDesc(&targetDesc);
      if (error != noErr)
         return(error);

      error = AESend(&appleEventToSend, &eventReply, kAENoReply, 
         kAENormalPriority, kAEDefaultTimeout, NULL, NULL);

      AEDisposeDesc(&appleEventToSend);
      if (error != noErr)
         return(error);

      AEDisposeDesc(&eventReply);

      return(error); 
   }

   bool COperatingSystem::shutdown(bool andRestart)
   {
      OSStatus error;
      if (andRestart)
         error = SendAppleEventToSystemProcess(kAERestart);
      else
         error = SendAppleEventToSystemProcess(kAEShutDown);

      if (error == noErr)
      {
         if (andRestart)
            YADOMS_LOG(information) << "System will restart...";
         else
            YADOMS_LOG(information) << "System will shutdown...";

         return true;
      }
      else
      {
         if (andRestart)
            YADOMS_LOG(information) << "System fails to restart...";
         else
            YADOMS_LOG(information) << "System fails to shutdown...";

         return false;
      }
   }
   
} //namespace tools
