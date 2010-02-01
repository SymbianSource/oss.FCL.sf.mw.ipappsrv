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
#include "mccvideosource.h"
#include "mccvideosourceimpl.h"
#include "mmcccodecinformation.h"
#include "mmccinterfacedef.h"
#include "mccvideosourcesinklogs.h"
#include "mccinternaldef.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMcpRtpDataSource::NewSourceL
// Static constructor.
// -----------------------------------------------------------------------------
//
MDataSource* CMccVideoSource::NewSourceL( TUid /*aImplementationUid*/, 
                                            const TDesC8& /*aInitData*/ )
    {
    CMccVideoSource* self = new ( ELeave ) CMccVideoSource();
    return static_cast<MDataSource*>( self );
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::ConstructSourceL
// -----------------------------------------------------------------------------
//
void CMccVideoSource::ConstructSourceL( const TDesC8& aInitData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::ConstructSourceL" )

	if ( aInitData.Length() > 0 )
	    {
	    __V_SOURCESINK_CONTROLL( "CMccVideoSource::ConstructSourceL, with init data" )
	    
	    TMccVideoSourceSettingBuf settingsBuf;
        settingsBuf.Copy( aInitData );
        TMccVideoSourceSetting& settings = settingsBuf();
            
        iCameraIndex = settings.iDeviceIndex;
        
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSource::ConstructSourceL, camera index:", iCameraIndex )
	    }
    
    delete iSourceImpl;
    iSourceImpl = NULL;
    iSourceImpl = CMccVideoSourceImpl::NewL( 
            MCC_ENDPOINT_ID( static_cast<MDataSource*>( this ) ), iMccResources );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::ConstructSourceL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSource::CMccVideoSource
// -----------------------------------------------------------------------------
//
CMccVideoSource::CMccVideoSource() :
    CMccDataSource( KMccVideoSourceUid )
    {
    }
        
// -----------------------------------------------------------------------------
// CMccVideoSource::~CMccVideoSource
// -----------------------------------------------------------------------------
//
CMccVideoSource::~CMccVideoSource()
    {
	delete iSourceImpl;	
	iUsers.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SetCameraHandler
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetCameraHandler( MMccCameraHandler& aCameraHandler )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SetCameraHandler" )
   
    if ( iSourceImpl )
	    {
	   	iSourceImpl->SetCameraHandler( aCameraHandler );
	    }
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::CameraIndex
// -----------------------------------------------------------------------------
//	
TInt CMccVideoSource::CameraIndex() const
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSource::CameraIndex, index:", iCameraIndex )   
    return iCameraIndex;
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SourcePrimeL
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SourcePrimeL()
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourcePrimeL" )
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
   
	iSourceImpl->SourcePrimeL( MultipleCodecs() );
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::SourcePlayL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SourcePlayL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourcePlayL" )
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    UpdateCurrentUserL();
    
	iSourceImpl->SourcePlayL();
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourcePlayL, done" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SourcePauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SourcePauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourcePauseL" )
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
    
	iSourceImpl->SourcePauseL();
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::SourceStopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SourceStopL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourceStopL" )
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccVideoSource::SourceDataTypeCode( TMediaId aMediaId )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::SourceDataTypeCode" )

    if ( iSourceImpl )
	    {
	  	return iSourceImpl->SourceDataTypeCode( aMediaId );
	    }
    else
	    {
	    return TFourCC( KMMFFourCCCodeNULL );
	    }
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSource::SetSourceDataTypeCode( 
    TFourCC aCodec, 
    TMediaId aMediaId )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::SetSourceDataTypeCode" )
    
    if ( iSourceImpl )
	    {
	   	return iSourceImpl->SetSourceDataTypeCode( aCodec, aMediaId );	
	    }
    else
	    {
	    __V_SOURCESINK_CONTROLL("CMccVideoSource::SetSourceDataTypeCode, \
iSourceImpl=NULL")
	    return KErrArgument;	
	    }
	}	

// -----------------------------------------------------------------------------
// CMccVideoSource::BufferEmptiedL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __V_SOURCESINK_MEDIA( "CMccVideoSource::BufferEmptiedL" )
    __ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );	
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::CanCreateSourceBuffer()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSource::CanCreateSourceBuffer()
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::CanCreateSourceBuffer, ETrue" )	
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccVideoSource::CreateSourceBufferL()
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccVideoSource::CreateSourceBufferL( 
	TMediaId aMediaId, 
    TBool& aReference )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::CreateSourceBufferL" )	

	return iSourceImpl->CreateSourceBufferL( aMediaId, aReference );
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::SourceThreadLogon()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSource::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourceThreadLogon" )	
    
    TInt err( KErrNotReady );
    if ( iSourceImpl )
        {
        err = iSourceImpl->SourceThreadLogon( aEventHandler );
        }
    
    if ( !err )
        {
        iSourceImpl->SetResources( iMccResources );
        }
    
    return err; 
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::SourceThreadLogoff()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SourceThreadLogoff()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::SourceThreadLogoff" )
    
    if ( iSourceImpl )
        {
        iSourceImpl->SourceThreadLogoff();
        TRAP_IGNORE( iSourceImpl->SourceStopL() );
        delete iSourceImpl;
        iSourceImpl = NULL;
        }
    }
	
// -----------------------------------------------------------------------------
// CMccVideoSource::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::FillBufferL( CMMFBuffer* aBuffer,
                  MDataSink* aConsumer,
                  TMediaId aMediaId )
	{
	__V_SOURCESINK_MEDIA( "CMccVideoSource::FillBufferL" )		
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	iSourceImpl->FillBufferL( aBuffer, aConsumer, aMediaId );
	}	
                  
// -----------------------------------------------------------------------------
// CMccVideoSource::NegotiateSourceL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::NegotiateSourceL( MDataSink& /*aDataSink*/ )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::NegotiateSourceL" ) 				
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::RateAdaptationRequest
// -----------------------------------------------------------------------------
//     
TInt CMccVideoSource::RateAdaptationRequest( const TMccEvent& aInputData, 
    										TMccEvent& aOutputData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::RateAdaptationRequest" )
    if ( iSourceImpl )
    	{                   
    	return iSourceImpl->RateAdaptationRequest( aInputData, aOutputData );
    	}
    return KErrNotSupported;
    }



// -----------------------------------------------------------------------------
// CMccVideoSource::SetVideoCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetVideoCodecL( const TMccCodecInfo& aVideoCodec )
	{
	__V_SOURCESINK_CONTROLL_STR8( 
	    "CMccVideoSource::SetVideoCodecL, sdpname:", aVideoCodec.iSdpName ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
    
    TMccVideoSourceUser* user = 
        MccUserArray<TMccVideoSourceUser>::FindUserEntryForCurrent( iUsers, iCurrentUser );
    if ( user )
        {
        user->iCodecInfo = aVideoCodec;
        }
        
    iSourceImpl->SetVideoCodecL( aVideoCodec );
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSource::GetVideoCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::GetVideoCodecL( TMccCodecInfo& aVideoCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::GetVideoCodecL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	iSourceImpl->GetVideoCodecL( aVideoCodec );	
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::GetSupportedVideoCodecsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::GetSupportedVideoCodecsL( RArray<TFourCC>& aVideoTypes )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::GetSupportedVideoCodecsL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	iSourceImpl->GetSupportedVideoCodecsL( aVideoTypes );		
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::SetAudioCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetAudioCodecL( const TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::SetAudioCodecL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->SetAudioCodecL( aAudioCodec );			
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::GetAudioCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::GetAudioCodecL( TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::GetAudioCodecL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->GetAudioCodecL( aAudioCodec );
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::GetSupportedAudioCodecsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::GetSupportedAudioCodecsL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->GetSupportedAudioCodecsL( aAudioTypes );
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::SetAudioEnabledL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetAudioEnabledL( TBool aEnabled )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::SetAudioEnabledL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->SetAudioEnabledL( aEnabled );
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::AudioEnabledL()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSource::AudioEnabledL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::AudioEnabledL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	return iSourceImpl->AudioEnabledL();
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::SetGainL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetGainL( TInt aGain )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::SetGainL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
	iSourceImpl->SetGainL( aGain );
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::GainL()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSource::GainL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::GainL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	return iSourceImpl->GainL();
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::MaxGainL()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSource::MaxGainL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSource::MaxGainL" ) 				
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	return iSourceImpl->MaxGainL();
	}

// -----------------------------------------------------------------------------
// CMccVideoSource::AddUser()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSource::AddUser( MAsyncEventHandler* aUser )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSource::AddUser, user:", reinterpret_cast<TInt>( aUser ) )
    TInt retVal( KErrNone );
    TMccVideoSourceUser* userEntry = 
        MccUserArray<TMccVideoSourceUser>::FindUserEntryForCurrent( iUsers, aUser );
    if ( !userEntry )
        {
        TRAP( retVal, AddUserL( aUser ) );
        }
    return retVal;
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::RemoveUser()
// -----------------------------------------------------------------------------
//        
void CMccVideoSource::RemoveUser( MAsyncEventHandler* aUser )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSource::RemoveUser, user:", reinterpret_cast<TInt>( aUser ) )

    MccUserArray<TMccVideoSourceUser>::RemoveCurrentUser( iUsers, aUser );
        
    if ( aUser == iCurrentUser )
        {
        if ( iUsers.Count() > 0 )
            {
            iCurrentUser = iUsers[ 0 ]->iEventHandler;
            TRAP_IGNORE( UpdateCurrentUserL() )
            }
        iCurrentUser = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SetCurrentUser()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::SetCurrentUser( MAsyncEventHandler* aUser )
    {
    iCurrentUser = aUser;
    }
    	
// -----------------------------------------------------------------------------
// CMccVideoSource::UpdateL()
// -----------------------------------------------------------------------------
//	
void CMccVideoSource::UpdateL( const TDesC8& aVal )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::UpdateL" ) 				

    // Only device index is used atm
    
    TMccVideoSourceSettingBuf settingsBuf;
    settingsBuf.Copy( aVal );
    TMccVideoSourceSetting& settings = settingsBuf();
    
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSource::UpdateL, current camera index:", 
                                  iCameraIndex ) 	
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSource::UpdateL, new camera index:", 
                                  settings.iDeviceIndex )
    	
    // If recorder is using the camera, camera index cannot be changed       
    if ( iSourceImpl && !iSourceImpl->IsStopped() )
        {
        __ASSERT_ALWAYS( iCameraIndex == settings.iDeviceIndex, 
                         User::Leave( KErrInUse ) );
        }
    iCameraIndex = settings.iDeviceIndex;
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::UpdateL, exit" ) 
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::GetParameterL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::GetParameterL( TUint32 aParam, TDes8& aVal )
    {
    switch ( aParam )
        {
        case KMccConfigKey:
            {
            __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrNotReady ) );
            TInt found( EFalse );
            for ( TInt i = 0; i < iUsers.Count() && !found; i++ )
            	{
            	if ( MCC_IS_AVC_USER_ENTRY( iUsers[ i ] ) )
            		{
            		iSourceImpl->GetConfigKeyL( iUsers[ i ]->iCodecInfo, aVal );
            		found = ETrue;
            		}
            	}
            __ASSERT_ALWAYS( found, User::Leave( KErrNotFound ) );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }  
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::SetParameterL()
// -----------------------------------------------------------------------------
//        
void CMccVideoSource::SetParameterL( TUint32 aParam, const TDesC8& /*aVal*/ )
    {
    switch ( aParam )
        {
        case KMccConfigKey:
            {
            // NOP
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }    
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::AddUserL()
// -----------------------------------------------------------------------------
// 
void CMccVideoSource::AddUserL( MAsyncEventHandler* aUser )
    {
    TMccVideoSourceUser* user = new ( ELeave ) TMccVideoSourceUser( aUser );
    CleanupStack::PushL( user );
    iUsers.AppendL( user );
    CleanupStack::Pop( user );
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::MultipleCodecs()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSource::MultipleCodecs() const
    {
    TBool multipleCodecs( EFalse );
    TMccCodecInfo* codecInfo = iUsers.Count() > 1 ? &iUsers[ 0 ]->iCodecInfo : NULL;
    for ( TInt i = 1; i < iUsers.Count() && codecInfo; i++ )
        {
        if ( codecInfo->iSdpName.CompareF( iUsers[ i ]->iCodecInfo.iSdpName ) != 0 )
            {
            multipleCodecs = ETrue;
            codecInfo = NULL;
            }
        }
    return multipleCodecs;
    }

// -----------------------------------------------------------------------------
// CMccVideoSource::UpdateCurrentUserL()
// -----------------------------------------------------------------------------
//
void CMccVideoSource::UpdateCurrentUserL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::UpdateCurrentUserL" ) 
    
    TMccVideoSourceUser* userEntry = 
        MccUserArray<TMccVideoSourceUser>::FindUserEntryForCurrent( iUsers, iCurrentUser );

    // If multiple different codecs may be using the video source, update source
    // according to current user

    TMccCodecInfo currCodecInfo;
    iSourceImpl->GetVideoCodecL( currCodecInfo );
    if ( userEntry && currCodecInfo.iSdpName.CompareF( userEntry->iCodecInfo.iSdpName ) != 0 )
        {
        iSourceImpl->SetVideoCodecL( userEntry->iCodecInfo );
        }
   
    __V_SOURCESINK_CONTROLL( "CMccVideoSource::UpdateCurrentUserL, exit" )
    }
        
// End of file

