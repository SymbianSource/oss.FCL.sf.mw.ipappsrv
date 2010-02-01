/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation of CMccQosHandler
*
*/




// INCLUDES
#include <networking/qos3gpp_subconparams.h>
#include <es_enum.h>

#include "mccqoshandler.h"
#include "mccqossockethandler.h"
#include "mccqoseventmonitor.h"
#include "mccqoslogging.h"

// Constants for QoS parameters
const TUint KTransferDelay = 600;
const TUint KMaxSduSize = 1024;
const TUint KMaxBitrate = 66000;
const TUint KMaxGuaBitrate = 15200;

// ======== MEMBER FUNCTIONS =================================================


// ---------------------------------------------------------------------------
// CMccQosHandler::CMccQosHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMccQosHandler::CMccQosHandler( MMccQosEventObserver& aObserver ) : 
    iObserver( aObserver )
    {

    }


// ---------------------------------------------------------------------------
// CMccQosPluginImpl::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//
CMccQosHandler* CMccQosHandler::NewL( MMccQosEventObserver& aObserver )
    {
    __QOSLOG( "CMccQosHandler::NewL" )
    
    CMccQosHandler* self = new ( ELeave ) CMccQosHandler( aObserver );
    return self;
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::~CMccQosHandler
// Destructor.
// ---------------------------------------------------------------------------
//
CMccQosHandler::~CMccQosHandler()
    {
    __QOSLOG( "CMccQosHandler::~CMccQosHandler" )
    
    delete iEventMonitor;
    HandleSocketHandlerDeletion();
    iSubConnection.Close();
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::EnableQosL
// ---------------------------------------------------------------------------
//
void CMccQosHandler::EnableQosL()
    {
    __QOSLOG("CMccQosHandler::DoEnableQoSL")
    
    __ASSERT_ALWAYS( iConnection, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iSocketServ, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iSocket, User::Leave( KErrNotReady ) );
    
    // This makes sure that we have not created eventhandler nor
    // sockethandler. This way we stay in correct state.
    if ( QoSEnabled() )
        {
        __QOSLOG( "CMccQosHandler::DoEnableQoSL KErrInUse" )

        User::Leave( KErrInUse );
        }
        
    // Qos functionality is broken in current environments and prevents
    // sending packets to network. Therefore, feature is disabled at the
    // moment. Only supported when unit testing.        
#ifndef TEST_EUNIT
    __QOSLOG( "CMccQosHandler::DoEnableQoSL FEATURE DISABLED!" )
    User::Leave( KErrNotSupported ); 
#endif
        
    // Create a parameter bundle & container for QoS subconnection parameters
    RSubConParameterBundle subConparams;
    CleanupClosePushL( subConparams );
    
    // Parambundle takes ownership
    CSubConParameterFamily* family = 
        CSubConParameterFamily::NewL( subConparams, KSubConQoSFamily );
    
    // Create a requested technology specific parameter set for QoS
    // (Qos family takes ownership).
    CSubConQosR99ParamSet* reqParams( CSubConQosR99ParamSet::NewL( *family,
        CSubConParameterFamily::ERequested ) );
    
    // Create acceptable parameter set. (Qos family takes ownership).
    CSubConQosR99ParamSet* acpParams( CSubConQosR99ParamSet::NewL( *family,
        CSubConParameterFamily::EAcceptable ) );
    
    // Set the requested params, request conversational class.
    reqParams->SetTrafficClass(
        RPacketQoS::ETrafficClassConversational );
    
    reqParams->SetDeliveryOrder(
        RPacketQoS::EDeliveryOrderNotRequired );
        
    reqParams->SetErroneousSDUDelivery( 
        RPacketQoS::EErroneousSDUNoDetection );
        
    reqParams->SetResidualBitErrorRatio( 
        RPacketQoS::EBERUnspecified );
        
    reqParams->SetSDUErrorRatio( 
        RPacketQoS::ESDUErrorRatioUnspecified );
        
    reqParams->SetTrafficHandlingPriority( 
        RPacketQoS::ETrafficPriorityUnspecified );
    
    // Accept streaming class
    acpParams->SetTrafficClass(
        RPacketQoS::ETrafficClassStreaming );
    
    reqParams->SetTransferDelay( KTransferDelay );
    reqParams->SetMaxSduSize( KMaxSduSize );
    reqParams->SetMaxBitrateUplink( KMaxBitrate );
    reqParams->SetMaxBitrateDownlink( KMaxBitrate );
    reqParams->SetGuaBitrateUplink( KMaxGuaBitrate );
    reqParams->SetGuaBitrateDownlink( KMaxGuaBitrate );
    
    // Open the subconnection
    User::LeaveIfError( iSubConnection.Open( *iSocketServ,
        RSubConnection::ECreateNew, *iConnection ) );
    
    // Set QoS properties of the subconnection
    User::LeaveIfError( iSubConnection.SetParameters( subConparams ) );
    
    acpParams = NULL;
    reqParams = NULL;
    family = NULL;
    CleanupStack::PopAndDestroy(); // subConparams
    
    
    // Start the socket adding process to the subconnections
    CMccQosSocketHandler* sockHandler = CMccQosSocketHandler::NewLC( iObserver, 
        iSubConnection, *iSocket );
    
    // Start the eventmonitoring of the subconnection.
    CMccQosEventMonitor* evHandler = CMccQosEventMonitor::NewLC( iObserver,
        iSubConnection );
        
    iEventMonitor = evHandler;
    CleanupStack::Pop( evHandler );
    evHandler = NULL;
    
    iQosSocketHandler = sockHandler;
    CleanupStack::Pop( sockHandler );
    sockHandler = NULL;
    
    __QOSLOG("CMccQosHandler::DoEnableQoSL exit")
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::DisableQoSL
// ---------------------------------------------------------------------------
//  
void CMccQosHandler::DisableQoS()
    {
    __QOSLOG( "CMccQosHandler::DisableQoS" )
    
    HandleSocketHandlerDeletion();
    delete iEventMonitor;
    iSubConnection.Close();
    iQosSocketHandler = NULL;
    iEventMonitor = NULL;
    iConnection = NULL;
    iSocketServ = NULL;
    iSocket = NULL;
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::SetSubConnectionParams
// ---------------------------------------------------------------------------
//    
void CMccQosHandler::SetSubConnectionParams( RConnection& aConnection, 
    RSocketServ& aSocketServ, RSocket& aSocket )
    {
    __QOSLOG( "CMccQosHandler::SetSubConnectionParams" )

    iConnection = &aConnection;
    iSocketServ = &aSocketServ;
    iSocket = &aSocket;
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::QoSEnabled
// ---------------------------------------------------------------------------
//
TBool CMccQosHandler::QoSEnabled() const
    {
    if ( iQosSocketHandler && iEventMonitor )
        {
        __QOSLOG( "CMccQosHandler::QoSEnabled ETrue" )

        return ETrue;
        }
    else
        {
        __QOSLOG( "CMccQosHandler::QoSEnabled EFalse" )
        
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CMccQosHandler::HandleSocketHandlerDeletion
// ---------------------------------------------------------------------------
//
void CMccQosHandler::HandleSocketHandlerDeletion()
    {
    __QOSLOG("CMccQosHandler::HandleSocketHandlerDeletion" )
    
    if ( iQosSocketHandler && iQosSocketHandler->IsActive() )
        {
        iQosSocketHandler->AsyncDeleteNeeded( ETrue );
        }
    else
        {
        delete iQosSocketHandler;
        iQosSocketHandler = NULL;
        }
    }
