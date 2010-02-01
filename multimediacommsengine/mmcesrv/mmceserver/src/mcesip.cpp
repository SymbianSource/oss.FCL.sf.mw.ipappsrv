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
* Description:  
*
*/




#include <e32math.h>
#include <siperr.h>
#include <sdpcodecerr.h>
#include <sdpdocument.h>
#include <siptoheader.h>
#include <sipdialog.h>
#include <sipcallidheader.h>
#include <sipcseqheader.h>
#include <sipretryafterheader.h>
#include <sipcontactheader.h>
#include <sipsupportedheader.h>
#include <siprequireheader.h>
#include <sipallowheader.h>
#include <sipprofile.h>
#include <siprseqheader.h>
#include <siprackheader.h>
#include "mcesip.h"
#include "mcedefs.h"


const CSIPTransactionBase::TState KMceTrxStateUndefined = 
    static_cast<CSIPTransactionBase::TState>(-1);

// -----------------------------------------------------------------------------
// MceSip::ResponseCode
// -----------------------------------------------------------------------------
//
TUint MceSip::ResponseCode( CSIPClientTransaction& aResponse )
    {
    return aResponse.ResponseElements()->StatusCode();
    }

// -----------------------------------------------------------------------------
// MceSip::ResponseType
// -----------------------------------------------------------------------------
//
TMceSipResponseType MceSip::ResponseType( CSIPClientTransaction& aResponse )
    {
    
    TUint responseCode = ResponseCode( aResponse );
    
    return ResponseType( responseCode );
    }
    
// -----------------------------------------------------------------------------
// MceSip::ResponseType
// -----------------------------------------------------------------------------
//
TMceSipResponseType MceSip::ResponseType( TUint aCode )
    {
    if ( aCode < KMceSipOK )//200
        {
        return E1XX;
        }
    else if ( aCode < KMceSipMultipleChoices )//300
        {
        return E2XX;
        }
    else if ( aCode < KMceSipBadRequest )//400
        {
        return E3XX;
        }
    else if ( aCode < KMceSipServerInternalError )//500
        {
        return E4XX;
        }
    else if ( aCode < KMceSipBusyEverywhere )//600
        {
        return E5XX;
        }
    else 
        {
        return E6XX;
        }   
    }

// -----------------------------------------------------------------------------
// MceSip::ReasonPhraseL
// -----------------------------------------------------------------------------
//
RStringF MceSip::ReasonPhraseL( TUint aStatusCode )
    {
    RStringF phrase;
    
    switch( aStatusCode )
        {
        case KMceSipOK:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseOk );
            break;
            }
        case KMceSipBadRequest:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseBadRequest );
            break;
            }
        case KMceSipUnauthorized:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseUnauthorized );
            break;
            }
        case KMceSipForbidden:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseForbidden );
            break;
            }
        case KMceSipNotFound:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseNotFound );
            break;
            }
        case KMceSipMethodNotAllowed:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseMethodNotAllowed );
            break;
            }
        case KMceSip4XXNotAcceptable:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseNotAcceptableHere );
            break;
            }
        case KMceSipProxyAuthenticationRequired:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseProxyAuthenticationRequired );
            break;
            }
        case KMceSipRequestTimeout:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseRequestTimeout );
            break;
            }
        case KMceSipUnsupportedMediaType:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseUnsupportedMediaType );
            break;
            }
        case KMceSipUnsupportedURIScheme:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseUnsupportedURIScheme );
            break;
            }
        case KMceSipBadExtension:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseBadExtension );
            break;
            }
        case KMceSipExtensionRequired:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseExtensionRequired );
            break;
            }
        case KMceSipSessionIntervalTooSmall:
        case KMceSipIntervalTooBrief:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseIntervalTooBrief );
            break;
            }
        case KMceSipTemporarilyUnavailable:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseTemporarilyNotAvailable );
            break;
            }
        case KMceSipCallOrTransactionDoesNotExist:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseCallTransactionDoesNotExist );
            break;
            }
        case KMceSipLoopDetected:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseLoopDetected );
            break;
            }
        case KMceSipTooManyHops:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseTooManyHops );
            break;
            }
        case KMceSipBusyHere:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseBusyHere );
            break;
            }
        case KMceSipRequestTerminated:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseRequestTerminated );
            break;
            }
        case KMceSipNotAcceptableHere:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseNotAcceptableHere );
            break;
            }
        case KMceSipBadEvent:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseBadEvent );
            break;
            }
        case KMceSipRequestPending:
            {
            phrase = SIPStrings::Pool().OpenFStringL( KMceSipPhraseRequestPending() );
            break;
            }
        case KMceSipServerInternalError:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseServerInternalError );
            break;
            }
        case KMceSipNotImplemented:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented );
            break;
            }        
        case KMceSipServiceUnavailable:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseServiceUnavailable );
            break;
            }
        case KMceSipVersionNotSupported:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseVersionNotSupported );
            break;
            }
        case KMceSipDecline:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseDecline );
            break;
            }
        case  KMceSipServerTimeOut:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EPhraseRequestTimeout );
            break;
            }

        default:
            {
            phrase = SIPStrings::StringF( SipStrConsts::EEmpty );
            break;
            }
        }
        
    return phrase;
    
    }


// -----------------------------------------------------------------------------
// MceSip::IsResponseTo
// -----------------------------------------------------------------------------
//
TBool MceSip::IsResponseTo( CSIPClientTransaction& aResponse, RStringF aMethod )
    {
    return &aResponse != NULL && aResponse.Type() == aMethod;
    }
    
    
// -----------------------------------------------------------------------------
// MceSip::TrxCompleted
// -----------------------------------------------------------------------------
//
TBool MceSip::TrxCompleted( CSIPTransactionBase& aTransaction )
    {
    // Invite transaction which is in confirmed state can be removed as well,
    // as it means that error response has been already sent and SIP is 
    // just waiting for acks for certain time (T4).
    CSIPTransactionBase::TState trxState = TrxState( aTransaction );
    return trxState == KMceTrxStateUndefined ||
           trxState == CSIPTransactionBase::ECompleted || 
           trxState == CSIPTransactionBase::ETerminated ||
           TrxType( aTransaction ) == SipStrConsts::EAck ||
           ( TrxType( aTransaction ) == SipStrConsts::EInvite &&
             trxState == CSIPTransactionBase::EConfirmed );
    }
    

// -----------------------------------------------------------------------------
// MceSip::TrxState
// -----------------------------------------------------------------------------
//
CSIPTransactionBase::TState MceSip::TrxState( 
                                    CSIPTransactionBase& aTransaction )
    {
    CSIPTransactionBase::TState trxState = KMceTrxStateUndefined;
    TRAP_IGNORE( ( trxState = aTransaction.StateL() ) );
    return trxState;
           
    }

// -----------------------------------------------------------------------------
// MceSip::TrxType
// -----------------------------------------------------------------------------
//
TInt MceSip::TrxType( CSIPTransactionBase& aTransaction )
    {
    return aTransaction.Type().Index( SIPStrings::Table() );
    }
    
    
    
            
// -----------------------------------------------------------------------------
// MceSip::WarningPhraseL
// -----------------------------------------------------------------------------
//
RStringF MceSip::WarningPhraseL( TUint aWarnCode )
    {
    
    RStringF warningPhrase;

    
    switch( aWarnCode )
        {
        case KMceSipWarnIncompatibleNetworkProtocol:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase300() );
            break;
            }
        case KMceSipWarnIncompatibleNetworkAddressFormat:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase301() );
            break;
            }
        case KMceSipWarnIncompatibleTransportProtocol:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase302() );
            break;
            }
        case KMceSipWarnIncompatibleBandwidthUnits:
            {
            warningPhrase =
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase303() );
            break;
            }
        case KMceSipWarnMediaTypeNotAvailable:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase304() );
            break;
            }
        case KMceSipWarnIncompatibleMediaFormat:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase305() );
            break;
            }
        case KMceSipWarnAttributeNotUnderstood:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase306() );
            break;
            }
        case KMceSipWarnSessionDescriptionParameterNotUnderstood:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase307() );
            break;
            }
        case KMceSipWarnMulticastNotAvailable:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase330() );
            break;
            }
        case KMceSipWarnUnicastNotAvailable:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase331() );
            break;
            }
        case KMceSipWarnInsufficientBandwidth:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase370() );
            break;
            }
        case KMceSipWarnMiscellaneous:
            {
            warningPhrase = 
                SIPStrings::Pool().OpenFStringL( KMceSipWarnPhrase399() );
            break;
            }
        default:
            {
            warningPhrase = SIPStrings::StringF( SipStrConsts::EEmpty );
            break;
            }
        }
    return warningPhrase;
    }



// -----------------------------------------------------------------------------
// MceSip::IsSIPStackError
// -----------------------------------------------------------------------------
//
TBool MceSip::IsSIPStackError( TInt aError )
    {
    return IsSDPError( aError ) ||
        ( aError >= KErrSIPInvalidURIType 
        && aError <= KErrSIPMalformedMessage );
    
    }

// -----------------------------------------------------------------------------
// MceSip::IsSDPError
// -----------------------------------------------------------------------------
//
TBool MceSip::IsSDPError( TInt aError )
    {
    return aError >= KErrSdpCodecDecode && aError <= KErrSdpCodecVersionField;
    
    }

// -----------------------------------------------------------------------------
// MceSip::ErrorResponse
// -----------------------------------------------------------------------------
//
void MceSip::ErrorResponse( TInt aError, 
                            TUint& aSIPResponseCode, 
                            TInt& aReasonPhrase )
    {
    
    if ( IsSDPError( aError ) )
        {
        aSIPResponseCode = KMceSipBadRequest;
        aReasonPhrase = SipStrConsts::EPhraseBadRequest;
        }
    else 
        {
        aSIPResponseCode = KMceSipServerInternalError;
        aReasonPhrase = SipStrConsts::EPhraseServerInternalError;
        }
    
    }
    

// -----------------------------------------------------------------------------
// MceSip::DiscardRequest
// -----------------------------------------------------------------------------
//
TInt MceSip::DiscardRequest( CSIPServerTransaction& aRequest, TInt aError,
                             TBool aRetryIndication  )
    {
    TInt status = KErrNone;
    TRAP( status, DoDiscardRequestL( aRequest, aError, aRetryIndication ) );
    return status;
    }

// -----------------------------------------------------------------------------
// MceSip::DiscardRequest
// -----------------------------------------------------------------------------
//
TInt MceSip::DiscardRequest( CSIPServerTransaction* aRequest, TInt aError,
                             TBool aRetryIndication  )
    {
    TInt status = DiscardRequest( *aRequest, aError, aRetryIndication );
    delete aRequest;
    
    return status;
        
    }

    
// -----------------------------------------------------------------------------
// MceSip::Method
// -----------------------------------------------------------------------------
//
RStringF MceSip::Method( CSIPServerTransaction& aRequest )
    {
    const CSIPRequestElements* request = aRequest.RequestElements();
    if ( request )
        {
        return request->Method();
        }
    else
        {
        return SIPStrings::StringF( SipStrConsts::EEmpty );
        }
        
    }
    
// -----------------------------------------------------------------------------
// MceSip::AddressToTextL
// -----------------------------------------------------------------------------
//
HBufC8* MceSip::AddressToTextL( const CSIPAddress& aToAddr )
    {
    HBufC8* address = aToAddr.ToTextLC();
    CleanupStack::Pop( address );
    return address;
    }

// -----------------------------------------------------------------------------
// MceSip::ToFromHeaderL
// -----------------------------------------------------------------------------
//
CSIPFromHeader* MceSip::ToFromHeaderL( const TDesC8& aAddress )
    {
	
	CSIPAddress* sipAddressFrom = CSIPAddress::DecodeL( aAddress );    
	CleanupStack::PushL( sipAddressFrom );
	CSIPFromHeader* fromHeader = CSIPFromHeader::NewL( sipAddressFrom );
    CleanupStack::Pop( sipAddressFrom );
    return fromHeader;
    
    }


// -----------------------------------------------------------------------------
// MceSip::ToContentTypeHeaderL
// -----------------------------------------------------------------------------
//
CSIPContentTypeHeader* 
    MceSip::ToContentTypeHeaderL( TMceContentType aContentType, 
                                  const HBufC8* aUserDefinedContentType )
    {
    
    CSIPContentTypeHeader* contentTypeHeader = NULL;
    
    if ( aUserDefinedContentType )
        {
        contentTypeHeader = 
                CSIPContentTypeHeader::DecodeL( *aUserDefinedContentType );
                
        }
    else if ( aContentType == EMceContentTypeSDP )
        {
        contentTypeHeader =
            CSIPContentTypeHeader::NewL( KMceSipMediaTypeApplication,
                                         KMceSipMediaSubTypeSDP );
    
        }
    else if ( aContentType == EMceContentTypeMultiPart )
        {
    	contentTypeHeader = 
    	    CSIPContentTypeHeader::NewLC( KMceSipMPartMediaTypeMultipart, 
    	                                  KMceSipMPartMediaSubTypeMixed );

    	RStringF paramKey = SIPStrings::Pool().OpenFStringL( KMceSipMPartBoundary );
    	CleanupClosePushL( paramKey );
    	RStringF paramValue = SIPStrings::Pool().OpenFStringL( KMceSipMPartMessageBoundary );
    	CleanupClosePushL( paramValue );

    	contentTypeHeader->SetParamL( paramKey, paramValue );
    	CleanupStack::PopAndDestroy(); // paramValue
    	CleanupStack::PopAndDestroy(); // paramKey
        CleanupStack::Pop( contentTypeHeader );
        
        }
    else
        {
        //NOP
        }
    
    return contentTypeHeader;
    
    }

    
// -----------------------------------------------------------------------------
// MceSip::ToSIPExtensionHeadersL
// -----------------------------------------------------------------------------
//
void MceSip::ToSIPExtensionHeadersL( RPointerArray<CSIPHeaderBase>& aSIPHeaders, 
                                     const MDesC8Array& aHeaders )
    {
    
	for ( int i = 0; i < aHeaders.MdcaCount(); i++ )
		{
		TPtrC8 param = aHeaders.MdcaPoint( i );
		TInt index = param.Locate( KMceSipSeparator );
		if ( index != KErrNotFound && 
					param.Left( index ) != KMceSipSubscriptionStateHeader )
			{
    		CSIPExtensionHeader* extHeader = CSIPExtensionHeader::NewL( 
    		                param.Left(index), 
    			            param.Right( param.Length() - ( index + 1 ) ) );
    		CleanupStack::PushL( extHeader );
    		User::LeaveIfError( aSIPHeaders.Append( extHeader ) );
    		CleanupStack::Pop( extHeader );
			}
		}
    }

// -----------------------------------------------------------------------------
// MceSip::UserHeadersToTextArrayL
// -----------------------------------------------------------------------------
//
CDesC8Array* MceSip::UserHeadersToTextArrayL( 
                                    CSIPTransactionBase& aTransaction,
                                    const CSIPDialog& aDialog )
    {
	CDesC8ArrayFlat* headers = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( headers );

	HBufC8* head = NULL;
    const CSIPHeaderBase* callid = NULL;
    const CSIPHeaderBase* cseq = NULL;
    const CSIPFromHeader* from = NULL;
    const CSIPToHeader* to = NULL;
    const CSIPMessageElements* elements = ToMessageElements( aTransaction );

    if ( aTransaction.IsSIPClientTransaction() )
        {
        const CSIPResponseElements* responseElements =
            static_cast<CSIPClientTransaction&>( aTransaction ).
                ResponseElements();
        cseq = responseElements ? responseElements->CSeqHeader() : NULL;

        from = responseElements ? responseElements->FromHeader() : NULL;
        to = responseElements ? responseElements->ToHeader() : NULL;

        }
    else
        {
        
        const CSIPRequestElements* requestElements =
            static_cast<CSIPServerTransaction&>( aTransaction ).
                RequestElements();
        from = requestElements ? requestElements->FromHeader() : NULL;
        to = requestElements ? requestElements->ToHeader() : NULL;
        
        }

    from = !from ? &aDialog.FromHeader() : from;
    to = !to ? &aDialog.ToHeader() : to;
    
    head = from->ToTextLC();
	headers->AppendL( *head );
	CleanupStack::PopAndDestroy( head );
    head = to->ToTextLC();
	headers->AppendL( *head );
	CleanupStack::PopAndDestroy( head );

    if ( cseq )
        {
	    head = cseq->ToTextLC();
		headers->AppendL( *head );
		CleanupStack::PopAndDestroy( head );
        }

    TRAPD( error, callid = &aDialog.CallIdL() );
    if ( error == KErrNone )
        {
        head = callid->ToTextLC();
    	headers->AppendL( *head );
    	CleanupStack::PopAndDestroy( head );
        }
        

	if ( elements )
		{
		const RPointerArray<CSIPHeaderBase>& sipHeaders = 
		    elements->UserHeaders();
		
		for ( TInt i=0; i < sipHeaders.Count(); i++ )
			{
			CSIPHeaderBase* anotherhead = sipHeaders[ i ];
			head = anotherhead->ToTextLC();
			headers->AppendL( *head );
			CleanupStack::PopAndDestroy( head );
		    }
        
            
		}
		
	CleanupStack::Pop( headers );
	return headers;
	
    }
    
// -----------------------------------------------------------------------------
// MceSip::ToContactHeaderL
// -----------------------------------------------------------------------------
//
CSIPContactHeader* MceSip::ToContactHeaderL( const TDesC8& aContact )
    {
    CSIPContactHeader* contact = NULL;
    
    RPointerArray< CSIPContactHeader > contacts = CSIPContactHeader::DecodeL( aContact );
    if ( contacts.Count() > 0 )
        {
        contact = contacts[0]; 
        contacts.Remove(0);
        contacts.ResetAndDestroy();
        }
    else
        {
        contacts.Close();
        User::Leave( KErrNotFound );
        }
        
    return contact;
    
    }


// -----------------------------------------------------------------------------
// MceSip::HasSipsUriInContact
// -----------------------------------------------------------------------------
//
TBool MceSip::HasSipsUriInContactL( const CSIPMessageElements& aMessage )
	{
	TBool hasSipsUri = EFalse;
	CSIPHeaderBase* contact = FindHeader( aMessage, SIPStrings::StringF( 
											SipStrConsts::EContactHeader ) );
                                    
    if ( !contact )
    	{
    	contact = FindHeader( aMessage, SIPStrings::StringF( 
											SipStrConsts::EContactHeaderCompact ) );
    	}
    
    if ( contact )
    	{
		const TDesC8& uriScheme = static_cast< CSIPContactHeader* >( contact )->
									SIPAddress()->Uri8().Uri().Extract( EUriScheme );
		if ( uriScheme.FindF( KMceSipUriSchemeSIPs ) != KErrNotFound )
			{
			hasSipsUri = ETrue;
			}
		}

	return hasSipsUri;		
	}

// -----------------------------------------------------------------------------
// MceSip::HasTlsInContact
// -----------------------------------------------------------------------------
//
TBool MceSip::HasTlsInContactL( const CSIPMessageElements& aMessage )
	{
	TBool hasTls = EFalse;
	CSIPHeaderBase* contact = FindHeader( aMessage, SIPStrings::StringF( 
											SipStrConsts::EContactHeader ) );
                                    
    if ( !contact )
    	{
    	contact = FindHeader( aMessage, SIPStrings::StringF( 
											SipStrConsts::EContactHeaderCompact ) );
    	}
    
    if ( contact )
    	{
	
		const TDesC8& uriPath = static_cast< CSIPContactHeader* >( contact )->SIPAddress()->Uri8().Uri().Extract( EUriPath );
		if ( uriPath.FindF( KMceSipTransportTLS ) != KErrNotFound )
			{
			hasTls = ETrue;
			}
		}
		
	return hasTls;
	}
// -----------------------------------------------------------------------------
// MceSip::AddSupportedHeaderL
// -----------------------------------------------------------------------------
//    
void MceSip::AddSupportedHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                  const TDesC8& aExtension )
    {
    RStringF tag = SIPStrings::Pool().OpenFStringL( aExtension );
    CleanupClosePushL( tag );
    CSIPSupportedHeader* header = CSIPSupportedHeader::NewL( tag );
    CleanupStack::PopAndDestroy(); // tag
    CleanupStack::PushL( header );
    aSIPHeaders.AppendL( header );
    CleanupStack::Pop( header );
    }

// -----------------------------------------------------------------------------
// MceSip::AddRequireHeaderL
// -----------------------------------------------------------------------------
//    
void MceSip::AddRequireHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                const TDesC8& aExtension )
    {
    RStringF tag = SIPStrings::Pool().OpenFStringL( aExtension );
    CleanupClosePushL( tag );
    CSIPRequireHeader* header = CSIPRequireHeader::NewL( tag );
    CleanupStack::PopAndDestroy(); // tag
    CleanupStack::PushL( header );
    aSIPHeaders.AppendL( header );
    CleanupStack::Pop( header );
    }

// -----------------------------------------------------------------------------
// MceSip::AddAllowHeaderL
// -----------------------------------------------------------------------------
//     
void MceSip::AddAllowHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                              TInt aMethod )
    {
    RStringF method = SIPStrings::StringF( aMethod );
    CSIPAllowHeader* header = CSIPAllowHeader::NewL( method );
    CleanupStack::PushL( header );
    aSIPHeaders.AppendL( header );
    CleanupStack::Pop( header );
    }

// -----------------------------------------------------------------------------
// MceSip::AddRAckHeaderL
// -----------------------------------------------------------------------------
//     
void MceSip::AddRAckHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                             const CSIPResponseElements& aResponse )
    {
    TInt rseq = RSeq( aResponse );
     
    if (rseq == KErrNotFound )
        {
           User::Leave( KErrArgument );
        }
       
    const CSIPCSeqHeader* cseqHeader = aResponse.CSeqHeader();
    __ASSERT_ALWAYS( MCE_NOT_NULL_PTR( cseqHeader ), User::Leave( KErrArgument ) );
     
    CSIPRAckHeader* header = CSIPRAckHeader::NewL( rseq, 
                                                   cseqHeader->Seq(),
                                                   cseqHeader->Method() );
    CleanupStack::PushL( header );
    aSIPHeaders.AppendL( header );
    CleanupStack::Pop( header );
    }
    
    
// -----------------------------------------------------------------------------
// MceSip::ToMessageElements
// -----------------------------------------------------------------------------
//
const CSIPMessageElements* MceSip::ToMessageElements( 
    CSIPTransactionBase& aTransaction )
    {
    
    const CSIPMessageElements* elements = NULL;
                    
    if ( aTransaction.IsSIPClientTransaction() )
        {
        CSIPClientTransaction& trx = 
            static_cast<CSIPClientTransaction&>( aTransaction );
        const CSIPResponseElements* resElements = trx.ResponseElements();

        elements = resElements ? &resElements->MessageElements() : NULL;

        }
    else
        {
        CSIPServerTransaction& trx = 
            static_cast<CSIPServerTransaction&>( aTransaction );
        const CSIPRequestElements* reqElements = trx.RequestElements();
        
        elements = reqElements ? &reqElements->MessageElements() : NULL;
        }

    return elements;
    
    }

// -----------------------------------------------------------------------------
// MceSip::HasContent
// -----------------------------------------------------------------------------
//
TBool MceSip::HasContent( CSIPTransactionBase& aTransaction )
    {
    const CSIPMessageElements* elements = ToMessageElements( aTransaction );
    if (elements)
    	return elements->Content().Length() > 0;
    else return EFalse;
    }
/*    
TBool MceSip::HasContent( CSIPTransactionBase& aTransaction )
    {
    TBool hasContent = EFalse;
    const CSIPMessageElements* elements = ToMessageElements( aTransaction );
    if ( elements && elements->Content().Length() > 0 )
        {
        hasContent = ETrue;
        }
    return hasContent;
    }*/
    
// -----------------------------------------------------------------------------
// MceSip::SIPStringIndex
// -----------------------------------------------------------------------------
//    
TInt MceSip::SIPStringIndex( const TDesC8& aSIPString )
    {
    TInt searchInd = KErrNotFound;
    const TStringTable& strTable = SIPStrings::Table();
    
    TUint strInd = 0;// index in table
    
    while ( searchInd == KErrNotFound && strInd < strTable.iCount )
        {
        if ( SIPStrings::StringF( strInd ).DesC().CompareF( aSIPString ) == 0 )
            {
            searchInd = strInd;
            }
        else
            {
            strInd++;
            }
        } 
    return searchInd;      
    }
    
// -----------------------------------------------------------------------------
// MceSip::DoDiscardRequestL
// -----------------------------------------------------------------------------
//
void MceSip::DoDiscardRequestL( CSIPServerTransaction& aRequest, TInt aError,
                                TBool aRetryIndication )
    {
    CSIPResponseElements* response = NULL;
    TUint responseCode = 0;
    
    if ( aError >= (TInt)KMceSipTrying )
        {
        responseCode = (TUint)aError;
        RStringF reasonPhrase = ReasonPhraseL( responseCode );
        CleanupClosePushL( reasonPhrase );
        response = 
            CSIPResponseElements::NewL( responseCode, reasonPhrase );
                                        
        CleanupStack::PopAndDestroy();//reasonPhrase
        }
    else
        {
        TInt reasonPhrase = 0;
           
        ErrorResponse( aError, responseCode, reasonPhrase );
    
        response = 
            CSIPResponseElements::NewL( responseCode, 
                                        SIPStrings::StringF( reasonPhrase ) );
        }
        
    CleanupStack::PushL( response );

    if ( aRetryIndication )
        {
        
        CSIPMessageElements& elements = response->MessageElements();
        
        //create Retry-After header
        RPointerArray<CSIPHeaderBase> headers;
        CSIPHeaderBase::PushLC( &headers );

        CSIPRetryAfterHeader* retryAfter = 
            CSIPRetryAfterHeader::NewLC( Random( KMceRandMinRetryValue,
                                                 KMceRandMaxRetryValue ) );
        headers.AppendL( retryAfter );
        CleanupStack::Pop( retryAfter );
        //send invitation
        elements.SetUserHeadersL( headers );
        CleanupStack::Pop( &headers );
        
        }
    
    aRequest.SendResponseL( response );
    CleanupStack::Pop( response );
        
    
    }

// -----------------------------------------------------------------------------
// MceSip::FindHeader
// -----------------------------------------------------------------------------
// 
CSIPHeaderBase* MceSip::FindHeader( const CSIPMessageElements& aMessage,
                                    RStringF aHeaderName )
    {
    TBool found = EFalse;
    CSIPHeaderBase* header = NULL;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    
    for ( TInt i=0; i < headers.Count() && !found; i++ )
        {
        header = headers[i];
        if ( header->Name() == aHeaderName )
            {
            found = ETrue;
            }
        else
            {
            header = NULL;
            }
        }
    return header;
    }

// -----------------------------------------------------------------------------
// MceSip::FindHeader
// -----------------------------------------------------------------------------
// 
CSIPHeaderBase* MceSip::FindHeader( const RPointerArray<CSIPHeaderBase>& aHeaders ,
                                    RStringF aHeaderName )
    {
    TBool found = EFalse;
    CSIPHeaderBase* header = NULL;
    
    for ( TInt i=0; i < aHeaders.Count() && !found; i++ )
        {
        header = aHeaders[i];
        if ( header->Name() == aHeaderName )
            {
            found = ETrue;
            }
        else
            {
            header = NULL;
            }
        }
    return header;
    }
// -----------------------------------------------------------------------------
// MceSip::FindHeadersL
// -----------------------------------------------------------------------------
// 
void MceSip::FindHeadersL( const CSIPMessageElements& aMessage,
                           RStringF aHeaderName,
                           RPointerArray<CSIPHeaderBase>& aHeaders )
    {
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    CSIPHeaderBase* header = NULL;
    
    for ( TInt i=0; i < headers.Count(); i++ )
        {
        header = headers[i];
        if ( header->Name() == aHeaderName )
            {
            aHeaders.AppendL( header );
            }
        }
    }

// -----------------------------------------------------------------------------
// MceSip::CompareUri
// -----------------------------------------------------------------------------
// 
TInt MceSip::CompareUri( const CUri8& aRemoteUriOne, const CUri8& aRemoteUriTwo )
    {  
    TInt ret = KErrGeneral;
    
    if ( aRemoteUriOne.Uri().Compare( aRemoteUriTwo.Uri(), EUriScheme ) == KErrNone &&
         aRemoteUriOne.Uri().Compare( aRemoteUriTwo.Uri(), EUriUserinfo ) == KErrNone &&
         aRemoteUriOne.Uri().Compare( aRemoteUriTwo.Uri(), EUriHost ) == KErrNone  )
        {
        ret = KErrNone;
        }
        
    return ret;
    }

// -----------------------------------------------------------------------------
// MceSip::HasHeaderValueL
// -----------------------------------------------------------------------------
// 
TBool MceSip::HasHeaderValueL( const CSIPMessageElements& aMessage,
                              TInt aHeaderName,
                              const TDesC8& aHeaderValue )
    {
    TBool found = EFalse;
    CSIPHeaderBase* header = NULL;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    
    for ( TInt i=0; i < headers.Count() && !found; i++ )
        {
        header = headers[i];
        if ( header->Name() == SIPStrings::StringF( aHeaderName ) )
            {
            HBufC8* value = header->ToTextValueL();
            if ( value->CompareF( aHeaderValue ) == 0 )
                {
                found = ETrue;
                }
            delete value;
            }
            
        if ( !found )
            {
            header = NULL;
            }
        }
    return found;
    }
    
// -----------------------------------------------------------------------------
// MceSip::HasHeaderValueL
// -----------------------------------------------------------------------------
// 
TBool MceSip::HasHeaderValueL( const RPointerArray<CSIPHeaderBase>& aHeaders,
                              TInt aHeaderName,
                              const TDesC8& aHeaderValue )
    {
    TBool found = EFalse;
    CSIPHeaderBase* header = NULL;
    
    for ( TInt i=0; i < aHeaders.Count() && !found; i++ )
        {
        header = aHeaders[i];
        if ( header->Name() == SIPStrings::StringF( aHeaderName ) )
            {
            HBufC8* value = header->ToTextValueL();
            if ( value->CompareF( aHeaderValue ) == 0 )
                {
                found = ETrue;
                }
            delete value;
            }
            
        if ( !found )
            {
            header = NULL;
            }
        }
    return found;
    }    
// MceSip::Random
// -----------------------------------------------------------------------------
// 
TUint MceSip::Random( TUint aMinValue, TUint aMaxValue )
    {
    TUint randomValue( aMinValue <= aMaxValue ? aMinValue : 0 );
    
    if ( aMinValue <= aMaxValue )
        {
        
        TTime time;
        time.HomeTime();
        TInt64 seed( time.Int64() );

        
        for ( TUint i = 0; i < ( aMaxValue - aMinValue ); i++ )
            {
            TInt random = Math::Rand( seed );
            TReal random2 =  ( TReal )random / KMceRandDividerOne;
            TUint random3 = ( TUint )( aMaxValue * random2 ) /
            KMceRandDividerTwo;

            if ( aMinValue <= random3 && aMaxValue >= random3 )
                {
                randomValue = random3;
                break;
                }
            }
        }
        
    return randomValue;
    
    }

    
// -----------------------------------------------------------------------------
// MceSip::HeaderValueL
// -----------------------------------------------------------------------------
// 
TUint MceSip::HeaderValueL( TPtr8 aHeader )
    {
	TUint value;
	aHeader.Trim();
	
	TInt index = aHeader.Locate( KMceSipHeaderSeparator );
	if( index != KErrNotFound )
		{
		TLex8 lexValue( aHeader.Left( index ) );
		User::LeaveIfError ( lexValue.Val( value, EDecimal ) );
		}
	else
		{
		TLex8 lexValue( aHeader );
		User::LeaveIfError ( lexValue.Val( value, EDecimal ) );
		}
		
    return value;
    
    }



// -----------------------------------------------------------------------------
// MceSip::RSeq     
// -----------------------------------------------------------------------------
// 

TInt MceSip::RSeq( const CSIPResponseElements& aResponseElements )
    {
        
    TInt value = KErrNotFound; // if its not found
    
    CSIPHeaderBase* rseqHeader = FindHeader( aResponseElements.MessageElements(),
                            SIPStrings::StringF( SipStrConsts::ERSeqHeader ) );
                                    
    if ( rseqHeader )
        {
        value = static_cast< CSIPRSeqHeader* >( rseqHeader )->Value(); 
        }
           
    return value;    
    }
    
 // -----------------------------------------------------------------------------
// MceSip::BasicAllowMethods
// -----------------------------------------------------------------------------
//    
void MceSip::BasicAllowMethodsL( RArray<TInt>& aArray )
	{
	User::LeaveIfError( aArray.Append( SipStrConsts::EInvite ));
	User::LeaveIfError( aArray.Append( SipStrConsts::EAck ));
	User::LeaveIfError( aArray.Append( SipStrConsts::ECancel ));
	User::LeaveIfError( aArray.Append( SipStrConsts::EOptions ));
	User::LeaveIfError( aArray.Append( SipStrConsts::EBye ));
	}
	    

//  End of File
