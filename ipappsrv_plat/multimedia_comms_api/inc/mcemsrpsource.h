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



#ifndef CMCEMSRPSOURCE_H
#define CMCEMSRPSOURCE_H

// INCLUDES
#include <mcemediasource.h>
#include <e32std.h>
#include <bamdesca.h>
#include<uri8.h>
#include<e32des8.h>
#include <mcemessagestream.h>
#include <badesca.h>

// CONSTANTS
const TMceSourceType KMceMSRPSource = 5;

// CLASS DECLARATION
/**
* Class representing a MSRP source (RFC 4975).
* 
*  @lib mceclient.lib
*/
class CMceMsrpSource: public CMceMediaSource
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.        
        */
        IMPORT_C static CMceMsrpSource* NewL();
	
		/**
        * Two-phased constructor.
        */
		IMPORT_C static CMceMsrpSource* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C ~CMceMsrpSource();

    public: // From CMceMediaSource

        /**
        * Sets the source enabled.
        */
        IMPORT_C void EnableL(); 

        /**
        * Sets the source disabled.
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
                                           const TUint aSize = 0 );
            
            /**
             * Sets the value for file-disposition attribute.
             * @param aVar indicate a preferred disposition of the file to be transferred
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
             * the MCE know the path where the received file will be saved
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
         * Calls to set the local media port from the MSRP URI
         * @return Symtem wide error if the path attribute is not set for the aParent
         */
        void InitializeL( CMceMediaStream& aParent );
      
      private: 
		
		/**
	     * C++ default constructor.
	     */
        CMceMsrpSource();		
		
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


#endif // CMCEMSRPSOURCE_H
