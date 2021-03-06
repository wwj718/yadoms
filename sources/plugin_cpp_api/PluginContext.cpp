#include "stdafx.h"
#include "PluginContext.h"
#include "ApiImplementation.h"
#include "CommandLine.h"
#include <shared/currentTime/Local.h>
#include <Poco/Debugger.h>


namespace yApi = shared::plugin::yPluginApi;

namespace plugin_cpp_api
{
   CPluginContext::CPluginContext(int argc,
                                  char** argv,
                                  boost::shared_ptr<IPlugin> plugin)
      : m_commandLine(boost::make_shared<CCommandLine>(argc, argv)),
        m_plugin(plugin),
        m_returnCode(kOk)
   {
      shared::currentTime::Provider().setProvider(boost::make_shared<shared::currentTime::Local>());
   }

   CPluginContext::~CPluginContext()
   {
   }

   void CPluginContext::run()
   {
      auto api = boost::make_shared<CApiImplementation>();

      try
      {
         openMessageQueues();

         api->setSendingMessageQueue(m_sendMessageQueue);

         m_msgReceiverThread = boost::thread(&CPluginContext::msgReceiverThreaded, this, api);

         api->waitInitialized();

         std::cout << api->getInformation()->getType() << " started" << std::endl;

         waitDebugger(api);
         
         if (!api->stopRequested())
         {
            // Execute plugin code
            m_plugin->doWork(api);
         }

         if (!api->stopRequested())
         {
            std::cerr << api->getInformation()->getType() << " has stopped itself." << std::endl;
            m_returnCode = kUnexpectedStop;
         }

         // Normal stop
         std::cout << api->getInformation()->getType() << " is stopped" << std::endl;
         m_returnCode = kOk;
      }
      catch (std::exception& e)
      {
         std::cerr << api->getInformation()->getType() << " crashed with exception : " << e.what();
         m_returnCode = kRuntimeError;
      }
      catch (...)
      {
         std::cerr << api->getInformation()->getType() << " crashed with unknown exception" << std::endl;
         m_returnCode = kRuntimeError;
      }

      m_msgReceiverThread.interrupt();
      m_msgReceiverThread.timed_join(boost::posix_time::seconds(20));

      closeMessageQueues();
   }

   void CPluginContext::waitDebugger(boost::shared_ptr<CApiImplementation> api) const
   {
      if (boost::filesystem::exists(api->getInformation()->getPath() / "waitForDebuggerAtStart"))
      {
         std::cout << "***********************************************" << std::endl;
         std::cout << " Wait for a debugger to attach current process " << std::endl;
         std::cout << "***********************************************" << std::endl;

         // Check every 300 ms, while 2 minutes
         const auto endTimePoint = shared::currentTime::Provider().now() + boost::posix_time::minutes(2);
         while (!Poco::Debugger::isAvailable() && shared::currentTime::Provider().now() < endTimePoint)
         {
            if (api->getEventHandler().waitForEvents(boost::posix_time::millisec(300)) == yApi::IYPluginApi::kEventStopRequested)
            {
               std::cout << "Stop requested" << std::endl;
               api->setPluginState(yApi::historization::EPluginState::kStopped);
               return;
            }
         }

         if (Poco::Debugger::isAvailable())
            std::cout << api->getInformation()->getType() << " attached to debugger" << std::endl;
         else
            std::cout << api->getInformation()->getType() << " failed to attach debugger after timeout" << std::endl;
      }
   }

   IPluginContext::EProcessReturnCode CPluginContext::getReturnCode() const
   {
      return m_returnCode;
   }

   void CPluginContext::openMessageQueues() const
   {
      // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

      try
      {
         const auto sendMessageQueueId(m_commandLine->yPluginApiAccessorId() + ".toYadoms");
         const auto receiveMessageQueueId(m_commandLine->yPluginApiAccessorId() + ".toPlugin");

         std::cout << "Opening message queues id " << m_commandLine->yPluginApiAccessorId() << std::endl;

         m_sendMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str());
         m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, receiveMessageQueueId.c_str());
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::runtime_error(std::string("CPluginContext::msgReceiverThreaded : Error opening message queue, ") + ex.what());
      }
      catch (...)
      {
         throw std::runtime_error("CPluginContext::msgReceiverThreaded : Unknown error");
      }
   }

   void CPluginContext::closeMessageQueues()
   {
      std::cout << "Close message queues" << std::endl;

      // Delete all global objects allocated by libprotobuf.
      google::protobuf::ShutdownProtobufLibrary();
   }

   void CPluginContext::msgReceiverThreaded(boost::shared_ptr<CApiImplementation> api) const
   {
      auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue->get_max_msg_size()));
      size_t messageSize;
      unsigned int messagePriority;

      try
      {
         while (true)
         {
            try
            {
               // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
               // polling and call boost::this_thread::interruption_point to exit properly
               // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
               auto messageWasReceived = m_receiveMessageQueue->timed_receive(message.get(),
                                                                              m_receiveMessageQueue->get_max_msg_size(),
                                                                              messageSize,
                                                                              messagePriority,
                                                                              boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1));
               boost::this_thread::interruption_point();

               if (messageWasReceived)
                  api->onReceive(message, messageSize);
            }
            catch (shared::exception::CInvalidParameter& ex)
            {
               std::cerr << "Error receiving/processing queue message : " << ex.what() << std::endl;
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
   }

} // namespace plugin_cpp_api
