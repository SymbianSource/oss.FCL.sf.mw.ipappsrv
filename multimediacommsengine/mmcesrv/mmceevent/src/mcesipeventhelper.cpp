/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Makes the contents of SIP messages using SIP API
*
*/




// INCLUDE FILES
#include <sipeventheader.h>
#include <siprefresh.h>
#include <sipsubscribedialogassoc.h>
#include <sipexpiresheader.h>
#include <sipacceptheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipsubscriptionstateheader.h>
#include <sipmessageelements.h>
#include <sipcontenttypeheader.h>
#include <sipextensionheader.h>
#include <sipresponseelements.h>
#include <sipsupportedheader.h>
#include <siprequireheader.h>
#include <sipeventheader.h>
#include "mcesipeventhelper.h"
#include "mcesipevent.h"
#include "mcecomevent.h"
#include "mcecomeventconstants.h"
#include "mcesipconnection.h"
#include "mcesip.h"

// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateMessageElementsLC
// -----------------------------------------------------------------------------
//    
CSIPMessageElements* 
    MCESIPEventHelper::CreateMessageElementsLC( CMceComEvent& aEvent,
                                                TInt aRequestType,
											    CMceRefer::TType aReferType )
    { 
    CSIPMessageElements* message = CSIPMessageElements::NewLC();
    
	//Headers from client
	RPointerArray< CSIPHeaderBase > sipHdrs;
	CSIPHeaderBase::PushLC( &sipHdrs );
	
    MceSip::ToSIPExtensionHeadersL( sipHdrs, aEvent.MsgHeaders( aRequestType ) );
	
	// Add extension specific headers:
	switch( aReferType )
	    {
	    case CMceRefer::ESuppressed:
	        {
	        // Refer-Sub: false
	        CSIPExtensionHeader* exHdr = 
	            CSIPExtensionHeader::NewLC( KMceSipHeaderReferSub, KMceSipValueFalse );
	        sipHdrs.AppendL( exHdr );
	        CleanupStack::Pop( exHdr );
	        
	        // Supported: norefersub
	        RStringF val = SIPStrings::Pool().OpenFStringL( KMceSipValueNoReferSub );
	        CleanupClosePushL( val );
	        CSIPSupportedHeader*  hdr = CSIPSupportedHeader::NewLC( val );
	        sipHdrs.AppendL( hdr );
	        CleanupStack::Pop( hdr );
	        CleanupStack::PopAndDestroy(); // val
	        break;
	        }
	        
	    case CMceRefer::EMultipleRefer:
	        {
	        // Require: multiple-refer, norefersub
	        // Create two headers, one with each parameter.
	        // SIP stack knows how to encode the header.
	        RStringF valMult = 
	            SIPStrings::Pool().OpenFStringL( KMceSipValueMultipleRefer );
	        CleanupClosePushL( valMult );
	        CSIPRequireHeader* hdrMult = CSIPRequireHeader::NewLC( valMult );
	        sipHdrs.AppendL( hdrMult );
	        CleanupStack::Pop( hdrMult );
	        CleanupStack::PopAndDestroy(); // valMult  
	        
	        RStringF valNoSub = 
	            SIPStrings::Pool().OpenFStringL( KMceSipValueNoReferSub );
	        CleanupClosePushL( valNoSub );
	        CSIPRequireHeader* hdrNoSub = CSIPRequireHeader::NewLC( valNoSub );
	        sipHdrs.AppendL( hdrNoSub );
	        CleanupStack::Pop( hdrNoSub );
	        CleanupStack::PopAndDestroy(); // valNoSub
	        
	        break;
	        }
	        
	    default:
	        {
	        // add nothing
	        break;
	        }
	    }
	    
	message->SetUserHeadersL( sipHdrs );
    CleanupStack::Pop( &sipHdrs );
    
    SetContentL( *message, aEvent );
    return message;
    }


// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateMessageElementsLC
// -----------------------------------------------------------------------------
//    
CSIPMessageElements* 
    MCESIPEventHelper::CreateMessageElementsLC( CMceComEvent& aEvent,
                                                TInt aRequestType,
											    TUint aExpiresValue   )
    {
    
    CSIPMessageElements* message = CSIPMessageElements::NewLC();
	
	//Headers from client
	RPointerArray< CSIPHeaderBase > sipHdrs;
	CSIPHeaderBase::PushLC( &sipHdrs );
    
	MceSip::ToSIPExtensionHeadersL( sipHdrs, aEvent.MsgHeaders( aRequestType ) );
	 
    // Expires
	CSIPExpiresHeader* expires = 
	    new ( ELeave ) CSIPExpiresHeader( aExpiresValue );
	CleanupStack::PushL( expires );
	sipHdrs.AppendL( expires );
	CleanupStack::Pop( expires );
	message->SetUserHeadersL( sipHdrs );
	CleanupStack::Pop( &sipHdrs );
	
	SetContentL( *message, aEvent );
    return message;
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateMessageElementsLC
// -----------------------------------------------------------------------------
//    
CSIPMessageElements* 
MCESIPEventHelper::CreateMessageElementsLC( CMceComEvent& aEvent, 
                                            TInt aRequestType )
    {    
    CSIPMessageElements* message = CSIPMessageElements::NewLC();
	
	//Headers from client
	RPointerArray< CSIPHeaderBase > sipHdrs;
	CSIPHeaderBase::PushLC( &sipHdrs );
    
    MceSip::ToSIPExtensionHeadersL( sipHdrs, aEvent.MsgHeaders( aRequestType ) );
	message->SetUserHeadersL( sipHdrs );
	CleanupStack::Pop( &sipHdrs );
	
	SetContentL( *message, aEvent );
	
	return message;
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateResponseL
// -----------------------------------------------------------------------------
//    
CSIPResponseElements* 
    MCESIPEventHelper::CreateResponseL( CMceComEvent& aEvent, 
                                        TUint aReason,
                                        TBool aAddClientData )
    {
    RStringF phrase = MceSip::ReasonPhraseL( aReason );
    
    return CreateResponseL( aEvent,  
                            aReason,
                            phrase,
                            aAddClientData );    
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateResponseL
// -----------------------------------------------------------------------------
//    
CSIPResponseElements* 
    MCESIPEventHelper::CreateResponseL( CMceComEvent& aEvent,
                                        TUint aReason,
                                        RStringF aPhrase,
                                        TBool aAddClientData )
    {    
    CSIPResponseElements* response = CSIPResponseElements::NewLC( aReason,
                                                                  aPhrase );
	
	//Headers from client
	RPointerArray< CSIPHeaderBase > sipHdrs;
	CSIPHeaderBase::PushLC( &sipHdrs );
   
    // add Refer-Sub header for suppressed refer
    if ( aReason >= KMceSipOK 
      && aReason < KMceSipMultipleChoices 
      && aEvent.ReferType() == CMceRefer::ESuppressed )
        {
        // Add Refer-Sub header when responding 2xx to suppressed REFER
        CSIPExtensionHeader* extHeader = 
            CSIPExtensionHeader::NewLC( KMceSipHeaderReferSub, KMceSipValueFalse );
        sipHdrs.AppendL( extHeader );
        CleanupStack::Pop( extHeader );
        }
   	
    CSIPMessageElements& message = response->MessageElements();
     
    if ( aAddClientData )
        {
        MceSip::ToSIPExtensionHeadersL( sipHdrs, 
                                        aEvent.MsgHeaders( KErrNotFound ) );
	    SetContentL( message, aEvent );    
        }
        
	message.SetUserHeadersL( sipHdrs );
    CleanupStack::Pop( &sipHdrs );
	
	CleanupStack::Pop( response );
	
	return response;
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::SetContentL
// -----------------------------------------------------------------------------
//    
void MCESIPEventHelper::SetContentL( CSIPMessageElements& aMsgElem,
                                     CMceComEvent& aEvent )
    {
    if( aEvent.MsgContentType().Length() )
    	{
    	CSIPContentTypeHeader* contenttype = 
    			CSIPContentTypeHeader::DecodeL( aEvent.MsgContentType() );
    	CleanupStack::PushL( contenttype );
    	aMsgElem.SetContentL( aEvent.MsgContent(), contenttype );
    	CleanupStack::Pop( contenttype );
    	}
    else
    	{
    	delete aEvent.MsgContent();
    	}
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::CreateReferNotifyMessageElementsLC
// -----------------------------------------------------------------------------
//    

CSIPMessageElements* 
    MCESIPEventHelper::CreateReferNotifyMessageElementsLC( 
                                                   CMceComEvent& aEvent )
        {
        CSIPMessageElements* message = CSIPMessageElements::NewLC();
        
        //Headers from client
	    RPointerArray< CSIPHeaderBase > sipHdrs;
	    CSIPHeaderBase::PushLC( &sipHdrs );
    
	    MceSip::ToSIPExtensionHeadersL( 
	                            sipHdrs, 
	                            aEvent.MsgHeaders( SipStrConsts::ENotify ) );
		message->SetUserHeadersL( sipHdrs );
	    CleanupStack::Pop( &sipHdrs );
	
        // content type
        CSIPContentTypeHeader* contentType = 
            CSIPContentTypeHeader::NewLC( KMceSipReferAcceptType, 
                                          KMceSipReferAcceptSubtype );
        RStringF version = SIPStrings::Pool().OpenFStringL( KMceSipVersion );
        CleanupClosePushL( version );
        contentType->SetParamL( version, 
            SIPStrings::StringF( SipStrConsts::EDefaultVersionNumber ) );
        
        //content                       
        HBufC8* content = KMceSipNotifyContent().AllocLC();
        
        message->SetContentL( content, contentType );
        
        // clean
        CleanupStack::Pop( content );
        CleanupStack::PopAndDestroy();// version
        CleanupStack::Pop( contentType );
        
        return message;
        }
  
// -----------------------------------------------------------------------------
// MCESIPEventHelper::HandleExpireHeaderL
// For Subscribe
// -----------------------------------------------------------------------------
void MCESIPEventHelper::HandleExpireHeaderL(
    								const CSIPMessageElements& aMessage,
    								TBool& aTerminated)
    {
    TBool terminated = EFalse;
    
    CSIPHeaderBase* header = MceSip::FindHeader( 
                                aMessage, 
                                SIPStrings::StringF( SipStrConsts::EExpires ) );
            
    User::LeaveIfError( header ? KErrNone : KErrNotFound );
            
    CSIPExpiresHeader* expireHeader = 
        static_cast<CSIPExpiresHeader*>(header);

	TUint expires = expireHeader->Value();
 	if (expires > 0)
        {
        terminated = EFalse;
        }
    else
        {
        terminated = ETrue;
        }
        
    aTerminated = terminated;
    }
 
// -----------------------------------------------------------------------------
// MCESIPEventHelper::HandleSubscriptionStateHeaderL
// Refer 
// -----------------------------------------------------------------------------
void MCESIPEventHelper::HandleSubscriptionStateHeaderL(
    								const CSIPMessageElements& aMessage,
    								TBool& aTerminated)
    {
    CSIPHeaderBase* header = MceSip::FindHeader( aMessage,
            SIPStrings::StringF( SipStrConsts::ESubscriptionStateHeader ) );
            
    User::LeaveIfError( header ? KErrNone : KErrNotFound );
            
    CSIPSubscriptionStateHeader* ssHeader = 
        static_cast<CSIPSubscriptionStateHeader*>(header);

        
    TPtrC8 state( ssHeader->SubStateValue() );
    
    if (state.CompareF(KMceSipStateActive) == 0 || 
        state.CompareF(KMceSipStatePending) == 0)
        {
        aTerminated = EFalse;
        }
    else if (state.CompareF(KMceSipStateTerminated) == 0)
        {
        aTerminated = ETrue;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::SetSubsStateHeaderValueAndParamsL
// 
// -----------------------------------------------------------------------------
void MCESIPEventHelper::SetSubsStateHeaderValueAndParamsL ( 
									CMceComEvent& aEvent,
									TInt aRequestType,
									CSIPSubscriptionStateHeader& aSubStateHeader,
									const TDesC8& aSubStateValue,
									TUint32 aExpireParam,
									TBool aTerminating )
	{
	TInt index = 0;

	if ( !HasSubscriptionStateHeader( aEvent, aRequestType, index ) )
		{
		aSubStateHeader.SetSubStateValueL( aSubStateValue );
        aSubStateHeader.SetExpiresParameterL( aExpireParam );
        // delete expires parameter
        if ( aTerminating ) 
        	{
        	RStringF expires = SIPStrings::StringF( SipStrConsts::EExpires );
        	aSubStateHeader.DeleteParam( expires );
        	}
        }
	else
		{
		// delete expires parameter and set by client if any
		RStringF expires = SIPStrings::StringF( SipStrConsts::EExpires );
        aSubStateHeader.DeleteParam( expires );
		SetSSHeaderValueAndParamL( 	aEvent.MsgHeaders( aRequestType ),
									index,
									aSubStateHeader );
	
		}
	
	}

// -----------------------------------------------------------------------------
// MCESIPEventHelper::SetSSHeaderValueAndParamL
// 
// -----------------------------------------------------------------------------
void MCESIPEventHelper::SetSSHeaderValueAndParamL( 	
											const MDesC8Array& aMsgHeader,
										   	TUint aHeaderIndex,
											CSIPSubscriptionStateHeader& aSubStateHeader )
	{
	TPtrC8 param  = aMsgHeader.MdcaPoint( aHeaderIndex );
	TInt index = param.Locate( KMceSipSeparator );
	User::LeaveIfError( index != KErrNotFound ? KErrNone : KErrArgument );

	CSIPSubscriptionStateHeader* ssHeader = 
			CSIPSubscriptionStateHeader::DecodeL( param.Right( param.Length() - ( index + 1 ) ) );
	CleanupStack::PushL( ssHeader );
	
	User::LeaveIfError( ssHeader->SubStateValue().Length() ? KErrNone : KErrArgument );
	
	aSubStateHeader.SetSubStateValueL( ssHeader->SubStateValue() );
	
	if ( ssHeader->ExpiresParameter() != KErrNotFound )
		{
		aSubStateHeader.SetExpiresParameterL( ssHeader->ExpiresParameter());
		}
	if ( ssHeader->RetryAfterParameter() != KErrNotFound )
		{
		aSubStateHeader.SetRetryAfterParameterL( ssHeader->RetryAfterParameter() );
		}
	
	RStringF reason = SIPStrings::StringF( SipStrConsts::EReason );	
	
	if ( ssHeader->HasParam( reason ) )
		{
		aSubStateHeader.SetParamL( reason, ssHeader->ParamValue( reason ) );
		}
	
	CleanupStack::PopAndDestroy( ssHeader );
	}
	
// -----------------------------------------------------------------------------
// MCESIPEventHelper::HasSubscriptionStateHeader
// 
// -----------------------------------------------------------------------------
TBool MCESIPEventHelper::HasSubscriptionStateHeader(
									CMceComEvent& aEvent,
									TInt aRequestType,
									TInt aIndex )
    {
    TBool hasHeader = EFalse;
    
    TUint count = aEvent.MsgHeaders( aRequestType ).MdcaCount();
    
   	for ( TUint i = 0; i < count; i++ )
		{
		TPtrC8 param = aEvent.MsgHeaders( aRequestType ).MdcaPoint( i );
		aIndex = param.FindF( KMceSipSubscriptionStateHeader );
		if ( aIndex != KErrNotFound )
			{
			hasHeader = ETrue;
			}
		}
	return hasHeader;
    }
    
// -----------------------------------------------------------------------------
// MCESIPEventHelper::NotifyRequestReceivedLC
// -----------------------------------------------------------------------------
// 
CSIPResponseElements* MCESIPEventHelper::NotifyRequestReceivedLC(
                                        CMceComEvent& aEvent,
                                        const CSIPMessageElements& aMessage )
    {
    CSIPResponseElements* response = NULL;
    if ( !SubscriptionStateOK( aMessage ) )
        {
        response = CreateResponseL( aEvent, KMceSipBadRequest );
        }
    else if( !EventOK( aMessage ) )
        {
        response = CreateResponseL( aEvent, KMceSipBadEvent );
        }
    else
        {
        response = CreateResponseL( aEvent, KMceSipOK );
        }
    CleanupStack::PushL( response );
    return response;
	}

// -----------------------------------------------------------------------------
// MCESIPEventHelper::ReferNotifyRequestReceivedLC
// -----------------------------------------------------------------------------
// 
CSIPResponseElements* MCESIPEventHelper::ReferNotifyRequestReceivedLC(
                                         CMceComEvent& aEvent,
                                         const CSIPMessageElements& aMessage )
    {
    CSIPResponseElements* response = NULL;
    if ( !( ReferContentTypeOK( aMessage ) && 
            SubscriptionStateOK( aMessage ) ) )
        {
        response = CreateResponseL( aEvent, KMceSipBadRequest );
        }
    else if( !ReferEventOK( aMessage ) )
        {
        response = CreateResponseL( aEvent, KMceSipBadEvent );
        }
    else
        {
        response = CreateResponseL( aEvent, KMceSipOK );
        }
    CleanupStack::PushL( response );
    return response;
	}
        
// -----------------------------------------------------------------------------
// MCESIPEventHelper::SubscribeRequestReceivedLC
// -----------------------------------------------------------------------------
// 
CSIPResponseElements* MCESIPEventHelper::SubscribeRequestReceivedLC(
                                    CMceComEvent& aEvent,
                                    const CSIPMessageElements& aMessage )
    {
    //no subscription-state checking only in Notify msg
    CSIPResponseElements* response = NULL;
    if ( !EventOK( aMessage ) )
        {
        response = CreateResponseL( aEvent, KMceSipBadEvent );
        }
    else
        {
        response = CreateResponseL( aEvent, KMceSipOK );    
        }
    CleanupStack::PushL( response );
    return response;
	}	

// -----------------------------------------------------------------------------
// MCESIPEventHelper::ReferContentTypeOK
// -----------------------------------------------------------------------------
//    
TBool MCESIPEventHelper::ReferContentTypeOK(
    const CSIPMessageElements& aMessage )
    {
    const CSIPContentTypeHeader* contentType = 
        aMessage.ContentType();
    if (!contentType)
        {
        return ETrue;//if there is no content in the notify msg
        }
    if (contentType->MediaType().CompareF(KMceSipReferAcceptType) != 0)
        {
        return EFalse;
        }
    return ( contentType->MediaSubtype().
        CompareF( KMceSipReferAcceptSubtype ) == 0);
    }
    
// -----------------------------------------------------------------------------
// MCESIPEventHelper::SubscriptionStateOK
// -----------------------------------------------------------------------------
//    
TBool MCESIPEventHelper::SubscriptionStateOK(
    const CSIPMessageElements& aMessage )
    {
    CSIPHeaderBase* header = MceSip::FindHeader( aMessage,
        SIPStrings::StringF( SipStrConsts::ESubscriptionStateHeader ) );
    return MCE_NOT_NULL_PTR( header );
    }    

// -----------------------------------------------------------------------------
// MCESIPEventHelper::EventOK
// -----------------------------------------------------------------------------
//    
TBool MCESIPEventHelper::EventOK( const CSIPMessageElements& aMessage )
    {
    CSIPHeaderBase* header = MceSip::FindHeader( aMessage,
        SIPStrings::StringF( SipStrConsts::EEventHeader ) );
    return MCE_NOT_NULL_PTR( header );
    }  

// -----------------------------------------------------------------------------
// MCESIPEventHelper::ReferEventOK
// -----------------------------------------------------------------------------
//    
TBool MCESIPEventHelper::ReferEventOK(
    const CSIPMessageElements& aMessage )
    {
    CSIPHeaderBase* header = MceSip::FindHeader( aMessage,
        SIPStrings::StringF( SipStrConsts::EEventHeader ) );
    if (!header)
        {
        return EFalse;
        }
    CSIPEventHeader* event = static_cast<CSIPEventHeader*>(header);
    return (event->EventPackage().CompareF(KMceSipReferEventName) == 0);
    }  

// -----------------------------------------------------------------------------
// MCESIPEventHelper::ResolveReferTypeL
// -----------------------------------------------------------------------------
//        
CMceRefer::TType MCESIPEventHelper::ResolveReferTypeL( 
                                        const CSIPMessageElements& aMessage )
    {
    /**
    * IF required "multiple-refer" THEN return EMultipleRefer
    *
    * ELSE IF there is "Refer-Sub: false" then return ESuppressed
    *
    * ELSE return ENoSuppression
    */
    
    CMceRefer::TType ret = CMceRefer::ENoSuppression;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    CSIPHeaderBase* header = NULL;
    
    for ( TInt ind = 0; 
          ret != CMceRefer::EMultipleRefer && ind < headers.Count() ; ind++ )
        {
        header = headers[ ind ]; 
        
        if ( header->Name() 
             == SIPStrings::StringF( SipStrConsts::ERequireHeader ) )
            {
            RStringF multiple = 
                SIPStrings::Pool().OpenFStringL( KMceSipValueMultipleRefer );
            CleanupClosePushL( multiple );
            
            if ( static_cast< CSIPTokenHeaderBase* >( header )->Value() 
                 == multiple )
                {
                // multiple-refer extension required.
                ret = CMceRefer::EMultipleRefer;
                }
            CleanupStack::PopAndDestroy(); // multiple
            }
        else
            {
            RStringF referSub = 
                SIPStrings::Pool().OpenFStringL( KMceSipHeaderReferSub );
            CleanupClosePushL( referSub );
            
            if ( header->Name() == referSub )
                {
                if ( static_cast< CSIPExtensionHeader* >( header )->
                    Value().CompareF( KMceSipValueFalse ) == 0 )
                    {
                    ret = CMceRefer::ESuppressed;
                    }
                }
            CleanupStack::PopAndDestroy(); // referSub
            }
        }
    
    return ret;
    }

// -----------------------------------------------------------------------------
// MCESIPEventHelper::MatchEvents
// -----------------------------------------------------------------------------
// 
TBool MCESIPEventHelper::MatchEvents( const CSIPEventHeader& aFirstEvent,
                                      const CSIPEventHeader& aSecondEvent )
    {
    TBool ret( EFalse );
    
    if ( aFirstEvent.EventPackage().CompareF( 
         aSecondEvent.EventPackage() ) == 0 )
        {
        // event packages match, concentrate on id params
        RStringF idParam = SIPStrings::StringF( SipStrConsts::EId );
        if ( aFirstEvent.HasParam( idParam ) )
            {
            // id's must match
            if ( aSecondEvent.HasParam( idParam ) )
                {
                if ( aFirstEvent.ParamValue( idParam ) == 
                     aSecondEvent.ParamValue( idParam ) )
                    {
                    ret = ETrue;
                    }
                }
            }
        else if ( aSecondEvent.HasParam( idParam ) )
            {
            // first has no, second has
            }
        else
            {
            // neither has
            ret = ETrue;
            }  
        }
    return ret;            
    }
    
// -----------------------------------------------------------------------------
// MCESIPEventHelper::MatchEvents
// -----------------------------------------------------------------------------
//     
TBool MCESIPEventHelper::MatchEventsL( const TDesC8& aEventPackage,
                                      TUint aIdValue,
                                      const CSIPEventHeader& aEvent )
    {
    TBool ret( EFalse );
    
    if ( aEvent.EventPackage().CompareF( aEventPackage ) == 0 )
        {
        // event packages match, concentrate on id params
        RStringF idParam = SIPStrings::StringF( SipStrConsts::EId );
        if ( aEvent.HasParam( idParam ) )
            {
            if ( aIdValue > KMceEventIdNotAssigned )
                {
                HBufC8* idParamBuf = 
                    aEvent.ParamValue( idParam ).DesC().AllocLC();
                TUint idParamValue = MceSip::HeaderValueL( idParamBuf->Des() );
                CleanupStack::PopAndDestroy( idParamBuf );
                if ( idParamValue == aIdValue )
                    {
                    ret = ETrue;
                    }
                }
            }
         else // no id param
            {
            ret = ETrue;
            }
        }
    return ret;                   
    }

//  End of File  
