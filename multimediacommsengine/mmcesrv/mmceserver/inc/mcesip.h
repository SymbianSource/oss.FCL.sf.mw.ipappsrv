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




#ifndef MCESIP_H
#define MCESIP_H


#include <e32std.h>
#include <bamdesca.h>
#include <badesca.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>
#include <siprequestelements.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipaddress.h>
#include <sipfromheader.h>
#include <sipextensionheader.h>
#include <siperr.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <uri8.h>
#include <sipcontenttypeheader.h>
#include "mceserial.h"
#include "mcesipextensions.h"

//  FORWARD DECLARATION

class CSdpDocument;
class CSIPDialog;
class CSIPContactHeader;

// CLASS DECLARATION


enum TMceSipResponseType
    {
    E1XX,
    E2XX,
    E3XX,
    E4XX,
    E5XX,
    E6XX
    };

//E1XX,
const TUint KMceSipTrying = 100;
const TUint KMceSipRinging = 180;
const TUint KMceSipCallIsBeingForwarded = 181;
const TUint KMceSipQueued = 182;
const TUint KMceSipSessionProgress = 183;

//E2XX,
const TUint KMceSipOK = 200;
const TUint KMceSipAccepted = 202;

//E3XX,
const TUint KMceSipMultipleChoices = 300;
const TUint KMceSipMovedPermanently = 301;
const TUint KMceSipMovedTemporarily = 302;
const TUint KMceSipUseProxy = 305;
const TUint KMceSipAlternativeService = 380;

//E4XX,
const TInt KMceSipBadRequest = 400;
const TUint KMceSipUnauthorized = 401;
const TUint KMceSipPaymentRequired = 402;
const TUint KMceSipForbidden = 403;
const TUint KMceSipNotFound = 404;
const TUint KMceSipMethodNotAllowed = 405;
const TUint KMceSip4XXNotAcceptable = 406;
const TUint KMceSipProxyAuthenticationRequired = 407;
const TUint KMceSipRequestTimeout = 408;
const TUint KMceSipGone = 410;
const TUint KMceSipequestEntityTooLarge = 413;
const TUint KMceSipRequestURITooLong = 414;
const TUint KMceSipUnsupportedMediaType = 415;
const TUint KMceSipUnsupportedURIScheme = 416;
const TUint KMceSipBadExtension = 420;
const TUint KMceSipExtensionRequired = 421;
const TUint KMceSipSessionIntervalTooSmall = 422;
const TUint KMceSipIntervalTooBrief = 423;
const TUint KMceSipTemporarilyUnavailable = 480;
const TUint KMceSipCallOrTransactionDoesNotExist = 481;
const TUint KMceSipLoopDetected = 482;
const TUint KMceSipTooManyHops = 483;
const TUint KMceSipAddressIncomplete = 484;
const TUint KMceSipAmbiguous = 485;
const TUint KMceSipBusyHere = 486;
const TUint KMceSipRequestTerminated = 487;
const TUint KMceSipNotAcceptableHere = 488;
const TUint KMceSipBadEvent = 489;
const TUint KMceSipRequestPending = 491;
const TUint KMceSipUndecipherable = 493;

const TUint KMceSipRefreshInterval = 60;


//E5XX,
const TUint KMceSipServerInternalError = 500;
const TUint KMceSipNotImplemented = 501;
const TUint KMceSipBadGateway = 502;
const TUint KMceSipServiceUnavailable = 503;
const TUint KMceSipServerTimeOut = 504;
const TUint KMceSipVersionNotSupported = 505;
const TUint KMceSipMessageTooLarge = 513;
//580 Precondition Failure: 
const TInt KMceSipPreconditionFailure = 580;

//E6XX,
const TUint KMceSipBusyEverywhere = 600;
const TUint KMceSipDecline = 603;
const TUint KMceSipDoesNotExistAnywhere = 604;
const TUint KMceSip6XXNotAcceptable = 606;



typedef TUint TMceSipWarningCode;

//300 Incompatible network protocol: One or more network protocols
//contained in the session description are not available. 
const TMceSipWarningCode KMceSipWarnIncompatibleNetworkProtocol = 300;

//301 Incompatible network address formats: One or more network
//address formats contained in the session description are not
//available.
const TMceSipWarningCode KMceSipWarnIncompatibleNetworkAddressFormat = 301;
 
//302 Incompatible transport protocol: One or more transport
//protocols described in the session description are not available.
const TMceSipWarningCode KMceSipWarnIncompatibleTransportProtocol = 302;
 
//303 Incompatible bandwidth units: One or more bandwidth
//measurement units contained in the session description were not understood.
const TMceSipWarningCode KMceSipWarnIncompatibleBandwidthUnits = 303;
 
//304 Media type not available: One or more media types 
//contained in the session description are not available.
const TMceSipWarningCode KMceSipWarnMediaTypeNotAvailable = 304;
 
//305 Incompatible media format: One or more media 
//formats contained in the session description are not available.
const TMceSipWarningCode KMceSipWarnIncompatibleMediaFormat = 305;
 
//306 Attribute not understood: One or more of the media 
//attributes in the session description are not supported.
const TMceSipWarningCode KMceSipWarnAttributeNotUnderstood = 306;
 
//307 Session description parameter not understood: A parameter
//other than those listed above was not understood.
const TMceSipWarningCode KMceSipWarnSessionDescriptionParameterNotUnderstood = 307;
 
//330 Multicast not available: The site where the user is located
//does not support multicast.
const TMceSipWarningCode KMceSipWarnMulticastNotAvailable = 330;
 
//331 Unicast not available: The site where the user is located does
//not support unicast communication (usually due to the presence of a firewall).
const TMceSipWarningCode KMceSipWarnUnicastNotAvailable = 331;

//370 Insufficient bandwidth: The bandwidth specified in the session
//description or defined by the media exceeds that known to be available.
const TMceSipWarningCode KMceSipWarnInsufficientBandwidth = 370;

//399 Miscellaneous warning: The warning text can include arbitrary
//information to be presented to a human user or logged.  A system receiving 
//this warning MUST NOT take any automated action.
const TMceSipWarningCode KMceSipWarnMiscellaneous = 399;

const TChar KMceSipSeparator = ':';
const TChar KMceSipHeaderSeparator = ';';
const TChar KMceSipEqualsTo = '=';
const TChar KMceSipAt = '@';
const TChar KMceSipBlank = ' ';
const TChar KMceSipDash = '-';

_LIT8( KMceSipNullAddress, "0.0.0.0" );
_LIT8( KMceSipNullAddressA, "c=IN IP4 0.0.0.0\n" );
_LIT8( KMceSipPhraseRequestPending, "Request Pending");
_LIT8( KMceSipPhraseAccepted, "Accepted" );

_LIT8( KMceSipWarnPhrase300, "Incompatible network protocol");
_LIT8( KMceSipWarnPhrase301, "Incompatible network address formats");
_LIT8( KMceSipWarnPhrase302, "Incompatible transport protocol");
_LIT8( KMceSipWarnPhrase303, "Incompatible bandwidth units");
_LIT8( KMceSipWarnPhrase304, "Media type not available");
_LIT8( KMceSipWarnPhrase305, "Incompatible media format");
_LIT8( KMceSipWarnPhrase306, "Attribute not understood");
_LIT8( KMceSipWarnPhrase307, "Session description parameter not understood");
_LIT8( KMceSipWarnPhrase330, "Multicast not available");
_LIT8( KMceSipWarnPhrase331, "Unicast not available");
_LIT8( KMceSipWarnPhrase370, "Insufficient bandwidth");
_LIT8( KMceSipWarnPhrase399, "Miscellaneous warning");
_LIT8( KMceSipErrPhrase580, "Precondition Failure");

_LIT8( KMceSipWarningPattern, "%d %S %S");
_LIT8( KMceSipContentTypePattern, "%S/%S");
_LIT8( KMceSipContactURIPattern, "sip:%S@localhost" );
_LIT8( KMceSipSdpSegPrecCurrentStatusPattern, "qos %S %S" );
_LIT8( KMceSipSdpSegPrecDesiredStatusPattern, "qos %S %S %S" );


_LIT8( KMceSipSdpSecPrecCurrentStatusPattern, "sec %S %S" );
_LIT8( KMceSipSdpSecPrecDesiredStatusPattern, "sec %S %S %S" );


_LIT8( KMceSipWarning, "Warning" );
_LIT8( KMceSipTimer, "timer" );
_LIT8( KMceSipRefresher, "refresher" );
_LIT8( KMceSipRefresherUAC, "uac" );
_LIT8( KMceSipRefresherUAS, "uas" );
_LIT8( KMceSipSdpQos, "qos" );
_LIT8( KMceSipSdpSec, "sec" );
_LIT8( KMceSipSdpPreconditionCurr, "curr" );
_LIT8( KMceSipSdpPreconditionDes, "des" );
_LIT8( KMceSipSdpQosNone, "none" );
_LIT8( KMceSipEventPackageRefer, "refer" );
_LIT8( KMceSipStateActive, "active" );
_LIT8( KMceSipStatePending, "pending" );
_LIT8( KMceSipStateTerminated, "terminated" );
_LIT8( KMceSipReferEventName, "refer" );
_LIT8( KMceSipReferAcceptType, "message" );
_LIT8( KMceSipReferAcceptSubtype, "sipfrag" );
_LIT8( KMceSipVersion, "version" );
_LIT8( KMceSipNotifyContent, "SIP/2.0 100 Trying\r\n" );
_LIT8( KMceSipValueNoReferSub, "norefersub");
_LIT8( KMceSipValueMultipleRefer,"multiple-refer" );
_LIT8( KMceSipValueFalse, "false" );
_LIT8( KMceSipHeaderReferSub, "Refer-Sub" );
_LIT8( KMceSipHeaderSessionExpires, "Session-Expires" );
_LIT8( KMceSipHeaderCompactFormOfSessionExpires, "x" );
_LIT8( KMceSipHeaderMinSE, "Min-SE" );
_LIT8( KMceSipMediaTypeApplication, "application" );
_LIT8( KMceSipMediaSubTypeSDP, "sdp" );
_LIT8( KMceSipMPartMediaTypeMultipart, "multipart" );
_LIT8( KMceSipMPartMediaSubTypeMixed, "mixed" );
_LIT8( KMceSipMPartBoundary, "boundary" );
_LIT8( KMceSipMPartMessageBoundary, "MessageBoundary" );
_LIT8( KMceSipMPartMessagePattern, 
"--MessageBoundary\r\n\
Content-Type: application/sdp\r\n\r\n\
%S\r\n\
--MessageBoundary\r\n\
Content-Type: %S\r\n\r\n\
%S\
--MessageBoundary--\r\n");

_LIT8( KMceSipSubscriptionStateHeader, "Subscription-State" );
_LIT8( KMceSipTransportTLS, "transport=tls");
_LIT8( KMceSipUriSchemeSIPs, "sips");
_LIT8( KMceSdpAttrRtcp, "rtcp" );
_LIT8( KMceSdpSpace, " " );

// Extensions
_LIT8( KMceSip100Rel,"100rel" );
_LIT8( KMceSipPrecondition, "precondition" );

const TInt KMceRandMaxRetryValue = 10;
const TInt KMceRandMinRetryValue = 1;
const TInt KMceRandDividerOne = 1000000;
const TInt KMceRandDividerTwo = 1000; 

const TBool KMceUseRetryAfter = ETrue;
const TBool KMceNoRetryAfter = EFalse;

// Time Conversion.
const TUint KConversionMilliToMicro = 1000;
const TUint KMceOneSecondAsMicros( 1000000 );
_LIT8( KMceSdpAttributeLineMarker, "a=" );
_LIT8( KMceSdpBandwidthLineMarker, "b=" );

/**
*  Static library providing basic helper methods
*  to handle SIP API related issues
*
*  @lib 
*  @since
*/
class MceSip
	{
	
public://transaction related
	
	/**
    * Returns method of server transaction (request)
    * @param aRequest the server transaction
    * @return method of server transaction
    */
    static RStringF Method( CSIPServerTransaction& aRequest );
    
	/**
    * Returns response code of client transaction (response)
    * @param aResponse the client transaction
    * @return response code (1XX-6XX)
    */
    static TUint ResponseCode( CSIPClientTransaction& aResponse );
    
	/**
    * Returns response type
    * @param aResponse the client transaction
    * @return response type (1XX-6XX)
    */
    static TMceSipResponseType ResponseType( 
                                CSIPClientTransaction& aResponse );
    /**
    * Returns response type
    * @param aCode the response type
    * @return response type (1XX-6XX)
    */
    static TMceSipResponseType ResponseType( TUint aCode );                            
	
	/**
    * Checks if client transction is response to method
    * @param aResponse the client transaction
    * @param aMethod the method
    * @return ETrue, if client transctions is response to method
    */
    static TBool IsResponseTo( CSIPClientTransaction& aResponse, 
                               RStringF aMethod );

	/**
    * Returns transaction's state
    * @param aTransaction the transaction
    * @return transaction's state
    */
    static CSIPTransactionBase::TState TrxState( 
                                    CSIPTransactionBase& aTransaction );
                                    
	/**
    * Returns ETrue, if transaction is completed
    * @param aTransaction the transaction
    * @return ETrue, if transaction is completed
    */
    static TBool TrxCompleted( CSIPTransactionBase& aTransaction );
    
	/**
    * Returns transaction's type as an index to string pool
    * @param aTransaction the transaction
    * @return transaction's type as an index to string pool
    */
    static TInt TrxType( CSIPTransactionBase& aTransaction );


	/**
    * Returns SIP message elements from transaction
    * @param aTransaction the transaction
    * @return SIP message elements
    */
    static const CSIPMessageElements* ToMessageElements( 
                                CSIPTransactionBase& aTransaction );

    /**
    * Checks the content of transaction
    * @param aTransaction the transaction
    * @return ETrue, if there is a content
    */
    static TBool HasContent( CSIPTransactionBase& aTransaction );
    
    
    /**
    * Checks whether the message contains the certain header with 
    * certain value.
    * @param aMessage message that header where to be found from.
    * @param aHeaderName headername as string pool index
    * @param aHeaderValue the value to be searched
    * @return ETrue, if given header with given value is present.
    */
    static TBool HasHeaderValueL( const CSIPMessageElements& aMessage,
							    TInt aHeaderName,
							    const TDesC8& aHeaderValue );

    /**
    * Checks whether the user headers contains the certain header with 
    * certain value.
    * @param aHeaders user header that specific header where to be found from.
    * @param aHeaderName headername as string pool index
    * @param aHeaderValue the value to be searched
    * @return ETrue, if given header with given value is present.
    */
    static TBool HasHeaderValueL( const RPointerArray<CSIPHeaderBase>& aHeaders,
                              TInt aHeaderName,
                              const TDesC8& aHeaderValue );
    /**
    * Finds Specific Headers
    * @param aMessage message that header where to be found from.
    * @param aHeaderName headername which want to be found in the message. 
    * @param aHeaders array for which the found headers are added.
    *        ownership of the objects is not transfered, 
    */
    static void FindHeadersL( const CSIPMessageElements& aMessage,
						      RStringF aHeaderName,
						      RPointerArray<CSIPHeaderBase>& aHeaders );
                                  
    /**
    * Compare remote uri with EUriScheme, EUriUserinfo and EUriHost.
    * @param aRemoteUriOne, a remoteUri to be compared.
    * @param aRemoteUriTwo, a remoteUri to be compared.
    * @return An integer value of zero if the components are the same, 
    *         any other value if the components are not the same. 
    */
    static TInt CompareUri( const CUri8& aRemoteUriOne, const CUri8& aRemoteUriTwo );                               
                                    
public://SIP string pool related
    
	/**
    * Returns reson phrase based on status (reponse) code
    * @param aStatusCode status code
    * @return reson phrase. Ownership is transferred
    */
    static RStringF ReasonPhraseL( TUint aStatusCode );
    
	/**
    * Returns warning phrase based on warning code
    * @param aWarnCode warning code
    * @return warning phrase. Ownership is transferred
    */
    static RStringF WarningPhraseL( TUint aWarnCode );

	/**
    * Returns index of SIP string pool if given string is
    * found from the pool
    * @param aSIPString a string
    * @return index of SIP string pool, KErrNotFound is
    * returned if string is not found
    */
    static TInt SIPStringIndex( const TDesC8& aSIPString );
    
    
public://header manipulation
    
	/**
    * Finds Specific Header
    * @param aMessage message that header where to be found from.
    * @param aHeaderName headername which want to be found in the message. 
    * @return pointer to the created request message. The
    * ownership is not transferred.  
    */
    static CSIPHeaderBase* FindHeader( const CSIPMessageElements& aMessage,
								       RStringF aHeaderName );
	
	/**
    * Finds Specific Header
    * @param aHeaders headers that specific header where to be found from.
    * @param aHeaderName headername which want to be found in the message. 
    * @return pointer to the created request message. The
    * ownership is not transferred.  
    */							       							       
	static CSIPHeaderBase* FindHeader( const RPointerArray<CSIPHeaderBase>& aHeaders ,
                                    RStringF aHeaderName );
	/**
    * Converts SIP address to text
    * @param aToAddr SIP Address
    * @return SIP address as text
    * ownership is transferred.  
    */
    static HBufC8* AddressToTextL( const CSIPAddress& aToAddr );
    
	/**
    * Converts SIP address to text
    * @param aToAddr SIP Address
    * @return SIP address as text
    * ownership is transferred.  
    */
    static CDesC8Array* UserHeadersToTextArrayL( 
                                CSIPTransactionBase& aTransaction, 
                                const CSIPDialog& aDialog );
                                
	/**
    * Converts SIP from address to from header
    * @param aAddress SIP Address as text
    * @return SIP address as SIP from header
    * ownership is transferred.  
    */
    static CSIPFromHeader* ToFromHeaderL( const TDesC8& aAddress );
    
	/**
    * Converts header string array to SIP header arrays
    * @param aSIPHeaders SIP header array to fill
    * @param aHeaders string array
    */
    static void ToSIPExtensionHeadersL( 
                                RPointerArray<CSIPHeaderBase>& aSIPHeaders, 
                                const MDesC8Array& aHeaders );
                                
	/**
    * Converts content type to SIP content type header
    * @param aContentType enum type to define if content type is predefined
    * such as application/sdp or user defined
    * @param aUserDefinedContentType if user defined content type, content type
    * as text
    * @return SIP content type header
    */
    static CSIPContentTypeHeader* ToContentTypeHeaderL( 
                                TMceContentType aContentType, 
                                const HBufC8* aUserDefinedContentType = NULL );

    /**
    * Creates a Supported header for given extension and adds it
    * to the header array.
    * @param aSIPHeaders SIP header array 
    * @param aExtension the extension to be supported.
    */
    
    static void AddSupportedHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                     const TDesC8& aExtension );
                                
    /**
    * Creates a Require header for given extension and adds it
    * to the header array.
    * @param aSIPHeaders SIP header array 
    * @param aExtension the extension to be required.
    */
    
    static void AddRequireHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                   const TDesC8& aExtension );
    
    /**
    * Creates an Allow header for given method and adds it
    * to the header array.
    * @param aSIPHeaders SIP header array 
    * @param aMethod the method to be allowed.
    */
    static void AddAllowHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                 TInt aMethod );

    /**
    * Creates a RAck header for given client transaction and adds it
    * to the header array.
    * @param aSIPHeaders SIP header array 
    * @param aMethod the method.
    */
    static void AddRAckHeaderL( RPointerArray<CSIPHeaderBase>& aSIPHeaders,
                                const CSIPResponseElements& aResponse );
                                
	/**
    * Converts "Contact"-header including the whole value of the header
    * except for the header's name ("Contact:") to SIP contact header
    * @param aContact value for contact header
    * @return SIP contect header
    * ownership is transferred.  
    */
    static CSIPContactHeader* ToContactHeaderL( const TDesC8& aContact );
    
    /**
    * Add sip methods to array for allow header
    * @param aArray array that thoses methods are added to
    * @return void
    */
	static void BasicAllowMethodsL( RArray<TInt>& aArray );
	
	/**
    * Find If "Contact"-header including SIPs URI in user headers
    * @param aMessage message response element including contact header
    * @return True if contact headers has SIPs URI, false otherwise
    */
	static TBool HasSipsUriInContactL( const CSIPMessageElements& aMessage );
	
	/**
    * Find If "Contact"-header including SIPs URI in user headers
    * @param aMessage message response element including contact header
    * @return True if contact headers has SIPs URI, false otherwise
    */
   	static TBool HasTlsInContactL( const CSIPMessageElements& aMessage );
   	
    
public://error situation handling

	/**
    * Returns response code and reason phrase based on given
    * error.
    * @param aError error
    * @param aSIPResponseCode reference of response code
    * @param aReasonPhrase reference of reason phrase pool index
    */
    static void ErrorResponse( TInt aError, 
                               TUint& aSIPResponseCode, 
                               TInt& aReasonPhrase );
                               
	/**
    * Returns ETrue, if error is SDP related error
    * @param aError error code
    * @return ETrue, if error is SDP related error
    */
    static TBool IsSDPError( TInt aError );

	/**
    * Returns ETrue, if error is SIP API related error
    * @param aError error code
    * @return ETrue, if error is SIP API related error
    */
    static TBool IsSIPStackError( TInt aError );


	/**
    * Discards request by sending response, which is resolved based
    * on given error. If error is greater it is interpreted as 
    * response code, which will be sent as response. By default 
    * 500 is sent as response. 
    * @param aRequest discarded request. Ownership is transferred
    * @param aError error
    * @param aRetryIndication if ETrue, Retry-After header is included
    * in response
    * @return status of operation
    */
    static TInt DiscardRequest( CSIPServerTransaction* aRequest, 
                                TInt aError = KErrNone,
                                TBool aRetryIndication = EFalse );
                                
	/**
    * Discards request by sending response, which is resolved based
    * on given error. If error is greater it is interpreted as 
    * response code, which will be sent as response. By default 
    * 500 is sent as response. 
    * @param aRequest discarded request.
    * @param aError error
    * @param aRetryIndication if ETrue, Retry-After header is included
    * in response
    * @return status of operation
    */
    static TInt DiscardRequest( CSIPServerTransaction& aRequest, 
                                TInt aError = KErrNone,
                                TBool aRetryIndication = EFalse );

public://misc
    
	/**
    * Gives random value between two values
    * @param aMinValue min value of random value
    * @param aMaxValue max value of random value
    * @return random value
    */
    static TUint Random( TUint aMinValue, TUint aMaxValue );
    
    
	/**
    * Returns first integer value from header
    * @param aHeader SIP header as text
    * @return integer value
    */
    static TUint HeaderValueL( TPtr8 aHeader );
    
    
public: // mics functions

    /**
    * Returns RSeq value from the ResponseElements
    * @param aResponseElements SIP Respomse Elements.
    * @return integer value 
    */
    
    static TInt RSeq( const CSIPResponseElements& aResponseElements );
        
    
private:

    static void DoDiscardRequestL( CSIPServerTransaction& aRequest, 
                                   TInt aError,
                                   TBool aRetryIndication );

    };


#endif

// End of File

