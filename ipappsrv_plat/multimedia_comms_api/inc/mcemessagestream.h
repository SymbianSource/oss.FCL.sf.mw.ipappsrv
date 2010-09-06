/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CMCEMESSAGESTREAM_H
#define CMCEMESSAGESTREAM_H

//  INCLUDES          
#include <e32std.h>
#include <mcemediastream.h>

// CONSTANTS
const TMceMediaType KMceMessage = 3;

// FORWARD DECLARATIONS
class CMceMessageCodec;

// CLASS DECLARATION

/**
* Class for representing a message stream.
*
* It defines the used messgae codecs for the stream.
* In order to be complete structure, at least one codec is required.
*
*  @lib mceclient.lib
*/
class CMceMessageStream: public CMceMediaStream
    {
    
    public:
        enum TConnSetup
            {
            EActive=0,
            EActpass,
            EPassive
            };
        enum TMsrpConnectionUsage
            {
            ENew = 0,
            EExisting
            };
    
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceMessageStream* NewL();

	    /**
	    * Two-phased constructor.
	    */
        IMPORT_C static CMceMessageStream* NewLC();

	    /**
	    * Destructor.
	    */
        IMPORT_C ~CMceMessageStream();

	public: // Functions

        /**
         * The MCE extracts the host addr and the port for both local and remote ends 
         * from the MSRP path attribute if passed parameter is set to TRUE 
         * otherwise it uses SDP c and m lines. 
         * @param usePathAttr MCE sets the host addr and port depende upon the valus of this bool parameter
         *        The default value is ETrue
         */
        IMPORT_C void DoExtractHostAddrAndPortFromMsrpPathAttr(TBool usePathAttr = ETrue);



	    /**
	    * Returns the available codecs to the session.
	    * For CMEOutSession, array contains all codecs supported by
	    * terminal in default. For CMEInSession, array contains codecs
	    * that were offered by remote terminal, and supported by local terminal.
	    * Codecs are in preference order in the array.
	    * Codec in the first element of the array has highest preference.
	    * @return codecs available for the session.
	    */
		IMPORT_C const RPointerArray<CMceMessageCodec>& Codecs();
	    /**
	    * Adds codec to the stream.
	    * @param aCodec codec added to the stream; ownership is transferred
	    */
		IMPORT_C void AddCodecL( CMceMessageCodec* aCodec );
	    /**
	    * Removes codec from the stream
	    * @param aCodec codec removed from stream
	    */
		IMPORT_C void RemoveCodecL( CMceMessageCodec& aCodec );

		/**
	    * Replaces codecs of the stream. Codecs should be in preference order in
	    * the passed array.
	    * @param aCodecs array of codecs which will replace existing codecs;
	    *        ownership is transferred
	    */
		IMPORT_C void ReplaceCodecsL( RPointerArray<CMceMessageCodec>* aCodecs );

         /* Returns true if Msrp Path attribute is used for either setting or getting
         * the host address and port number for both local and remote end points
         * @return Either ETRUE or EFALSE  
         */
        IMPORT_C TBool IsMsrpPathAttrPreferredForHostAddrAndPort();

        /*
         * Used to set up the connection negostiaiton .The value of this decides who initiates the 
         *  Msrp TCP connection establishment. 
         * refer http://tools.ietf.org/html/draft-ietf-simple-msrp-acm-02 on how to set this API.
         * @param TConnSetup.  
         */
        IMPORT_C void ConnectionSetUpL(TConnSetup  aSetup);
        
        /*
         * Returns the conneciton setup value. 
         * @return TConnSetup. Retunrs the value configured using the API ConnectionSetUp
         */
        IMPORT_C TConnSetup GetConnectionSetup();


	public: // serialization
	

        /**
         * Re-establishment of the MSRP TCP based connection as per RFC 4145
         * It creates the SDP media level line (a=connection)
         * @param aReUse  The default value for this is 'EFalse'
         */
        //IMPORT_C void ReuseMsrpConnectionL(TMsrpConnectionUsage aUsage = CMceMessageStream::ENew);
        IMPORT_C void ReuseMsrpConnectionL(TBool aReUse = EFalse);



	    /**
	    * Internalizes
	    * @param aReadStream read stream
	    * @param aTarget to which read to
	    */
	    void InternalizeL( MMceComSerializationContext& aSerCtx );

	    /**
	    * Externalizes
	    * @param aWriteStream write stream
	    */
	    void ExternalizeL( MMceComSerializationContext& aSerCtx );


	public: //from CMceMediaStream

	    /**
	    * Initializes
	    * @param aParent the parent
	    */
	    void InitializeL( CMceSession& aParent );

	    /**
	    * Initializes
	    * @param aManager the manager. Ownership is NOT transferred
	    * @param aSession the Session.
	    */
        void InitializeL( CMceManager* aManager, CMceSession& aSession );

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt EventReceivedL( TMceEvent& aEvent );

	    /**
	    * Called file source wants to synchronized with stream
	    * @param aFile the file source
	    */
        void SynchronizeWithFileL( CMceFileSource& aFile );

        /**
        * Searches for codec matching with input codec instance or sdp name
        * @param aCodec the search term
        * @return matching codec or NULL if not found
        */
        CMceCodec* FindCodec( CMceCodec& aCodec );

    public: //internal

      /**
	    * Arranges codec priority order based on codecs preference values.
	    * @param aDiscardUnusedCodecs ETrue if all disabled codecs should
	    *        be removed from the stream
	    */
        void ReorderCodecsByPreferenceL( TBool aDiscardUnusedCodecs );
        
        /**
        * Removes codec
        * @param aCodec
        * @param aBinderOriginated
        */
        void RemoveCodecL( CMceMessageCodec& aCodec, TBool aBinderOriginated );

        /**
        * Return codecs of stream.
        * @return list of codecs in the stream.
        */
        const RPointerArray<CMceCodec>& BaseCodecs();
	
	private:
			    /**
	     * C++ default constructor.
	     */
		CMceMessageStream();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();

		//for testing

		MCE_UNIT_TEST_DEFS 

	private: // Owned data

	    /**
	     * codecs
	     */
		RPointerArray<CMceMessageCodec> iCodecs;
    };

#endif // CMCEMESSAGESTREAM_H
