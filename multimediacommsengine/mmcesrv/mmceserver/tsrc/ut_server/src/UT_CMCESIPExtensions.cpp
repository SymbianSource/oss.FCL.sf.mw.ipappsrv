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
#include "ut_cmcesipextensions.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "ut_cmcesipextensions.h"
#include "MCETestHelper.h"
#include "mcesipextensions.h"
#include "mcedictionary.h"
#include "sipclienttransaction.h"
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <siprequireheader.h>
#include <sipsupportedheader.h>
#include <sipallowheader.h>

_LIT8( KUnKnownSupported, "MySupported" );
_LIT8( KUnKnownRequire, "MyRequire" );

#define MCE_ASSERT_LOCAL_EXTENSIONS( prack, qos, update, allowPrack, subscribe, refer, notify )\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::E100rel ) == prack );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::EQosPreconds ) == qos );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::EUpdate ) == update );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::EPrack ) == allowPrack );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::ESubscribe ) == subscribe );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::ERefer ) == refer );\
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::ENotify ) == notify );
    
#define MCE_ASSERT_REMOTE_EXTENSIONS( prack, qos, update, allowPrack, subscribe, refer, notify )\
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::E100rel ) == prack );\
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::EQosPreconds ) == qos );\
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::EUpdate ) == update );\
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::EPrack ) == allowPrack );\
	EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::ESubscribe ) == subscribe );\
	EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::ERefer ) == refer );\
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::ENotify ) == notify );
    
#define MCE_RESET_EXTENSIONS()\
    iExtensions->SetLocal( CMceSipExtensions::E100rel, CMceSipExtensions::ESupported );\
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ESupported );\
    iExtensions->SetLocal( CMceSipExtensions::EUpdate, CMceSipExtensions::ESupported );\
    iExtensions->SetLocal( CMceSipExtensions::EPrack, CMceSipExtensions::ESupported );\
	iExtensions->SetLocal( CMceSipExtensions::ESubscribe, CMceSipExtensions::ESupported );\
	iExtensions->SetLocal( CMceSipExtensions::ERefer, CMceSipExtensions::ESupported );\
	iExtensions->SetLocal( CMceSipExtensions::ENotify, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::E100rel, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::EUpdate, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::EPrack, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::ESubscribe, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::ERefer, CMceSipExtensions::ESupported );\
    iExtensions->SetRemote( CMceSipExtensions::ENotify, CMceSipExtensions::ESupported );

    
// CONSTRUCTION
UT_CMceSipExtensions* UT_CMceSipExtensions::NewL()
    {
    UT_CMceSipExtensions* self = UT_CMceSipExtensions::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipExtensions* UT_CMceSipExtensions::NewLC()
    {
    UT_CMceSipExtensions* self = new( ELeave ) UT_CMceSipExtensions();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipExtensions::~UT_CMceSipExtensions()
    {
    }

// Default constructor
UT_CMceSipExtensions::UT_CMceSipExtensions()
    {
    }

// Second phase construct
void UT_CMceSipExtensions::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipExtensions::SetupL(  )
    {
    SIPStrings::OpenL();
    iExtensions = CMceSipExtensions::NewL();
    } 

void UT_CMceSipExtensions::Teardown(  )
    {
    delete iExtensions;
    SIPStrings::Close();
    }

void UT_CMceSipExtensions::UT_CMceSipExtensions_LocalL()
    {
    CMceKeyTValuePair<CMceSipExtensions::TLevel>* item = 
        static_cast<CMceKeyTValuePair<CMceSipExtensions::TLevel>*>
        ( iExtensions->iLocal->Pair( CMceSipExtensions::E100rel ) );
    EUNIT_ASSERT( item );
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::E100rel )
        == item->Value() );
    
    // remove one item, to test not finding
    iExtensions->iLocal->Delete( CMceSipExtensions::E100rel );
    EUNIT_ASSERT( iExtensions->Local( CMceSipExtensions::E100rel ) ==
        CMceSipExtensions::ENotSupported );
    }  

void UT_CMceSipExtensions::UT_CMceSipExtensions_RemoteL()
    {
    CMceKeyTValuePair<CMceSipExtensions::TLevel>* item = 
        static_cast<CMceKeyTValuePair<CMceSipExtensions::TLevel>*>
        ( iExtensions->iRemote->Pair( CMceSipExtensions::EUpdate ) );
    EUNIT_ASSERT( item );
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::EUpdate )
        == item->Value() );

    // remove one item, to test not finding
    iExtensions->iRemote->Delete( CMceSipExtensions::E100rel );
    EUNIT_ASSERT( iExtensions->Remote( CMceSipExtensions::E100rel ) ==
        CMceSipExtensions::ENotSupported );
    }
         
void UT_CMceSipExtensions::UT_CMceSipExtensions_SetLocalL()
    {
    CMceKeyTValuePair<CMceSipExtensions::TLevel>* item = 
        static_cast<CMceKeyTValuePair<CMceSipExtensions::TLevel>*>
        ( iExtensions->iLocal->Pair( CMceSipExtensions::EQosPreconds ) );
    EUNIT_ASSERT( item );
    item->SetValue( CMceSipExtensions::ENotSupported );
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, 
                           CMceSipExtensions::ERequired );
    EUNIT_ASSERT( item->Value() == CMceSipExtensions::ERequired );    
    
    // remove one item, to test not finding
    iExtensions->iLocal->Delete( CMceSipExtensions::E100rel );
    iExtensions->SetLocal( CMceSipExtensions::E100rel, 
                           CMceSipExtensions::ESupported );
                       
    }
        
void UT_CMceSipExtensions::UT_CMceSipExtensions_SetRemoteL()
    {
    CMceKeyTValuePair<CMceSipExtensions::TLevel>* item = 
        static_cast<CMceKeyTValuePair<CMceSipExtensions::TLevel>*>
        ( iExtensions->iRemote->Pair( CMceSipExtensions::E100rel ) );
    EUNIT_ASSERT( item );
    item->SetValue( CMceSipExtensions::ENotSupported );
    iExtensions->SetRemote( CMceSipExtensions::E100rel, 
                           CMceSipExtensions::ERequired );
    EUNIT_ASSERT( item->Value() == CMceSipExtensions::ERequired );    
    
    // remove one item, to test not finding
    iExtensions->iRemote->Delete( CMceSipExtensions::E100rel );
    iExtensions->SetRemote( CMceSipExtensions::E100rel, 
                           CMceSipExtensions::ESupported );
    }

void UT_CMceSipExtensions::UT_CMceSipExtensions_LevelLL()
    {
    /*
    CSIPTransactionBase& aTransaction,
    CMceSipExtensions::TExtension aExtension )
    */
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    // no message elements
    RStringF str = SIPStrings::StringF( SipStrConsts::EInvite );
    CSIPTransactionBase* tran = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( tran );
    CMceSipExtensions::TLevel result = iExtensions->LevelL( *tran, CMceSipExtensions::E100rel );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // 100rel
    // NOP
    // no headers
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, 
                                              hdrs );
    CleanupStack::PushL( tran );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::E100rel );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // unknown extensions                                               
    RStringF unknownSuppStr = SIPStrings::Pool().OpenFStringL( KUnKnownSupported );
    CleanupClosePushL( unknownSuppStr );
    CSIPSupportedHeader* unknownSupp = CSIPSupportedHeader::NewLC( unknownSuppStr );
    hdrs.AppendL( unknownSupp );
    CleanupStack::Pop( unknownSupp );
    CleanupStack::PopAndDestroy(); //unknownSuppStr
    
    RStringF unknownReqStr = SIPStrings::Pool().OpenFStringL( KUnKnownRequire );
    CleanupClosePushL( unknownReqStr );
    CSIPRequireHeader* unknownReq = CSIPRequireHeader::NewLC( unknownReqStr );
    hdrs.AppendL( unknownReq );
    CleanupStack::Pop( unknownReq );
    CleanupStack::PopAndDestroy(); // unknownReqStr
    
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
   
    
    result = iExtensions->LevelL( *tran, CMceSipExtensions::E100rel );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // Supported:100rel
    RStringF supp100RelStr = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
    CleanupClosePushL( supp100RelStr );
    CSIPSupportedHeader* supp100Rel = CSIPSupportedHeader::NewLC( supp100RelStr );
    hdrs.AppendL( supp100Rel );
    CleanupStack::Pop( supp100Rel );
    CleanupStack::PopAndDestroy(); // supp100RelStr
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, 
                                              hdrs );
   
    CleanupStack::PushL( tran );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::E100rel );
    EUNIT_ASSERT( result == CMceSipExtensions::ESupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // Require:100rel
    RStringF req100RelStr = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
    CleanupClosePushL( req100RelStr );
    CSIPRequireHeader* require = CSIPRequireHeader::NewLC( req100RelStr );
    hdrs.AppendL( require );
    CleanupStack::Pop( require );
    CleanupStack::PopAndDestroy(); // req100RelStr
    
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
    
    result = iExtensions->LevelL( *tran, CMceSipExtensions::E100rel );
    EUNIT_ASSERT( result == CMceSipExtensions::ERequired );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    
    // EQosPreconds
    // NOP
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, 
                                              hdrs );
    CleanupStack::PushL( tran );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::EQosPreconds );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // Supported
    RStringF suppQosStr = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( suppQosStr );
    CSIPSupportedHeader* suppQos = CSIPSupportedHeader::NewLC( suppQosStr );
    hdrs.AppendL( suppQos );
    CleanupStack::Pop( suppQos );
    CleanupStack::PopAndDestroy(); // suppQosStr
    
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
    
    result = iExtensions->LevelL( *tran, CMceSipExtensions::EQosPreconds );
    EUNIT_ASSERT( result == CMceSipExtensions::ESupported );
    CleanupStack::PopAndDestroy( tran );
    tran  = NULL;
    
    // Required
    RStringF reqQosStr = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( reqQosStr );
    CSIPRequireHeader* reqQos = CSIPRequireHeader::NewLC( reqQosStr );
    hdrs.AppendL( reqQos );
    CleanupStack::Pop( reqQos );
    CleanupStack::PopAndDestroy(); // reqQosStr
    
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, 
                                              hdrs );
    CleanupStack::PushL( tran );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::EQosPreconds );
    EUNIT_ASSERT( result == CMceSipExtensions::ERequired );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // UPDATE
    // Not supported
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::EUpdate );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran  = NULL;
    
    // Supported
    RStringF updateStr = SIPStrings::StringF( SipStrConsts::EUpdate );
    CSIPAllowHeader* allowUpdate = CSIPAllowHeader::NewLC( updateStr );
    hdrs.AppendL( allowUpdate );
    CleanupStack::Pop( allowUpdate );
    
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, 
                                              hdrs );
    
    CleanupStack::PushL( tran );
    result = iExtensions->LevelL( *tran, CMceSipExtensions::EUpdate );
    EUNIT_ASSERT( result == CMceSipExtensions::ESupported );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    // unknown
    
    CMceSipExtensions::TExtension unknown = static_cast< CMceSipExtensions::TExtension >( KErrNotSupported );
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
    
    result = iExtensions->LevelL( *tran, unknown );
    EUNIT_ASSERT( result == CMceSipExtensions::ENotSupported );
    CleanupStack::PopAndDestroy( tran );
    tran  = NULL;
    
    CleanupStack::PopAndDestroy( &hdrs );
    }
   
void UT_CMceSipExtensions::UT_CMceSipExtensions_UpdateLL()
    {
    // server transaction
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    // no message elements
    CSIPServerTransaction* tran = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );

    CleanupStack::PushL( tran );
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported/*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
            
    // local: 100rel = supported, preconds = supported
    // remote: 100rel = not supported, preconds not supported
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite,
                                              hdrs );
    CleanupStack::PushL( tran );
    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported/*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/  ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
                                 
    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ENotSupported /*prack*/, 
                                  CMceSipExtensions::ENotSupported /*qos*/, 
                                  CMceSipExtensions::ENotSupported /*update*/  ,
                                 CMceSipExtensions::ENotSupported /*allowprack*/,
                                 CMceSipExtensions::ENotSupported /*subscribe*/,
                                 CMceSipExtensions::ENotSupported /*refer*/,
                                 CMceSipExtensions::ENotSupported /*notify*/ );
                                 
    // Remote requires 100rel but locally not supported.
    iExtensions->SetLocal( CMceSipExtensions::E100rel, CMceSipExtensions::ENotSupported );
    RStringF req100RelStr = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
    CleanupClosePushL( req100RelStr );
    CSIPRequireHeader* req100Rel = CSIPRequireHeader::NewLC( req100RelStr );
    hdrs.AppendL( req100Rel );
    CleanupStack::Pop( req100Rel );
    CleanupStack::PopAndDestroy(); // req100RelStr
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite,
                                              hdrs );
    CleanupStack::PushL( tran );
    
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ENotSupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/  ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // Remote requires qos preconds but locally not supported.
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ENotSupported );
    RStringF reqQosStr = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( reqQosStr );
    CSIPRequireHeader* reqQos = CSIPRequireHeader::NewLC( reqQosStr );
    hdrs.AppendL( reqQos );
    CleanupStack::Pop( reqQos );
    CleanupStack::PopAndDestroy(); // reqQosStr
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite,
                                              hdrs );
    CleanupStack::PushL( tran );
    
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ENotSupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/  ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
                                     
    MCE_RESET_EXTENSIONS();
    
    // Remote requires 100rel && qos preconds, and allows update
    RStringF req100RelStr2 = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
    CleanupClosePushL( req100RelStr2 );
    CSIPRequireHeader* req100Rel2 = CSIPRequireHeader::NewLC( req100RelStr2 );
    hdrs.AppendL( req100Rel2 );
    CleanupStack::Pop( req100Rel2 );
    CleanupStack::PopAndDestroy(); // req100RelStr2
    
    RStringF reqQosStr2 = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( reqQosStr2 );
    CSIPRequireHeader* reqQos2 = CSIPRequireHeader::NewLC( reqQosStr2 );
    hdrs.AppendL( reqQos2 );
    CleanupStack::Pop( reqQos2 );
    CleanupStack::PopAndDestroy(); // reqQosStr2
    
    RStringF updateStr = SIPStrings::StringF( SipStrConsts::EUpdate );
    CSIPAllowHeader* allowUpdate = CSIPAllowHeader::NewLC( updateStr );
    hdrs.AppendL( allowUpdate );
    CleanupStack::Pop( allowUpdate );
    
    tran = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite,
                                              hdrs );
    CleanupStack::PushL( tran );
    
    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ERequired /*prack*/, 
                                 CMceSipExtensions::ERequired /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ERequired /*prack*/, 
                                  CMceSipExtensions::ERequired /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ENotSupported /*allowprack*/,
                                 CMceSipExtensions::ENotSupported /*subscribe*/,
                                 CMceSipExtensions::ENotSupported /*refer*/,
                                 CMceSipExtensions::ENotSupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    CleanupStack::PopAndDestroy( &hdrs );
    }

void UT_CMceSipExtensions::UT_CMceSipExtensions_UpdateL2L()
    {
    // client transaction 
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    // No message
    CSIPClientTransaction* tran = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    MCE_DELETE( tran );
    
    // NOP -- KMceSipBadRequest
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipBadRequest, 
                                               SipStrConsts::EPhraseBadRequest );

    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // 1xx UPDATE not allowed
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipRinging, 
                                               SipStrConsts::EPhraseRinging );
                                               
    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ENotSupported /*update*/ ,
                                 CMceSipExtensions::ENotSupported /*allowprack*/,
                                 CMceSipExtensions::ENotSupported /*subscribe*/,
                                 CMceSipExtensions::ENotSupported /*refer*/,
                                 CMceSipExtensions::ENotSupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // 1xx, 100rel locally required, but not in 1xx
    iExtensions->SetLocal( CMceSipExtensions::E100rel, CMceSipExtensions::ERequired );
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipRinging, 
                                               SipStrConsts::EPhraseRinging );
                                               
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ERequired /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // 2xx UPDATE allowed
    RStringF update = SIPStrings::StringF( SipStrConsts::EUpdate );
    CSIPAllowHeader* allowUpdate = CSIPAllowHeader::NewLC( update );
    hdrs.AppendL( allowUpdate );
    CleanupStack::Pop( allowUpdate );
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipOK, 
                                               SipStrConsts::EPhraseOk );
    
    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ENotSupported /*allowprack*/,
                                 CMceSipExtensions::ENotSupported /*subscribe*/,
                                 CMceSipExtensions::ENotSupported /*refer*/,
                                 CMceSipExtensions::ENotSupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    
    // KMceSipExtensionRequired, no preconds required
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipExtensionRequired, 
                                               SipStrConsts::EPhraseExtensionRequired );
    // There will leave when don't have require header field.
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // KMceSipExtensionRequired, preconds required, locally supported
    RStringF reqQosStr = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( reqQosStr );
    CSIPRequireHeader* reqQos = CSIPRequireHeader::NewLC( reqQosStr );
    hdrs.AppendL( reqQos );
    CleanupStack::Pop( reqQos );
    CleanupStack::PopAndDestroy(); // reqQosStr
    
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipExtensionRequired, 
                                               SipStrConsts::EPhraseExtensionRequired );
    
    iExtensions->UpdateL( *tran );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ERequired /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    // KMceSipExtensionRequired, preconds required, locally supported
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ENotSupported );
    iExtensions->SetLocal( CMceSipExtensions::ESecPreconds, CMceSipExtensions::ENotSupported );
    RStringF reqQosStr2 = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
    CleanupClosePushL( reqQosStr2 );
    CSIPRequireHeader* reqQos2 = CSIPRequireHeader::NewLC( reqQosStr2 );
    hdrs.AppendL( reqQos2 );
    CleanupStack::Pop( reqQos2 );
    CleanupStack::PopAndDestroy(); // reqQosStr2
    
    tran = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, 
                                               hdrs, 
                                               KMceSipExtensionRequired, 
                                               SipStrConsts::EPhraseExtensionRequired );
    
    EUNIT_ASSERT_LEAVE( iExtensions->UpdateL( *tran ) );
    CleanupStack::PopAndDestroy( tran );
    tran = NULL;
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ENotSupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    MCE_ASSERT_REMOTE_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                  CMceSipExtensions::ESupported /*qos*/, 
                                  CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();
    
    CleanupStack::PopAndDestroy( &hdrs );
    }
    
void UT_CMceSipExtensions::UT_CMceSipExtensions_RemoteIMSRel5L()
    {
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ENotSupported );
    EUNIT_ASSERT( !iExtensions->RemoteIMSRel5() );
    
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ESupported );
    EUNIT_ASSERT( !iExtensions->RemoteIMSRel5() );
    
    iExtensions->SetLocal( CMceSipExtensions::EQosPreconds, CMceSipExtensions::ERequired );
    EUNIT_ASSERT( iExtensions->RemoteIMSRel5() );
    }

void UT_CMceSipExtensions::UT_CMceSipExtensions_UpdateL()
    {
    /*
    TMceSessionModifier aModifier, 
    TUint aValue
    */
    // KMcePreconditions
    // KMcePreconditionsSupported
    iExtensions->Update( KMcePreconditions, KMcePreconditionsSupported );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    
    // KMcePreconditions
    // KMcePreconditionsNotUsed
    iExtensions->Update( KMcePreconditions, KMcePreconditionsNotUsed );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ENotSupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();                                   
    
    // KMcePreconditions
    // Unknown
    iExtensions->Update( KMcePreconditions, ( TUint )( KErrNotFound ) );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );

    // KMce100Rel
    // KMcePreconditionsSupported
    iExtensions->Update( KMce100Rel, KMcePreconditionsSupported );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    
    // KMce100Rel
    // KMcePreconditionsNotUsed
    iExtensions->Update( KMce100Rel, KMcePreconditionsNotUsed );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ENotSupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    MCE_RESET_EXTENSIONS();                                   
                                       
    // KMce100Rel
    // Unknown
    iExtensions->Update( KMce100Rel, ( TUint )( KErrNotFound ) );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    
    // default
    iExtensions->Update( ( TMceSessionModifier )( KErrNotFound ), 
                         ( TUint )( KErrNotFound ) );
    MCE_ASSERT_LOCAL_EXTENSIONS( CMceSipExtensions::ESupported /*prack*/, 
                                 CMceSipExtensions::ESupported /*qos*/, 
                                 CMceSipExtensions::ESupported /*update*/ ,
                                 CMceSipExtensions::ESupported /*allowprack*/,
                                 CMceSipExtensions::ESupported /*subscribe*/,
                                 CMceSipExtensions::ESupported /*refer*/,
                                 CMceSipExtensions::ESupported /*notify*/ );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipExtensions,
    "UT_CMceSipExtensions",
    "UNIT" )

EUNIT_TEST(
    "Local - method ",
    "CMceSipExtensions",
    "Local",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_LocalL, Teardown)

EUNIT_TEST(
    "Remote - method ",
    "CMceSipExtensions",
    "Remote",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_RemoteL, Teardown)

EUNIT_TEST(
    "SetLocal - method ",
    "CMceSipExtensions",
    "SetLocal",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_SetLocalL, Teardown)

EUNIT_TEST(
    "SetRemote - method ",
    "CMceSipExtensions",
    "SetRemote",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_SetRemoteL, Teardown)

EUNIT_TEST(
    "Level - method ",
    "CMceSipExtensions",
    "Level",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_LevelLL, Teardown)

EUNIT_TEST(
    "UpdateL - method, sta ",
    "CMceSipExtensions",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_UpdateLL, Teardown)

EUNIT_TEST(
    "UpdateL - method, cta ",
    "CMceSipExtensions",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_UpdateL2L, Teardown)

EUNIT_TEST(
    "RemoteIMSRel5 - method ",
    "CMceSipExtensions",
    "RemoteIMSRel5",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_RemoteIMSRel5L, Teardown)
    
EUNIT_TEST(
    "Update - method, session modifier ",
    "CMceSipExtensions",
    "Update",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipExtensions_UpdateL, Teardown)
    
   
EUNIT_END_TEST_TABLE

//  END OF FILE
