#pragma once

#include "entities/Entities.h"


namespace database { 

   class IEventLoggerRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief           Add new event log entry
      /// \param [in]      eventType   the event type
      /// \param [in]      who   a free string (usually the component name)
      /// \param [in]      what   a free string (usually the error message or stacktrace)
      /// \return          enventId generated by the database
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual int addEvent(const entities::ESystemEventCode & eventType, const std::string & who, const std::string & what) = 0;
      
      //--------------------------------------------------------------
      /// \brief           Get the events history 
      /// \return          list of events
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEvents() = 0;

      //--------------------------------------------------------------
      /// \brief           Get event information
      /// \param [in]      eventId   The event id
      /// \return          Event information (may be null if not found)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CEventLogger> getEvent(const int eventId) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the last event
      /// \return          The last event (may be null)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CEventLogger> getLastEvent() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the events history from the eventId 
      /// \param [in]      eventId   The event id to filter from
      /// \return          list of events
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEventsFrom(const int eventId) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the events history in a range, from 'offset', return 'count' elements
      /// \param [in]      offset   The index (reverse, last events first) of the first row
      /// \param [in]      count    The row count
      /// \return          list of events
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEventsRange(const int offset, const int count) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IEventLoggerRequester()
      {
      }
   };
 
} //namespace database 
