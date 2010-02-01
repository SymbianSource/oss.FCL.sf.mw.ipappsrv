/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_MCCRTPMEDIACLOCK_H
#define C_MCCRTPMEDIACLOCK_H

#include <e32base.h>
#include <e32cmn.h>


const TUint K8Khz( 8000 );
const TInt K20ms( 20 );

/**
 * Mcc media clock registration info class
 * 
 * @lib N/A
 * @since Series 60 3.2
 */
NONSHARABLE_CLASS( TMccRegInfo )
    {
    public:
    inline TMccRegInfo() :
            iSamplingFreq( K8Khz ),
            iHwFrametime( K20ms ),
            iKey( 0 )
            { };
    
    TUint32 iKey;
    TUint iSamplingFreq;
    TInt iHwFrametime;
    };
    
/**
 * Media clock provides common RTP timestamp space.
 *  
 * Clock that forms common timeline for all media formats using 
 * same SSRC. Typical client for media clock is 
 * one of supported payload formatters.
 *
 * @lib    mmccsubcontroller.lib
 * @since  S60 v3.2
 */
NONSHARABLE_CLASS( CMccRtpMediaClock ) : public CBase
    {
public:

    static CMccRtpMediaClock* NewL();

    virtual ~CMccRtpMediaClock();

    /**
     * Method for client registration. Media clock's client must be registered
     * in order to use media clock services. If client has changed its
     * parameters after first registration, new registration is needed.
     *
     * @since   S60 3.2
     * @param   aSamplingFreq   Used sampling frequency.
     * @param   aHwFrametime    Used frame time.
     * @return  key             Unique key for client to use media clock
     */
    virtual TUint32 RegisterMediaFormat( TUint aSamplingFreq,
                                 TInt aHwFrametime );

    /**
     * Client can query next rtp time stamp. NOT for keep alive packets!
     *
     * @since   S60 3.2
     * @param   aKey            key got from registration
     * @param   aTimeStamp      time stamp is returned
     * @return  KErrNone if ok, else KErrNotFound
     */
    virtual TInt GetTimeStamp( TUint32 aKey, TUint32& aTimeStamp ); 
                      
    /**
     * Latest consumed rtp time stamp can be queried through this method.
     * Designed for keep alive packets.
     *
     * @since   S60 3.2
     * @param   aTimeStamp      latest consumed rtp time stamp is returned
     * @return  void
     */
    virtual void GetLatestConsumedTS( TUint32& aTimeStamp );
    
    /**
     * Unregisters a media format with given key. Does nothing if given
     * key is not found from registry.
     *
     * @since S60 v3.2
     * @param aKey              Registration key from RegisterMediaFormat.
     * @return void
     */
    virtual void UnregisterMediaFormat( TUint32 aKey );
    
    /**
     * Checks whether 'timebased' or 'callback' increment has been used
     */
    virtual TBool TimeBasedIncrement() const;
    
private:

    CMccRtpMediaClock();

    void ConstructL();
    
    /**
     * Calculates new rtp time stamp.
     *
     * @since   S60 3.2
     * @return  new rtp time stamp
     */
    TUint32 TimeStampIncrement(); 
     
    /**
     * Check clients key to verify registration and to set variables
     *
     * @since   S60 3.2
     * @param   aKey     client's unique key
     * @param   aCell    
     * @return  System wide error code
     */
    TInt CheckKey( TUint32 aKey, TInt& aCell );
    

private: // data

    /**
     * Used sampling frequency
     */
    TUint iSamplingFreq; 
    
    /**
     * Used frame time
     */
    TInt iHwFrametime;
    
    /**
     * current rtp time stamp
     */
    TUint32 iCurRtpTS;
    
    /**
     * Currently used registeration key 
     */ 
    TUint32 iUsedKey;
    
    /**
     * Array to maintain registration info
     */
    RArray<TMccRegInfo> iRegister;
    
    /**
     * Previous time when time stamp was updated
     */
    TTime iPreviousTimestamp;
    
    /**
     * Whether 'timebased' or 'callback' increment has been used
     */
    TBool iTimeBasedIncrement;
    
    };


#endif // C_MCCRTPMEDIACLOCK_H