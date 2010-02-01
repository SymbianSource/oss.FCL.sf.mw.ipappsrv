/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Media stream class implementation.
*
*/

#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natfwcredentials.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspcontentparser.h"
#include "nsputil.h"
#include "nspcontrollerif.h"
#include "nspdefs.h"

const TInt KMaxLengthOfFQDN = 255;
const TUint KMaxPasswordLength = 100;
const TUint KNSPRtpComponentId = 1;
const TUint KNSPRtcpComponentId = 2;

static void UpdateConnFieldL( CSdpMediaField& aMediaField,
        const TDes8& aAddress )
	{
	if ( aMediaField.ConnectionFields().Count() )
		{
		NSPUtil::UpdateConnectionFieldL( *aMediaField.ConnectionFields()[0],
		    aAddress );
		}
	}

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPMediaStream::CNSPMediaStream
// ---------------------------------------------------------------------------
//
CNSPMediaStream::CNSPMediaStream( MNSPControllerIF& aController,
        TUint aSessionId, TUint aCollectionId )
    : iController( aController ),
      iSessionId( aSessionId ),
      iCollectionId( aCollectionId )
    {
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ConstructL( CSdpMediaField& aMediaField,
        const TDesC8& aAddress, TUint aProtocol, TUint aMediaTos )
    {
    iMediaField = aMediaField.CloneL( EFalse );
    
    RBuf8 addr;
    addr.CleanupClosePushL();
    addr.CreateL( KMaxLengthOfFQDN );
    addr = CONN_ADDR( aMediaField, aAddress );
    TUint port = iMediaField->Port();
    
    CNSPMediaStreamComponent* rtp = CNSPMediaStreamComponent::NewLC(
            iController, iSessionId, iCollectionId, KNSPRtpComponentId,
            addr, port, aProtocol, aMediaTos );
    User::LeaveIfError( iStreamComponentArray.Append( rtp ) );
    CleanupStack::Pop( rtp );
    
    iController.ContentParser().FindRTCP( aMediaField, addr, ++port );
    
    CNSPMediaStreamComponent* rtcp = CNSPMediaStreamComponent::NewLC(
            iController, iSessionId, iCollectionId, KNSPRtcpComponentId,
            addr, port, aProtocol, aMediaTos );
    User::LeaveIfError( iStreamComponentArray.Append( rtcp ) );
    CleanupStack::Pop( rtcp );
    
    CleanupStack::PopAndDestroy( &addr );
    
    iInboundCredentials = CNATFWCredentials::NewL();
    iInboundCredentials->SetDirection( CNATFWCredentials::EInbound );
    HBufC8* buffer = HBufC8::NewLC( KMaxPasswordLength );
    TPtr8 ptr( buffer->Des() );
    iController.GenerateUsernameL( ptr );
    iInboundCredentials->SetUsernameL( buffer->Des() );
    iController.GeneratePasswordL( ptr );
    iInboundCredentials->SetPasswordL( buffer->Des() );
    CleanupStack::PopAndDestroy( buffer );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::NewL
// ---------------------------------------------------------------------------
//
CNSPMediaStream* CNSPMediaStream::NewL( MNSPControllerIF& aController,
        CSdpMediaField& aMediaField, TUint aSessionId, TUint aCollectionId,
        const TDesC8& aAddress, TUint aProtocol, TUint aMediaTos )
    {
    CNSPMediaStream* self = CNSPMediaStream::NewLC( aController,
            aMediaField, aSessionId, aCollectionId, aAddress,
            aProtocol, aMediaTos );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::NewLC
// ---------------------------------------------------------------------------
//
CNSPMediaStream* CNSPMediaStream::NewLC( MNSPControllerIF& aController,
        CSdpMediaField& aMediaField, TUint aSessionId, TUint aCollectionId,
        const TDesC8& aAddress, TUint aProtocol, TUint aMediaTos )
    {
    CNSPMediaStream* self = new ( ELeave ) CNSPMediaStream(
            aController, aSessionId, aCollectionId );
    CleanupStack::PushL( self );
    self->ConstructL( aMediaField, aAddress, aProtocol, aMediaTos );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::~CNSPMediaStream
// ---------------------------------------------------------------------------
//
CNSPMediaStream::~CNSPMediaStream()
    {
    delete iMediaField;
    delete iInboundCredentials;
    delete iOutboundCredentials;
    iPendingArray.Close();
    iStreamComponentArray.ResetAndDestroy();
    iStreamComponentArray.Close();
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::HasMediaComponent
// ---------------------------------------------------------------------------
//
TBool CNSPMediaStream::HasMediaComponent( TUint aStreamId )
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        if ( iStreamComponentArray[index]->StreamId() == aStreamId )
            {
            return ETrue;
            }
        }
        
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::Notify
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStream::ControlMediaL( TUint aStreamId,
		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
		TInt aError, TAny* aData )
	{
    TNatReturnStatus status = KErrNone;
    const TInt index = FindMediaComponent( aStreamId );
	
    if ( KErrNotFound != index )
    	{
        status = iStreamComponentArray[index]->ControlMediaL( aEvent, aError, aData );
        TNatReturnStatus cont = RemovePending( aStreamId, KNatReady == status );
        status = ( KNatReady == status ? cont : status );    	
    	}
    else
    	{
    	status = KErrNotFound;
    	}
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ModifyStunConnL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ModifyStunConnL( CSdpMediaField& aMediaField,
        TDes8& aAddress ) const
    {
    NSPLOG_STR( "CNSPMediaStream::ModifyStunConnL(), Entry" )
    
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        RPointerArray<CNATFWCandidate>& localcand = component->LocalCandidates();
        NSPUtil::SortCandidatesL( localcand ); // leave if empty
        
        if ( KNSPRtpComponentId == component->ComponentId() )
            {
            aAddress = localcand[0]->TransportDomainAddr();
            aMediaField.SetPortL( localcand[0]->TransportDomainPort() );
            UpdateConnFieldL( aMediaField, aAddress );
            }
        else if ( KNSPRtcpComponentId == component->ComponentId() )
            {
            iController.ContentParser().ModifyRTCPL( aMediaField,
                    localcand[0]->TransportDomainAddr(),
                    localcand[0]->TransportDomainPort() );
            }
        else
            {
            User::Leave( KErrNotFound );
            }
        }
    
    NSPLOG_STR( "CNSPMediaStream::ModifyStunConnL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ModifyIceConnL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ModifyIceConnL( CSdpMediaField& aMediaField,
        TDes8& aAddress ) const
    {
    NSPLOG_STR( "CNSPMediaStream::ModifyIceConnL(), Entry" )
    
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        const CNATFWCandidate& localcandidate = component->IceLocalCandidateL();
        
        if ( KNSPRtpComponentId == component->ComponentId() )
            {
            aAddress = localcandidate.TransportDomainAddr();
            aMediaField.SetPortL( localcandidate.TransportDomainPort() );
            UpdateConnFieldL( aMediaField, aAddress );
            }
        else if ( KNSPRtcpComponentId == component->ComponentId() )
            {
            iController.ContentParser().ModifyRTCPL( aMediaField,
                    localcandidate.TransportDomainAddr(),
                    localcandidate.TransportDomainPort() );
            }
        else
            {
            User::Leave( KErrNotFound );
            }
        }
    
    NSPLOG_STR( "CNSPMediaStream::ModifyIceConnL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ModifyLocalConnL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ModifyLocalConnL( CSdpMediaField& aMediaField,
        TDes8& aAddress ) const
    {
    NSPLOG_STR( "CNSPMediaStream::ModifyLocalConnL(), Entry" )
    
    TUint port( 0 );
    RBuf8 address;
    address.CreateL( KMaxLengthOfFQDN );
    address.CleanupClosePushL();
    
    const TInt streamcompcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < streamcompcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        User::LeaveIfError( component->OutgoingAddress( address, port ) );
        
        if ( KNSPRtpComponentId == component->ComponentId() )
            {
            aAddress = address;
            aMediaField.SetPortL( port );
            UpdateConnFieldL( aMediaField, aAddress );
            }
        else if ( KNSPRtcpComponentId == component->ComponentId() )
            {
            iController.ContentParser().ModifyRTCPL( aMediaField, address, port );
            }
        else
            {
            User::Leave( KErrNotFound );
            }
        }
    
    CleanupStack::PopAndDestroy( &address );
    
    NSPLOG_STR( "CNSPMediaStream::ModifyLocalConnL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::AddLocalCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::AddLocalCandidatesL( CSdpMediaField& aMediaField ) const
    {
    RPointerArray<CNATFWCandidate> localcand;
    CleanupClosePushL( localcand );
    LocalCandidatesL( localcand );
    iController.ContentParser().AddCandidatesL( aMediaField, localcand );
    CleanupStack::PopAndDestroy( &localcand );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::GetCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::GetCandidatesL( CSdpMediaField& aMediaField,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates ) const
    {
    iController.ContentParser().GetCandidatesL( aMediaField, aRemoteCandidates );
    AttachCandidatesL( aRemoteCandidates );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::MediaConnInfoL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStream::MediaConnInfoL( CSdpMediaField& aMediaField,
        const TDesC8& aAddress ) const
    {
    TNatReturnStatus status = KNatReady;
    TNatReturnStatus mediastatus = status;
    const TDesC8& address = CONN_ADDR( aMediaField, aAddress );

    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    const TInt streamcompcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < streamcompcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        TUint port = aMediaField.Port();
        addr = address;
        
        if ( KNSPRtcpComponentId == component->ComponentId() )
            {
            iController.ContentParser().FindRTCP( aMediaField, addr, ++port );
            }
        
        const TDesC8& remoteAddr = component->RemoteAddress();
        const TUint remotePort = component->RemotePort();
        mediastatus = ( ( !addr.Compare( remoteAddr ) && remotePort == port ) ||
        				NSPUtil::IsUnspecifiedL( addr, port ) ? KNatReady : KNatAsync );
        
        if ( KNatAsync == mediastatus )
            {
            component->ActivateL( addr, port );
            }
        
        status = ( KNatReady == status ? mediastatus : status );
        }
    
    CleanupStack::PopAndDestroy( &addr );
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::AddRemoteCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::AddRemoteCandidatesL( CSdpMediaField& aMediaField ) const
    {
    RPointerArray<CNATFWCandidate> remotecandidates;
    CleanupClosePushL( remotecandidates );
    
    const TInt streamcompcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < streamcompcount; index++ )
        {
        remotecandidates.AppendL( &iStreamComponentArray[index]->IceLocalCandidateL() );
        }
    
    iController.ContentParser().AddRemoteCandidatesL( aMediaField, remotecandidates );
    CleanupStack::PopAndDestroy(); // remotecandidates
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::AddCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::AddCredentialsL( CSdpMediaField& aMediaField ) const
    {
    iController.ContentParser().AddCredentialsL( aMediaField, *iInboundCredentials );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::LocalCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::LocalCandidatesL(
			RPointerArray<CNATFWCandidate>& aLocalcand ) const
    {
    const TInt streamcompcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < streamcompcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        RPointerArray<CNATFWCandidate>& localcand = component->LocalCandidates();
        NSPUtil::SortCandidatesL( localcand ); // leave if empty
        
        const TInt localcandcount( localcand.Count() );
        for ( TInt jndex = 0; jndex < localcandcount ; jndex++ )
            {
            aLocalcand.AppendL( localcand[jndex] );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::FetchCandidateL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        component->FetchCandidateL();
        iPendingArray.AppendL( component->StreamId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::FetchCandidatesL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        component->FetchCandidatesL();
        iPendingArray.AppendL( component->StreamId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::PerformCandidateChecksL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::PerformCandidateChecksL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        component->PerformCandidateChecksL();
        iPendingArray.AppendL( component->StreamId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ActivateL( CSdpMediaField& aMediaField,
        const TDesC8& aAddress )
    {
    const TDesC8& address = CONN_ADDR( aMediaField, aAddress );
    
    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        TUint port = aMediaField.Port();
        addr = address;
        
        if ( KNSPRtcpComponentId == component->ComponentId() )
            {
            iController.ContentParser().FindRTCP( aMediaField, addr, ++port );
            }
        
        iStreamComponentArray[index]->ActivateL( addr, port );
        iPendingArray.AppendL( iStreamComponentArray[index]->StreamId() );
        }
    
    CleanupStack::PopAndDestroy( &addr );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ActivateL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        component->ActivateL();
        iPendingArray.AppendL( component->StreamId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::DeActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::DeActivateL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        CNSPMediaStreamComponent* component = iStreamComponentArray[index];
        component->DeActivateL();
        iPendingArray.AppendL( component->StreamId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::CollectionId
// ---------------------------------------------------------------------------
//
TUint CNSPMediaStream::CollectionId() const
    {
    return iCollectionId;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::MediaField
// ---------------------------------------------------------------------------
//
const CSdpMediaField& CNSPMediaStream::MediaField() const
    {
    return (*iMediaField);
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::SetInboundCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::SetInboundCredentialsL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        iStreamComponentArray[index]->SetCredentialsL( *iInboundCredentials );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::SetOutboundCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::SetOutboundCredentialsL()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        iStreamComponentArray[index]->SetCredentialsL( *iOutboundCredentials );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::SetOutboundCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::SetOutboundCredentials( CNATFWCredentials* aOutboundCredentials )
    {
    if ( iOutboundCredentials != aOutboundCredentials )
        {
        delete iOutboundCredentials;
        iOutboundCredentials = aOutboundCredentials;
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::NeedToUpdate
// ---------------------------------------------------------------------------
//
TBool CNSPMediaStream::NeedToUpdateL()
    {
    TBool needUpdate = EFalse;
    
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount && !needUpdate; index++ )
        {
        const CNATFWCandidate& local =
                (*iStreamComponentArray[index]->LocalCandidates()[0]);
        const CNATFWCandidate& working =
                iStreamComponentArray[index]->IceLocalCandidateL();
        
        needUpdate = !(
                !working.TransportDomainAddr().Compare( local.TransportDomainAddr() ) &&
                working.TransportDomainPort() == local.TransportDomainPort() );
        }
    
    return needUpdate;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::ResetAndDestroyCandidates
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::ResetAndDestroyCandidates()
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        iStreamComponentArray[index]->ResetAndDestroyCandidates();
        }    
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::RemovePending
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStream::RemovePending( TUint aStreamId, TBool aRemove )
    {
    TNatReturnStatus status = KNatAsync;
    
    if ( aRemove )
        {
        const TInt index = iPendingArray.Find( aStreamId );
        
        if ( KErrNotFound != index )
            {
            iPendingArray.Remove( index );
            iPendingArray.Compress();
            status = ( iPendingArray.Count() ? KNatAsync : KNatReady );
            }
        else
            {
            status = KNatAsync;
            }
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::AttachCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStream::AttachCandidatesL(
        RPointerArray<CNATFWCandidate>& aRemoteCandidates ) const
    {
    const TInt candidatecount( aRemoteCandidates.Count() );
    for ( TInt index = 0; index < candidatecount; index++ )
        {
        aRemoteCandidates[index]->SetSessionId( iSessionId );
        aRemoteCandidates[index]->SetStreamCollectionId( iCollectionId );
        
        TBool attach( EFalse );
        const TInt componentcount( iStreamComponentArray.Count() );
        for ( TInt jndex = 0; jndex < componentcount && !attach ; jndex++ )
            {
            attach = iStreamComponentArray[jndex]->AttachCandidateL(
                                *aRemoteCandidates[index] );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStream::FindMediaComponent
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStream::FindMediaComponent( TUint aStreamId )
    {
    const TInt componentcount( iStreamComponentArray.Count() );
    for ( TInt index = 0; index < componentcount; index++ )
        {
        if ( iStreamComponentArray[index]->StreamId() == aStreamId )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }

// end of file
