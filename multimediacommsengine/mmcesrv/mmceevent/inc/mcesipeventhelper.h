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
* Description:    Makes the contents of SIP messages using SIP API.
*
*/





#ifndef MCESIPEVENTHELPER_H
#define MCESIPEVENTHELPER_H

//INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipsubscriptionstateheader.h>
#include "mcerefer.h"

// CONSTANTS
const TUint KMceEventIdNotAssigned = 0;
// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CSIPMessageElements;
class CSIPSubscribeDialogAssoc; 
class CSIPResponseElements;
class CSIPHeaderBase; 
class CSIPEventHeader;
class CMceSipEvent;
class CMceComEvent;  

// CLASS DECLARATION
/**
*  Makes the contents of SIP messages using SIP API
*
*  @lib 
*  @since
*/
class MCESIPEventHelper
    {

    public: // New functions
    
        /**
        * Creates a request message elements. Leaves in error cases.
        * this is for new request-subscribe, subscribeupdate.
        * Adds user defined headers, body and content type if present and
        * expires header.
        * @param aEvent the com event 
        * @param aRequestType the request type as SIP string
        * @param aExpiresValue the expiration value
        * @return CSIPMessageElements*, pointer to the created request message. 
        * The ownership is transferred.
        */
        static CSIPMessageElements* CreateMessageElementsLC(
                                                CMceComEvent& aEvent,
                                                TInt aRequestType,
											    TUint aExpiresValue );
        
        /**
        * Creates a request message elements. Leaves in error cases.
        * this is for notify request msg.
        * @param aEvent the com event 
        * @param aHeaders the user headers
        * @return CSIPMessageElements*, pointer to the created request message. 
        * The ownership is transferred.
        */
        static CSIPMessageElements* CreateMessageElementsLC(
                                                CMceComEvent& aEvent,
                                                TInt aRequestType );
        
        /**
        * Creates a request message elements. Leaves in error cases.
        * Used for REFER request. Adds user defined headers, body and 
        * content type if present, and support for subscription suppression.
        * according to the givent
        * @param aEvent the com event 
        * @param aRequestType the request type as SIP string
        * @param aReferType the REFER type
        * @return CSIPMessageElements*, pointer to the created request message. 
        * The ownership is transferred.
        */
        static CSIPMessageElements* CreateMessageElementsLC(
                                                CMceComEvent& aEvent,
                                                TInt aRequestType,
                                                CMceRefer::TType aReferType );
        
        
        /**
        * Creates a message elements for first Notify to REFER.
        * @param aEvent the com event
        * @return CSIPMessageElements*, pointer to the created message elements. 
        * The ownership is transferred.
        */    
        static CSIPMessageElements* CreateReferNotifyMessageElementsLC( 
                                                CMceComEvent& aEvent );
        
        /**
        * Creates response elements. The reason phrase is derived
        * from the reason code.
        * @param aEvent the com event
        * @param aReason the reason code as SIP string
        * @param aAddClientData ETrue, if there's client data to be  added
        */
        static CSIPResponseElements* CreateResponseL( 
                                            CMceComEvent& aEvent, 
                                            TUint aReason,
                                            TBool aAddClientData = EFalse );

        /**
        * Creates response elements. Arbitrary reason phrase and code.
        * @param aEvent the com event
        * @param aReason the reason code,
        * @param aPhrase reason phrase
        * @param aAddClientData ETrue, if there's client data to be  added
        */
        static CSIPResponseElements* CreateResponseL( 
                                            CMceComEvent& aEvent,
                                            TUint aReason,
                                            RStringF aPhrase,
                                            TBool aAddClientData = EFalse );
                                                    
        /**
        * Handle Expire Header
        * @param aSubscribeAssoc, the SubscribeDialogAssociation
        * @param aMessage, message elements
        * @param aTerminated, True if the expire value==0 
        * @return NULL
        */
       	static void HandleExpireHeaderL( const CSIPMessageElements& aMessage,
    							         TBool& aTerminated );
    	/**
        * Handle SubscriptionState Header
        * @param aMessage, message elements
        * @return NULL
        */    	
    	static void HandleSubscriptionStateHeaderL(
    							    const CSIPMessageElements& aMessage,
    								TBool& aTerminated);
    	

    	/**
        * Creates a response to given request
        * @param aEvent the com event
        * @param aMessage, a request message which response is created from.
        * @return CSIPResponseElements*, pointer to the created request message.
        * The ownership is transferred.  
        */
        static CSIPResponseElements* NotifyRequestReceivedLC(
                                    CMceComEvent& aEvent,
                                    const CSIPMessageElements& aMessage );
        
        /**
        * Creates a response to given refer request
        * @param aEvent the com event
        * @param aMessage, a request message which response is created from.
        * @return CSIPResponseElements*, pointer to the created request message.
        * The ownership is transferred.  
        */
        static CSIPResponseElements* ReferNotifyRequestReceivedLC(
                                        CMceComEvent& aEvent,
        								const CSIPMessageElements& aMessage );
        
        /**
        * Creates a response to given request
        * @param aEvent the com event
        * @param aMessage, a request message which response is created from.
        * @return CSIPResponseElements*, pointer to the created request message.
        * The ownership is transferred.  
        */
        static CSIPResponseElements* SubscribeRequestReceivedLC(
                                        CMceComEvent& aEvent,
                                        const CSIPMessageElements& aMessage );
        
		/**
		* Resolves the REFER type from message elements.
		* @param aMessage, a message to be resolved.
		* @return CMceRefer::TType, the REFER type.
		*/
		static CMceRefer::TType ResolveReferTypeL( const CSIPMessageElements& aMessage );
		
		/**
        * Matches the given Event headers. They match, if the event
        * packages are same and either id params are the same or
        * none of them has id param.
        * @param aFirstEvent first event to be compared
        * @param aSecondEvent secnd event to be compared
        * @return ETrue if events match
        */                                
        static TBool MatchEvents( const CSIPEventHeader& aFirstEvent,
                                  const CSIPEventHeader& aSecondEvent );

        /**
        * Matches the given event package and id to the given 
        * Event header. This is a match, if the event packages
        * are the same and the id param is either not present in the
        * Event header or matches the given value, which in this case
        * must be greater than 0.
        * @param aEvent the Event header
        * @param aEventPackage the event package
        * @param aIdValue the value of id param.
        */
        static TBool MatchEventsL( const TDesC8& aEventPackage,
                                  TUint aIdValue,
                                  const CSIPEventHeader& aEvent );
        
       
		
		/**
        * Set Subscription header State value and parameters
        * if there is MsgHeader set by client then the value will be
        * set as client specified
        * @param aEvent the Event header
        * @param aRequestType type of request
        * @param aSubStateHeader the header will be set to
        * @param aSubStateValue subscription state default value ex termiante
        * @param aExpireParam default expire value
        * @param aTerminating if it is setting to terminating state 
        */									
		static void SetSubsStateHeaderValueAndParamsL( 
									CMceComEvent& aEvent,
									TInt aRequestType,
									CSIPSubscriptionStateHeader& aSubStateHeader,
									const TDesC8& aSubStateValue,
									TUint32 aExpireParam,
									TBool aTerminating = ETrue ); 										                       
    		
    private: // New functions
        
        /**
        * To Check if the request ContentType is valid
        * @param aMessage, the request message to be checked
        * @return Etrue or EFalse  
        */
		static TBool ReferContentTypeOK( const CSIPMessageElements& aMessage );
		
        /**
        * To Check if the request SubscriptionState header is valid
        * @param aMessage, the request message to be checked
        * @return Etrue or EFalse 
        */
        static TBool SubscriptionStateOK( const CSIPMessageElements& aMessage );
        
        /**
        * To Check if the request Event header is valid
        * @param aMessage, the request message to be checked
        * @return Etrue or EFalse 
        */
        static TBool EventOK( const CSIPMessageElements& aMessage );
        
        /**
        * To Check if the request Event header is valid
        * @param aMessage, the request message to be checked
        * @return Etrue or EFalse 
        */
        static TBool ReferEventOK( const CSIPMessageElements& aMessage );
        
        /**
        * Creates a request message elements. Adds content type and content
        * if present.
        * @param aEvent the com event containing the data
        * @return CSIPMessageElements*, pointer to the created request message. 
        * The ownership is transferred.
        */
        static void SetContentL( CSIPMessageElements& aMsgElem,
                                 CMceComEvent& aEvent );	
        
        
        /**
        * sets a subscription-state header from aMsgHeader
        * function leaves if the subscription header in MsgHeader is malformate
        * @param aMsgHeader headers set by client
        * @param aHeaderIndex the index where subscription state located, 
        * @param aSubStateHeader subscription state header which will be set to
        */
        static void SetSSHeaderValueAndParamL( 
        									const MDesC8Array& aMsgHeader,
										   	TUint aHeaderIndex,
											CSIPSubscriptionStateHeader& aSubStateHeader );
        
   
   		/**
        * Check if there is subscription state header from 
        * ComEvent MsgHeader
        * @param aEvent the Event header
        * @param aRequestType requestType
        * @return ETrue or EFalse
        */
        static TBool HasSubscriptionStateHeader( CMceComEvent& aEvent,
											TInt aRequestType,
											TInt aIndex  );   
    private: // EUNIT
        
        friend class UT_MCESIPEventHelper;
        };

#endif    // MCESIPEVENTHELPER_H

// End of File
