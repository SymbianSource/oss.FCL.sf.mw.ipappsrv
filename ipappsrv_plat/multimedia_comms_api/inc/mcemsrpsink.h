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



#ifndef CMCEMSRPSINK_H
#define CMCEMSRPSINK_H

// INCLUDES  
#include <mcemediasink.h>
#include <e32std.h>
#include <bamdesca.h>
#include<uri8.h>
#include<e32des8.h>
#include <mcemessagestream.h>
#include <badesca.h>

// CONSTANTS
const TMceSinkType KMceMSRPSink = 5;

// CLASS DECLARATION

/**
* Class representing a MSRP sink (RFC 4975).
* 
*  @lib mceclient.lib
*/
class CMceMsrpSink: public CMceMediaSink
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.        
        */
        IMPORT_C static  CMceMsrpSink* NewL();
	
		/**
        * Two-phased constructor.
        */
		IMPORT_C static CMceMsrpSink* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C ~CMceMsrpSink();

    public: // From CMceMediaSink

        /**
        * Sets the sink enabled. 
        */
        IMPORT_C void EnableL(); 

        /**
        * Sets the sink disabled.
        */
        IMPORT_C void DisableL();

    public: // New functions

	/**
        * Gets the value for "path" attribute.
        */
        IMPORT_C RPointerArray<TUriC8>& Path();

	/**
        * Sets the value for "path" attribute.
        */
        IMPORT_C void SetPathL( const RPointerArray<TUriC8>& aPath );

	/**
        * Gets the values for "accept-types" attribute.
        */
        IMPORT_C void AcceptTypesL(CDesC8ArrayFlat& aAcceptTypes);

	/**
        * Sets the values for "accept-types" attribute.
        */
        IMPORT_C void SetAcceptTypesL( const MDesC8Array& aAcceptTypes );
        
        /**
         * Gets the values for "accept-wrapped-types" attribute
         */
        
        IMPORT_C TDes8& AcceptWrappedTypesL();
        
        /**
         * Sets the values for "accept-wrapped-types" attribute.
         * @param aAcceptWrappedTypes list of MIME types that are allowed inside compound types 
         */
        
        IMPORT_C void SetAcceptWrappedTypesL ( const TDes8& aAcceptWrappedTypes );
        
        /**
         * Sets the values for file-selector attributes ('name', 'type', 'size').
         * @param aName name of the file to be transferred
         * @param aPath path of the file to be transferred
         */            
        IMPORT_C void SetFileSelectorL(const TDesC& aName = KNullDesC,
                                       const TDesC8& aFileType = KNullDesC8,
                                       TUint aSize = 0 );
        
        /**
         * Sets the value for file-disposition attribute.
         * @param aDispositionValue indicate a preferred disposition of the file to be transferred
         */            
        IMPORT_C void SetFileDispositionL (const TDes8& aDispositionValue);
        
        /**
         * Sets the value for file-transfer-id attribute.
         * @returns the value of the file-transfer-id
         */
        IMPORT_C void SetFileTransferIdL (TDes8& aId);
        
        /**
         * Sets the information attribue.
         * @param aInfo information to be set 
         */
        IMPORT_C void SetInformationAttrL(const TDes8& aInfo);
        
        /**
         * Set the path to save the received file in file sharing use case
         * It is mandatory for the clients to set this path to let
         * the MCE know the path where the received file will be shared
         * 
         * @Param aFilePath Holds the path to save the received file
         */
        IMPORT_C void SetPathToSaveReceivedFile(const TDesC16& aFilePath);
        
        /*
         * Used to get the file transfer progress notifications like transferred x bytes 
         * out of total y file size. This is true by default. 
         * @param enableFlag Default value is ETrue. 
         */
        IMPORT_C void EnableFileTransferProgressNotifications(TBool enableFlag = ETrue);
        
    public: // for internal purpose
        /**
         * 
         */
        void InitializeL( CMceMediaStream& aParent );
        
        
    private: 
		/**
	     * C++ default constructor.
	     */
        CMceMsrpSink();
		/**
	     * C++ parameterized constructor.
	     */        
	
		/**
	     * Second phase constructior.
	     */
		 void ConstructL();
		 
         /**
          * Sets the local media port form the MSRP path attribute
          * @par aMsrpUri The Uri from which the port has to be extracted
          * @aStream  Sets the local media port with the port extracted form the aMsrpUri 
          */
         void SetPortFromPathAttrL(TUriC8& aMsrpUri, CMceMessageStream& aStream);
         
         //for testing

         MCE_UNIT_TEST_DEFS  
    };

#endif // CMCEMSRPSINK_H
