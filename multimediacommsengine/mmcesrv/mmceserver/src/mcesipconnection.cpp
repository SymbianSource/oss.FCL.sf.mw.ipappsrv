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




#include <badesca.h>
#include <in_pkt.h>
#include <sip.h>
#include <sipprofile.h>
#include <sipdialog.h>
#include <sipinvitedialogassoc.h>
#include <sipcontactheader.h>
#include <sipprofileregistry.h>
#include "mcesip.h"
#include "mcesrvlogs.h"

#include "mcesipconnection.h"
#include "mcesipmanager.h"
#include "mcesipsession.h"
#include "mcecssessionimplementation.h"
#include "mcelocaladdrresolver.h"

//const TInt KMaxAddressLength = 256;
    
// -----------------------------------------------------------------------------
// CMceSipConnection::NewLC
// -----------------------------------------------------------------------------
//

CMceSipConnection* CMceSipConnection::NewLC( 
    CMceSipManager& aSessionManager,
    CSIPProfile& aProfile,
    CSIPProfileRegistry& aProfileRegistry )
    {
    CMceSipConnection* self = new (ELeave) CMceSipConnection( aSessionManager,
    														  aProfile );
    CleanupStack::PushL( self );
    self->ConstructL( aProfile, aProfileRegistry );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::NewLC
// -----------------------------------------------------------------------------
//

CMceSipConnection* CMceSipConnection::NewLC( CMceSipManager& aSessionManager, 
                                             TUint32 aIAPId )
    {
    CMceSipConnection* self = new (ELeave) CMceSipConnection( aSessionManager, 
                                                              aIAPId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

#define MCE_SIP_CONNECTION_UNUSABLE( connection )\
    (connection->State() == CSIPConnection::EInactive ||\
     connection->State() == CSIPConnection::EUnavailable )

// -----------------------------------------------------------------------------
// CMceSipConnection::~CMceSipConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection::~CMceSipConnection()
    {
    MCESRV_DEBUG("CMceSipConnection::CMceSipConnection, Entry")  
    
    delete iClientConnection;
    iSessions.Reset();
    iSessions.Close();
    
    delete iLocalAddrResolver;
    MCESRV_DEBUG("CMceSipConnection::CMceSipConnection, Exit")   
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::Connection
// -----------------------------------------------------------------------------
//
CSIPConnection& CMceSipConnection::Connection() const
    {
    return *iClientConnection;
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::State
// -----------------------------------------------------------------------------
//
CSIPConnection::TState CMceSipConnection::State() const
    {
    return iClientConnection->State();
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::AccessPointId
// -----------------------------------------------------------------------------
//
TUint32 CMceSipConnection::AccessPointId() const
    {
    return iIAPId;
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::AttachL
// -----------------------------------------------------------------------------
//
void CMceSipConnection::AttachL( CMceCsSubSession& aSession )
    {
    User::LeaveIfError( MCE_SIP_CONNECTION_UNUSABLE( iClientConnection ) ? 
                        KErrNotReady : KErrNone );
    User::LeaveIfError( 
        iSessions.Find( &aSession ) < 0 ? KErrNone : KErrArgument );
    
    iSessions.AppendL( &aSession );  
    }
    
// -----------------------------------------------------------------------------
// CMceSipConnection::Detach
// -----------------------------------------------------------------------------
//
TInt CMceSipConnection::Detach( CMceCsSubSession& aSession )
    {
    
    TInt index = iSessions.Find( &aSession );
    
    if ( index >= KErrNone )
        {
        iSessions.Remove( index );
        }
    
    return iSessions.Count();
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::CMceSipConnection
// -----------------------------------------------------------------------------
//

CMceSipConnection::CMceSipConnection ( CMceSipManager& aSessionManager, 
                                       CSIPProfile& aProfile )
   : iIAPId( (TUint32)KErrNotFound ),
     iSessionManager( aSessionManager )
    {
    aProfile.GetParameter( KSIPAccessPointId, iIAPId );
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::CMceSipConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection::CMceSipConnection ( CMceSipManager& aSessionManager, 
                                       TUint32 aIAPId )
   : iIAPId( aIAPId ),
     iSessionManager( aSessionManager )
    {
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ConstructL
// -----------------------------------------------------------------------------
// 
void CMceSipConnection::ConstructL( CSIPProfile& aProfile, 
									CSIPProfileRegistry& aProfileRegistry )
    {
    MCESRV_DEBUG("CMceSipConnection::ConstructL, Entry")
	
	if ( !aProfileRegistry.IsEnabled( aProfile ) )
	    {
        MCESRV_DEBUG("CMceSipConnection::ConstructL, enabling profile")
        aProfileRegistry.EnableL( aProfile, *this );
        }

	iClientConnection = aProfileRegistry.ConnectionL( aProfile );
	
    iLocalAddrResolver = 
        CMceLocalAddrResolver::NewL( 
            iSessionManager.SocketServ(), 
            AccessPointId() );
	
    MCESRV_DEBUG("CMceSipConnection::ConstructL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ConstructL
// -----------------------------------------------------------------------------
// 
void CMceSipConnection::ConstructL( )
    {
    MCESRV_DEBUG("CMceSipConnection::ConstructL, Entry")

    MCESRV_DEBUG("CMceSipConnection::ConstructL, new connection")
    
    iClientConnection = 
        CSIPConnection::NewL( iSessionManager.SIPClient(), 
                              AccessPointId(),
                              *this );
                              
    iLocalAddrResolver = 
        CMceLocalAddrResolver::NewL( 
            iSessionManager.SocketServ(), 
            AccessPointId() );
	
    MCESRV_DEBUG("CMceSipConnection::ConstructL, Exit")
    }    
    
// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingRequest
// -----------------------------------------------------------------------------
//
void CMceSipConnection::IncomingRequest( 
    CSIPServerTransaction* aTransaction )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingRequest( sa ), Entry")
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( *aTransaction ).DesC() )
    TInt error = KErrNone;
    TBool sessionConsumed = EFalse;
    TBool requestConsumed = EFalse;
    
    sessionConsumed = iSessionManager.Consumes( *aTransaction );
    if ( sessionConsumed )
        {
        TRAP( error, iSessionManager.CreateSubSessionL( aTransaction, *this ) );
        requestConsumed = error ? EFalse : ETrue;
        }
    else
        {
        CMceCsSubSession* session = NULL;
        TInt index = 0;
        while ( !requestConsumed && index < iSessions.Count() )
            {
            session = iSessions[ index++ ];
            sessionConsumed = session->Consumes( *aTransaction );
            if ( sessionConsumed )
                {
                requestConsumed = session->MessageRequestReceived( aTransaction );
                }
            }
        }
    
    if ( !sessionConsumed )
        {
        MCESRV_DEBUG("IncomingRequest( sa ): not consumed ->discard")
        MceSip::DiscardRequest( aTransaction, KMceSipNotImplemented );
        }
    else if ( !requestConsumed || error )
        {
        MCESRV_DEBUG("IncomingRequest( sa ): error->discard")
        MceSip::DiscardRequest( aTransaction );
        }
    else
        {
        // NOP
        }

    iSessionManager.Cleanup();
        
    MCESRV_DEBUG("CMceSipConnection::IncomingRequest( sa ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingRequest
// -----------------------------------------------------------------------------
//
void CMceSipConnection::IncomingRequest ( 
    CSIPServerTransaction* aTransaction,
    CSIPDialog& aDialog )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingRequest( inside dialog ), Entry")
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( *aTransaction ).DesC() )
    CMceCsSubSession* session = NULL;
    CMceCsSubSession* assocSession = NULL;
    TBool sessionConsumed = EFalse;
    TBool requestConsumed = EFalse;
    TInt index = 0;
    
    while ( !requestConsumed && index < iSessions.Count() )
        {
        TBool assocDialog( EFalse );
        session = iSessions[ index++ ];

        sessionConsumed = session->Consumes( aDialog, *aTransaction, assocDialog );
        if ( sessionConsumed )
            {
            requestConsumed = session->DialogRequestReceived( aTransaction, aDialog );
            }
            
        if ( assocDialog && !assocSession )
            {
            assocSession = session;
            }
        }
        
    TInt error = KErrNone;  
    if( !requestConsumed )
        {
        index = 0;
        while ( !requestConsumed && index < iSessions.Count() )
            {
            session = iSessions[ index++ ];
            if ( !session->Client().IsOrphan() && 
                  session->Dialog()->Dialog() == aDialog )
                {
                TRAP( error, iSessionManager.CreateSubSessionL( 
                                        aTransaction, 
                                        session->Client().ClientSession(), 
                                        *this, 
                                        assocSession ) );
                sessionConsumed = error ? EFalse : ETrue;
                requestConsumed = sessionConsumed;   
                }
            }
        }
    
    if ( !sessionConsumed )
        {
        // Unknown method
        MCESRV_DEBUG("IncomingRequest: not consumed -> discard")
        MceSip::DiscardRequest( aTransaction, KMceSipNotImplemented );
        }
    else if ( !requestConsumed || error )
        {
        MCESRV_DEBUG("IncomingRequest: error -> discard")
        MceSip::DiscardRequest( aTransaction );
        }
    else
        {
        // NOP
        }
    iSessionManager.Cleanup();
        
    MCESRV_DEBUG("CMceSipConnection::IncomingRequest( inside dialog ), Exit")      
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingResponse
// -----------------------------------------------------------------------------
//
void CMceSipConnection::IncomingResponse(
    CSIPClientTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( standalone ), Entry")
    MCESRV_DEBUG_DVALUE("response", MceSip::ResponseCode( aTransaction ) )
    MCESRV_DEBUG_SVALUE("to", aTransaction.Type().DesC() )
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        consumed = session->Consumes( aTransaction );
        if ( consumed )
            {
            session->MessageResponseReceived( aTransaction );
            }
        }
        
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( standalone ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingResponse
// -----------------------------------------------------------------------------
//
void CMceSipConnection::IncomingResponse(
    CSIPClientTransaction& aTransaction,
    CSIPDialogAssocBase& aDialogAssoc )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( inside dialog ), Entry")
    MCESRV_DEBUG_DVALUE("response", MceSip::ResponseCode( aTransaction ) )
    MCESRV_DEBUG_SVALUE("to", aTransaction.Type().DesC() )
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        if ( session->Consumes( aTransaction ) )
            {
            consumed = ETrue;
            session->ResponseReceivedWithinDialog( aTransaction, aDialogAssoc );
            }
        }
        
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( inside dialog ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingResponse
// -----------------------------------------------------------------------------
//
void CMceSipConnection::IncomingResponse (
    CSIPClientTransaction& aTransaction,
    CSIPInviteDialogAssoc* aDialogAssoc )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( forked ), Entry")
    MCESRV_DEBUG_DVALUE("response", MceSip::ResponseCode( aTransaction ) )
    MCESRV_DEBUG_SVALUE("to", aTransaction.Type().DesC() )
    
    // Find the session
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    TInt error = KErrNone;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        if ( session->Consumes( aTransaction ) && 
             session->Type() == CMceCsSubSession::EOutSIPSession )
            {
            consumed = ETrue;
            TRAP( error, static_cast<CMceSipSession*>( session )->
                  InviteForkedL( aTransaction, aDialogAssoc ) );
            }
        }
    
    if ( error != KErrNone || !consumed )
        {
        Reject( aDialogAssoc );
        }
        
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( forked ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IncomingResponse
// -----------------------------------------------------------------------------
//
void
CMceSipConnection::IncomingResponse( 
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
    {
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( reg ), Entry")
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( reg ), NOT IMPLEMENTED")
    //NOT IMPLEMENTED
    MCESRV_DEBUG("CMceSipConnection::IncomingResponse( reg ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured( 
    TInt aError,
    CSIPTransactionBase& aTransaction )
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( standalone ), Entry")
    MCESRV_DEBUG_DVALUE("error", aError )
    
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        consumed = session->Consumes( aTransaction );
        if ( consumed )
            {
            session->ErrorOccured( aError, aTransaction );
            }
        }
    
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( standalone ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured (
	TInt aError,
    CSIPTransactionBase& aTransaction,
	CSIPDialogAssocBase& aDialogAssoc)
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( inside dialog ), Entry")
    MCESRV_DEBUG_DVALUE("error", aError )
    
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        if ( session->Dialog() == &aDialogAssoc &&
             session->Consumes( aTransaction ) )
            {
            consumed = ETrue;
            session->ErrorOccured( aError, aTransaction );
            }
        }
        
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured(inside dialog ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured(
    TInt aError,			
    CSIPDialogAssocBase& aDialogAssoc )
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( dialog ), Entry")
    MCESRV_DEBUG_DVALUE("error", aError )
    
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        if ( session->Dialog() == &aDialogAssoc )
            {
            session->ErrorOccured( aError );
            consumed = ETrue;
            }
        }
    
    iSessionManager.Cleanup();
    
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( dialog ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::InviteCompleted
// -----------------------------------------------------------------------------
//
void CMceSipConnection::InviteCompleted( 
    CSIPClientTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceSipConnection::InviteCompleted, Entry")
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        consumed = session->Consumes( aTransaction );
        if ( consumed )
            {
            session->InviteCompleted( aTransaction );
            }
        }
        
    MCESRV_DEBUG("CMceSipConnection::InviteCompleted, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::InviteCanceled
// -----------------------------------------------------------------------------
//
void CMceSipConnection::InviteCanceled(
    CSIPServerTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceSipConnection::InviteCanceled, Entry")
    CMceCsSubSession* session = NULL;
    TBool consumed = EFalse;    
    TInt index = 0;
    
    while ( !consumed && index < iSessions.Count() )
        {
        session = iSessions[ index++ ];
        consumed = session->Consumes( aTransaction );
        if ( consumed )
            {
            session->Canceled();
            }
        }
    MCESRV_DEBUG("CMceSipConnection::InviteCanceled, Exit")
    }
    
// -----------------------------------------------------------------------------
// CMceSipConnection::ConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ConnectionStateChanged(
    CSIPConnection::TState aState )
    {
    MCESRV_DEBUG("CMceSipConnection::ConnectionStateChanged, Entry")
    MCESRV_DEBUG_DVALUE("state", aState )
    
    TInt index = 0;
    while ( index < iSessions.Count() )
        {
        CMceCsSubSession* session = iSessions[ index++ ];
        session->ConnectionStateChanged( aState );
        
        if ( MCE_SIP_CONNECTION_UNUSABLE( iClientConnection ) )
            {
            session->Canceled();
            }
        }
    
    iSessionManager.Cleanup();
    MCESRV_DEBUG("CMceSipConnection::ConnectionStateChanged, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured(
    TInt /*aError*/,
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/)
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( reg ), Entry")
    MCESRV_DEBUG("NOT IMPLEMENTED")
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( reg ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured(
    TInt /*aError*/,
    CSIPRefresh& /*aSIPRefresh*/ )
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( refresh ), Entry")
    MCESRV_DEBUG("NOT IMPLEMENTED")
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( refresh ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ErrorOccured(
    TInt /*aError*/,					
    CSIPRegistrationBinding& /*aRegistration*/ )
    {
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( sa reg ), Entry")
    MCESRV_DEBUG("NOT IMPLEMENTED")
    MCESRV_DEBUG("CMceSipConnection::ErrorOccured( sa reg ), Exit")
    }    

// -----------------------------------------------------------------------------
// CMceSipConnection::Reject
// -----------------------------------------------------------------------------
//
void CMceSipConnection::Reject( CSIPInviteDialogAssoc* aDialogAssoc )
    {
    MCESRV_DEBUG("CMceSipConnection::Reject( assoc ), Entry")
    
    delete aDialogAssoc;
    
    MCESRV_DEBUG("CMceSipConnection::Reject( assoc ), Entry")
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::LocalIPAddressL
// -----------------------------------------------------------------------------
//
TInetAddr CMceSipConnection::LocalIPAddressL( TInetAddr* aNextHop )
    {
    MCESRV_DEBUG("CMceSipConnection::LocalIPAddressL, Entry")
    TInetAddr localIpAddress;
    iLocalAddrResolver->RefreshLocalAddressesL();
    FillWithMatchingAddrFamily( localIpAddress, aNextHop );  
    MCESRV_DEBUG("CMceSipConnection::LocalIPAddressL, Exit")
	return localIpAddress;
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::UpdateConnectionL
// -----------------------------------------------------------------------------
//
void CMceSipConnection::UpdateConnectionL( TUint aSignallingTypeOfService )
    {
    if ( aSignallingTypeOfService != iCurrentSignallingTypeOfService )
        {
        MCESRV_DEBUG_DVALUE( "CMceSipConnection::UpdateConnectionL, tos", 
                             aSignallingTypeOfService )
                             
        iClientConnection->SetOptL( KSoIpTOS, 
                                    KProtocolInetIp, 
                                    aSignallingTypeOfService );
                                    
        iCurrentSignallingTypeOfService = aSignallingTypeOfService;
        
        MCESRV_DEBUG("CMceSipConnection::UpdateConnectionL, tos set")
        }
    }
    
// -----------------------------------------------------------------------------
// CMceSipConnection::FillWithMatchingAddrFamily
// -----------------------------------------------------------------------------
//
void CMceSipConnection::FillWithMatchingAddrFamily( TInetAddr& aAddr,
                                                 const TInetAddr* aNextHop )
    {
    MCESRV_DEBUG("CMceSipConnection::FillWithMatchingAddrFamily, Entry")
    
    MCESRV_DEBUG_ADDRVALUE(
        "CMceSipConnection::FillWithMatchingAddrFamily: local IP addr",
        iLocalAddrResolver->Addr() )
    
    MCESRV_DEBUG_ADDRVALUE(
        "CMceSipConnection::FillWithMatchingAddrFamily: local IPv4 addr",               
        iLocalAddrResolver->IPv4Addr() )
    
    if ( iLocalAddrResolver->HasIPv4Addr() )
	    {
	    aAddr = iLocalAddrResolver->IPv4Addr();
	    }
	else
	    {
	    aAddr = iLocalAddrResolver->Addr();
	    }
    
	if ( aNextHop )
	    {
        MCESRV_DEBUG_ADDRVALUE(
            "CMceSipConnection::FillWithMatchingAddrFamily: next hop",               
            *aNextHop )
	    
        MCESRV_DEBUG_DVALUE(
            "CMceSipConnection::FillWithMatchingAddrFamily: next hop family",
            aNextHop->Family() )
                       
	    if ( aNextHop->Family() == KAfInet )
	        {
	        if ( !iLocalAddrResolver->HasIPv4Addr() &&
	             ( aAddr.IsV4Compat() || aAddr.IsV4Mapped() ) )
	            {
	            aAddr.ConvertToV4();
	            }
	        }
	    else
	        {
	        aAddr = iLocalAddrResolver->Addr();
	        if ( aAddr.Family() == KAfInet )
	            {
	            aAddr.ConvertToV4Compat();
	            }
	        }
	    }
	MCESRV_DEBUG_ADDRVALUE(
	    "CMceSipConnection::FillWithMatchingAddrFamily returns", aAddr )
	    
    MCESRV_DEBUG("CMceSipConnection::FillWithMatchingAddrFamily, Exit")	
    }

// -----------------------------------------------------------------------------
// CMceSipConnection::IsProfileUsedInSession
// -----------------------------------------------------------------------------
//
TBool CMceSipConnection::IsProfileUsedInSession( CSIPProfile& aProfile )
    {
    TBool result = EFalse;
    for( TInt i = 0; i < iSessions.Count() && !result; i++ )
    	{
    	result = ( &aProfile == &iSessions[ i ]->Profile() );
    	}
    return result;
    }

//EOF
