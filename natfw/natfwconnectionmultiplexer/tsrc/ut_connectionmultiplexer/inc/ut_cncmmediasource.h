/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CNCMMEDIASOURCE_H__
#define __UT_CNCMMEDIASOURCE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>


//  INTERNAL INCLUDES
#include "cncmmediasource.h"
#include "mncmmediasourceobserver.h"
#include "natfwmediawrapper.h"
#include "mncmsourceobserver.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( UT_CNcmMediaSource )
	: public CEUnitTestSuiteClass, public MNcmMediaSourceObserver,
	public MNATFWMediaWrapper, public MNcmSourceObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNcmMediaSource* NewL();
        static UT_CNcmMediaSource* NewLC();
        /**
         * Destructor
         */
        ~UT_CNcmMediaSource();


    /**
     * From MNcmSourceObserver
     */ 
    void OutgoingMessageAvailable( const TDesC8& aMessage,
        MNcmSenderObserver* aSenderObserver, const TInetAddr& aDestAddr );

    /**
     * From MNcmMediaSourceObserver
     * 
     * Called by Media Source after wrapper has had sending error
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aError              The error code
     * @return  void
     */ 
    void WrapperSenderError( TUint aStreamId, TInt aError );

    /**
     * From MNcmMediaSourceObserver
     * 
     * Called by Media Source if current local port is already in use.
     * Gets new free port.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aPort               New free port
     * @return  void
     */     
    void GetFreePort( TUint aStreamId, TUint& aPort );
    
    /**
     * From MNcmMediaSourceObserver
     * 
     * Notifies that the first media packet is sent to
     * the remote end.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @return  void
     */     
    void FirstMediaPacketSent( TUint aStreamId );
    
    
    
    /**
     * From MNATFWMediaWrapper
     * 
     * Enables wrapper to forward packets from mediaengine to connection
     * multiplexer.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */     
    void SetReceivingStateForMuxWrapper(
        TNATFWStreamingState aState );             
        
    /**
     * From MNATFWMediaWrapper
     * 
     * Send data to connected media engine.
     *
     * @since S60 v3.2
     * @param aStreamPortion a reference to buffer to be sent
     * @return void
     */
    void SendL( const TDesC8& aStreamPortion ); 
    
    /**
     * From MNATFWMediaWrapper
     * 
     * Returns The stream ID which wrapper is associated with
     *
     * @since S60 v3.2
     * @return ID which wrapper is associated with
     */
    TUint StreamId();
    
    /**
     * From MNATFWMediaWrapper
     * 
     * Register media observer.
     *
     * @since S60 v3.2
     * @param aObserver  media observer 
     * @param aServer    socket server handle
     * @param aLocalAddr wrapper's local address
     * @return void   
     */
    void RegisterMediaWrapperObserverL(
        MNATFWMediaWrapperObserver* aObserver,
        const RSocketServ& aServer,
        const TInetAddr& aLocalAddr );

    /**
     * From MNATFWMediaWrapper
     * 
     * Closes wrapper instance by deleting it.
     *
     * Implementing class MUST call its destructor on this method and free
     * allocated heap. Usually this is done by "delete this".
     *
     * @since S60 v3.2
     * @return void
     */    
    void Close();

    /**
     * From MNATFWMediaWrapper
     * 
     * Gets wrapper's local and remote addresses
     *
     * @since S60 v3.2
     * @param aLocalAddr    wrapper's local address 
     * @param aRemoteAddr   wrapper's remote addresse
     * @return void   
     */
    void GetAddresses(
        TInetAddr& aLocalAddr,
        TInetAddr& aRemoteAddr );
    
    
    
    
    private:    // Constructors and destructors

        UT_CNcmMediaSource();
        void ConstructL();

    private:    // New methods

         void SetupL();

         void Teardown();
        
         void T_CNcmMediaSource__CNcmMediaSourceL();
        
         void T_CNcmMediaSource_RegisterMessageObserverL();
        
         void T_CNcmMediaSource_UnregisterMessageObserverL();
        
         void T_CNcmMediaSource_SetReceivingStateL();
        
         void T_CNcmMediaSource_PutMessageLL();
        
         void T_CNcmMediaSource_SendingCompletedL();
        
         void T_CNcmMediaSource_ReceivedFromL();
        
         void T_CNcmMediaSource_GetNewFreePortL();
        
         void T_CNcmMediaSource_MessageSentL();
        
         void T_CNcmMediaSource_MessageSentFailureL();
        

    private:    // Data
		
        CNcmMediaSource* iCNcmMediaSource;
        EUNIT_DECLARE_TEST_TABLE;

        // Dummies
        RSocketServ iSocketServ;
        TInetAddr iAddress;

    };

#endif      //  __UT_CNCMMEDIASOURCE_H__

// End of file
