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





#ifndef CSIPDIALOG_H
#define CSIPDIALOG_H

// INCLUDES
#include <e32base.h>
#include <stringpool.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CSIPToHeader;
class CSIPFromHeader;
class CUri8;
class CSIPCallIDHeader;

// CLASS DECLARATION

/**
*  Class for managing SIP dialogs.
*  It provides services querying dialog state, obtaining the dialog related
*  SIP headers and getting all dialog associations.
*
*  The user of the class cannot instantiate this class.
*  @lib sipclient
*/

class CSIPDialog: public CBase
	{
	public:
		/** Dialog states */
		enum TState
			{
			/** Initiliazed state */
			EInit,
			/** Early state */
			EEarly,
			/** Confirmed state */
			EConfirmed,
			/** Terminated state */
			ETerminated
			};

	public: //Constructors and destructor, for internal use
		/**
		* Two-phased constructor. 
		* This constructor should be used if the user has received
		* SIP request that creates a SIP dialog association.
        *
        * @param aConnImplementation Implementation of the used SIP connection
        * @return New object, ownership is transferred.
		*/
		static CSIPDialog* NewL();

        static CSIPDialog* NewL( const CUri8& aRemoteUri,
                                 const CSIPFromHeader& aFrom,
                                 const CSIPToHeader& aTo );

		/**
		* Destructor
		*/
		~CSIPDialog();

	public: //New functions
		/**
		* Gets dialog state		
		* @return dialog state
		*/
        IMPORT_C CSIPDialog::TState State() const;

        /**
		* Compares this object to another object
		* @param aDialog CSIPDialog object to compare
		* @returns ETrue if the objects are equal, otherwise EFalse
		*/
        IMPORT_C TBool operator==(const CSIPDialog& aDialog) const;
        
        /**
		* Gets originator's address
		* @return originator's address (From-header)
		*/
		IMPORT_C const CSIPFromHeader& FromHeader() const;

		/**
		* Gets recipient's address
		* @return recipient's address (To-header)
		*/
		IMPORT_C const CSIPToHeader& ToHeader() const;

		/**
		* Gets remote-uri used during dialog creation
		* @return Remote target uri
		*/
		IMPORT_C const CUri8& RemoteURI() const;

		/**
		* Gets Call-ID of SIP dialog
		* @pre State()==CSIPDialog::EEarly || State()==CSIPDialog::EConfirmed
		* @return Call-ID of SIP dialog
		* @leave KErrSIPInvalidDialogState if dialog doesn't yet have a Call-ID
		*/
		IMPORT_C const CSIPCallIDHeader& CallIdL() const;
		
		// in stub for controlling the state
        IMPORT_C void SetState( TState aState );
        

	public: // New functions, for internal use

    
        inline void Attach() 
            {
            iReferenceCount++;
            }
        
        inline TInt Detach() 
            {
            iReferenceCount--;
            return iReferenceCount;
            }
        
    private: // Constructors

        CSIPDialog();        

        void ConstructL( const CUri8& aRemoteUri,
                          const CSIPFromHeader& aFrom,
                          const CSIPToHeader& aTo);

public: // Data

    TState iState;

private: // Data


    CUri8* iRemoteUri;
    CSIPFromHeader* iFrom;
    CSIPToHeader* iTo;
	CSIPCallIDHeader* iCallId;

    TInt iReferenceCount;
        
private: // For testing purposes

	};

#endif
