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
* Description:  Container for NAT FW streams, class implementation.
*
*/

#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include "natfwcredentials.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspcontrollerif.h"
#include "nspcontentparser.h"
#include "nsputil.h"
#include "nspdefs.h"

const TInt KMaxLengthOfFQDN = 255;
const TUint KDefaultTosValue = 184;
const TUint KMaxTosValue = 255;

#define FINDSTREAM_L( stream, mediafield, streamarray )\
    User::LeaveIfNull( stream = NSPUtil::FindMediaStream( mediafield, streamarray ) )
#define FINDSTREAM( mediafield, streamarray )\
    NSPUtil::FindMediaStream( mediafield, streamarray )
#define FINDSTREAM_NOREJECT( mediafield, streamarray )\
    ( !NSPUtil::IsReject( mediafield ) ? FINDSTREAM( mediafield, streamarray ) : NULL )

#define FINDFIELD_L( field, mediastream, fieldarray )\
    User::LeaveIfNull( field = NSPUtil::FindMediaField( mediastream, fieldarray ) )
#define FINDFIELD( mediastream, fieldarray )\
    NSPUtil::FindMediaField( mediastream, fieldarray )
#define FINDFIELD_NOREJECT( mediastream, fieldarray )\
    ( !NSPUtil::IsReject( *FINDFIELD( mediastream, fieldarray ) ) ? FINDFIELD( mediastream, fieldarray ) : NULL )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::CNSPMediaStreamContainer
// ---------------------------------------------------------------------------
//
CNSPMediaStreamContainer::CNSPMediaStreamContainer(
        MNSPControllerIF& aController, TUint aSessionId, TUint aProtocol )
    : iController( aController ),
      iSessionId( aSessionId ),
      iProtocol( aProtocol ),
      iMediaTos( KDefaultTosValue )
    {
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::NewL
// ---------------------------------------------------------------------------
//
CNSPMediaStreamContainer* CNSPMediaStreamContainer::NewL(
        MNSPControllerIF& aController, TUint aSessionId, TUint aProtocol )
    {
    CNSPMediaStreamContainer* self = CNSPMediaStreamContainer::NewLC(
            aController, aSessionId, aProtocol );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::NewLC
// ---------------------------------------------------------------------------
//
CNSPMediaStreamContainer* CNSPMediaStreamContainer::NewLC(
        MNSPControllerIF& aController, TUint aSessionId, TUint aProtocol )
    {
    CNSPMediaStreamContainer* self =  new ( ELeave ) CNSPMediaStreamContainer(
            aController, aSessionId, aProtocol );
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::~CNSPMediaStreamContainer
// ---------------------------------------------------------------------------
//
CNSPMediaStreamContainer::~CNSPMediaStreamContainer()
    {
    iPendingArray.Close();
    iStreamArray.ResetAndDestroy();
    iStreamArray.Close();
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ControlMediaL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::ControlMediaL( TUint aStreamId,
		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
		TInt aError, TAny* aData )
	{
    TNatReturnStatus status = KErrNone;
    const TInt index = FindMediaStream( aStreamId );
	
    if ( KErrNotFound != index )
        {
        status = iStreamArray[index]->ControlMediaL( aStreamId, aEvent, aError, aData );
        const TInt collectionId = iStreamArray[index]->CollectionId();
        TNatReturnStatus cont = RemovePending( collectionId, KNatReady == status );
        status = ( KNatReady == status ? cont : status );
        }
    else
    	{
    	status = KErrNotFound;
    	}
    
    return status;
	}


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::UpdateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::UpdateL( RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __ASSERT_ALWAYS( &aRemoteCands, User::Leave( KErrArgument ) );
    iController.UpdateIceProcessingL( iSessionId, aRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::SetRoleL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::SetRoleL( TNATFWIceRole aRole )
    {
    iController.SetRoleL( iSessionId, aRole );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::SetInboundCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::SetInboundCredentialsL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->SetInboundCredentialsL();
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::SetOutboundCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::SetOutboundCredentialsL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->SetOutboundCredentialsL();
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ResetAndDestroyCandidates
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ResetAndDestroyCandidates()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->ResetAndDestroyCandidates();
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::LoadPluginL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::LoadPluginL( CDesC8Array& aPlugins, TInt& aPluginIndex )
    {
    __ASSERT_ALWAYS( &aPlugins, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( &aPluginIndex, User::Leave( KErrArgument ) );
    
    iController.LoadPluginL( iSessionId, aPlugins, aPluginIndex );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::FetchCandidateL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->FetchCandidateL();
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::FetchCandidatesL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->FetchCandidatesL();
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::PerformCandidateChecksL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::PerformCandidateChecksL(
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __ASSERT_ALWAYS( &aRemoteCandidates, User::Leave( KErrArgument ) );
    
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->PerformCandidateChecksL();
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    
    iController.PerformCandidateChecksL( iSessionId, aRemoteCandidates );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ActivateL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    CSdpMediaField* field = NULL;
    CSdpConnectionField* connField = aDoc.ConnectionField();
    const TDesC8& address = ( connField ? connField->Address() : KNullDesC8 );
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        field = NSPUtil::FindMediaField( *iStreamArray[index], mediafields );
        iStreamArray[index]->ActivateL( *field, address );
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ActivateL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->ActivateL();
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::DeActivateL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::DeActivateL()
    {
    const TInt mediastreamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamcount ; index++ )
        {
        iStreamArray[index]->DeActivateL();
        iPendingArray.AppendL( iStreamArray[index]->CollectionId() );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::CreateStreamsL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::CreateStreamsL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    const CSdpConnectionField* connField = aDoc.ConnectionField();
    const TDesC8& address = ( connField ? connField->Address() : KNullDesC8 );
    
    TNatReturnStatus status = KNatReady;
    CNSPMediaStream* stream = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldcount( mediafields.Count() );
    
    __ASSERT_ALWAYS( mediafieldcount != 0, User::Leave( KErrNotFound ) );
    
    for ( TInt index = 0; index < mediafieldcount; index++ )
         {
         stream = NSPUtil::FindMediaStream( *mediafields[index], iStreamArray );
         if( NSPUtil::IsReject( *mediafields[index] ) || ( stream && 
        	  stream->MediaField().Port() ==  ( *mediafields[index] ).Port() ) )
            {
            NSPLOG_STR( "No need to create stream" )
            }
		    else 
		        {
            if ( stream ) 
            	  { 
			          RemoveStream( stream );
			          }
            stream = CNSPMediaStream::NewLC( iController, *mediafields[index],
        	               iSessionId, iController.CreateUniqueId(), address,
        	               iProtocol, iMediaTos );
        	  iStreamArray.AppendL( stream );
        	  CleanupStack::Pop( stream );
        	  status = KNatAsync;
		        }
	        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::RemoveStreamsL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::RemoveStreamsL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TNatReturnStatus status = KNatReady;
    CSdpMediaField* field = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldcount( mediafields.Count() );
    
    __ASSERT_ALWAYS( mediafieldcount != 0, User::Leave( KErrNotFound ) );
    
    for ( TInt index = 0; index < iStreamArray.Count(); )
        {
        field = FINDFIELD_NOREJECT( *iStreamArray[index], mediafields );
        
        if ( !field )
            {
            delete iStreamArray[index];
            iStreamArray.Remove(index);
            iStreamArray.Compress();
            status = KNatAsync;
            }
        else
            {
            index++;
            }
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ModStunSolvedL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ModStunSolvedL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TBool sessionlevel = EFalse;
    CNSPMediaStream* stream = NULL;
    CSdpConnectionField* connField = aDoc.ConnectionField();
    CSdpOriginField* originField = aDoc.OriginField();
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );

    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        stream = FINDSTREAM_NOREJECT( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            stream->ModifyStunConnL( *mediafields[index], addr );
            
            if ( !sessionlevel )
                {
                if ( connField )
                    {
                    NSPUtil::UpdateConnectionFieldL( *connField, addr );
                    }
                
                if ( originField )
                    {
                    NSPUtil::UpdateOriginFieldL( *originField, addr );
                    }
                
                sessionlevel = ETrue;
                }
            }
        }
    
    CleanupStack::PopAndDestroy( &addr );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ModIceSolvedL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ModIceSolvedL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TBool sessionlevel = EFalse;
    CNSPMediaStream* stream = NULL;
    CSdpConnectionField* connField = aDoc.ConnectionField();
    CSdpOriginField* originField = aDoc.OriginField();
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );
    
    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        stream = FINDSTREAM_NOREJECT( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            stream->ModifyIceConnL( *mediafields[index], addr );
            
            if ( !sessionlevel )
                {
                if ( connField )
                    {
                    NSPUtil::UpdateConnectionFieldL( *connField, addr );
                    }
                
                if ( originField )
                    {
                    NSPUtil::UpdateOriginFieldL( *originField, addr );
                    }
                
                sessionlevel = ETrue;
                }
            }
        }
    
    CleanupStack::PopAndDestroy( &addr );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::ModLocalSolvedL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::ModLocalSolvedL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TBool sessionlevel = EFalse;
    CNSPMediaStream* stream = NULL;
    CSdpConnectionField* connField = aDoc.ConnectionField();
    CSdpOriginField* originField = aDoc.OriginField();
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );
    
    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        stream = FINDSTREAM_NOREJECT( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            stream->ModifyLocalConnL( *mediafields[index], addr );
            
            if ( !sessionlevel )
                {
                if ( connField )
                    {
                    NSPUtil::UpdateConnectionFieldL( *connField, addr );
                    }
                
                if ( originField )
                    {
                    NSPUtil::UpdateOriginFieldL( *originField, addr );
                    }
                
                sessionlevel = ETrue;
                }
            }
        else
            {
            NSPUtil::RejectL( *mediafields[index] );
            }
        }
    
    CleanupStack::PopAndDestroy( &addr );
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::AddIceContentL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::AddIceContentL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    CNSPMediaStream* stream = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        FINDSTREAM_L( stream, *mediafields[index], iStreamArray );
        stream->AddCredentialsL( *mediafields[index] );
        stream->AddLocalCandidatesL( *mediafields[index] );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::AddRemoteCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPMediaStreamContainer::AddRemoteCandidatesL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    CNSPMediaStream* stream = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldcount( mediafields.Count() );
    for ( TInt index = 0; index < mediafieldcount; index++ )
        {
        FINDSTREAM_L( stream, *mediafields[index], iStreamArray );
        stream->AddRemoteCandidatesL( *mediafields[index] );
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::GetCandidatesL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::GetCandidatesL( CSdpDocument& aDoc,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( &aRemoteCandidates, User::Leave( KErrArgument ) );
    
    TNatReturnStatus status = KNatReady;
    CNSPMediaStream* stream = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( iStreamArray.Count() != 0, User::Leave( KErrNotFound ) );
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        RPointerArray<CNATFWCandidate> remotecands;
        CleanupStack::PushL( TCleanupItem( NSPUtil::CleanupArrayItem, &remotecands ) );
        stream = FINDSTREAM( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            stream->GetCandidatesL( *mediafields[index], remotecands );
            
            while ( 0 < remotecands.Count() )
                {
                if ( NSPUtil::IsDuplicate( *remotecands[0], aRemoteCandidates ) )
                    {
                    delete remotecands[0];
                    }
                else
                    {
                    aRemoteCandidates.AppendL( remotecands[0] );// ownership transferred
                    status = KNatAsync;
                    }
                
                remotecands.Remove(0);
                remotecands.Compress();
                }
            }
        
        CleanupStack::PopAndDestroy(); // remotecands
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::GetCredentialsL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::GetCredentialsL( CSdpDocument& aDoc,
        CNATFWCredentials& aSessionlevel )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( &aSessionlevel, User::Leave( KErrArgument ) );
    
    TNatReturnStatus status = KNatReady;
    CNSPMediaStream* stream = NULL;
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );
    
    CNATFWCredentials* sessionLevel = CNATFWCredentials::NewLC();
    sessionLevel->SetDirection( CNATFWCredentials::EOutbound );
    iController.ContentParser().GetCredentialsL( aDoc, *sessionLevel );
    
    if ( !CNATFWCredentials::Compare( aSessionlevel, *sessionLevel ) )
        {
        aSessionlevel.CopyL( *sessionLevel );
        status = KNatAsync;
        }
    
    CleanupStack::PopAndDestroy( sessionLevel );
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        CNATFWCredentials* mediaLevel = CNATFWCredentials::NewLC( aSessionlevel );
        iController.ContentParser().GetCredentialsL( *mediafields[index], *mediaLevel );
        CleanupStack::Pop( mediaLevel );
        
        stream = FINDSTREAM( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            stream->SetOutboundCredentials( mediaLevel ); // ownership changed.
            }
        else
            {
            delete mediaLevel;
            }
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::GetSessionConnInfoL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::GetSessionConnInfoL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TNatReturnStatus status = KNatReady;
    TNatReturnStatus mediastatus = status;
    CNSPMediaStream* stream = NULL;
    CSdpConnectionField* connField = aDoc.ConnectionField();
    const TDesC8& address = ( connField ? connField->Address() : KNullDesC8 );
    RPointerArray<CSdpMediaField>& mediafields = aDoc.MediaFields();
    
    const TInt mediafieldscount( mediafields.Count() );
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediafieldscount != 0, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );

    RBuf8 addr;
    addr.CreateL( KMaxLengthOfFQDN );
    addr.CleanupClosePushL();
    
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        stream = FINDSTREAM( *mediafields[index], iStreamArray );
        
        if ( stream )
            {
            addr = address;
            mediastatus = stream->MediaConnInfoL( *mediafields[index], addr );
            status = ( KNatReady == status ? mediastatus : status );
            addr.Zero();
            }
        else
            {
            NSPUtil::RejectL( *mediafields[index] );
            }
        }
    
    CleanupStack::PopAndDestroy( &addr );
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::CheckUpdateL
// ---------------------------------------------------------------------------
//
CSdpDocument* CNSPMediaStreamContainer::CheckUpdateL( CSdpDocument& aDoc )
    {
    __ASSERT_ALWAYS( &aDoc, User::Leave( KErrArgument ) );
    
    TBool needToUpdate = EFalse;
    CSdpDocument* update = NULL;
    
    const TInt mediastreamscount( iStreamArray.Count() );
    
    __ASSERT_ALWAYS( mediastreamscount != 0, User::Leave( KErrNotFound ) );
    
    for ( TInt index = 0; index < mediastreamscount && !needToUpdate; index++ )
        {
        needToUpdate = iStreamArray[index]->NeedToUpdateL();
        }
    
    if ( needToUpdate )
        {
        ModIceSolvedL( aDoc );
        update = aDoc.CloneL();
        }
    
    return update;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::SetMediaTos
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStreamContainer::SetMediaTos( TUint aMediaTos )
    {
    if ( KMaxTosValue >= aMediaTos )
        {
        iMediaTos = aMediaTos;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::MediaTos
// ---------------------------------------------------------------------------
//
TUint CNSPMediaStreamContainer::MediaTos() const
    {
    return iMediaTos;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::RemovePending
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPMediaStreamContainer::RemovePending(
        TUint aComponentId, TBool aRemove )
    {
    TNatReturnStatus status = KNatAsync;
    
    if ( aRemove )
        {
        const TInt index = iPendingArray.Find( aComponentId );
        
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
// CNSPMediaStreamContainer::FindMediaStream
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStreamContainer::FindMediaStream( TUint aStreamId )
    {
    const TInt streamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < streamcount; index++ )
        {
        if ( iStreamArray[index]->HasMediaComponent( aStreamId ) )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::FindCollection
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStreamContainer::FindCollection( TUint aCollectionId )
    {
    const TInt streamcount( iStreamArray.Count() );
    for ( TInt index = 0; index < streamcount; index++ )
        {
        if ( iStreamArray[index]->CollectionId() == aCollectionId )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }
    
    
// ---------------------------------------------------------------------------
// CNSPMediaStreamContainer::RemoveStream
// ---------------------------------------------------------------------------
//
TInt CNSPMediaStreamContainer::RemoveStream( CNSPMediaStream* aStream )
    {
    TInt status = KErrNotFound;
    const TInt mediastreamscount( iStreamArray.Count() );
    for ( TInt index = 0; index < mediastreamscount; index++ )
         {
         if ( iStreamArray[index] == aStream ) 
             {
			       delete iStreamArray[index];
	           iStreamArray.Remove(index);
	           iStreamArray.Compress();
	           status = KErrNone;
	           return status;
	           }      
		     }
    return status;
    }


// end of file
