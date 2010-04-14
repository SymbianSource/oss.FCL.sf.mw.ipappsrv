/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include "mccresourceitem.h"
#include "mccuids.hrh"
#include "mcccontrollerlogs.h"
#include "mccmultiplexer.h"
#include "mccinternaldef.h"
#include "mcccamerahandler.h"

#include <mmf/server/mmfaudioinput.h>
#include <mmf/server/mmfaudiooutput.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KMccOneResourceUser = 1;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccResourceItem::NewLC
// -----------------------------------------------------------------------------
//
CMccResourceItem* CMccResourceItem::NewLC(
    TUint32 aCurrentUserStreamId,
    MDataSink* aSink, 
    MDataSource* aSource, 
    TUint32 aEndpointId,
    TBool aIsMultiplexer )
    {
    CMccResourceItem* self = 
        new ( ELeave ) CMccResourceItem( aCurrentUserStreamId, 
                                         aEndpointId, 
                                         aIsMultiplexer );
    CleanupStack::PushL( self );
    self->ConstructL( aSink, aSource );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::NewL
// -----------------------------------------------------------------------------
//
CMccResourceItem* CMccResourceItem::NewL(
    TUint32 aCurrentUserStreamId,
    MDataSink* aSink, 
    MDataSource* aSource, 
    TUint32 aEndpointId,
    TBool aIsMultiplexer )
    {
    CMccResourceItem* self = 
        CMccResourceItem::NewLC( aCurrentUserStreamId,
                                 aSink, 
                                 aSource, 
                                 aEndpointId, 
                                 aIsMultiplexer );
    CleanupStack::Pop( self );
    return self;
    } 
    
// -----------------------------------------------------------------------------
// CMccResourceItem::~CMccResourceItem
// -----------------------------------------------------------------------------
//       
CMccResourceItem::~CMccResourceItem()
    {
    __CONTROLLER( "CMccResourceItem::~CMccResourceItem" )
    
    delete iMultiplexerItem;
    delete iMultiplexer;
    iUsers.Reset();
    iUsers.Close();
    
    iKeyFrameInfo.Close();
        
    __CONTROLLER( "CMccResourceItem::~CMccResourceItem, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IncreaseRefCount
// -----------------------------------------------------------------------------
//
TInt CMccResourceItem::IncreaseRefCount( const TMccResourceParams& aUser )
    {
    __CONTROLLER( "CMccResourceItem::IncreaseRefCount" )
    
    TInt err = iUsers.Append( aUser );
    if ( !err && iMultiplexerItem )
        {
        __CONTROLLER( "CMccResourceItem::IncreaseRefCount, call multiplexer" )
        
        // Inform also the multiplexer
        err = iMultiplexerItem->IncreaseRefCount( aUser );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::DecreaseRefCount
// -----------------------------------------------------------------------------
//        
TBool CMccResourceItem::DecreaseRefCount( const TMccResourceParams& aUser )
    {
    __CONTROLLER( "CMccResourceItem::DecreaseRefCount" )
    
    TIdentityRelation<TMccResourceParams> comparison( UserMatch );
    TInt index = iUsers.Find( aUser, comparison );
    if ( index != KErrNotFound )
        {
        iUsers.Remove( index );
        
        // Inform also the multiplexer
        if ( iMultiplexerItem )
            {
            __CONTROLLER( "CMccResourceItem::DecreaseRefCount, call multiplexer" )
            
            if ( iMultiplexerItem->DecreaseRefCount( aUser ) )
                {
                __CONTROLLER( "CMccResourceItem::DecreaseRefCount, delete multiplexer" )
                
                delete iMultiplexerItem;
                iMultiplexerItem = 0;
                }
            }
        }
    // If no more users, resource item can be deleted
    if ( iUsers.Count() == 0 )
        {
        __CONTROLLER( "CMccResourceItem::DecreaseRefCount, can be removed" )
        
        return ETrue;
        }
    
    __CONTROLLER( "CMccResourceItem::DecreaseRefCount, cannot be removed" )
    
    iCurrentUserStreamId = iUsers[ 0 ].iStreamId;
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::RefCount
// -----------------------------------------------------------------------------
//    
TInt CMccResourceItem::RefCount() const
    {
    return iUsers.Count();
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::EndpointId
// -----------------------------------------------------------------------------
//
TUint32 CMccResourceItem::EndpointId() const
    {
    return iEndpointId;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsNetworkResource
// -----------------------------------------------------------------------------
//    
TBool CMccResourceItem::IsNetworkResource() const
    {
    TBool isNetworkResource = iSink ? 
        iSink->DataSinkType().iUid == KImplUidRtpDataSink :
        iSource->DataSourceType().iUid == KImplUidRtpDataSource;
    
    return isNetworkResource;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsResourceActive
// -----------------------------------------------------------------------------
// 
TBool CMccResourceItem::IsResourceActive() const
    {
    return ( iState == MMccResources::EPrepared || 
             iState == MMccResources::EStarted );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsSharedResource
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::IsSharedResource() const
    {
    TBool sharable = iIsMultiplexer || ( RefCount() > KMccOneResourceUser );
    
    return sharable;
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::IsStandbyResource
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::IsStandbyResource() const
    {
    TBool standby( EFalse );
    if ( iSink && iSink->DataSinkType() == KUidMmfAudioOutput )
        {
        for ( TInt i = 0; i < iUsers.Count() && !standby; i++ )
            {
            standby = iUsers[ i ].iIsStandby;
            }
        }
    return standby;
    }
 
// -----------------------------------------------------------------------------
// CMccResourceItem::NeedsCamera
// -----------------------------------------------------------------------------
// 
TBool CMccResourceItem::NeedsCamera( CMccCameraHandler& aCameraHandler )
    {
    TBool needsCamera( EFalse );
    if ( iState == MMccResources::EStarted )
        {
        needsCamera = ETrue;
        }
    else if ( iState == MMccResources::EPrepared )
        {
        needsCamera = aCameraHandler.IsViewFinderEnabled();
        }
    else if ( iState == MMccResources::EResourcePaused )
        {
        needsCamera = aCameraHandler.IsViewFinderEnabled();
        }		
    else
        {
        // NOP
        }
    return needsCamera;
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::SetResourceStateL
// -----------------------------------------------------------------------------
//    
TBool CMccResourceItem::SetResourceStateL( 
    TUint32 aStreamId, 
    MMccResources::TResourceState aState,
    TBool aStandbyControl,
    TBool aDtmfControl )
    {
    __CONTROLLER_INT1( "CMccResourceItem::SetResourceStateL, old state:", iState )
    __CONTROLLER_INT1( "CMccResourceItem::SetResourceStateL, proposed state:", aState )

    TBool effectiveStateChange = EFalse;
    
    if ( iMultiplexerItem )
        {
        __CONTROLLER( "CMccResourceItem::SetResourceStateL, multiplexer decision" )
        
        effectiveStateChange = 
            iMultiplexerItem->SetResourceStateL( 
                aStreamId, aState, aStandbyControl, aDtmfControl );
        
        // Actual resource follows multiplexer states
        iState = iMultiplexerItem->CurrentState();
        }
    else
        {
        __CONTROLLER( "CMccResourceItem::SetResourceStateL, resource usage allowed" )
        
        // For shared resources, all resource state change operations should
        // be passed further.
        effectiveStateChange = IsSharedResource();
        
        TBool isStandbyResource = IsStandbyResource();
        
        if ( isStandbyResource || aDtmfControl )
            {
            // Special handling for standby resource and dtmf
            SpecialStateChangeL( aStreamId, 
                                 aState, 
                                 aStandbyControl, 
                                 aDtmfControl, 
                                 effectiveStateChange );
            }
        else
            {
            TBool stateOK( EFalse );
            
            switch ( aState )
                { 
                case MMccResources::EPrepared:
                    {
                    if ( MMccResources::EConstructed == iState )
                        {
                        stateOK = ETrue;
                        }
                    else if ( iState == MMccResources::EStarted ||
                              iState == MMccResources::EResourcePaused )
                        {
                        // Endpoint is already started by other user,
                        // state is not "downgraded"
                        aState = iState;
                        stateOK = ETrue;
                        }
                    else
                        {
                        // NOP
                        }
                    break;
                    }
                case MMccResources::EStarted:
                    {
                    if ( MMccResources::EPrepared == iState || 
                         MMccResources::EResourcePaused == iState )
                        {
                        stateOK = ETrue;
                        }  
                    break;
                    }
                case MMccResources::EResourcePaused:
                    {
                    if ( MMccResources::EStarted == iState )
                        {
                        stateOK = ETrue;
                        }  
                    break;
                    }
                case MMccResources::EResourceStopped:
                    {
                    // Endpoint cannot be stopped until all its users
                    // want to stop it.
                    if ( !UserStopL( aStreamId ) )
                        {
                        aState = iState;
                        effectiveStateChange = EFalse;
                        }
                    stateOK = ETrue; 
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            
            if ( aState != iState )
                {
                __ASSERT_ALWAYS( stateOK || effectiveStateChange, 
                                 User::Leave( KErrNotReady ) );
                
                iState = aState;
                
                effectiveStateChange = ETrue;
                }
            }
        }
    
    __CONTROLLER_INT1( "CMccResourceItem::SetResourceStateL, effective change:", 
                       effectiveStateChange )
    __CONTROLLER_INT1( "CMccResourceItem::SetResourceStateL, exit with state:", 
                       iState )
        
    return effectiveStateChange;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsSink
// -----------------------------------------------------------------------------
//
TBool CMccResourceItem::IsSink() const
    {
    return ( iSink != 0 );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsSource
// -----------------------------------------------------------------------------
//        
TBool CMccResourceItem::IsSource() const
    {
    return ( iSource != 0 );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsInternal
// -----------------------------------------------------------------------------
//        
TBool CMccResourceItem::IsInternal() const
    {
    TBool isInternal = iSink ? IsInternal( iSink->DataSinkType().iUid ) :
                               IsInternal( iSource->DataSourceType().iUid );
    
    return isInternal;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::Source
// -----------------------------------------------------------------------------
//        
MDataSource* CMccResourceItem::Source()
    {
    return iSource;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::Sink
// -----------------------------------------------------------------------------
//       
MDataSink* CMccResourceItem::Sink()
    {
    return iSink;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::MultiplexerL
// -----------------------------------------------------------------------------
//
CMccMultiplexer* CMccResourceItem::MultiplexerL()
    {
    if ( !iMultiplexerItem )
        {
        iMultiplexerItem = 
            CMccResourceItem::NewL( iCurrentUserStreamId,
                                    iSink, 
                                    iSource, 
                                    iEndpointId, 
                                    ETrue );
        
        // Update ref count immediately
        for ( TInt i = 0; i < iUsers.Count(); i++ )
            {
            User::LeaveIfError( 
                iMultiplexerItem->IncreaseRefCount( iUsers[ i ] ) );
            }
        }
    return iMultiplexerItem->GetMultiplexerL();
    }
 
// -----------------------------------------------------------------------------
// CMccResourceItem::GetMultiplexerL
// -----------------------------------------------------------------------------
//   
CMccMultiplexer* CMccResourceItem::GetMultiplexerL()
    {
    __ASSERT_ALWAYS( iMultiplexer, User::Leave( KErrNotReady ) );
    
    return iMultiplexer;
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::UserSessionMatch
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::UserSessionMatch( 
    const TMccResourceParams& aUser1, 
    const TMccResourceParams& aUser2 )
    {
    return ( aUser1.iSessionId == aUser2.iSessionId );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::UserMatch
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::UserMatch( 
    const TMccResourceParams& aUser1, 
    const TMccResourceParams& aUser2 )
    {
    return ( aUser1.iStreamId == aUser2.iStreamId );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::UserMatchNotStrict
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::UserMatchNotStrict( 
    const TMccResourceParams& aUser1, 
    const TMccResourceParams& aUser2 )
    {
    if ( aUser1.iLinkId )
        {
        return ( aUser1.iLinkId == aUser2.iLinkId );
        }
    return UserMatch( aUser1, aUser2 );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::UserMatchActive
// -----------------------------------------------------------------------------
//     
TBool CMccResourceItem::UserMatchActive( 
    const TMccResourceParams& /*aUser1*/, 
    const TMccResourceParams& aUser2 )
    {
    // First argument is the search term, in this case it can be ignored
    return ( !aUser2.iIsStopped );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::UsersInfoL
// -----------------------------------------------------------------------------
//   
void CMccResourceItem::UsersInfoL( RArray<TMccResourceParams>& aUsers )
    {
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        aUsers.AppendL( iUsers[ i ] );
        }
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::CurrentState
// -----------------------------------------------------------------------------
//    
MMccResources::TResourceState CMccResourceItem::CurrentState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::Match
// -----------------------------------------------------------------------------
//    
TBool CMccResourceItem::Match( const TMccResourceParams& aUser ) const
    {
    TIdentityRelation<TMccResourceParams> comparison( UserMatchNotStrict );
    TInt index = iUsers.Find( aUser, comparison );
    return ( index != KErrNotFound );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::MatchSession
// -----------------------------------------------------------------------------
//    
TBool CMccResourceItem::MatchSession( const TUint32 aSessionId ) const
    {
    TMccResourceParams searchTerm( aSessionId, 0, 0, 0, EFalse, 0 );
    TIdentityRelation<TMccResourceParams> comparison( UserSessionMatch );
    TInt index = iUsers.Find( searchTerm, comparison );
    return ( index != KErrNotFound );
    }
	
// -----------------------------------------------------------------------------
// CMccResourceItem::StoreKeyFrameInfoL
// -----------------------------------------------------------------------------
//
void CMccResourceItem::StoreKeyFrameInfoL( CMMFBuffer& aKeyFrame )
    {
    __CONTROLLER_INT1( "CMccResourceItem::StoreKeyFrameInfoL, timestamp:", 
                       aKeyFrame.TimeToPlay().Int64() )
                       
    if ( iKeyFrameInfo.Count() >= KMccMaxNumKeyFrames )
        {
        iKeyFrameInfo.Remove( 0 );
        iKeyFrameInfo.Compress();
        }
    TMccKeyFrameInfo info;
    info.iSeqNum = aKeyFrame.FrameNumber();
    info.iTimestamp = aKeyFrame.TimeToPlay().Int64();
    iKeyFrameInfo.AppendL( info );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::IsKeyFrame
// -----------------------------------------------------------------------------
//
TBool CMccResourceItem::IsKeyFrame( CMMFBuffer& aBuffer )
    {
    TBool isKeyFrame( EFalse );
    for ( TInt i = 0; i < iKeyFrameInfo.Count() && !isKeyFrame; i++ )
        {
        TMccKeyFrameInfo& info = iKeyFrameInfo[ i ];
        isKeyFrame = ( info.iSeqNum == aBuffer.FrameNumber() && 
                       info.iTimestamp == aBuffer.TimeToPlay().Int64() );
        }
    return isKeyFrame;
    }
        
// -----------------------------------------------------------------------------
// CMccResourceItem::IsCurrentUser
// -----------------------------------------------------------------------------
//
TBool CMccResourceItem::IsCurrentUser( TUint32 aStreamId ) const
    {
    return ( iCurrentUserStreamId == aStreamId );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::NonSharableResource
// -----------------------------------------------------------------------------
//    
TBool CMccResourceItem::NonSharableResource( TUid aUid ) const
    {
    // TBD! sharing audiooutput should be possible with multiplexer
    return ( aUid == KUidMmfAudioInput ||
             aUid == KUidMmfAudioOutput );
    }
    
// -----------------------------------------------------------------------------
// CMccResourceItem::IsInternal
// -----------------------------------------------------------------------------
//        
TBool CMccResourceItem::IsInternal( TUint32 aUid ) const
    {
    return MCC_INTERNAL_ENDPOINT( aUid );
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::CMccResourceItem
// -----------------------------------------------------------------------------
//      
CMccResourceItem::CMccResourceItem( 
    TUint32 aCurrentUserStreamId, 
    TUint32 aEndpointId,
    TBool aIsMultiplexer ) :
    iCurrentUserStreamId( aCurrentUserStreamId ),
    iEndpointId( aEndpointId ),
    iIsMultiplexer( aIsMultiplexer )
    {
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::ConstructL
// -----------------------------------------------------------------------------
// 
void CMccResourceItem::ConstructL( 
    MDataSink* aSink, 
    MDataSource* aSource )
    {
    __CONTROLLER( "CMccResourceItem::ConstructL" )
    
    // Resource item can contain sink OR source
    
    if ( aSink )
        {
        __ASSERT_ALWAYS( !aSource, User::Leave( KErrArgument ) );
        iSink = aSink;
        }
    else if ( aSource )
        {
        __ASSERT_ALWAYS( !aSink, User::Leave( KErrArgument ) );
        iSource = aSource;
        }
    else
        {
        User::Leave( KErrArgument );
        }

    if ( iIsMultiplexer )
        {
        __CONTROLLER( "CMccResourceItem::ConstructL, create real multiplexer" )
        
        __ASSERT_ALWAYS( iSource, User::Leave( KErrNotReady ) );
        iMultiplexer = CMccMultiplexer::NewL( iSource );
        }
    
    __CONTROLLER( "CMccResourceItem::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::SpecialStateChangeL
// -----------------------------------------------------------------------------
//    
void CMccResourceItem::SpecialStateChangeL( 
    TUint32 aStreamId,
    MMccResources::TResourceState aState,
    TBool aStandbyControl,
    TBool aDtmfControl,
    TBool& aEffectiveStateChange )
    {
    __CONTROLLER( "CMccResourceItem::StandbyStateChangeL" )
    __CONTROLLER_INT1( "CMccResourceItem::StandbyStateChangeL, paused by user:", 
                       iPausedByUser )
    __CONTROLLER_INT1( "CMccResourceItem::StandbyStateChangeL, standby control:",
                       aStandbyControl )
    
    // For standby resource, pausing prepared resource is ok but it's not
    // an effective state change as pause is not really needed. If standby
    // resource has been paused by user, standby control will not have
    // any effect. Additionally, user paused stream is not resumed if user
    // tries to do it. Dtmf stream cannot pause its endpoints if others are
    // using them.
    
    TBool stateOK( EFalse );
    
    TBool nonEffective = 
        ( iPausedByUser && aStandbyControl ) || 
        ( !aStandbyControl && !aDtmfControl && aState == MMccResources::EStarted ) ||
        ( aDtmfControl && aStandbyControl && aState == MMccResources::EResourcePaused && 
            ( RefCount() > KMccOneResourceUser ) );
    
    switch ( aState )
        { 
        case MMccResources::EPrepared:
            {
            if ( MMccResources::EConstructed == iState )
                {
                stateOK = ETrue;
                }
             else if ( iState == MMccResources::EStarted ||
                       iState == MMccResources::EResourcePaused )
                {
                // Endpoint is already started by other user,
                // state is not "downgraded"
                aState = iState;
                stateOK = ETrue;
                }
            else
                {
                // NOP
                }
            break;
            }
        case MMccResources::EStarted:
            {
            if ( MMccResources::EPrepared == iState || 
                 MMccResources::EResourcePaused == iState )
                {
                stateOK = ETrue;
                }  
            break;
            }
        case MMccResources::EResourcePaused:
            {
            if ( MMccResources::EStarted == iState )
                {
                stateOK = ETrue;
                }
            else if ( MMccResources::EPrepared == iState )
                {
                stateOK = ETrue;
                nonEffective = ETrue;
                }
            else
                {
                }
            
            break;
            }
        case MMccResources::EResourceStopped:
            {
            nonEffective = !UserStopL( aStreamId );
            stateOK = ETrue; 
            break;
            }
        default:
            {
            break;
            }
        }
     
    if ( aState != iState )
        {
        if ( !nonEffective )
            {
            __ASSERT_ALWAYS( stateOK, User::Leave( KErrNotReady ) );
                    
            iState = aState;
            }
        
        aEffectiveStateChange = !nonEffective;
        }
           
    if ( !aStandbyControl )
        {
        // Update paused by user info
        iPausedByUser = ( MMccResources::EResourcePaused == aState ||
                          MMccResources::EPrepared == aState );
        }
        
    __CONTROLLER( "CMccResourceItem::StandbyStateChangeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccResourceItem::UserStopL
// -----------------------------------------------------------------------------
// 
TBool CMccResourceItem::UserStopL( TUint32 aStreamId )
    {
    TMccResourceParams searchTerm( 0, 0, aStreamId, 0, EFalse, 0 );
    TIdentityRelation<TMccResourceParams> comparison( UserMatch );
    TInt index = iUsers.Find( searchTerm, comparison );
    __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
    iUsers[ index ].iIsStopped = ETrue;
    
    TIdentityRelation<TMccResourceParams> comparison2( UserMatchActive );
    index = iUsers.Find( searchTerm, comparison2 );
    
    // If didn't found any active ones, the resource can be stopped
    return ( index == KErrNotFound );
    }
    
// End of file

