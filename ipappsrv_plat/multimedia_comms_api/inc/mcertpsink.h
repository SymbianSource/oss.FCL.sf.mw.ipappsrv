/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CMCERTPSINK_H
#define CMCERTPSINK_H

// INCLUDES
#include <e32std.h>  
#include <mcemediasink.h>

// CONSTANTS
const TMceSinkType KMceRTPSink = 1;

// CLASS DECLARATION

/**
* Class representing outgoing RTP stream in MCE.
*
* CMceRtpSink can be used to send media data to network.
* It can be configured not to use RTCP during play in order
* to save bandwith. 
*
* CMceRtpSink allows also explicitly sending of RTP RR and SR
* messages.  
*
* Enable and Disable operations to it are considered
* to be local, so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceRtpSink: public CMceMediaSink
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceRtpSink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceRtpSink* NewLC();

	    /**
	    * Two-phased constructor.
	    * @param aSuppressRTCP ETrue if RTCP not sent during play,
		*		 EFalse otherwise
	    * @param aIdentity Identity of user placed in RTCP, ownership not transferred.
	    */
		IMPORT_C static CMceRtpSink* NewL( TBool aSuppressRTCP, HBufC8* aIdentity = 0 );

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceRtpSink();

	public: // From base class

	    /**
	    * Locally resumes sending of RTP packets.
		*/
		IMPORT_C void EnableL(); 

	    /**
	    * Locally pauses sending of RTP packets.
		*/
		IMPORT_C void DisableL(); 

	public: // Functions

	    /**
	    * Sends RTCP sender report to recipient.
	    */
		IMPORT_C void SendSRL();

	    /**
	    * Returns the sequence id of the last packet
		* sent to network. The value is updated only
		* when sink is disabled.
		* @pre IsEnabled() == EFalse
	    * @return sequence number of last sent packet.
	    */
		IMPORT_C TUint32 LastPacket() const;
		
		/**
	    * Returns the RTP stream source identifier 
	    * used in network (SSRC). 
	    * @pre CMceSession::State() == CMceSession::EEstablished 
	    * @return source identifier used in network (SSRC).
	    */
		IMPORT_C TUint32 Ssrc() const;

	    /**
	    * Updates settings of rtp sink.
	    * @pre CMceSession::State() == CMceSession::EIncoming 
	    * @param aSuppressRTCP ETrue if RTCP not sent during play,
		*		 EFalse otherwise
	    * @param aIdentity Identity of user placed in RTCP, ownership not transferred.
	    */
		IMPORT_C void UpdateL( TBool aSuppressRTCP, HBufC8* aIdentity = 0 );
		
	public: //from CMceMediaSink

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt EventReceivedL( TMceEvent& aEvent );
		
	private:

	    /**
	    * C++ default constructor.
	    */
		CMceRtpSink();
	  				
	    /**
	    * second-phase constructor
	    * @param aSuppressRTCP ETrue if RTCP not sent during play,
	    *		 EFalse otherwise
	    * @param aIdentity Identity of user placed in RTCP, ownership not transferred.
	    */
		void ConstructL( TBool aSuppressRTCP, HBufC8* aIdentity );

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt HandleEvent( TMceEvent& aEvent );

		void DoITCSendL( TUint8 aITCFunction );

	private: // Data

		//for testing

	    MCE_UNIT_TEST_DEFS	

	};

#endif
