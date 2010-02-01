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
* Description:    Plugin interface to be implemented in order to get transcoding
*                functionality
*
*/




#ifndef MCCTRANSCODER_H
#define MCCTRANSCODER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmccevents.h>

// CLASS DECLARATION
class CMccTranscoderImpl;

// CONSTS

// Plugin Interface UID
#define KMmfUidPluginInterfaceMccTranscoder  0x10282898

/**
* Class for setting up codec values for transcoding.
*/  
class TMccTranscoderCodecInfo
	{
    public:
		TUint32 iBitrate;
		TReal iFramerate;
		TUint32 iVideoWidth; 
		TUint32 iVideoHeight;
		TUint32 iSamplingFreq; 
		TBuf8<256> iMimeType;
		TUid iFourCC; 
		TUint32 iReserved1; 
		TUint32 iReserved2; 
	};

/**
* Class for setting up transcoding.
*/  
class TMccTranscodeFileMsg
    {
    public:
    
        TFileName  iSourceFile;
        
        TFileName  iDesFile;
        
        TUint32    iQuality;
        
        TMccTranscoderCodecInfo iVideoCodec;
        
        TMccTranscoderCodecInfo iAudioCodec;
    };
    
typedef TPckgBuf<TMccTranscodeFileMsg> TMccTranscodeFileMsgBuffer; 


/**
*  Observer interface for clients to receive events from transcoder.
*/
class MMccTranscoderObserver
    {
	public:
    
    /**
    * Callback function to receive transcoder events
    * @param aEvent Received transcoder events
    */
    virtual void MccTranscoderEventReceived( TMccEvent& aEvent ) = 0;
    };
    
/**
*  MCC subsystem transcoder interface. Class is used for transcoding audio and video
*  files from one codec to another. Transcoding can be also done with old codec in order
*  to change bitrate.
*/
class CMccTranscoder : public CBase
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CMccTranscoder* NewL();

        /**
        * Destructor.
        */
        virtual ~CMccTranscoder();

    public:  // New functions

       /**
        * Registers observer for transcoding events.
        * @param aObserver observer for transcoding events.
        */
        virtual void RegisterEventObserver( MMccTranscoderObserver& aObserver );
        
       /**
        * Unregisters observer for transcoding events.
        */
        virtual void UnregisterEventObserver();
       
       /**
        * Starts transcoding of media file.
        * @param aSessionId session identifier for transcoding, updated on return
        * @param aVal TMccTranscodeFileMsgBuffer containing transcode parameters
        */
        virtual void TranscodeFileL( TUint32& aSessionId, const TDesC8& aVal );
        
       /**
        * Cancels transcoding of media file.
        * @param aSessionId session identifier for transcoding
        */
        virtual void CancelTranscodeFileL( TUint32 aSessionId );
    
    private:
        /**
        * C++ default constructor.
        */
        CMccTranscoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL();

    private:    // data
        CMccTranscoderImpl* iTranscoderImpl;
        
    };

#endif 
