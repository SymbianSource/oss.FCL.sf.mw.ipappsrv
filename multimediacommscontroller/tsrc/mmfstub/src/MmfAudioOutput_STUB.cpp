/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "MmfAudioOutput.h"
#include <ecom/implementationproxy.h>
#include <mmf/server/mmfformat.h>

#include "mcctestuids.hrh"


void Panic(TInt aPanicCode)
	{
	_LIT(KMMFAudioOutputPanicCategory, "MMFAudioOutput");
	User::Panic(KMMFAudioOutputPanicCategory, aPanicCode);
	}

/**
Allocates and constructs a new audio output sink.

Static standard SymbianOS 2 phase constuction method.

@return A pointer to the new sink.
*/
MDataSink* CMMFAudioOutput::NewSinkL()
	{
	CMMFAudioOutput* self = new (ELeave) CMMFAudioOutput ;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return static_cast<MDataSink*>( self ); 
	}

/**
Standard SymbianOS ConstructL.

Used to initialise member varibles with device specific behaviour.
*/
void CMMFAudioOutput::ConstructL()
	{
	iInitializeState = KErrNone;
	iDataTypeCode = KMMFFourCCCodePCM16;
	iNeedsSWConversion = EFalse;
	iSourceSampleRate = 0;
	}

/**
Overridable constuction specific to this datasource.

The default implementation does nothing.

@param  aInitData
        The initialisation data.
*/
void CMMFAudioOutput::ConstructSinkL( const TDesC8& /*aInitData*/ )
	{
	}


/**
@deprecated

Gets audio from hardware device abstracted MMFDevsound (not used).

@param  aBuffer
        The data to write out to a Hardware Device.
@param  aSupplier
        The MDataSource consuming the data contained in aBuffer
*/
void CMMFAudioOutput::HWEmptyBufferL(CMMFBuffer* /*aBuffer*/, MDataSource* /*aSupplier*/)
	{
	}

/**
Sends audio to MMFDevsound.

@param  aBuffer
        The data to write out.
@param  aSupplier
        The search criteria for the supplier.
@param  aMediaId
        The type of data supplied - currently ignored.
*/
void CMMFAudioOutput::EmptyBufferL(CMMFBuffer* aBuffer, MDataSource* aSupplier, TMediaId /*aMediaId*/)
	{
	iSupplier = aSupplier;

	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);

	if ((aBuffer != NULL) && (!CMMFBuffer::IsSupportedDataBuffer(aBuffer->Type())))
		User::Leave(KErrNotSupported);

	if (aSupplier == NULL)
		User::Leave(KErrArgument);
	}



/**
Negotiates with the source to set, for example, the sample rate and number of channels.

Called if the sink's setup depends on source.

@param  aSource
        The data source with which to negotiate.
*/
void CMMFAudioOutput::NegotiateL(MDataSource& aSource)
	{
	if (aSource.DataSourceType() == KUidMmfFormatDecode)
		{//source is a clip so for now set sink settings to match source
		iSourceSampleRate = ((CMMFFormatDecode&)aSource).SampleRate();
		iSourceChannels = ((CMMFFormatDecode&)aSource).NumChannels(); 
		iSourceFourCC.Set(aSource.SourceDataTypeCode(TMediaId(KUidMediaTypeAudio)));
	
		((CMMFFormatDecode&)aSource).SuggestSourceBufferSize(KAudioOutputDefaultFrameSize);
		}
		
	// Query DevSound capabilities and Try to use DevSound sample rate and 
	// mono/stereo capability
	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);

	TMMFState prioritySettingsState = iPrioritySettings.iState; //should be EMMFStatePlaying
	//to use the GetSupportedInputDatatypes but we'll save it just in case it's not
	iPrioritySettings.iState = EMMFStatePlaying; //if playing does not support any output data types
	iPrioritySettings.iState = prioritySettingsState;
	
	iDataTypeCode = KMMFFourCCCodePCM16;	
	}

/**
Sets the sink's priority settings.

@param  aPrioritySettings
        The sink's priority settings. Takes enumerations to determine audio playback priority. 
        Higher numbers mean high priority (can interrupt lower priorities).
*/
void CMMFAudioOutput::SetSinkPrioritySettings(const TMMFPrioritySettings& aPrioritySettings)
	{
	iPrioritySettings = aPrioritySettings;
	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);
	}

/**
Gets the sink's data type code.

Used by datapath MDataSource / MDataSink for codec matching.

@param  aMediaId
        The Media ID. Optional parameter to specifiy specific stream when datasource contains more
        than one stream of data.

@return	The 4CC of the data expected by this sink.
*/
TFourCC CMMFAudioOutput::SinkDataTypeCode(TMediaId /*aMediaId*/)
	{
	return iDataTypeCode;
	}

/**
Sets the sink's data type code.

@param  aSinkFourCC
        The 4CC of the data to be supplied to this sink.
@param  aMediaId
        The Media ID. Optional parameter to specifiy specific stream when datasource contains more 
        than one stream of data.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
TInt CMMFAudioOutput::SetSinkDataTypeCode(TFourCC aSinkFourCC, TMediaId /*aMediaId*/)
	{//will check with devsound to see if aSinkFourCC is supported
	 //when this is added to devsound
	iDataTypeCode = aSinkFourCC;
	return KErrNone;
	}

/**
Prime's the sink.

This is a virtual function that each derived class must implement, but may be left blank for default 
behaviour.

Called by CMMFDataPath::PrimeL().
*/
void CMMFAudioOutput::SinkPrimeL()
	{
	iFirstBufferSent = EFalse;
	if (iState == EIdle)
		{
		if (!iMMFDevSound) User::Leave(KErrNotReady);
		iState = EDevSoundReady;
		}
	}

/**
Pauses the sink.

This is a virtual function that each derived class must implement, but may be left blank for default 
behaviour.
*/
void CMMFAudioOutput::SinkPauseL()
	{
	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);
	else
		iMMFDevSound->Pause();
	iFirstBufferSent = EFalse;
	iState = EPaused;
	}

/**
Starts playing the sink.

This is a virtual function that each derived class must implement, but may be left blank for default
behaviour.
*/
void CMMFAudioOutput::SinkPlayL()
	{
	if (iState == EPaused)
		{
		// DevSound remains initialised when paused
		return;
		}
	iState = EDevSoundReady;
	}

/**
Stops the sink.

This is a virtual function that each derived class must implement, but may be left blank for default
behaviour.
*/
void CMMFAudioOutput::SinkStopL()
	{
	if (iState == EDevSoundReady)
		{//not waiting on a buffer being played so stop devsound now
		iState = EIdle;
		if (iFirstBufferSent)
			{
			if (!iMMFDevSound)
				{
				Panic(EMMFAudioOutputDevSoundNotLoaded);
				}
			else
				{
				iFirstBufferSent = EFalse;
				iMMFDevSound->Stop();
				}
			}
		}
	else if (iState == EPaused)	//DEF46250 need to handle pause separately as we should always stop regardless of the state of iFirstBufferSent
		{
		iFirstBufferSent = EFalse;
		iMMFDevSound->Stop();
		iState = EIdle; 
		}
	} 

/**
Returns the playback state (EStopped, EPlaying, EPaused etc) of this sink
*/
TInt CMMFAudioOutput::State()
	{
	return iState;
	}

/**
Logs on the sink's thread.

Thread specific initialization procedure for this device. Runs automatically on thread construction.

@param  aEventHandler
        The event handler.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
TInt CMMFAudioOutput::SinkThreadLogon(MAsyncEventHandler& aEventHandler)
	{
	iEventHandler = &aEventHandler;
	TInt err = KErrNone;
	if (!iDevSoundLoaded)
		TRAP(err, LoadL());
	return err;
	}

/**
Logs off the sink thread.

Thread specific destruction procedure for this device. Runs automatically on thread destruction.
*/
void CMMFAudioOutput::SinkThreadLogoff()
	{
	if(iMMFDevSound)
		{
		iMMFDevSound->Stop();
		delete iMMFDevSound;
		iMMFDevSound = NULL;
		}
	iDevSoundLoaded = EFalse;
	iState = EIdle;
	}

/**
Called by MDataSource to pass back a full buffer to the sink. 

Should never be called by a sink, as sinks empty buffers, not fill them.

@param  aBuffer
        The filled buffer.
*/
void CMMFAudioOutput::BufferFilledL(CMMFBuffer* /*aBuffer*/)
	{
	Panic(EMMFAudioOutputPanicBufferFilledLNotSupported);
	}

/**
Tests whether a sink buffer can be created.

The default implementation returns true.

@return A boolean indicating if the sink buffer can be created. ETrue if it can, otherwise EFalse.
*/
TBool CMMFAudioOutput::CanCreateSinkBuffer()
	{
	return ETrue;
	}

/**
Creates a sink buffer.

Intended for asynchronous usage (buffers supplied by Devsound device)

@param  aMediaId
        The Media ID.
@param	aReference
		A boolean indicating if MDataSink owns the buffer. ETrue if does, otherwise EFalse.

@return A sink buffer.
*/
CMMFBuffer* CMMFAudioOutput::CreateSinkBufferL(TMediaId /*aMediaId*/, TBool &aReference)
	{
	//iDevSoundBuffer = CMMFDataBuffer::NewL(KAudioOutputDefaultFrameSize);
	iDevSoundBuffer = NULL;		//DevSound supplies this buffer in first callback
	aReference = ETrue;
	if ( iNeedsSWConversion )
		return iConvertBuffer;
	else
		return iDevSoundBuffer;
	}

/**
Standard SymbianOS destructor.
*/
CMMFAudioOutput::~CMMFAudioOutput()
	{
	// The following will never have been allocated unless
	// software conversion was required, and due to certain DevSound
	// implementations, this requirement can change dynamically.
	delete iChannelAndSampleRateConverterFactory;
	delete iConvertBuffer;

	if (iMMFDevSound)
		{
		iMMFDevSound->Stop();
		
		}
    delete iMMFDevSound;
    iMMFDevSound = NULL;
	}

void CMMFAudioOutput::ConfigDevSoundL()
	{
	iMMFDevSound->SetConfigL(iDevSoundConfig);
	}


/**
@deprecated

This method should not be used - it is provided to maintain SC with v7.0s.

@param  aAudioType
        The 4CC of the data supplied by this source.
*/
void CMMFAudioOutput::SetDataTypeL(TFourCC aAudioType)
	{
	if (aAudioType != KMMFFourCCCodePCM16)
		{
		User::Leave(KErrNotSupported);
		}
	}


/**
@deprecated

This method should not be used - it is provided to maintain SC with v7.0s.

@return The 4CC of the data supplied by this source.
*/
TFourCC CMMFAudioOutput::DataType() const
	{
	return KMMFFourCCCodePCM16;
	}


/**
Loads audio device drivers and initialise this device.
*/
void CMMFAudioOutput::LoadL()
	{
	if ( iMMFDevSound )
	    {
	    iDevSoundLoaded = ETrue;
	    return;
	    }
	    
	iFirstBufferSent = EFalse;
	if (iState != EDevSoundReady)
		iState = EIdle;
	
	iMMFDevSound = CMMFDevSound::NewL();

	//This is done to maintain compatibility with the media server
	iMMFDevSound->SetVolume(iMMFDevSound->MaxVolume() - 1);

	//note cannot perform further initlaisation here untill the datatype is known

	iDevSoundLoaded = ETrue;
	}

/**
DeviceMessage MMFDevSoundObserver
*/
void CMMFAudioOutput::DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/)
	{
	}


/**
ToneFinished MMFDevSoundObserver called when a tone has finished or interrupted

Should never get called.
*/
void CMMFAudioOutput::ToneFinished(TInt /*aError*/)
	{
	//we should never get a tone error in MMFAudioOutput!
	__ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicToneFinishedNotSupported));
	}


/**
RecordError MMFDevSoundObserver called when recording has halted.

Should never get called.
*/
void CMMFAudioOutput::RecordError(TInt /*aError*/)
	{
	//we should never get a recording error in MMFAudioOutput!
	__ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicRecordErrorNotSupported));
	}

/**
InitializeComplete MMFDevSoundObserver called when devsound initialisation completed.
*/
void CMMFAudioOutput::InitializeComplete(TInt aError)
	{
	
	if (aError == KErrNone)
		{
		iState = EDevSoundReady;
		}
	
	if(iInitializeState == KRequestPending)
		{
		iInitializeState = aError;
		iActiveSchedulerWait->AsyncStop();
		}
	}

/**
BufferToBeEmptied MMFDevSoundObserver - should never get called.
*/
void CMMFAudioOutput::BufferToBeEmptied(CMMFBuffer* /*aBuffer*/)
	{
	__ASSERT_DEBUG(EFalse, Panic(EMMFAudioOutputPanicRecordErrorNotSupported));
	}

/**
BufferToBeFilled MMFDevSoundObserver.
Called when buffer used up.
*/
void CMMFAudioOutput::BufferToBeFilled(CMMFBuffer* aBuffer)
	{
	TInt err = KErrNone;

	TRAP(err, iSupplier->BufferEmptiedL(aBuffer));

	//This error needs handling properly
	__ASSERT_DEBUG(!err, Panic(err));
	}

/**
PlayError MMFDevSoundObserver.

Called when stopped due to error or EOF.
*/
void CMMFAudioOutput::PlayError(TInt aError)
	{
	iMMFDevsoundError = aError;

	//send EOF to client
	TMMFEvent event(KMMFEventCategoryPlaybackComplete, aError);
	SendEventToClient(event);

	//stop stack overflow / looping problem - AD
	if (aError == KErrCancel)
		return;

	// NB KErrInUse, KErrDied OR KErrAccessDenied may be returned by the policy server
	// to indicate that the sound device is in use by another higher priority client.
	if (aError == KErrInUse || aError == KErrDied || aError == KErrAccessDenied)
		return;

	if (iState == EIdle)
		{//probably have stopped audio output and have got an underflow from last buffer
		iMMFDevSound->Stop();
		iFirstBufferSent = EFalse;
		}
	}


/**
ConvertError MMFDevSoundObserver.

Should never get called.
*/
void CMMFAudioOutput::ConvertError(TInt /*aError*/)
	{
	}


/**
Returns the number of bytes played.

@return	The number of bytes played. If 16-bit divide this number returned by 2 to get word length.
*/
TInt CMMFAudioOutput::BytesPlayed()
	{
	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);
	return iMMFDevSound->SamplesPlayed();
	}

/**
Returns the sound device.

Accessor function exposing public CMMFDevsound methods.

@return	A reference to a CMMFDevSound objector.
*/
CMMFDevSound& CMMFAudioOutput::SoundDevice()
	{
	if (!iMMFDevSound)
		Panic(EMMFAudioOutputDevSoundNotLoaded);
	return *iMMFDevSound;
	}

void CMMFAudioOutput::SendEventToClient(const TMMFEvent& aEvent)
	{
	iEventHandler->SendEventToClient(aEvent);
	}
// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs



const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KMccUidAudioOutputInterface,	CMMFAudioOutput::NewSinkL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

