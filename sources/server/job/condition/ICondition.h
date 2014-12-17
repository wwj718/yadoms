#pragma once

namespace job { namespace condition
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
      virtual bool eval() const = 0;
   };
	
} } // namespace job::condition
	
	