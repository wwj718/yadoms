#pragma once

//--------------------------------------------------------------
/// \brief	The sequence number provider interface
//--------------------------------------------------------------
class ISequenceNumberProvider
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ISequenceNumberProvider() {}

   //--------------------------------------------------------------
   /// \brief     Reset the sequence number
   //--------------------------------------------------------------
   virtual void reset() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number (increment counter and returns new value)
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual unsigned char next() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the last sequence number (returns current value)
   /// \return    Current sequence number
   //--------------------------------------------------------------
   virtual unsigned char last() const = 0;
};