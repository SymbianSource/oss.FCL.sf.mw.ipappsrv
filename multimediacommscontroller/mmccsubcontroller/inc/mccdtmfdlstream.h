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
* Description:    Provides DTMF support
*
*/




#ifndef MCCDTMFDLSTREAM_H
#define MCCDTMFDLSTREAM_H

#include "mccsymstreambase.h"

class MAsyncEventHandler;
class MMccResources;
class CMccRtpManager;
class CMMFFormatEncode;
class CMMFFormatDecode;
class TMccCodecInfo;

/**
 *  This class provides DTMF receiving support.
 *
 *  @lib mmccsubcontroller.dll
 *  @since S60 v4.0
 */
NONSHARABLE_CLASS( CMccDtmfDlStream ) : public CMccSymStreamBase
    {

public:

    static CMccDtmfDlStream* NewL( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventhandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock );

    static CMccDtmfDlStream* NewLC( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventhandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock );

    virtual ~CMccDtmfDlStream();

// from base class CMccSymStreamBase

    /**
     * From CMccSymStreamBase.
     * Sets the priority settings for this stream.
     *
     * @since   S60 3.0
     * @param   aPriority       MMF Priority settings
     * @return  void
     */
    void SetPrioritySettingsL( const TMMFPrioritySettings& aPriority );
    
    /**
     * From CMccSymStreamBase.
     * Adds the sink and source to the stream.
     *
     * @since   S60 3.0
     * @param   aDatasink       MDataSink
     * @param   aDatasource     MDataSource
     * @return  void
     */
    void AddSinkAndSourceL( MDataSink* aDatasink, MDataSource* aDatasource );

    /**
     * From CMccSymStreamBase.
     * Loads a codec in the stream.
     *
     * @since   S60 3.0
     * @param   aCodecInfo      Contains mediaformat settings
     * @return  void
     */
    void LoadCodecL( const TMccCodecInfo& aCodecInfo, const TDesC8& aFmtp );
    
    /**
     * From CMccSymStreamBase.
     * Primes the audio stream ready.
     *
     * @since   S60 3.0
     * @param   aEndpointId     Endpoint to be primed
     * @return  void
     */
    void PrimeL( const TUint32 aEndpointId );
    
    /**
     * From CMccSymStreamBase.     
     * Starts the audio streaming.
     *
     * @since   S60 3.0
     * @param   aEndpointId     Endpoint to be played
     * @param   aStreamPaused   Whether to pause stream after play
     * @param   aEnableRtcp     Enable/disable RTCP
     * @return  void
     */
    void PlayL( const TUint32 aEndpointId, TBool aStreamPaused, 
        TBool aEnableRtcp );
    
    /**
     * From CMccSymStreamBase.    
     * Pauses the audio streaming.
     *
     * @since   S60 3.0
     * @param   aEndpointId     Endpoint to be paused
     * @param   aEnableRtcp     Enable/disable RTCP
     * @return  void
     */
    void PauseL( const TUint32 aEndpointId, TBool aEnableRtcp );
    
    /**
     * From CMccSymStreamBase.    
     * Resumes the audio streaming.
     *
     * @since   S60 3.0
     * @param   aEndpointId     Endpoint to be resumed
     * @param   aEnableRtcp     Enable/disable RTCP
     * @return  void
     */
    void ResumeL( const TUint32 aEndpointId, TBool aEnableRtcp );
    
    /**
     * From CMccSymStreamBase.
     * Stops the audio streaming.
     *
     * @since   S60 3.0
     * @param   aEndpointId     Endpoint to be stopped
     * @return  void
     */
    void StopL( const TUint32 aEndpointId );
    
    /**
     * From CMccSymStreamBase.
     * Resets Jitterbuffer.
     *
     * @since   S60 3.0
     * @return  void
     */
    void ResetCountersL();
    
    /**
     * From CMccSymStreamBase.
     * Sets the current record/play balance depending on the stream type.
     *
     * @since   S60 3.0
     * @param   aLeftbal    Left balance
     * @param   aRightbal   Right balance
     * @return  void
     */
    void SetBalanceL( TInt aLeftbal, TInt aRightbal );
    
    /**
     * From CMccSymStreamBase.
     * Returns the current record/play balance depending on the stream type.
     *
     * @since   S60 3.0
     * @param   aLeftbal    Left balance
     * @param   aRightbal   Right balance
     * @return  void
     */
    void GetBalanceL( TInt& aLeftbal, TInt& aRightbal );
    
    /**
     * From CMccSymStreamBase.
     * Starts inactivity timer for a stream in a given session.
     *
     * @since   S60 3.0
     * @param   aTimeoutTime     Time after which inactivity is reported
     * @return  void
     */
    void StartInactivityTimerL( TUint32 aTimeoutTime );

    /**
     * From CMccSymStreamBase.
     * Stops inactivity timer for a stream in a given session.
     *
     * @since   S60 3.0
     * @return  void
     */
    void StopInactivityTimerL();
    
    /**
     * From CMccSymStreamBase.
     * Gets synchronization source this stream is using.
     *
     * @since   S60 3.0
     * @param   aSSRCValue  Synchronization source
     * @return  void
     */
    void GetSSRCL( TUint32& aSSRCValue );
    
    /**
     * From CMccSymStreamBase.
     */
    TBool IsUplink();
    
    /**
     * From CMccSymStreamBase.
     */
    void StandByL();
            
protected:

    /**
     * From CMccSymStreamBase.
     * Gets sink of the multiplexer.
     */
    MDataSink* MultiplexerSink();
    
private:

    CMccDtmfDlStream( TUint32 aMccStreamId, 
        MAsyncEventHandler* aEventhandler, MMccResources* aMccResources,
        CMccRtpManager* aManager, TFourCC aFourCC, TInt aStreamType,
        CMccRtpMediaClock& aClock );

    void ConstructL();
    
    /**
     * Creates the payload format decoder.
     * @since   S60 3.0
     * @return  void
     */
    void CreatePayloadFormatDecoderL();
    
    /**
     * Updates codec information.
     * @since   S60 3.0
     * @param   aCodecInfo      Update parameters
     * @return  void
     */
    void UpdateCodecInformationL( TMccCodecInfo& aCodecInfo );

private: // data

    /**
     * FourCC code identifying codec.
     */
    TFourCC	iFourCC;
    
    
    /**
     * Payload format encoder used to encode DTMF RTP payload.
     * Own.
     */
    CMMFFormatEncode* iFormatEncode;
    
    /**
     * Payload format decoder used to decode DTMF RTP payload.
     * Own.
     */
    CMMFFormatDecode* iFormatDecode;
    
    };


#endif // MCCDTMFDLSTREAM_H
