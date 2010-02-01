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




#ifndef CMCECSESSIONIMPLEMENTATION_H
#define CMCECSESSIONIMPLEMENTATION_H

#include <badesca.h>

#include "mcesrv.h"
#include "mceclientserver.h"
#include "mcecomaudiocodec.h"
#include "mcecomvideocodec.h"

//  FORWARD DECLARATION
class CMceCsSession;
class CMceSipManager;
class TMceIds;
class CMceCsSubSession;
class CMceSipDefaults;
class TMceFileInfo;
class TMceMccComEvent;

// CLASS DECLARATION


/**
*  MCE CS session implementation
*
*  @lib 
*  @since
*/
class CMceCsSessionImplementation : public CBase
	{
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aManager SIP manager
	* @param aITCProvider provides ITC functionality. Ownership is NOT transferred
	* @return CS session implementation
	*/
	static CMceCsSessionImplementation* NewL( CMceSipManager& aManager,
	                                          CMceCsSession* aITCProvider );

	/**
	* Two-phased constructor.
	* @param aManager SIP manager
	* @param aITCProvider provides ITC functionality. Ownership is NOT transferred
	* @return CS session implementation
	*/
	static CMceCsSessionImplementation* NewLC( CMceSipManager& aManager,
	                                           CMceCsSession* aITCProvider );

	/**
	* Destructor.
	*/
	~CMceCsSessionImplementation();


public: // New


	/**
	* Creates sub session
    * @param aIds set of ids defining ITC call context
	* @param aParams parameters for creation
	*/
	void CreateSubSessionL( TMceIds& aIds,
                            CDesC8Array& aParams);
                            
	/**
	* Removes sub session
	* @param aSessionId aSessionID
	*/
    void RemoveSubSessionL( TUint32 aSessionId );
    
	/**
	* Gets all sub sessions
	* @return all sub sessions
	*/
    RPointerArray<CMceCsSubSession>& SubSessions();
    
	/**
	* Finds sub session based on session id
	* @param aSessionId session id
	* @return sub session
	*/
	CMceCsSubSession* SubSessionL( TUint32 aSessionId );

	/**
	* Finds sub session based on dialog id
	* @param aDialogId dialog id
	* @return sub session
	*/
	const CMceCsSubSession* FindSubSession( TUint32 aDialogId );

	/**
	* Returns supported audio codecs
	* @return supported audio codecs as encoded string
	*/
	HBufC8* SupportedAudioCodecsL();

	/**
	* Returns supported video codecs
	* @return supported video codecs as encoded string
	*/
	HBufC8* SupportedVideoCodecsL();
	
	/**
	* Stores default headers for given methods.
	* @param aMethod the methods
	* @param aHeaders the default headers
	*/
	void StoreDefaultHeadersL( CDesC8Array& aMethods, CDesC8Array* aHeaders );
	
	/**
	* Returns the default headers for the given method
	* @param aMethodInd the method index in SIP Strings
	* @return default headers
	*/ 
	CDesC8Array& DefaultHeaders( TInt aMethodInd );
	    
    /**
	* Returns the encoded default headers for the given method
	* @param aMethod the method
	* @return encoded default headers
	*/ 
	HBufC8* DefaultHeadersL( TDesC8& aMethod );
    
    void DoServiceL( TMceIds& aIds, TInt aFunction, TPtr8& aInfo );

    void DoServiceL( TMceIds& aIds, TInt aFunction, const TDesC8& aInfo );
    
    HBufC8* DoServiceL( TMceIds& aIds, TInt aFunction );
    
    void DoServiceL( TMceIds& aIds, TInt aFunction, CMceMsgBase& aMessage );
    
    void StoreClientContent( HBufC8* aClientContent );
    
    /**
    * Callback function to indicate that event has been received
    * @param aEvent, received event
    */
    void EventReceived( TMceMccComEvent& aEvent );
	
    
public: 

    
	/**
	* Sends data via callback mechanism to client
	* @param aIds set of ids defining ITC callback context
	* @param aContext encoded context of message
	* @param aContent content of message
	*/
    void SendToClientL( TMceIds& aIds, HBufC8* aContext, HBufC8* aContent );
   
	/**
	* Removes all subsession
	* @param aMakeOrphan if ETrue, makes subsession orphan, 
	* if session do not terminate
	*/
    void RemoveAllSubSessions( TBool aMakeOrphan = ETrue );
   
	/**
	* Returns ETrue, if session implementation is orphan
	* @return ETrue, if orphan
	*/
    TBool IsOrphan();
    
    /**
	* Returns reference to CS session
	* @return CS session
	*/
    CMceCsSession& ClientSession();
    
    
    
protected:


	CMceCsSessionImplementation ( CMceSipManager& aManager, CMceCsSession& aITCProvider );
	void ConstructL ();
    static void ArrayCleanup(TAny* aArray);
	
    void GetFileInfoL( TMceIds& aIds, TMceFileInfo& aFileInfo );
    
	
private: // Owned data

    //sub sessions
	RPointerArray<CMceCsSubSession> iSubSessions;
	//default headers
	CMceSipDefaults* iDefaults;
    

private: // NOT owned Data

    //SIP manager instance
    CMceSipManager& iManager;
    //ITC provider
	CMceCsSession* iITCProvider;
	
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
	
	};


#endif

// End of File
