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
* Description:    Provides services for SDP Security Descriptions.
*
*/





#ifndef CMCESECUREMEDIASESSION_H
#define CMCESECUREMEDIASESSION_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <e32std.h> 
#include <in_sock.h>
#include <stringpool.h>

#include "mcemediadefs.h"
#include "mcesrv.h"
#include "mcecomsession.h"

// FORWARD DECLARATIONS
class CSdpDocument;
class CMceComSession;
class CMceComAudioStream;
class CSdpMediaField;
class CMceMediaManager;
class CMceSdpCodec;
class CMceSecureDesStream;
class CMccSecureInterface;
class CMceSrvStream;
class CMceComCodec;
class CMceComMediaStream;

    
/*enum TSecStreamCryptoType
		{
		E_CryptoIn =1,
		E_CryptoOut 
		};	    
*/
const TUint KCyptoCountSendRecv = 2;
const TUint KCyptoCountSendOrRecv = 1;
/**
*  Class for storing session specific SDP parameters
*/
class CMceSecureMediaSession : public CBase
    {

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMceSecureMediaSession* NewL( CMceComSession& aSession,
                                     CMceMediaManager& aManager,
                                     CMccSecureInterface& aSecureInterface );

        /**
        * Two-phased constructor.
        */
        static CMceSecureMediaSession* NewLC( CMceComSession& aSession,
                                      CMceMediaManager& aManager,
                                      CMccSecureInterface& aSecureInterface  );
        
        /**
        * Destructor.
        */
        ~CMceSecureMediaSession();
        
    public: 

        /**
        * Createas a SDPDocument based on the streams in iSession. For each stream
        * a media line is created in the sdp document. Fails if one of the streams 
        * is not supported / failed
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return KErrNone if successful, otherwise failed
        */
        void EncodeSecureDesSdpOfferL(	CMceComMediaStream& aStream,
        								CSdpMediaField& aMediaLine);
        
        /**
        * Goes through the secure line from answer and store the secure keys information 
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return void 
        */
        void  DecodeSecureDesSdpAnswerL(	CMceComMediaStream& aStream,
        									CSdpMediaField& aMediaLine); 
        
        /**
        * Decodes the incoming offer. For each media line in the offer, create corresponding
        * secure stream and secure keys information
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return void
        */
        void DecodeSecureDesSdpOfferL( CMceComMediaStream& aStream,
        								CSdpMediaField& aMediaLine);
        
        /**
        * Creates an answer based on the OFFER received and the application's
        * updated preferences. 
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return void   
        */
        void EncodeSecureDesSdpAnswerL( CMceComMediaStream& aStream,
        								CSdpMediaField& aMediaLine); 
        
        /**
        * Decodes an medialine based on the OFFER received and the application's
        * updated preferences. 
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return void    
        */
        void DecodeSecureDesSdpUpdateL( CMceComMediaStream& aStream,
        								CSdpMediaField& aMediaLine );
 
        
        /**
        * Cleans secure crypto information in SdpDocuemnt MediaLine 
        * @param aStream a MceComMediaStream
        * @param aMediaLine
        * @return void
        */ 
        void CleanSecureCryptoInfoL(CSdpMediaField& aMediaLine);
        
        
        /**
	     * Binds crypto context into stream. Binding must be done
	     * before stream is prepared.
	     * @since Series 60 3.2
	     * @return KErrNone if success; otherwise system wide error code.
	     */
 		 TInt BindStreamCrypto();
 		
 		 /**
	     * Set stream to be updated when MccEvent called back 
	     * @since Series 60 3.2
	     * @param aContextId context needed to be updated
	     * @return KErrNone if success; otherwise system wide error code.
	     */
 		void ContextNeedUpdated( TUint32 aContextId );
     	
     	/**
	     * Copy secure description streams into new secure media session
	     * @since Series 60 3.2
	     * @param aSession a secure media session to be copied from
	     * @return void
	     */
     	void CopyStreamsL(CMceSecureMediaSession& aSession);
     	
     	
     	TInt SdpCryptoAttributeCount( CSdpMediaField& aMediaLine);
     	
     	void RemoveSecureCrypto();
     	
     	void ForceUpdateSecureStreamL( CMceComMediaStream& aStream, 
     			                       CSdpMediaField& aMediaLine );
    private:
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * C++ default constructor
        */
        CMceSecureMediaSession( CMceComSession& aSession, 
        						CMceMediaManager& aManager,
                                CMccSecureInterface& aSecureInterface);
        
        TBool IfStreamMatchMedia(CMceComMediaStream& aMediaStream,
        								CMceSecureDesStream& aStream,
													CSdpMediaField& aMediaLine);                        

    	TBool IsSecureSdpMsgL( CSdpMediaField& aMediaLine );
    
    public:    // Data
        
        CMceMediaManager& iManager;
        
        // reference to corresponding server stream
        CMceComSession& iSession;

        CMccSecureInterface& iSecureInterface;
        
        TBool iKeyNeedUpdated;
        TBool iCryptoContextUpdate;
        TBool iLSReadyToBind;

    private:
        
        RStringPool iStringPool;
        const TStringTable* iStringTable;
        
        RPointerArray< CMceSecureDesStream > iMceSecureDesStreams;
    	//definitions for unit testing
    	MCEMM_UT_DEFINITIONS
     	friend class UT_CMceSecureMediaSession; 
    };

#endif      // CMCESECUREMEDIASESSION_H
            
// End of File
