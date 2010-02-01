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
#include "mccfilesource.h"
#include "mccfilesourceimpl.h"
#include "mmcccodecinformation.h"
#include "mmccinterfacedef.h"
#include "mccfilesourcelogs.h"
#include "mccinternaldef.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccFileSource::NewSourceL
// -----------------------------------------------------------------------------
//
MDataSource* CMccFileSource::NewSourceL( TUid /*aImplementationUid*/, 
                                         const TDesC8& /*aInitData*/ )
    {
    CMccFileSource* self = new ( ELeave ) CMccFileSource();
    return static_cast<MDataSource*>( self );
    }

// -----------------------------------------------------------------------------
// CMccFileSource::ConstructSourceL
// -----------------------------------------------------------------------------
//
void CMccFileSource::ConstructSourceL( const TDesC8& aInitData )
    {    	
	iSourceImpl = CMccFileSourceImpl::NewL( 
        MCC_ENDPOINT_ID( static_cast<MDataSource*>( this ) ) );
  
    TPckgBuf<TFileName> initData;
    initData.Copy( aInitData );
    
    OpenFileL( initData() );
    }
	    
// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
CMccFileSource::CMccFileSource (): 
    CMccDataSource( KMccFileSourceUid )
    {
    }
        
// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMccFileSource::~CMccFileSource ()
    {
	delete iSourceImpl;    
    }

// -----------------------------------------------------------------------------
// CMccFileSource::GetParameterL
// -----------------------------------------------------------------------------
//    
void CMccFileSource::GetParameterL( TUint32 aParam, TDes8& aVal )
    {
    if ( aParam == KMccFileFastForward )
        {
        // aVal is TPckgBuf<TInt>
        User::Leave( KErrNotSupported );
        }
    else
        {
        TMccFileSourceSetting& settings = 
            reinterpret_cast<TMccFileSourceSettingBuf&>( aVal )();
        
        switch ( aParam )
            {
            case KMccFileInfo:
                {
                // Read all available values 
                GetFileInfoL( settings );
                break;
                }
            case KMccFileName:
                {
                settings.iFileName = FileNameL();
                break;
                }
            case KMccFileDuration:
                {
                settings.iDuration = DurationL();
                break;
                }
            case KMccFilePosition:
                {
                settings.iPosition = PositionL();
                break;
                }
            case KMccFileVideoFrameRate:
                {
                settings.iVideoFrameRate = VideoFrameRateL();
                break;
                }
            case KMccFileVideoBitRate:
                {
                settings.iVideoBitRate = VideoBitRateL();
                break;
                } 
            case KMccFileVideoFrameSize:
                {
                settings.iVideoFrameSize = VideoFrameSizeL();
                break;
                }
            case KMccFileAudioBitRate:
                {
                settings.iAudioBitRate = AudioBitRateL();
                break;
                }
            case KMccFileAudioFourCC:
                {
                settings.iAudioFourCC = AudioFourCCL();
                break;
                }
            case KMccFileVideoFourCC:
                {
                settings.iVideoFourCC = VideoFourCCL();
                break;
                }
            case KMccConfigKey:
                {
                __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );
                iSourceImpl->GetConfigKeyL( aVal );
                break;
                }
            default:
                {
                User::Leave( KErrArgument );
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccFileSource::SetParameterL
// -----------------------------------------------------------------------------
//
void CMccFileSource::SetParameterL( TUint32 aParam, const TDesC8& aVal )
    {
    if ( aParam == KMccFileFastForward )
        {
        // aVal is TPckgBuf<TInt>
        User::Leave( KErrNotSupported );
        }
    else if ( aParam == KMccPrerollTime )
        {
        __ASSERT_ALWAYS(iSourceImpl, User::Leave(KErrNotReady));
        const TPckgBuf<TInt> *packageBuffer = 
            reinterpret_cast<const TPckgBuf<TInt>* >(&aVal);
        __ASSERT_DEBUG( packageBuffer != NULL, 
            User::Panic(_L("CMccFileSource::SetParameterL: casting packageBuffer failed"), 
            KErrArgument) );
        __ASSERT_ALWAYS( packageBuffer != NULL, User::Leave(KErrArgument) );
        TInt preroll = (*packageBuffer)();
        iSourceImpl->SetDelayAfterStop(preroll);
        }
    else
        {
        const TMccFileSourceSetting& settings = 
            reinterpret_cast<const TMccFileSourceSettingBuf&>( aVal )();
            
        switch ( aParam )
            {
            case KMccFileName:
                {
                OpenFileL( settings.iFileName, ETrue );
                break;
                }
            case KMccFilePosition:
                {
                SetPositionL( settings.iPosition );
                break;
                }
            case KMccFileVideoFrameRate:
            case KMccFileVideoBitRate:
            case KMccFileVideoFrameSize:
            case KMccFileAudioBitRate:
            case KMccFileDuration:
            case KMccFileInfo:
            case KMccFileAudioFourCC:
            case KMccFileVideoFourCC:
            default:
                {
                User::Leave( KErrArgument );
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccFileSource::OpenFileL(TFileName aFileName)
//
// Opens a 3gp file for streaming and reads media descriptions into
// member variables
// -----------------------------------------------------------------------------
//
void CMccFileSource::OpenFileL( TFileName aFileName, TBool aFileChangeMode )
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	iSourceImpl->OpenFileL( aFileName, aFileChangeMode );
    }

// -----------------------------------------------------------------------------
// CMccFileSource::CloseFileL()
//
// Closes the 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSource::CloseFileL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	iSourceImpl->CloseFileL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::FileNameL()
//
// Gets filename
// -----------------------------------------------------------------------------
//   
TFileName& CMccFileSource::FileNameL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

	return iSourceImpl->FileNameL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::SetPositionL(TTimeIntervalMicroSeconds aPosition)
// 
// Sets a new streaming position
// -----------------------------------------------------------------------------
//
void CMccFileSource::SetPositionL( TTimeIntervalMicroSeconds aPosition )
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->SetPositionL( aPosition );
	}
    
// -----------------------------------------------------------------------------
// CMccFileSource::VideoFrameRateL()
// 
// Returns current video frame rate
// -----------------------------------------------------------------------------
//
TReal CMccFileSource::VideoFrameRateL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->VideoFrameRateL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::AudioBitRateL()
// 
// Returns current audio bit rate
// -----------------------------------------------------------------------------
//
TUint32 CMccFileSource::AudioBitRateL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->AudioBitRateL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::DurationL()
// 
// Returns duration of current media clip
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSource::DurationL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->DurationL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::VideoFrameSizeL()
// 
// Returns current video frame size
// -----------------------------------------------------------------------------
//
TSize CMccFileSource::VideoFrameSizeL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->VideoFrameSizeL();
    }
    
// -----------------------------------------------------------------------------
// CMccFileSource::VideoBitRateL()
// 
// Returns current video bitrate
// -----------------------------------------------------------------------------
//
TUint32 CMccFileSource::VideoBitRateL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->VideoBitRateL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::PositionL()
// 
// Returns current streaming position
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSource::PositionL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->PositionL();	
    }

// -----------------------------------------------------------------------------
// CMccFileSource::AudioFourCCL()
// 
// Returns audio fourcc (zero if audio not present)
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSource::AudioFourCCL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->AudioFourCCL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::VideoFourCCL()
// 
// Returns video fourcc (zero if video not present)
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSource::VideoFourCCL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    return iSourceImpl->VideoFourCCL();
    } 
    
// -----------------------------------------------------------------------------
// CMccFileSource::PlayL()
// 
// Starts streaming from 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSource::SourcePlayL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::PauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccFileSource::SourcePauseL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->SourcePauseL();
	}

// -----------------------------------------------------------------------------
// CMccFileSource::StopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccFileSource::SourceStopL()
    {
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CMccFileSource::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSource::SourceDataTypeCode( TMediaId aMediaId )
	{
	if ( iSourceImpl )
		{
    	return iSourceImpl->SourceDataTypeCode( aMediaId );	
		}
	else
		{
		__FILESOURCE_CONTROLL( "CMccFileSource::SourceDataTypeCode,\
iSourceImpl= NULL" )
		return TFourCC();
		}
	}
	
// -----------------------------------------------------------------------------
// CMccFileSource::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccFileSource::SetSourceDataTypeCode( TFourCC aCodec, 
                            TMediaId aMediaId )
	{
	if ( iSourceImpl )
		{
        return iSourceImpl->SetSourceDataTypeCode( aCodec, aMediaId );	
		}
	else
		{
		__FILESOURCE_CONTROLL( "CMccFileSource::SetSourceDataTypeCode,\
iSourceImpl= NULL" )

		return KErrNotFound;	
		}
	}	

// -----------------------------------------------------------------------------
// CMccFileSource::BufferEmptiedL()
// -----------------------------------------------------------------------------
//
void CMccFileSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __FILESOURCE_CONTROLL( "CMccFileSource::BufferEmptiedL" )
    
   	__ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );
	}
	
// -----------------------------------------------------------------------------
// CMccFileSource::CanCreateSourceBuffer()
// -----------------------------------------------------------------------------
//
TBool CMccFileSource::CanCreateSourceBuffer()
	{
	__FILESOURCE_CONTROLL( "CMccFileSource::CanCreateSourceBuffer" )

    return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccFileSource::CreateSourceBufferL()
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccFileSource::CreateSourceBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__FILESOURCE_CONTROLL( "CMccFileSource::CreateSourceBufferL" )

    CMMFBuffer* buffer = NULL;
   
   	__ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );
	
	return buffer;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileSource::SourceThreadLogon()
// -----------------------------------------------------------------------------
//
TInt CMccFileSource::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
    __FILESOURCE_CONTROLL( "CMccFileSource::SourceThreadLogon" )
    
    if ( iSourceImpl )
	    {
	    return iSourceImpl->SourceThreadLogon( aEventHandler );
	    }
	else
		{
		__FILESOURCE_CONTROLL( "CMccFileSource::SourceThreadLogon, \
iSourceImpl=NULL" )

		return KErrNotFound;	
		}
	}
	
// -----------------------------------------------------------------------------
// CMccFileSource::SourceThreadLogoff()
// -----------------------------------------------------------------------------
//
void CMccFileSource::SourceThreadLogoff()
	{
    __FILESOURCE_CONTROLL( "CMccFileSource::SourceThreadLogoff" )
    if ( iSourceImpl )
	    {
	    iSourceImpl->SourceThreadLogoff();
	    TRAP_IGNORE( iSourceImpl->SourceStopL() );	
	    }
	else
		{
		__FILESOURCE_CONTROLL( "CMccFileSource::SourceThreadLogoff, \
iSourceImpl=NULL" )
		}
	}
	
// -----------------------------------------------------------------------------
// CMccFileSource::SourcePrimeL()
// -----------------------------------------------------------------------------
//
void CMccFileSource::SourcePrimeL()
	{
    __FILESOURCE_CONTROLL( "CMccFileSource::SourcePrimeL" )
    __ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->SourcePrimeL();	
	}

// -----------------------------------------------------------------------------
// CMccFileSource::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccFileSource::FillBufferL( 
	CMMFBuffer* aBuffer,
    MDataSink* aConsumer,
    TMediaId aMediaId )
	{
	__ASSERT_ALWAYS( iSourceImpl, User::Leave( KErrArgument ) );

    iSourceImpl->FillBufferL( aBuffer, aConsumer, aMediaId );
	}	
                  
// -----------------------------------------------------------------------------
// CMccFileSource::NegotiateSourceL()
// -----------------------------------------------------------------------------
//
void CMccFileSource::NegotiateSourceL( MDataSink& /*aDataSink*/ )
	{
    __FILESOURCE_CONTROLL( "CMccFileSource::NegotiateSourceL" )
	}

// -----------------------------------------------------------------------------
// CMccFileSource::GetFileInfoL()
// -----------------------------------------------------------------------------
//
void CMccFileSource::GetFileInfoL( TMccFileSourceSetting& aSettings )
	{
    aSettings.iFileName = FileNameL();
    aSettings.iDuration = DurationL();
    aSettings.iPosition = PositionL();
    aSettings.iVideoFrameRate = VideoFrameRateL();
    aSettings.iVideoBitRate = VideoBitRateL();
    aSettings.iVideoFrameSize = VideoFrameSizeL();
    aSettings.iAudioBitRate = AudioBitRateL();
    aSettings.iAudioFourCC = AudioFourCCL();
    aSettings.iVideoFourCC = VideoFourCCL();
        
    __FILESOURCE_CONTROLL( "CMccFileSource::GetFileInfoL, file:" )
    #ifdef __MCC_FILESOURCE_CONTROLL
        TMccLog::Print( KFileSource, aSettings.iFileName );  
    #endif
    __FILESOURCE_CONTROLL_INT1( "duration:", aSettings.iDuration.Int64() )  
    __FILESOURCE_CONTROLL_INT1( "position:", aSettings.iPosition.Int64() )  
    __FILESOURCE_CONTROLL_INT1( "videoframerate:", (TInt) aSettings.iVideoFrameRate )  
    __FILESOURCE_CONTROLL_INT1( "videobitrate:", aSettings.iVideoBitRate )  
    __FILESOURCE_CONTROLL_INT2( "videoframesize width:", 
                                aSettings.iVideoFrameSize.iWidth, 
                                " height:",
                                aSettings.iVideoFrameSize.iHeight )  
    __FILESOURCE_CONTROLL_INT1( "audiobitrate:", aSettings.iAudioBitRate ) 
    __FILESOURCE_CONTROLL_INT1( "audio fourcc:", aSettings.iAudioFourCC.FourCC() ) 
    __FILESOURCE_CONTROLL_INT1( "video fourcc:", aSettings.iVideoFourCC.FourCC() ) 
    __FILESOURCE_CONTROLL( "CMccFileSource::GetFileInfoL, exit" ) 
    }


