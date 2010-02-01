/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "ut_mcesip.h"
#include "MCETestHelper.h"
#include <SIPStrings.h>
#include <SipContactHeader.h>
#include <SdpCodecStringPool.h>
#include <sipstrings.h>
#include <SdpCodecErr.h>
#include <siprseqheader.h> 


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcesip.h"
#include "MCETestHelper.h"

const CSIPTransactionBase::TState KMceTrxStateUndefined = 
    static_cast<CSIPTransactionBase::TState>(-1);



// CONSTRUCTION
UT_MCESIP* UT_MCESIP::NewL()
    {
    UT_MCESIP* self = UT_MCESIP::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_MCESIP* UT_MCESIP::NewLC()
    {
    UT_MCESIP* self = new( ELeave ) UT_MCESIP();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_MCESIP::~UT_MCESIP()
    {
    }

// Default constructor
UT_MCESIP::UT_MCESIP()
    {
    }

// Second phase construct
void UT_MCESIP::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_MCESIP::SetupL(  )
    {
    CMCETls::OpenL();
    iStorage = CMCETls::Storage();
    SIPStrings::OpenL();
    } 

void UT_MCESIP::Teardown(  )
    {
    CMCETls::Close();
    SIPStrings::Close();
    }
    

void UT_MCESIP::UT_MCESIP_ReasonPhraseLL()
    {
    RStringF phrase;
    phrase = MceSip::ReasonPhraseL( KMceSipOK );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipBadRequest );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseBadRequest ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipUnauthorized );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseUnauthorized ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipForbidden );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseForbidden ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipNotFound );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseNotFound ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipMethodNotAllowed );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseMethodNotAllowed ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSip4XXNotAcceptable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseNotAcceptableHere ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipProxyAuthenticationRequired );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseProxyAuthenticationRequired ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipRequestTimeout );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseRequestTimeout ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipUnsupportedMediaType );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseUnsupportedMediaType ) );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::ReasonPhraseL( KMceSipUnsupportedURIScheme );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseUnsupportedURIScheme ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipBadExtension );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseBadExtension ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipExtensionRequired );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseExtensionRequired ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipSessionIntervalTooSmall );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseIntervalTooBrief ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipIntervalTooBrief );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseIntervalTooBrief ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipTemporarilyUnavailable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseTemporarilyNotAvailable ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipCallOrTransactionDoesNotExist );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseCallTransactionDoesNotExist ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipLoopDetected );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseLoopDetected ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipTooManyHops );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseTooManyHops ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipBusyHere );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseBusyHere ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipRequestTerminated );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseRequestTerminated ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipNotAcceptableHere );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseNotAcceptableHere ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipBadEvent );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseBadEvent ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipRequestPending );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipPhraseRequestPending ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipServerInternalError );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseServerInternalError ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipNotImplemented );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipServiceUnavailable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseServiceUnavailable ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipVersionNotSupported );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseVersionNotSupported ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipDecline );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() > 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EPhraseDecline ) );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::ReasonPhraseL( KMceSipDoesNotExistAnywhere );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Length() == 0 );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    CleanupStack::PopAndDestroy();

    
    }

void UT_MCESIP::UT_MCESIP_TrxCompletedL()
    {
    CSIPServerTransaction* trx = MCETestHelper::ServerTransactionL(  SipStrConsts::EInvite );
    CleanupStack::PushL( trx );
    
    trx->iState = CSIPTransactionBase::ETrying;
    EUNIT_ASSERT( !MceSip::TrxCompleted( *trx ) );
    trx->iState = CSIPTransactionBase::ECalling;
    EUNIT_ASSERT( !MceSip::TrxCompleted( *trx ) );
    trx->iState = CSIPTransactionBase::EProceeding;
    EUNIT_ASSERT( !MceSip::TrxCompleted( *trx ) );
    trx->iState = CSIPTransactionBase::EConfirmed;
    EUNIT_ASSERT( MceSip::TrxCompleted( *trx ) );
    trx->iState = CSIPTransactionBase::ECompleted;
    EUNIT_ASSERT( MceSip::TrxCompleted( *trx ) );
    trx->iState = CSIPTransactionBase::ETerminated;
    EUNIT_ASSERT( MceSip::TrxCompleted( *trx ) );
    trx->iState = (CSIPTransactionBase::TState)KMceTrxStateUndefined;
    EUNIT_ASSERT( MceSip::TrxCompleted( *trx ) );
    
    CleanupStack::PopAndDestroy( trx );
    
    
    }
    
void UT_MCESIP::UT_MCESIP_WarningPhraseLL()
    {
    RStringF phrase;

    phrase = MceSip::WarningPhraseL( KMceSipWarnIncompatibleNetworkProtocol );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase300 ) == 0 );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::WarningPhraseL( KMceSipWarnIncompatibleNetworkAddressFormat );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase301 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnIncompatibleTransportProtocol );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase302 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnIncompatibleBandwidthUnits );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase303 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnMediaTypeNotAvailable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase304 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnIncompatibleMediaFormat );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase305 ) == 0 );
    CleanupStack::PopAndDestroy();
    
    phrase = MceSip::WarningPhraseL( KMceSipWarnAttributeNotUnderstood );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase306 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnSessionDescriptionParameterNotUnderstood );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase307 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnMulticastNotAvailable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase330 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnUnicastNotAvailable );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase331 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnInsufficientBandwidth );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase370 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnMiscellaneous );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase.DesC().Compare( KMceSipWarnPhrase399 ) == 0 );
    CleanupStack::PopAndDestroy();

    phrase = MceSip::WarningPhraseL( KMceSipWarnMiscellaneous + 1 );
    CleanupClosePushL( phrase );
    EUNIT_ASSERT( phrase == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    CleanupStack::PopAndDestroy();
    
    }
    

void UT_MCESIP::UT_MCESIP_IsSIPStackErrorL()
    {
    
    EUNIT_ASSERT( !MceSip::IsSIPStackError( KErrSdpCodecDecode - 1 ) );
    EUNIT_ASSERT( !MceSip::IsSIPStackError( KErrSdpCodecVersionField + 1 ) );
    EUNIT_ASSERT( !MceSip::IsSIPStackError( KErrSIPInvalidURIType - 1 ) );
    EUNIT_ASSERT( !MceSip::IsSIPStackError( KErrSIPMalformedMessage + 1 ) );
    
    EUNIT_ASSERT( MceSip::IsSIPStackError( KErrSIPInvalidURIType ) );
    EUNIT_ASSERT( MceSip::IsSIPStackError( KErrSIPMalformedMessage ) );
    EUNIT_ASSERT( MceSip::IsSIPStackError( KErrSIPTransportFailure ) );
    EUNIT_ASSERT( MceSip::IsSIPStackError( KErrSdpCodecRepeatField ) );
    
    }

void UT_MCESIP::UT_MCESIP_MethodL()
    {

    CSIPServerTransaction* trx = MCETestHelper::ServerTransactionL(  SipStrConsts::EInvite );
    CleanupStack::PushL( trx );

    EUNIT_ASSERT( MceSip::Method( *trx ) == SIPStrings::StringF( SipStrConsts::EInvite ) );
    
    delete trx->iRequestElements;
    trx->iRequestElements = NULL;
    
    EUNIT_ASSERT( MceSip::Method( *trx ) == SIPStrings::StringF( SipStrConsts::EEmpty ) );

    CleanupStack::PopAndDestroy( trx );
    
    }

void UT_MCESIP::UT_MCESIP_ToContentTypeHeaderLL()
    {
    CSIPContentTypeHeader* header = NULL;
    
    header = MceSip::ToContentTypeHeaderL( EMceContentTypeSDP );
    CleanupStack::PushL( header );
    EUNIT_ASSERT( header->MediaType().Compare( KMceSipMediaTypeApplication ) == 0 );
    EUNIT_ASSERT( header->MediaSubtype().Compare( KMceSipMediaSubTypeSDP ) == 0 );
    CleanupStack::PopAndDestroy( header );
    

    header = MceSip::ToContentTypeHeaderL( EMceContentTypeMultiPart );
    CleanupStack::PushL( header );
    EUNIT_ASSERT( header->MediaType().Compare( KMceSipMPartMediaTypeMultipart ) == 0 );
    EUNIT_ASSERT( header->MediaSubtype().Compare( KMceSipMPartMediaSubTypeMixed ) == 0 );
    CleanupStack::PopAndDestroy( header );

    HBufC8* userDefinedCT = KContentType().AllocLC();
    
    header = MceSip::ToContentTypeHeaderL( EMceContentTypeUserDefined, userDefinedCT );
    CleanupStack::PushL( header );
    EUNIT_ASSERT( header->MediaType().Compare( _L8("text" ) ) == 0 );
    EUNIT_ASSERT( header->MediaSubtype().Compare( _L8("plain" ) ) == 0 );
    CleanupStack::PopAndDestroy( header );
    CleanupStack::PopAndDestroy( userDefinedCT );
    
    header = NULL;
    header = MceSip::ToContentTypeHeaderL( (TMceContentType)-1 );
    EUNIT_ASSERT( header == NULL );
    
    
    }


void UT_MCESIP::UT_MCESIP_UserHeadersToTextArrayLL()
    {
    
    CDesC8Array* headers = NULL;
    CSIPContentTypeHeader* header = NULL;
    CSIPDialog* dialog = CSIPDialog::NewL();
    CleanupStack::PushL( dialog );

    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );
    
    header = MceSip::ToContentTypeHeaderL( EMceContentTypeSDP );
    CleanupStack::PushL( header );
    sipHeaders.AppendL( header );
    CleanupStack::Pop( header );
    
    CSIPServerTransaction* trx = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( trx );

    headers = MceSip::UserHeadersToTextArrayL( *trx, *dialog );
    CleanupStack::PushL( headers );
    EUNIT_ASSERT( headers->MdcaCount() == 4 );
    CleanupStack::PopAndDestroy( headers );
    CleanupStack::PopAndDestroy( trx );


    trx = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( trx );

    delete trx->iRequestElements;
    trx->iRequestElements = NULL;

    headers = MceSip::UserHeadersToTextArrayL( *trx, *dialog );
    CleanupStack::PushL( headers );
    EUNIT_ASSERT( headers->MdcaCount() == 3 );
    CleanupStack::PopAndDestroy( headers );
    CleanupStack::PopAndDestroy( trx );


    CSIPClientTransaction* clitrx = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK );
    headers = MceSip::UserHeadersToTextArrayL( *clitrx, *dialog );
    CleanupStack::PushL( headers );
    EUNIT_ASSERT( headers->MdcaCount() == 5 );
    CleanupStack::PopAndDestroy( headers );
    CleanupStack::PopAndDestroy( clitrx );

    clitrx = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
    headers = MceSip::UserHeadersToTextArrayL( *clitrx, *dialog );
    CleanupStack::PushL( headers );
    EUNIT_ASSERT( headers->MdcaCount() == 3 );
    CleanupStack::PopAndDestroy( headers );
    CleanupStack::PopAndDestroy( clitrx );
    
    CMCETls::Storage()->SetLeave( KErrNotFound );
    clitrx = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
    headers = MceSip::UserHeadersToTextArrayL( *clitrx, *dialog );
    CleanupStack::PushL( headers );
    EUNIT_ASSERT( headers->MdcaCount() == 2 );
    CleanupStack::PopAndDestroy( headers );
    CleanupStack::PopAndDestroy( clitrx );
    
    CleanupStack::PopAndDestroy( dialog );
    
    
    }

void UT_MCESIP::UT_MCESIP_RandomL()
    {
    for ( TInt i = 0; i < KMceTestLoops; i++ )
        {
        for (TInt j = 0; j < KMceTestLoops; j++)
            {
            TInt rand = MceSip::Random( i, j );
            EUNIT_ASSERT( ( j<i && rand == 0 ) || rand <= j );
            EUNIT_ASSERT( ( j<i && rand == 0 ) || rand >= i );
            }
        }
    }

void UT_MCESIP::UT_MCESIP_DiscardInviteL()
    {
    
    }
    
void UT_MCESIP::UT_MCESIP_CompareUriL()
    {
    // compare
    TUriParser8 uri8Parser;
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    CUri8* remoteUriOne = CUri8::NewLC( uri8Parser );
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:w@172.21.11.69") ) );
    CUri8* remoteUriTwo = CUri8::NewLC( uri8Parser );
    
    EUNIT_ASSERT( MceSip::CompareUri( *remoteUriOne, *remoteUriTwo) != 0 );
    
    CleanupStack::PopAndDestroy( remoteUriTwo );
    CleanupStack::PopAndDestroy( remoteUriOne );
    
    //compare
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.121") ) );
    remoteUriOne = CUri8::NewLC( uri8Parser );
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    remoteUriTwo = CUri8::NewLC( uri8Parser );
    
    EUNIT_ASSERT( MceSip::CompareUri( *remoteUriOne, *remoteUriTwo) != 0 );
    
    CleanupStack::PopAndDestroy( remoteUriTwo );
    CleanupStack::PopAndDestroy( remoteUriOne );

    // compare
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.121") ) );
    remoteUriOne = CUri8::NewLC( uri8Parser );
    User::LeaveIfError( uri8Parser.Parse( _L8("sips:n@172.21.11.121") ) );
    remoteUriTwo = CUri8::NewLC( uri8Parser );
    
    EUNIT_ASSERT( MceSip::CompareUri( *remoteUriOne, *remoteUriTwo) != 0 );
    
    CleanupStack::PopAndDestroy( remoteUriTwo );
    CleanupStack::PopAndDestroy( remoteUriOne );
    
    // compare
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.121") ) );
    remoteUriOne = CUri8::NewLC( uri8Parser );
    User::LeaveIfError( uri8Parser.Parse( _L8("sips:w@172.21.11.69") ) );
    remoteUriTwo = CUri8::NewLC( uri8Parser );
    
    EUNIT_ASSERT( MceSip::CompareUri( *remoteUriOne, *remoteUriTwo) != 0 );
    
    CleanupStack::PopAndDestroy( remoteUriTwo );
    CleanupStack::PopAndDestroy( remoteUriOne );
    
    // compare
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.121") ) );
    remoteUriOne = CUri8::NewLC( uri8Parser );
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.121") ) );
    remoteUriTwo = CUri8::NewLC( uri8Parser );
    
    EUNIT_ASSERT( MceSip::CompareUri( *remoteUriOne, *remoteUriTwo) == 0 );
    
    CleanupStack::PopAndDestroy( remoteUriTwo );
    CleanupStack::PopAndDestroy( remoteUriOne );
    }
    
    
void UT_MCESIP::UT_MCESIP_RSeq()
    {
     
    TUint seq = 2;
		
	_LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
       
    CSIPResponseElements* aElements = CSIPResponseElements::NewL( 180,ringing );
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PushL( aElements );
        
    CSIPMessageElements* mElements;
    mElements = &aElements->MessageElements();
    
    
    // RSeq Test without RSeq Header.
    
    EUNIT_ASSERT( MceSip::RSeq( *aElements ) == KErrNotFound );    
    
    
    // RSeq Test with RSeq Header.
    // Add the RSeqHeader to CSIPResponseElements i.e. to the CSIPMessgeElements of Response
    RPointerArray<CSIPHeaderBase> respHeaders; 
    CSIPHeaderBase::PushLC( &respHeaders );
          
    CSIPRSeqHeader* rseqHdr = new( ELeave ) CSIPRSeqHeader( seq );
    //CleanupStack::PopAndDestroy();
    CleanupStack::PushL( rseqHdr ); 
        
    User::LeaveIfError( respHeaders.Append( rseqHdr ) );
    CleanupStack::Pop( rseqHdr );

    mElements->SetUserHeadersL( respHeaders );
    CleanupStack::Pop( &respHeaders );
        
	EUNIT_ASSERT( MceSip::RSeq( *aElements ) == 2 );
    
    CleanupStack::PopAndDestroy( aElements );
           
    }    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_MCESIP,
    "ut_cmCESIP",
    "UNIT" )

EUNIT_TEST(
    "ReasonPhraseL - test ",
    "MceSip",
    "ReasonPhraseL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_ReasonPhraseLL, Teardown)

EUNIT_TEST(
    "TrxCompleted - test ",
    "MceSip",
    "TrxCompleted",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_TrxCompletedL, Teardown)

EUNIT_TEST(
    "WarningPhraseL - test ",
    "MceSip",
    "WarningPhraseL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_WarningPhraseLL, Teardown)

EUNIT_TEST(
    "IsSIPStackError - test ",
    "MceSip",
    "IsSIPStackError",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_IsSIPStackErrorL, Teardown)
    
EUNIT_TEST(
    "Method - test ",
    "MceSip",
    "Method",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_MethodL, Teardown)

EUNIT_TEST(
    "ToContentTypeHeaderL - test ",
    "MceSip",
    "ToContentTypeHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_ToContentTypeHeaderLL, Teardown)

EUNIT_TEST(
    "UserHeadersToTextArrayL - test ",
    "MceSip",
    "UserHeadersToTextArrayL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_UserHeadersToTextArrayLL, Teardown)

EUNIT_TEST(
    "Random - test ",
    "MceSip",
    "Random",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_RandomL, Teardown)

EUNIT_TEST(
    "DiscardInvite - test ",
    "MceSip",
    "DiscardInvite",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_DiscardInviteL, Teardown)

EUNIT_TEST(
    "CompareUri - test ",
    "MceSip",
    "CompareUri",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_CompareUriL, Teardown)
    
    
EUNIT_TEST(
    "RSeq - test ",
    "MceSip",
    "Check RSeqi",
    "FUNCTIONALITY",
    SetupL, UT_MCESIP_RSeq, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE
