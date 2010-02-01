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




#ifndef CFCPLUGINENGINE_H
#define CFCPLUGINENGINE_H

// INCLUDES
#include <e32base.h>
#include "fcinterface.h"
#include <ecom/ecom.h>
#include "fcmessageobserver.h"
#include "fcpluginstore.h"
#include "fcobserver.h"

// FORWARD DECLARATIONS
class CFCPlugInInfo;
class TFCInterfaceInitParams;
class TFCPlugInIter;
class CFCSession;
class CSdpFmtAttributeField;
class CSdpDocument;

// CLASS DECLARATION

/**
* Class provides functions for using Floor Control Plugin
* @lib CFCPlugInEngine.lib
*/
class CFCPlugInEngine : public CBase,
			public MFCPlugInstore, public MFCMessageObserver
	{
	public: // Constructors and destructor

		/**
		* Constructs an object; leaves on failure.
		* @param aFCObserver an interface for requesting floor control events
		* @return new instance
		*/
		IMPORT_C static CFCPlugInEngine* NewL(MFCObserver& aFCObserver);

		/**
		* Constructs an object and adds the pointer to the cleanup stack;
		* leaves on failure.
		* @param aFCObserver an interface for requesting FC events
		* @return new instance
		*/
		IMPORT_C static CFCPlugInEngine* NewLC(	MFCObserver& aFCObserver);

		/**
		* Destructor
		*/
		IMPORT_C ~CFCPlugInEngine();
		
		/**
		* Gets all supported Floor contrl PlugIn
		* @return supported Floor control plugins names (e.g. TBCP);
        *  the ownership is transferred
		*/
		IMPORT_C CDesC8Array* SupportedProtocolL();
		
		/**
		* Gets the specific Floor contrl PlugIn
		* @return specific Floor control plugins name (e.g. TBCP);
        *  the ownership is transferred
		*/
		IMPORT_C MFCPlugIn* PlugInByNameL(const TDesC8& aProtocolName) ;
		
		/**
		* Gets the new FC session. 
		* @param aType, Plugin type
		* @param aIapId, The IAP ID
		* @param aPort, The port to be used-
		* @return const CFCSession&, reference to FC session 
        *  function will leave if there is no suitable plugIn           
		*/
		IMPORT_C CFCSession& CreateNewSessionL(const TDesC8& aType, 
		                                             TUint32 aIapId,
		                                             TUint32 aPort);		
		
		
		/**
		* Updates the FC session local port.
		* @param aSession, reference to FC session.
		* @param aNewPort, the new port.
        */
        IMPORT_C void SetSessionPortL( CFCSession& aFCSession, TUint32 aNewPort );
		
		/**
		* Updates the FC session local port and the receiving status.
		* KFCDefaultPort means that the session will no be receiving.
		* @param aSession, reference to FC session.
		* @param aNewPort, the new port.
        */
        IMPORT_C void UpdateSessionL( CFCSession& aFCSession, TUint32 aNewPort );
		
		/**
		* Releases the FC session.
		* @param aSession, reference to FC session.
        */
		IMPORT_C void ReleaseSession(const CFCSession& aSession);
		
		/**
		* Gets the set of media format level attributes.
		* @return RPointerArray<CSdpFmtAttributeField>&: The set of media 
        *  format level attributes.
		*/
		IMPORT_C RPointerArray<CSdpFmtAttributeField>& 
			FormatAttributeFieldsL(const CFCSession& aSession);
		
		/**
		* Sets media format level attributes.
		* @param aAttributes, The set of media format level attributes.
		*/
		IMPORT_C void SetFormatAttributeFieldsL(const CFCSession& aSession, 
						RPointerArray<CSdpFmtAttributeField>* aAttributes);
        
		/**
		* Reads remote parameters of FC from offer/answer document.
		* @param aSession FC Session
		* @param aSdpDocument SDP document containing offer/answer
		*/
		IMPORT_C void PeekDocumentL(const CFCSession& aSession, const CSdpDocument& aSdpDocument);

		/**
		* Modifies local parameters of FC to offer document.
		* Adds a media line or modifies existing one.
		* @param aSession FC Session
		* @param aSdpDocument SDP document containing offer.
		*/
		IMPORT_C void UpdateOfferL(const CFCSession& aSession, CSdpDocument& aSdpDocument);
		
		/**
		* Modifies local parameters of FC to answer document.
		* @param aSession FC Session
		* @param aSdpDocument SDP document containing answer, updating the port and addr only
		*/
		IMPORT_C void UpdateAnswerL(const CFCSession& aSession, CSdpDocument& aSdpDocument);
				
		/**
		* Session is ready to send, Socket is open, and start to listen
		* @param aSession FC Session
		*/
		IMPORT_C void StartConnectionL( const CFCSession& aSession );
		
		/**
		* Send the Data to the predefined addresses
		* @param aSession FC Session
		* @param aData, sending via this plugIn
		*/
		IMPORT_C void SendToNetL( const CFCSession& aSession, 
		                          HBufC8* aData );
		
		/**
		* Returns truth value whether or not this the plugin is 
		* to be receive data in this sessions port. Leaves with
		* KErrNotFound, if the session is not found.
		* @param aSession FC Session
		*/
		IMPORT_C TBool IsReceiverL( const CFCSession& aSession );
		
		/**
		* Checks if the floor control paramaters have changed for
		* the given session. Leaves with KErrNotFound, if the 
		* session is not found.
		* @param aSession FC Session
		*/
		IMPORT_C TBool UpdateNeededL( const CFCSession& aSession );
		
		
	public:	// From MFCMessageObserver	
			
		/**
		* Called by a FloorControl plugin
		* @param aData, Receving Data
		* @param aPlugIn, The plug-in
		*/
		void ReceivedData(HBufC8* aData, MFCPlugIn* aPlugIn ) ;

		/**
		* Called by a FloorControl plugin
		* @param aErrCode KErrNone if connection succesfully, otherwise
		*		 one of the system-wide error codes.
		* @param aPlugIn, The plug-in
		*/
		void ErrorNotify( TInt aErrCode, MFCPlugIn* aPlugIn ) ;
  	
	public: 

		/**
		* Gets all PlugIn 
		* @return the iterator of plugin
		*/
		TFCPlugInIter& FCPlugInIter();
		
		/**
		* Load the plugin uses the pre-difined UID
		*/
		void ListMechanismImplementationsL();
		
      
	private:
		// Constructor
		CFCPlugInEngine(MFCObserver& aFCObserver);
		
		void ConstructL();
		
		static void ArrayCleanup(TAny* aArray);
		
		CFCSession* FindFCSession( MFCPlugIn* aPlugIn );
	
	private:	
		//data
		MFCObserver& iFCObserver;
		RPointerArray< CFCPlugInInfo > iFCPlugIns;
		TFCPlugInIter iFCPlugInIter;
		RPointerArray<CFCSession> iFCSessions;
    
    private: // EUnit
        friend class UT_CFCPlugInEngine;
		
	};

#endif // CFCPlugInEngine_H

// End of File
