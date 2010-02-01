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
#include "ut_mcesipeventhelper.h"
#include "mcesipeventhelper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipeventheader.h>
#include <sipsubscriptionstateheader.h>
#include <sipextensionheader.h>
#include <siprequireheader.h>

//  INTERNAL INCLUDES
#include "mcesipeventhelper.h"
#include "MCEEventTestConsts.h"
#include "sipsubscribedialogassoc.h"
#include "sipmessageelements.h"
#include "siprefresh.h"
#include "mcecomevent.h"
#include "mcesipevent.h"
#include "sipresponseelements.h"
#include "mcesip.h"
#include "sipprofile.h"
#include "mcesipconnection.h"

// CONSTRUCTION
UT_MCESIPEventHelper* UT_MCESIPEventHelper::NewL()
    {
    UT_MCESIPEventHelper* self = UT_MCESIPEventHelper::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_MCESIPEventHelper* UT_MCESIPEventHelper::NewLC()
    {
    UT_MCESIPEventHelper* self = new( ELeave ) UT_MCESIPEventHelper();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_MCESIPEventHelper::~UT_MCESIPEventHelper()
    {
    }

// Default constructor
UT_MCESIPEventHelper::UT_MCESIPEventHelper()
    {
    }

// Second phase construct
void UT_MCESIPEventHelper::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_MCESIPEventHelper::SetupL(  )
    {
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
    MCE_DELETE( iSipProfile->iContactParams );
    iSipProfile->iContactParams = new( ELeave ) CDesC8ArrayFlat( 1 ); 
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iEvent = iSIPEvent->iEvent;
    } 

void UT_MCESIPEventHelper::Teardown(  )
    {
    SIPStrings::Close();
    iEvent = NULL;
    delete iSIPEvent;
	
    delete iConnection;
    
    delete iSipProfile;
    }

void UT_MCESIPEventHelper::SetupEmpty(  )
    {
    } 

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_SetContentLCL()
    {
    // headers, contenttype
    CSIPMessageElements* msgElem = CSIPMessageElements::NewL();
    CleanupStack::PushL( msgElem );
    
    HBufC8* content = KContent().AllocLC();
    HBufC8* contentType = KContentType().AllocLC();   
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );
    
    MCESIPEventHelper::SetContentL(*msgElem, *iEvent );
           
    EUNIT_ASSERT( msgElem->Content().Compare( KContent ) == 0 );
    
    CleanupStack::PopAndDestroy( msgElem );
        
    // headers, empty contenttype, body
    
    msgElem = CSIPMessageElements::NewL();
    CleanupStack::PushL( msgElem );
    HBufC8* content2 = KContent().AllocLC();
    HBufC8* emptyContentType = KNullDesC8().AllocLC();
    
    iEvent->SetMsgContentL( emptyContentType, content2 );
    MCESIPEventHelper::SetContentL( *msgElem, *iEvent );
    CleanupStack::Pop( emptyContentType );
    CleanupStack::Pop( content2 );
    
    EUNIT_ASSERT( msgElem->Content().Length() == 0 );
    
    CleanupStack::PopAndDestroy( msgElem );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateMessageElementsLCL(  )
    {
    /*
    CMceComEvent& aEvent,
    TInt aRequestType
    */
    // headers, length null, contenttype, body
    CSIPMessageElements* msgElem = NULL;
    CDesC8ArrayFlat* headers = new (ELeave) CDesC8ArrayFlat(1);
    iEvent->SetMsgHeaders( headers );
    HBufC8* content = KContent().AllocLC();
    HBufC8* contentType = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );
    
    msgElem = MCESIPEventHelper::CreateMessageElementsLC( *iEvent, SipStrConsts::ENotify );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    
    // headers, empty contenttype, body
    msgElem = NULL;
    CDesC8ArrayFlat* headers2 = new (ELeave) CDesC8ArrayFlat(1);
    CleanupStack::PushL( headers2 );
    headers2->AppendL( KModifyHeader() );
    iEvent->SetMsgHeaders( headers2 );
    CleanupStack::Pop( headers2 );
    
    HBufC8* content2 = KContent().AllocLC();
    HBufC8* emptyContentType = KNullDesC8().AllocLC();
    iEvent->SetMsgContentL( emptyContentType, content2 );
    CleanupStack::Pop( emptyContentType );
    CleanupStack::Pop( content2 );
    
    msgElem = MCESIPEventHelper::CreateMessageElementsLC( *iEvent, SipStrConsts::ENotify );
    
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateMessageElementsLC2L(  )
    {
    // headers, contenttype, body, expires
    CDesC8ArrayFlat* headers = new (ELeave) CDesC8ArrayFlat(1);
    CleanupStack::PushL( headers );
    headers->AppendL( KModifyHeader() );
    iEvent->SetMsgHeaders( headers );
    CleanupStack::Pop( headers );
    
    HBufC8* content = KContent().AllocLC();
    HBufC8* contentType = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );
    
    CSIPMessageElements* msgElem = 
        MCESIPEventHelper::CreateMessageElementsLC( *iEvent,
                                                    SipStrConsts::ESubscribe, 
                                                    KDefaultExpires );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    
    // headers, empty contenttype, body, expires
    msgElem = NULL;
    CDesC8ArrayFlat* headers2 = new (ELeave) CDesC8ArrayFlat(1);
    CleanupStack::PushL( headers2 );
    headers2->AppendL( KModifyHeader() );
    iEvent->SetMsgHeaders( headers2 );
    CleanupStack::Pop( headers2 );
    
    HBufC8* content2 = KContent().AllocLC();
    HBufC8* emptyContentType = KNullDesC8().AllocLC();
    iEvent->SetMsgContentL( emptyContentType, content2 );
    CleanupStack::Pop( emptyContentType );
    CleanupStack::Pop( content2 );
    
    msgElem = MCESIPEventHelper::CreateMessageElementsLC( *iEvent,
                                                          SipStrConsts::ESubscribe,
                                                          KDefaultExpires );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    }
 
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateMessageElementsLC3L()
    {
    // REFER message elements with differen refer types
    // ENoSuppression
    CDesC8ArrayFlat* headers = new (ELeave) CDesC8ArrayFlat(1);
    iEvent->SetMsgHeaders( headers );
    HBufC8* content = KContent().AllocLC();
    HBufC8* contentType = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );
    
    
    CSIPMessageElements* msgElem = 
        MCESIPEventHelper::CreateMessageElementsLC( *iEvent, 
                                                    SipStrConsts::ERefer, 
                                                    CMceRefer::ENoSuppression );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    
    // ESuppressed
    msgElem = NULL;
    HBufC8* content2 = KContent().AllocLC();
    HBufC8* contentType2 = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType2, content2 );
    CleanupStack::Pop( contentType2 );
    CleanupStack::Pop( content2 );
    
    msgElem = MCESIPEventHelper::CreateMessageElementsLC( *iEvent,
                                                          SipStrConsts::ERefer,
                                                          CMceRefer::ESuppressed );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    
    // EMultipleRefer
    msgElem = NULL;
    HBufC8* content3 = KContent().AllocLC();
    HBufC8* contentType3 = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType3, content3 );
    CleanupStack::Pop( contentType3 );
    CleanupStack::Pop( content3 );
    
    msgElem = MCESIPEventHelper::CreateMessageElementsLC( *iEvent, 
                                                          SipStrConsts::ERefer, 
                                                          CMceRefer::EMultipleRefer );
    EUNIT_ASSERT( msgElem );
    CleanupStack::PopAndDestroy( msgElem );
    }


void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateReferNotifyMessageElementsLCL()
    {
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    iEvent->SetMsgHeaders( hdrs );
    
    CSIPMessageElements* msgElem = msgElem = 
        MCESIPEventHelper::CreateReferNotifyMessageElementsLC( *iEvent );
    EUNIT_ASSERT( msgElem );
    
    _LIT8( KExpectedNotifyContent, "SIP/2.0 100 Trying\r\n" );
    
    EUNIT_ASSERT_EQUALS( KExpectedNotifyContent(), msgElem->Content() )
    
    CleanupStack::PopAndDestroy( msgElem );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleSubscriptionStateHeaderLL(  )
    {
    // active
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    TBool t = EFalse;
    
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    MCESIPEventHelper::HandleSubscriptionStateHeaderL( *elem, t );
    CleanupStack::PopAndDestroy( elem );
    EUNIT_ASSERT( !t );
    
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL2L(  )
    {
    //pending
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    TBool t = EFalse;
    
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
   
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStatePending );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    MCESIPEventHelper::HandleSubscriptionStateHeaderL( *elem, t );
    CleanupStack::PopAndDestroy( elem );
    EUNIT_ASSERT( !t );
    
    }


void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL3L()
    {
    // terminated
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    TBool t = EFalse;
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    MCESIPEventHelper::HandleSubscriptionStateHeaderL( *elem, t );
    CleanupStack::PopAndDestroy( elem );
    EUNIT_ASSERT( t );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL4L()
    {
    // other
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    TBool t = EFalse;
    
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
   
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateOther );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    EUNIT_ASSERT_LEAVE( MCESIPEventHelper::HandleSubscriptionStateHeaderL( *elem, t ) );
    CleanupStack::PopAndDestroy( elem );
    EUNIT_ASSERT( !t );// not changed
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL5L()
    {
    //other
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    TBool t = EFalse;
   
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateOther );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    EUNIT_ASSERT_LEAVE( MCESIPEventHelper::HandleSubscriptionStateHeaderL( *elem, t ) );
    CleanupStack::PopAndDestroy( elem );
    EUNIT_ASSERT( !t );    
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_NotifyRequestReceivedLCL(  )
    {
    // no headers
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    CSIPResponseElements* resp = MCESIPEventHelper::NotifyRequestReceivedLC( *iEvent, *elem );
    EUNIT_ASSERT( resp->StatusCode() == 400  );
    
    CleanupStack::PopAndDestroy( resp );
    CleanupStack::PopAndDestroy( elem );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_NotifyRequestReceivedLC2L(  )
    {
    // subscription state
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    CSIPResponseElements* resp = MCESIPEventHelper::NotifyRequestReceivedLC( *iEvent, *elem );
    EUNIT_ASSERT( resp->StatusCode() == 489  );
    
    CleanupStack::PopAndDestroy( resp );
    CleanupStack::PopAndDestroy( elem );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_NotifyRequestReceivedLC3L(  )
    {
    // subscriptionstate + event
    CSIPMessageElements* elem = CSIPMessageElements::NewL();
    CleanupStack::PushL( elem );
    RPointerArray<CSIPHeaderBase> r;
    CSIPHeaderBase::PushLC( &r );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    r.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    r.AppendL( e );
    CleanupStack::Pop( e );
    
    elem->SetUserHeadersL( r );
    CleanupStack::Pop( &r );
    CSIPResponseElements* resp = MCESIPEventHelper::NotifyRequestReceivedLC( *iEvent, *elem );
    EUNIT_ASSERT( resp->StatusCode() == 200  );
    
    CleanupStack::PopAndDestroy( resp );
    CleanupStack::PopAndDestroy( elem );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_ReferNotifyRequestReceivedLCL(  )
    {
    // Call with no headers, for coverage, other parts covered
    CSIPMessageElements* m = CSIPMessageElements::NewL();
    CleanupStack::PushL( m );
    CSIPResponseElements* resp = MCESIPEventHelper::ReferNotifyRequestReceivedLC( *iEvent, *m );
    EUNIT_ASSERT( resp->StatusCode() == 400 );
    CleanupStack::PopAndDestroy( resp );
    CleanupStack::PopAndDestroy( m );
    
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_SubscribeRequestReceivedLCL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_HandleExpireHeaderLL(  )
    {
    // Call with no expires headers, for coverage, other parts covered
    CSIPMessageElements* m = CSIPMessageElements::NewL();
    CleanupStack::PushL( m );
    TBool t;
    EUNIT_ASSERT_LEAVE( MCESIPEventHelper::HandleExpireHeaderL( *m, t ) );
    CleanupStack::PopAndDestroy( m );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_ResolveReferTypeLL()
    {
    // no suppression
    CMceRefer::TType result = CMceRefer::ESuppressed;
    
    // first with not user headers
    CSIPMessageElements* msgElem = CSIPMessageElements::NewLC();
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    msgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    result = MCESIPEventHelper::ResolveReferTypeL( *msgElem );
    EUNIT_ASSERT( result == CMceRefer::ENoSuppression );
    
    CleanupStack::PopAndDestroy( msgElem );
    
    // add a random header
    CSIPMessageElements* msgElem2 = CSIPMessageElements::NewLC();
    RPointerArray< CSIPHeaderBase > hdrs2;
    CSIPHeaderBase::PushLC( &hdrs2 );
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KReferEventPackage );
    CleanupStack::PushL( e );
    hdrs2.AppendL( e );
    CleanupStack::Pop( e );
    msgElem2->SetUserHeadersL( hdrs2 );
    CleanupStack::Pop( &hdrs2 );
    result = MCESIPEventHelper::ResolveReferTypeL( *msgElem2 );
    EUNIT_ASSERT( result == CMceRefer::ENoSuppression );
    
    CleanupStack::PopAndDestroy( msgElem2 );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_ResolveReferTypeL2L()
    {
    // Suppressed
    CMceRefer::TType result = CMceRefer::ENoSuppression;
    CSIPMessageElements* msgElem = CSIPMessageElements::NewLC();
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPExtensionHeader* ex = CSIPExtensionHeader::NewLC( KMceSipHeaderReferSub, KMceSipValueFalse );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    msgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    EUNIT_ASSERT_NO_LEAVE( result = MCESIPEventHelper::ResolveReferTypeL( *msgElem ) );
    EUNIT_ASSERT( result == CMceRefer::ESuppressed );
    
    CleanupStack::PopAndDestroy( msgElem );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_ResolveReferTypeL3L()
    {
    // Multiple Refer
    CMceRefer::TType result = CMceRefer::ENoSuppression;
    CSIPMessageElements* msgElem = CSIPMessageElements::NewLC();
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    RStringF valNoSub = 
        SIPStrings::Pool().OpenFStringL( KMceSipValueNoReferSub );
    CleanupClosePushL( valNoSub );
    CSIPRequireHeader* hdrNoSub = CSIPRequireHeader::NewLC( valNoSub );
    hdrs.AppendL( hdrNoSub );
    CleanupStack::Pop( hdrNoSub );
    CleanupStack::PopAndDestroy(); // valNoSub
    
    RStringF valMult = 
        SIPStrings::Pool().OpenFStringL( KMceSipValueMultipleRefer );
    CleanupClosePushL( valMult );
    CSIPRequireHeader* hdrMult = CSIPRequireHeader::NewLC( valMult );
    hdrs.AppendL( hdrMult );
    CleanupStack::Pop( hdrMult );
    CleanupStack::PopAndDestroy(); // valMult  
    
    msgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    EUNIT_ASSERT_NO_LEAVE( result = MCESIPEventHelper::ResolveReferTypeL( *msgElem ) );
    EUNIT_ASSERT( result == CMceRefer::EMultipleRefer );
    
    CleanupStack::PopAndDestroy( msgElem );
    
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_ResolveReferTypeL4L()
    {
    // Suppressed, but value "true"
    CMceRefer::TType result = CMceRefer::ENoSuppression;
    CSIPMessageElements* msgElem = CSIPMessageElements::NewLC();
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPExtensionHeader* ex = CSIPExtensionHeader::NewLC( KMceSipHeaderReferSub, KSupprValueTrue );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    msgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    EUNIT_ASSERT_NO_LEAVE( result = MCESIPEventHelper::ResolveReferTypeL( *msgElem ) );
    EUNIT_ASSERT( result == CMceRefer::ENoSuppression );
    
    CleanupStack::PopAndDestroy( msgElem );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_MatchEventsL()
    {
    /*
    const CSIPEventHeader& aFirstEvent,
    const CSIPEventHeader& aSecondEvent
    */
    
    CSIPEventHeader* first = CSIPEventHeader::NewLC( KEventPackage );
    CSIPEventHeader* second = CSIPEventHeader::NewLC( KReferEventPackage );
    RStringF idParam = SIPStrings::StringF( SipStrConsts::EId );
    RStringF idParamValue = SIPStrings::Pool().OpenFStringL( KIdParamValue );
    CleanupClosePushL( idParamValue );
    RStringF idParamValue2 = SIPStrings::Pool().OpenFStringL( KIdParamValue2 );
    CleanupClosePushL( idParamValue2 );
    // event packages do not match
    EUNIT_ASSERT( !MCESIPEventHelper::MatchEvents( *first, *second ) );
    
    second->SetEventPackageL( KEventPackage );
    
    // neither has id param
    EUNIT_ASSERT( MCESIPEventHelper::MatchEvents( *first, *second ) );
    
    // second has id param, first has not
    second->SetParamL( idParam, idParamValue );
    
    EUNIT_ASSERT( !MCESIPEventHelper::MatchEvents( *first, *second ) );
    
    // both have same id param
    first->SetParamL( idParam, idParamValue );
    EUNIT_ASSERT( MCESIPEventHelper::MatchEvents( *first, *second ) );
    
    // different id params
    second->SetParamL( idParam, idParamValue2 );
    EUNIT_ASSERT( !MCESIPEventHelper::MatchEvents( *first, *second ) );
   
    // first has id param, second has not.
    second->DeleteParam( idParam );
    EUNIT_ASSERT( !MCESIPEventHelper::MatchEvents( *first, *second ) );
    
    CleanupStack::PopAndDestroy( 2 ); // idParamValue, idParamValue2
    CleanupStack::PopAndDestroy( second );
    CleanupStack::PopAndDestroy( first );
    }
    
void UT_MCESIPEventHelper::UT_MCESIPEventHelper_MatchEventsLL()
    {
    /*
    const TDesC8& aEventPackage,
    TUint aIdValue,
    const CSIPEventHeader& aEvent 
    */
    TBool val( EFalse );
    CSIPEventHeader* event = CSIPEventHeader::NewLC( KReferEventPackage );
    
    RStringF idParam = SIPStrings::StringF( SipStrConsts::EId );
    RStringF idParamValue = SIPStrings::Pool().OpenFStringL( KIdParamValue );
    CleanupClosePushL( idParamValue );
    
    // event packages do not match
    val = MCESIPEventHelper::MatchEventsL( KEventPackage, KCSeqValue, *event );
    EUNIT_ASSERT( !val );
    
    // no id param in header
    val = MCESIPEventHelper::MatchEventsL( KReferEventPackage, KCSeqValue, *event );
    EUNIT_ASSERT( val );
    
    event->SetParamL( idParam, idParamValue );
    
    // aIdValue == KMceEventIdNotAssigned
    val = MCESIPEventHelper::MatchEventsL( KReferEventPackage, KMceEventIdNotAssigned, *event );
    EUNIT_ASSERT( !val );
    
    // !( idParamValue == aIdValue )
    val = MCESIPEventHelper::MatchEventsL( KReferEventPackage, KCSeqValue+3, *event );
    EUNIT_ASSERT( !val );
    
    // match
    val = MCESIPEventHelper::MatchEventsL( KReferEventPackage, KCSeqValue, *event );
    EUNIT_ASSERT( val );
    
    
    CleanupStack::PopAndDestroy();//idParamValue
    CleanupStack::PopAndDestroy( event );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateResponseLL()
    {
    /*
    CMceComEvent& aEvent, 
    TInt aResponseType,
    TUint aReason,
    TBool aAddClientData
    */
    RStringF contact = SIPStrings::StringF( SipStrConsts::EContactHeader );
    CSIPResponseElements* response = 
        MCESIPEventHelper::CreateResponseL( *iEvent, KMceSipOK );
    CleanupStack::PushL( response );
    EUNIT_ASSERT( response );
    EUNIT_ASSERT( response->ReasonPhrase() == SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
    EUNIT_ASSERT( response->StatusCode() == KMceSipOK );
    CSIPMessageElements& message = response->MessageElements();
    RPointerArray< CSIPHeaderBase > headers = message.UserHeaders();
    EUNIT_ASSERT_EQUALS( 0, headers.Count() );
    EUNIT_ASSERT( !MceSip::FindHeader( message, contact ) );;
    
    CleanupStack::PopAndDestroy( response );
    }

void UT_MCESIPEventHelper::UT_MCESIPEventHelper_CreateResponseL2L()
    {
    /*
    CMceComEvent& aEvent, 
    TInt aResponseType,
    TUint aReason,
    RStringF aPhrase,
    TBool aAddClientData )
    */
    RStringF contact = SIPStrings::StringF( SipStrConsts::EContactHeader );
    RStringF ok = SIPStrings::StringF( SipStrConsts::EPhraseOk );
    RStringF ringing = SIPStrings::StringF( SipStrConsts::EPhraseOk );
    
    // no client data, add contact
    CSIPResponseElements* response = MCESIPEventHelper::CreateResponseL( 
        *iEvent,
        KMceSipOK,
        ok );
    CleanupStack::PushL( response ); 
    EUNIT_ASSERT( response );
    EUNIT_ASSERT( response->ReasonPhrase() == ok );
    EUNIT_ASSERT( response->StatusCode() == KMceSipOK );
    CSIPMessageElements& message = response->MessageElements();
    RPointerArray< CSIPHeaderBase > headers = message.UserHeaders();
    EUNIT_ASSERT_EQUALS( 0, headers.Count() );
    EUNIT_ASSERT( !MceSip::FindHeader( message, contact ) );
    
    CleanupStack::PopAndDestroy( response );
      
    // client data add contact
    // hdrs
    CDesC8Array* hdrs = new ( ELeave ) CDesC8ArrayFlat(1);
    CleanupStack::PushL( hdrs );
    hdrs->AppendL( KModifyHeader );
    CleanupStack::Pop( hdrs );
    iEvent->SetMsgHeaders( hdrs );
    
    // content & content type
    HBufC8* content = KContent().AllocLC();
    HBufC8* contentType = KContentType().AllocLC();
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );

    
    response = MCESIPEventHelper::CreateResponseL( 
        *iEvent,
        KMceSipOK,
        ok,
        ETrue );    
    
    CleanupStack::PushL( response );
    EUNIT_ASSERT( response );
    EUNIT_ASSERT( response->ReasonPhrase() == ok );
    EUNIT_ASSERT( response->StatusCode() == KMceSipOK );
    
    CSIPMessageElements& message2 = response->MessageElements();
    headers = message2.UserHeaders();
    EUNIT_ASSERT_EQUALS( 1, headers.Count() );
    EUNIT_ASSERT( !MceSip::FindHeader( message2, contact ) );
    // First is the user header
    CSIPHeaderBase* hdr = headers[ 0 ];
    HBufC8* txtHdr = hdr->ToTextLC();
    EUNIT_ASSERT( txtHdr->Compare( KModifyHeader ) == 0 );
    CleanupStack::PopAndDestroy( txtHdr );
    
    EUNIT_ASSERT( message2.Content().Compare( KContent ) == 0 );
    EUNIT_ASSERT( message2.ContentType() );
    HBufC8* txtCt = message2.ContentType()->ToTextValueLC();
    EUNIT_ASSERT( txtCt->Compare( KContentType ) == 0 );
    CleanupStack::PopAndDestroy( txtCt );
    CleanupStack::PopAndDestroy( response );
    
    // no client date nor contact
    response = MCESIPEventHelper::CreateResponseL( 
        *iEvent,
        KMceSipRinging,
        ringing );    
    
    CleanupStack::PushL( response );
    EUNIT_ASSERT( response );
    EUNIT_ASSERT( response->ReasonPhrase() == ringing );
    EUNIT_ASSERT( response->StatusCode() == KMceSipRinging );
    CSIPMessageElements& message3 = response->MessageElements();
    EUNIT_ASSERT( message3.UserHeaders().Count() == 0);
    EUNIT_ASSERT( message3.Content().Length() == 0 );
    EUNIT_ASSERT( !message3.ContentType() );
    CleanupStack::PopAndDestroy( response );
    }
    
// callbacks
void UT_MCESIPEventHelper::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_MCESIPEventHelper::IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	if ( aReferTo )
	    delete aReferTo;
	
	if ( aHeaders )
	    delete aHeaders;
	
	if ( aContentType )
	    delete aContentType;
	
	if ( aBody )
	    delete aBody;	
	}
void UT_MCESIPEventHelper::EventStateChangedL(TUint32 /*aState*/)
	{
	}
		
void UT_MCESIPEventHelper::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_MCESIPEventHelper,
    "UT_MCESIPEventHelper",
    "UNIT" )

EUNIT_TEST(
    "SetContentL - test",
    "CMceComEventSipMsgHelper",
    "SetContentL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_SetContentLCL, Teardown)

EUNIT_TEST(
    "CreateMessageElementsLC - test 2",
    "CMceComEventSipMsgHelper",
    "CreateMessageElementsLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateMessageElementsLCL, Teardown)

EUNIT_TEST(
    "CreateMessageElementsLC - test 3",
    "CMceComEventSipMsgHelper",
    "CreateMessageElementsLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateMessageElementsLC2L, Teardown)

EUNIT_TEST(
    "CreateMessageElementsLC - test 4",
    "CMceComEventSipMsgHelper",
    "CreateMessageElementsLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateMessageElementsLC3L, Teardown)


EUNIT_TEST(
    "CreateNotifyMessageElementsLC - test ",
    "CMceComEventSipMsgHelper",
    "CreateNotifyMessageElementsLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateReferNotifyMessageElementsLCL, Teardown)

EUNIT_TEST(
    "HandleSubscriptionStateHeaderL - test",
    "CMceComEventSipMsgHelper",
    "HandleSubscriptionStateHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleSubscriptionStateHeaderLL, Teardown)

EUNIT_TEST(
    "HandleSubscriptionStateHeaderL - test 2",
    "CMceComEventSipMsgHelper",
    "HandleSubscriptionStateHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL2L, Teardown)

EUNIT_TEST(
    "HandleSubscriptionStateHeaderL - test 3",
    "CMceComEventSipMsgHelper",
    "HandleSubscriptionStateHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL3L, Teardown)

EUNIT_TEST(
    "HandleSubscriptionStateHeaderL - test 4",
    "CMceComEventSipMsgHelper",
    "HandleSubscriptionStateHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL4L, Teardown)

EUNIT_TEST(
    "HandleSubscriptionStateHeaderL - test 5",
    "CMceComEventSipMsgHelper",
    "HandleSubscriptionStateHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL5L, Teardown)


EUNIT_TEST(
    "NotifyRequestReceivedLC - test ",
    "CMceComEventSipMsgHelper",
    "NotifyRequestReceivedLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_NotifyRequestReceivedLCL, Teardown)

EUNIT_TEST(
    "NotifyRequestReceivedLC - test 2",
    "CMceComEventSipMsgHelper",
    "NotifyRequestReceivedLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_NotifyRequestReceivedLC2L, Teardown)

EUNIT_TEST(
    "NotifyRequestReceivedLC - test 3",
    "CMceComEventSipMsgHelper",
    "NotifyRequestReceivedLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_NotifyRequestReceivedLC3L, Teardown)

EUNIT_TEST(
    "ReferNotifyRequestReceivedLC - test ",
    "CMceComEventSipMsgHelper",
    "ReferNotifyRequestReceivedLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_ReferNotifyRequestReceivedLCL, Teardown)

/* comment out empty cases

EUNIT_TEST(
    "SubscribeRequestReceivedLC - test ",
    "CMceComEventSipMsgHelper",
    "SubscribeRequestReceivedLC",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_SubscribeRequestReceivedLCL, Teardown)
*/

EUNIT_TEST(
    "HandleExpireHeaderL - test ",
    "CMceComEventSipMsgHelper",
    "HandleExpireHeaderL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_HandleExpireHeaderLL, Teardown)

EUNIT_TEST(
    "ResolveReferTypeL - test - no suppression",
    "CMceComEventSipMsgHelper",
    "ResolveReferTypeL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_ResolveReferTypeLL, Teardown)

EUNIT_TEST(
    "ResolveReferTypeL - test - suppressed",
    "CMceComEventSipMsgHelper",
    "ResolveReferTypeL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_ResolveReferTypeL2L, Teardown)

EUNIT_TEST(
    "ResolveReferTypeL - test - multiple refer",
    "CMceComEventSipMsgHelper",
    "ResolveReferTypeL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_ResolveReferTypeL3L, Teardown)

EUNIT_TEST(
    "ResolveReferTypeL - test - suppression value true ",
    "CMceComEventSipMsgHelper",
    "ResolveReferTypeL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_ResolveReferTypeL4L, Teardown)

EUNIT_TEST(
    "MatchEvents - test ",
    "CMceComEventSipMsgHelper",
    "MatchEvents",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_MatchEventsL, Teardown)

EUNIT_TEST(
    "MatchEventsL - test",
    "CMceComEventSipMsgHelper",
    "MatchEventsL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_MatchEventsLL, Teardown)

EUNIT_TEST(
    "CreateResponseL - test",
    "CMceComEventSipMsgHelper",
    "CreateResponseL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateResponseLL, Teardown)
    
EUNIT_TEST(
    "CreateResponseL - test",
    "CMceComEventSipMsgHelper",
    "CreateResponseL",
    "FUNCTIONALITY",
    SetupL, UT_MCESIPEventHelper_CreateResponseL2L, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE