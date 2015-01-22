#pragma once
#include "../INotificationObserverForRulesManager.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The condition interface
   //-----------------------------------------------------
   class ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ICondition() {}

      //-----------------------------------------------------
      ///\brief               Evaluate the condition
      ///\return              Return the evaluation result
      //-----------------------------------------------------
      virtual bool eval() = 0;

      //-----------------------------------------------------
      ///\brief               Register to notification center
      ///\param[in] notificationObserver  The notification observer
      ///\param[in] conditionRootNotifier The condition root notifier
      //-----------------------------------------------------
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier) = 0;

      //-----------------------------------------------------
      ///\brief               Unregister to notification center
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver) = 0;
   };
	
} } // namespace automation::condition
	
	