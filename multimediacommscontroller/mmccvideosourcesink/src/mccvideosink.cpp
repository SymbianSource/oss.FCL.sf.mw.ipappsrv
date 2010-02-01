/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mccvideosink.h"
#include "mccvideosinkimpl.h"
#include "mmcccodecinformation.h"
#include "mccinternalcodecs.h"
#include "mccvideosourcesinklogs.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"
#include "rtpheader.h"

// MACROS

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccVideoSink::CMccVideoSink
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccVideoSink::CMccVideoSink() :
    CMccDataSink( KMccVideoSinkUid )
    {
    iAudioFourCC = TFourCC( KMccFourCCIdAMRNB );
    iVideoFourCC = TFourCC( KMccFourCCIdH263 );
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::NewSinkL
// Static constructor.
// -----------------------------------------------------------------------------
//
MDataSink* CMccVideoSink::NewSinkL( TUid /*aImplementationUid*/, 
                                      const TDesC8& /*aInitData*/ )
    {       
    __V_SOURCESINK_CONTROLL("CMccVideoSink::NewSinkL" )
    CMccVideoSink* self = new ( ELeave ) CMccVideoSink();
    return static_cast<MDataSink*>( self ); 
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::ConstructSinkL
// class MDataSink inherited 2nd phase construction
// -----------------------------------------------------------------------------
//
void CMccVideoSink::ConstructSinkL( const TDesC8& aInitData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::ConstructSinkL (initdata)" )

    __ASSERT_ALWAYS( aInitData.Length(), User::Leave( KErrArgument ) );
    
    TMccVideoSinkSettingBuf settingsBuf;
        settingsBuf.Copy( aInitData );
        TMccVideoSinkSetting& settings = settingsBuf();
                               
    iAudioReady = ETrue;
    iVideoReady = ETrue;
	
	delete iMediaInterface;
	iMediaInterface = NULL;    
    iMediaInterface = CMccVideoSinkImpl::NewL( 
            settings, 
            MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) ) );
                                 
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::ConstructSinkL complete")
    

    }

// -----------------------------------------------------------------------------
// CMccVideoSink::~CMccVideoSink
// Destructor
// -----------------------------------------------------------------------------
//
CMccVideoSink::~CMccVideoSink()
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSink::~CMccVideoSink" )
    delete iMediaInterface;
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSink::SinkThreadLogon
//
// Method to 'logon' the data sink to the same thread that sink will be consuming
// data in. Thread specific initialisation is done here.
// -----------------------------------------------------------------------------
//
TInt CMccVideoSink::SinkThreadLogon( MAsyncEventHandler& aEventHandler ) 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkThreadLogon" )

	if ( iMediaInterface )
		{
		return iMediaInterface->SinkThreadLogon( aEventHandler );
		}
	else
		{
		return KErrNotFound;	
		}
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkThreadLogoff
//
// Method to 'logoff' the data sink from the same thread that sink consumes 
// data in. Thread specific releasing of resources is done here.
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SinkThreadLogoff() 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkThreadLogoff" )
    if ( iMediaInterface )
        {        
        __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkThreadLogoff, stopping player" )
        if ( iMediaInterface->SinkThreadLogoff() )
            {
            iMediaInterface->Stop();
            }
        }
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkThreadLogoff, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SetSinkDataTypeCode
// Sets the datatype code ( codec )
// ----------------------------------------------------------------------------- 
//
TInt CMccVideoSink::SetSinkDataTypeCode( TFourCC aCodec, TMediaId aMediaId )
    {
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSink::SetSinkDataTypeCode, fourcc:",
    aCodec.FourCC(), "media type:", aMediaId.iMediaType.iUid )	

    if ( KUidMediaTypeVideo == aMediaId.iMediaType &&
    	aCodec == iVideoFourCC )
        {
        iVideoReady = EFalse;
        return KErrNone;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType &&
    	aCodec == iAudioFourCC )
        {
        iAudioReady = EFalse;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
	
	}

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkDataTypeCode
// Returns the datatype code ( codec )
// -----------------------------------------------------------------------------
//
TFourCC CMccVideoSink::SinkDataTypeCode( TMediaId aMediaId )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkDataTypeCode" ) 

    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        iVideoReady = EFalse;
        return iVideoFourCC;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType  )
        {
        iAudioReady = EFalse;
        return iAudioFourCC;
        }
    else
        {
        return TFourCC( KMMFFourCCCodeNULL );
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::EmptyVideoSinkBufferL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
void CMccVideoSink::EmptyBufferL( CMMFBuffer* aBuffer, 
                                  MDataSource* aSupplier, 
                                  TMediaId aMediaId )
    {
    __V_SOURCESINK_MEDIA( "CMccVideoSink::EmptyBufferL" ) 
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
                         
    TRtpRecvHeader header;
    header.iTimestamp = aBuffer->TimeToPlay().Int64();
	header.iMarker = aBuffer->LastBuffer();
	header.iSeqNum = aBuffer->FrameNumber();
	
	if ( aMediaId.iMediaType == KUidMediaTypeAudio )
	    {
	    header.iPayloadType = iAudioPayloadType;
	    }
	else
	    {
	    header.iPayloadType = iVideoPayloadType;
	    }
	
    EmptyVideoSinkBufferL(aBuffer, aSupplier, aMediaId, header);
    aSupplier->BufferEmptiedL( aBuffer );
    }
 
// -----------------------------------------------------------------------------
// CMccVideoSink::SetVideoCodecL
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SetVideoCodecL( const TMccCodecInfo& aVideoCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSink::SetVideoCodecL" ) 
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );

	iVideoFourCC = aVideoCodec.iFourCC;
    iMaxVideoBitRate = aVideoCodec.iMaxBitrate;	
	iVideoCodecMode = aVideoCodec.iCodecMode;
	iVideoPayloadType = aVideoCodec.iPayloadType;
	iMediaInterface->FormatVideoSDPInfoL( aVideoCodec );	
    iVideoReady = ETrue;		
	}

// -----------------------------------------------------------------------------
// CMccVideoSink::SetAudioCodecL
// -----------------------------------------------------------------------------
//	
void CMccVideoSink::SetAudioCodecL( const TMccCodecInfo& aAudioCodec )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SetAudioCodecL" ) 
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );
    
    iAudioFourCC = aAudioCodec.iFourCC;
	iAudioCodecMode = aAudioCodec.iCodecMode;
	iAudioPayloadType = aAudioCodec.iPayloadType;
	iMediaInterface->FormatAudioSDPInfoL( aAudioCodec );	
    iAudioReady = ETrue;
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SetCurrentUser
// -----------------------------------------------------------------------------
//    
void CMccVideoSink::SetCurrentUser( MAsyncEventHandler* aEventHandler )
    {
   	__V_SOURCESINK_CONTROLL( "CMccVideoSink::SetCurrentUser" ) 
    if ( iMediaInterface )
        {
        iMediaInterface->SetCurrentUser( aEventHandler );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSink::EmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccVideoSink::EmptyVideoSinkBufferL( CMMFBuffer* aBuffer, 
		                          MDataSource* aSupplier,
		                          TMediaId aMediaId,
		                          TRtpRecvHeader& aHeaderInfo )
	{
    __V_SOURCESINK_MEDIA( "CMccVideoSink::EmptyVideoSinkBufferL with \
RTP header param:" )   
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iPadding=" , 
                             aHeaderInfo.iPadding )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iExtension=", 
                             aHeaderInfo.iExtension )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iCsrcCount=", 
                             aHeaderInfo.iCsrcCount )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iMarker=", 
                             aHeaderInfo.iMarker )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iPayloadType=", 
                             aHeaderInfo.iPayloadType )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iSeqNum=", 
                             aHeaderInfo.iSeqNum )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iTimestamp=", 
                             aHeaderInfo.iTimestamp )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iHeaderExtension=",
                             (TInt)aHeaderInfo.iHeaderExtension )
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSink::EmptyVideoSinkBufferL iCsrcList=",
                             (TInt)aHeaderInfo.iCsrcList )
                          
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    
    iMediaInterface->EmptyBufferL( aBuffer, aSupplier, aMediaId, aHeaderInfo );    	
	}
	
		                       
// -----------------------------------------------------------------------------
// CMccVideoSink::BufferFilledL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
void CMccVideoSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    __V_SOURCESINK_MEDIA( "CMccVideoSink::EmptyBufferL, \
Leave with KErrNotSupported" )

    __ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::CanCreateSinkBuffer
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
TBool CMccVideoSink::CanCreateSinkBuffer()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::CanCreateSinkBuffer, EFalse" ) 
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::CreateSinkBufferL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccVideoSink::CreateSinkBufferL( TMediaId /*aMediaId*/,
                                                TBool& /*aReference*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::CreateSinkBufferL, \
leave with KErrNotSupported" ) 
	User::Leave( KErrNotSupported );	
	return NULL;
	}

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkPrimeL
// Prime the sink. 
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SinkPrimeL()
    { 
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkPrimeL" )
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );

    if ( iVideoReady && iAudioReady )
        {        
        iMediaInterface->OpenL();
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkPlayL
// Start the playout operation.
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SinkPlayL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkPlayL" )
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );
    iMediaInterface->StartL();
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkPauseL
// Pauses the playout operation.
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SinkPauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkPauseL" )
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );
    iMediaInterface->PauseL();
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SinkStopL
// Stop the playout operation. 
// -----------------------------------------------------------------------------
//
void CMccVideoSink::SinkStopL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkStopL" )
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrArgument ) );
    iMediaInterface->Stop();  
    }

// -----------------------------------------------------------------------------
// CMccVideoSink::SetSinkPrioritySettings
// -----------------------------------------------------------------------------
//   
void CMccVideoSink::SetSinkPrioritySettings( 
    const TMMFPrioritySettings& aPrioritySettings )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SetSinkPrioritySettings" )
    
    if ( iMediaInterface )
        {
        iMediaInterface->SetPriority( aPrioritySettings.iPriority, 
                                      aPrioritySettings.iPref );
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSink::SinkCustomCommand()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSink::SinkCustomCommand( TMMFMessage& /*aMessage*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SinkCustomCommand" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSink::GetParameterL()
// ---------------------------------------------------------------------------
//
void CMccVideoSink::GetParameterL( TUint32 aParam, TDes8& aVal )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::GetParameterL" )
    
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrNotReady ) );
    
        
    switch ( aParam )
        {
        case KMccDisplayLocalVideo:
            {
            TMccVideoSinkSettingBuf settingsBuf;
	        settingsBuf.Copy( aVal );
	        TMccVideoSinkSetting& settings = settingsBuf();
            iMediaInterface->GetCurrentSettings( settings );
            aVal.Copy( settingsBuf );
            break;
            }
        case KMccPrerollTime:
            {
            TInt preroll = 0;
            iMediaInterface->GetActualPrerollL( preroll );
            (*reinterpret_cast<TPckgBuf<TInt>* >(&aVal))() = preroll;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
        
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::GetParameterL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSink::SetParameterL() 
// ---------------------------------------------------------------------------
//       
void CMccVideoSink::SetParameterL( TUint32 aParam, const TDesC8& aVal )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SetParameterL" )
    
    __ASSERT_ALWAYS( iMediaInterface, User::Leave( KErrNotReady ) );
 
    switch ( aParam )
        {
        case KMccDisplayLocalVideo:
            {
            TMccVideoSinkSettingBuf settingsBuf;
        	settingsBuf.Copy( aVal );
        	TMccVideoSinkSetting& settings = settingsBuf();
            iMediaInterface->UpdateSettingsL( settings );
            break;
            }
        case KMccSpeakerVolume:
            {
            TPckgBuf<TInt> volBuf;
        	volBuf.Copy( aVal );
            iMediaInterface->SetVolumeL( volBuf() );
            break;
            }
        case KMccSpeakerMaxVolume:
            {
            TPckgBuf<TInt> maxVolBuf;
        	maxVolBuf.Copy( aVal );
            iMediaInterface->SetMaxVolume( maxVolBuf() );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSink::SetParameterL, exit" )
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  

