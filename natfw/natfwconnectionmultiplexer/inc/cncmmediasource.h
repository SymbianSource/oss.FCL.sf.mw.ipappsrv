/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Defines source for multiplexer, where Media Wrapper is mounted to
*
*/




#ifndef C_CNCMMEDIASOURCE_H
#define C_CNCMMEDIASOURCE_H

#include <e32base.h> 

#include "natfwmediawrapperobserver.h"
#include "natfwconnectivityapidefs.h"
#include "mncmsenderobserver.h"

class MNcmSourceObserver;
class MNcmMediaSourceObserver;
class MNATFWMediaWrapper;
class RSocketServ;
class TInetAddr;

/**
 *  Defines source for multiplexer, where Media Wrapper is mounted to
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmMediaSource ) : public CBase, 
    public MNATFWMediaWrapperObserver, public MNcmSenderObserver
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aMediaObserver      Media source observer
     * @param   aMediaWrapper       Media wrapper
     * @param   aSocketServ         Socket server
     * @param   aLocalAddr          Local address
     */
    static CNcmMediaSource* NewL(
        MNcmMediaSourceObserver& aMediaObserver,
        MNATFWMediaWrapper& aMediaWrapper,
        const RSocketServ& aSocketServ,
        const TInetAddr& aLocalAddr );

    /**
     * Two-phased constructor
     *
     * @param   aMediaObserver      Media source observer
     * @param   aMediaWrapper       Media wrapper
     * @param   aSocketServ         Socket server
     * @param   aLocalAddr          Local address
     */
    static CNcmMediaSource* NewLC(
        MNcmMediaSourceObserver& aMediaObserver,
        MNATFWMediaWrapper& aMediaWrapper,
        const RSocketServ& aSocketServ,
        const TInetAddr& aLocalAddr );

    /**
     * Destructor
     */
    virtual ~CNcmMediaSource();

    /**
     * Registers message observer. Enables media sending to media socket wrapper
     *
     * @since   S60 v3.2
     * @param   aMessageObserver    The message observer        
     */    
    void RegisterMessageObserver( MNcmSourceObserver& aMessageObserver );
    
    /**
     * Unregisters message observer. Disables media sending to media socket wrapper
     *
     * @since   S60 v3.2
     * @param   aMessageObserver        The message observer
     */    
    void UnregisterMessageObserver(
        const MNcmSourceObserver& aMessageObserver );

    /**
     * Enables/disables media receiving from mux to wrapper.
     *
     * @since   S60 v3.2
     * @param   aState        The receiving state
     */ 
    void SetReceivingState( TNATFWStreamingState aState );
 
    /**
     *
     * @since   S60 v3.2
     * @param   aMessage        Message is a reference to message which
     *                          is copied by caller
     */
    void PutMessageL( const TDesC8& aMessage );
    
// from base class MNATFWMediaWrapperObserver
    
    /**
     * Indicates how sending to user succeed
     *
     * @since   S60 v3.2
     * @param   aError system-wide error code
     */
    void SendingCompleted( TInt aError );
    
    /**
     * From MNATFWMediaWrapperObserver.
     *
     * Called after data were sent to wrapper. 
     *
     * @since   S60 v3.2
     * @param   aStreamId           An ID for stream the data
     *                              was received from
     * @param   aStreamPortion      Stream portion which were sent by client
     * @return  system-wide error code
     */
    TInt ReceivedFrom( TUint aStreamId, TDes8& aStreamPortion );
    
    /**
     * From MNATFWMediaWrapperObserver.
     *
     * Called by socket media wrapper object when local port is allready in use.
     * Get new free port.
     *
     * @since   S60 v3.2
     * @param   aPort       New free port
     */ 
    void GetNewFreePort( TUint& aPort );

// from base class MNcmSenderObserver

    /**
     * From MNcmSenderObserver.
     *
     * Called when sending the message is succesfully completed.
     *
     * @since   S60 v3.2
     * @return  void
     */ 
    void MessageSent();

    /**
     * From MNcmSenderObserver.
     *
     * Called when sending the message is failed.
     *
     * @since   S60 v3.2
     * @param   aError      A reason for the failure.
     * @return  void
     */
    void MessageSentFailure( TInt aError );
    
private:

    CNcmMediaSource(
        MNcmMediaSourceObserver& aMediaObserver,
        MNATFWMediaWrapper& aMediaWrapper );
        
    void ConstructL( const RSocketServ& aSocketServ,
                     const TInetAddr& aLocalAddr );    

private: // data


    /**
     * Observer for this source.
     * Not own.
     */
    MNcmSourceObserver* iMessageObserver;
    
    /**
     * Observer for this media source
     */  
    MNcmMediaSourceObserver& iMediaObserver;
    
    /**
     * Media wrapper associated with this source
     */
    MNATFWMediaWrapper& iMediaWrapper;  

    /**
     * Receiving status
     */
    TNATFWStreamingState iReceivingState;
    
    /**
     * Indicates that the first media packet is sent to the remote end
     */
    TBool iFirstMediaPacketIsSent;   
    };


#endif // C_CNCMMEDIASOURCE_H
