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
* Description:    The class provides the FC session information
*
*/




#ifndef CFCSESSION_H
#define CFCSESSION_H

//  INCLUDES
#include <e32base.h>
#include <sdpcodecstringpool.h>


// FORWARD DECLARATIONS
class MFCPlugIn;
class CFCInterface;
class CSdpDocument;
class CSdpMediaField;
class CSdpConnectionField;
class SdpCodecStringPool;
class CSdpFmtAttributeField;

// CLASS DECLARATION

/**
 * Class contains FCSession information of particular FC sesseion service
 * If the preconditions are not met function leaves with KErrArgument system
 * error code.
 * @lib CFCPlugInEngine.lib
 */
class CFCSession: public CBase
	{
	public:
		/** Parameter for Session comparision*/
		enum TComparison
			{
			/** Port Number in SDP */
			EPort = 1,
			/** PlugIn Name in SDP */
			EName = 2
			};

		/**
		* Constructs an object
		* @param aPlugIn the plug-in
		* @param aIapId the IAP ID
		* @param aPort the port
		* @returns new instance
		*/
		static CFCSession* NewL( MFCPlugIn* aPlugIn, 
		                         TUint32 aIapId, 
		                         TUint32 aPort );

		/**
		* Constructs an object and adds the pointer to the cleanup stack;
		* leaves on failure
		* @param aPlugIn the plug-in
		* @param aIapId the IAP ID
		* @param aPort the port
		* @return new instance
		*/
		static CFCSession* NewLC( MFCPlugIn* aPlugIn, 
		                          TUint32 aIapId, 
		                          TUint32 aPort );

		/**
		* Destructor.
		*/
		~CFCSession();
		
		/**
		* Sets media port defined for the FCSession.
		* @param aPort The port
		*/
		void SetPort( const TUint aPort );

		/**
		* Gets media port defined for the FCSession
		* @return media port
		*/
		TUint Port() const;

		/**
		* Gets the negotiated floor control plugIn
		* @return negotiated floor control plugIn
		*/
		const TDesC8& NegotiatedFCPlugIn() const;

		/**
		* Gets the set of media format level attributes.
		* @return RPointerArray<CSdpFmtAttributeField>&: The set of media 
        *         format level attributes.
		*/
		RPointerArray<CSdpFmtAttributeField>& FormatAttributeFields();
		
		/**
		* Sets media format level attributes.
		* @param aAttributes, The set of media format level attributes.
		*        The ownership is transferred.
		*/
		void SetFormatAttributeFieldsL(RPointerArray<CSdpFmtAttributeField>* aAttributes);
		
   		/**
		* Reads & Store remote parameters of FC from offer/answer document.
		* @param aSdpDocument, SDP document containing SDP information
		*/
		void StoreInfoL(const CSdpDocument& aSdpDocument);
	
		/**
		* Adds or modifies the media line for FCSession to SDP document
	    * @param aSdpDocument SDP document to be add/modify the media information
		*/
		void UpdateSDPL( CSdpDocument& aSdpDocument );

		/**
		*Session is released 
		*/
		void ReleaseSession();
		
		/**
		* Session is ready to send
		* tb_priority might be =0;
		* Socket is open, and start to listen
		*/
		void ConnectionReadyL();
		
		/**
		* Send the Data to the predefined addresses
		* tb_priority must > 0
		* @param aData, sending via this plugIn
		*/
		void SendToNetL(HBufC8* aData);
		
		/**
		* Return a pointer to the FC plug-in, ownership is not transfered.
		* @return MFCPlugin& The FCPlugin
		*/
		MFCPlugIn* PlugIn() const;
		
		/**
		* Returns truth value whether or not this the plugin is 
		* to be receive data in this sessions port.
		*/
		TBool IsReceiver();
		
		/**
		* Updates the port and receiver flag. KFCDefaultPort means, that
		* the session will not be receiving.
		*/
		void Update( TUint32 aPort );
		
		/**
        * Are the floor control parameters changed
        */
		TBool UpdateNeeded();
		
	private:
		/**
		* Default constructor
		*/
		CFCSession( TUint32 aPort );

        /**
        * Symbian 2nd phase constructor.
        */
		void ConstructL( MFCPlugIn* aPlugIn, TUint32 aIapId );
		
		/**
		* Validate media format level attributes.
		* @return 0 if iSdpFmtAttributeFields length is 0 or notset
		*	      1 if iSdpFmtAttributeFields is set and length is not 0
		*/
		TBool IsFmtAttributeValid();
		
		/**
		* Finds the index off FC media line in given array.
		* @param aMediaFields, the array of media fields.
		* @return index in the array, KErrNotFound if not found.
		*/
		TInt FindMediaField( 
		    const RPointerArray< CSdpMediaField >& aMediaFields ) const;
		    
		/**
		* Get the port number from SDP media fields based on the
		* media protocol specified by plugin. Returns 0, if there's no
		* media line for the protocol.
		* @param aMediaFields, media fields
		* @return a specific SDP media port based on the media protocol.
		*/
		TUint DecodeSDPMediaPort( 
		    const RPointerArray< CSdpMediaField >& aMediaFields ) const;
		
		/**
		* Get the connectinon address from SDP doc
		* @param aSdpDocument, SDP document containing SDP media information
		* @return, the connection address
		*/
		HBufC* DecodeConnectionAddrL( const CSdpConnectionField* aConnectionField ) const;
		
		/**
		* Clean-up method, that can be given to the cleanup stack.
		*/
		static void ArrayCleanup(TAny* aArray);
		
		
		static void ArrayCleanupAndDelete(TAny* aArray);
	
		/**
		* Sets media format level attributes.
		* @param aAttributes, The set of media format level attributes.
		*        The ownership is transferred.
		*/
		void DoSetFormatAttributeFieldsL(RPointerArray<CSdpFmtAttributeField>* aAttributes);
		
	private:
		
		TUint iPort;
		RPointerArray<CSdpFmtAttributeField>* iSdpFmtAttributeFields;
		// The plug-in, owned
		CFCInterface* iPlugIn;
		TBool iIsReceiver;
		TBool iUpdateNeeded;
		
	private: // For testing purposes
		friend class UT_CFCSession;
	};

#endif // CFCSession_H

//  End of File
