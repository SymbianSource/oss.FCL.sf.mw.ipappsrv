/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    Server side SIP event class
*
*/





#ifndef CMCECOMEVENT_H
#define CMCECOMEVENT_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "mcerefer.h"
#include "mceeventcontext.h"
#include "mcesipevent.h"
#include "mceclientserver.h"

// FORWARD DECLARATIONS
class CMceEventContext;

// CLASS DECLARATION

/**
*  Server side SIP event base class.
*
*  @lib 
*  @since
*/
class CMceComEvent : public CBase
                            
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMceComEvent* NewL( CMceSipEvent& aSIPEvent,
                                   TUint32 aRefreshInterval );
 
        /**
        * Destructor.
        */
        virtual ~CMceComEvent();

    public: // New functions
       
        /**
        * Passes an Subscribe action initialized by MCE Client to the event 
        * state machine
        * @since Series 60 3.0
        * @param aEventAction The initialized action as ITC function.
        * @return None
        */
        void PerformActionL( TMceItcFunctions aEventAction );

        
        /**
        * Notifies the subscribe event, that the ongoing client or server 
        * transaction  proceeds. The transaction is already 
        * stored in the event.It is for passing incoming response 
        * and sip error occured
        * @param None
        * @return None
        */
        void ProceedL();
        

        /**
        * Passes the server transaction to the event.
        * @param aServerTransaction The server transaction
        * @param aDialog the dialog belong to the transaction
        * @return None
        */
        void ReceivedRequestL();
        
        /**
        * Returns the event context
        * @param None
        * @return CMCESipEventContext& Reference to the event context
        */
        CMceEventContext& EventContext();
      
        /**
        * Returns the previous event action.
        * @param None
        * @return The action as ITC function
        */
        TInt PreviousAction() const;
  
        /**
        * Set the previous event action.
        * @param aEventAction The action as ITC function.
        * @return None
        */
        void SetPreviousAction( TInt aEventAction );
        
        /**
        * Returns the refresh interval.It is using for SubscribeEvent
        * @param None
        * @return The refresh interval and expire value for the subscribe event
        */
        TUint32 RefreshInterval();
        
        /**
        * Sets the refresh interval. Applicable for MO subscribe.
        * @param aInterval The refresh interval
        * @return None
        */
        void SetRefreshInterval( TUint32 aInterval );
        

        /**
        * set the Msg header array from client, ownership is transfered
        * @param aMsgHeaders the list of Msg headers from client
        * @return none
        */
        void SetMsgHeaders( CDesC8Array* aMsgHeaders);
        
        /**
        * Saves the content from client
        * @since Series 60 3.0
        * @param aMsgContent. Contains the content taken from client
        * @return None
        */
        void SetMsgContentL(HBufC8* aContentType, 
        					HBufC8* aContent );
       
        /**
        * Returns the content from client
        * @param None
        * @return iMsgContent. Contains the content taken from client. 
        *                      Ownership is transfered.
        */
        HBufC8* MsgContent();
        
        /**
        * Returns the content from client
        * @param None
        * @return iMsgContent. Contains the content taken from client
        */
        TDesC8& MsgContentType();
        
        /**
        * Returns the message headers from client. If not set in this class,
        * returns the default headers from c/s session by the given method.
        * @param aMethodInd the SIP string index for the method 
        *                   headers are asked for.
        * @return CDesC8Array& the headers;
        */
        CDesC8Array& MsgHeaders( TInt aMethodInd );
        
        /**
        * Saves the response reasonphrase from client
        * @since Series 60 3.0
        * @param aReasonPhrase. Contains the ReasonPhrase taken from client
        * @return None
        */
        void SetReasonPhraseL( TUint32 aStatusCode,
        					   HBufC8* aReasonPhrase );

        /**
        * Returns the Reason parameters
        * @param None
        * @return iReason
        */
        TDesC8& ReasonPhrase();
        
        /**
        * Returns the statuscode
        * @param None
        * @return iStatusCode
        */
        TUint32 StatusCode() const;
        
        /*
        * Sets the type, applicable only for REFER.
        * Possible types:
        *   ENoSuppression : The subscription is established.
        *   ESuppressed : The subscription is proposed not to
        *                 be established. 
        *   EMultipleRefer : The subscription is required not 
        *                    to be established.
        * @param aType The type.
        * @return None
        */
        void SetReferType( CMceRefer::TType aType );
        
        /*
        * Returns truth value is the suppression on or off.
        * @return TBool The suppression value
        */
        CMceRefer::TType ReferType() const;
        
        /**
        * Sets the silen suppression on. Used for REFER, when the
        * client application requested for suppression,
        * but the remote end did not understand it.
        */
        
        void SetSilentSuppressionOn();
        
        /**
        * Returns the value of silent suppression.
        * @return TBool silent suppression
        */
        TBool SilentSuppression() const;
        
        /**
        * Sets the id value, used for matching NOTIFY:s
        * @param aIdValue the id value, ownership is taken
        */
        void SetIdValue( TUint aIdValue );
        
        /**
        * Returns the id value used for matcing NOTIFY:s
        * @return the id value
        */
        TUint IdValue() const;
       
    public: 
    	CMceSipEvent& SIPEvent();

    protected:  // New functions
        
        
        /**
        * Creates the event context
        * @param None
        * @return None
        */
        void CreateEventContextL();

    protected:  // Functions from base classes
        
         /**
        * C++ default constructor.
        */
        CMceComEvent( CMceSipEvent& aSIPEvent,
                      TUint32 aRefreshInterval );

 
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    private:  // Data
        
        // Message headers
        CDesC8Array* iMsgHeaders;
        // The event observer.
        CMceSipEvent& iSIPEvent;
        // The event context, holds the state machine
        CMceEventContext* iEventContext;
   
        HBufC8* iMsgContent;
        
        HBufC8* iMsgContentType;
        
        HBufC8* iReason;
        
        TUint32 iStatusCode;
        
        // The previous event action indicated by the MCE Client
        TInt iPreviousAction;

        // Event refresh interval
        TUint32  iRefreshInterval;
        
        CMceRefer::TType iReferType;
        
        TBool iSilentSuppression;
        
        TUint iIdValue;
        
     private: //friend class
     	friend class UT_CMceComEvent; 
 		friend class UT_CMceEventReferMtEstablishedState;
    };

#endif      //CMCECOMEVENT_H
            
// End of File
