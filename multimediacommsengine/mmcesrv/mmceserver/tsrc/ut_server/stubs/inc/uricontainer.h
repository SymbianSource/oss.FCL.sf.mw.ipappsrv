/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CURICONTAINER_H
#define CURICONTAINER_H

//  INCLUDES
#include <e32base.h>
#include <s32mem.h>
#include <uri8.h>
#include <stringpool.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CSIPURI;

// CLASS DECLARATION
/**
* A wrapper class for a generic URI that encapsulates 
* a CUri8 or a CSIPURI instance 
*
* @lib sipcodec.lib
*/
class CURIContainer : public CBase
	{
	public:	// Constructors and destructor

		/**
		* Constructs a CURIContainer from textual representation 
		* @param aValue a generic URI as text
		* @return a new instance of CURIContainer
		*/
		IMPORT_C static CURIContainer* DecodeL(const TDesC8& aValue);

		/**
		* Creates a new CURIContainer instance
		* @pre aUri8 != 0
		* @param aUri8 the ownership is transferred
		* @return a new instance of CURIContainer
		*/
		IMPORT_C static CURIContainer* NewL(CUri8* aUri8);

		/**
		* Creates a new CURIContainer instance and pushes it to CleanupStack
		* @pre aUri8 != 0
		* @param aUri8 the ownership is transferred
		* @return a new instance of CURIContainer
		*/
		IMPORT_C static CURIContainer* NewLC(CUri8* aUri8);

		/**
		* Creates a deep copy of the given CURIContainer instance
		* @param aURI CURIContainer to be copied
		* @return a new instance of CURIContainer
		*/
		IMPORT_C static CURIContainer* NewL(const CURIContainer& aURI);
	
		/**
		* Creates a deep copy of the given CURIContainer instance and 
		* pushes it to CleanupStack.
		* @param aURI CURIContainer to be copied
		* @return a new instance of CURIContainer
		*/
		IMPORT_C static CURIContainer* NewLC(const CURIContainer& aURI);

		/**
		* Destructor, deletes the resources of CURIContainer.
		*/
		IMPORT_C ~CURIContainer();
	

	public: // New functions

		/**
		* Checks if the wrapped URI is SIP/SIPS-URI
		* @return ETrue if the wrapped URI is SIP/SIPS-URI, otherwise EFalse
		*/
		IMPORT_C TBool IsSIPURI() const;

		/**
		* Gets the SIP URI if present.
		* @pre IsSIPURI()==ETrue
		* @return a modifiable CSIPURI or NULL if not present.
		*/
		IMPORT_C CSIPURI* SIPURI();

		/**
		* Gets the SIP URI if present.
		* @pre IsSIPURI()==ETrue
		* @return a non-modifiable CSIPURI or NULL if not present.
		*/
		IMPORT_C const CSIPURI* SIPURI() const;

		/**
		* Gets the CUri8.
		* @return a non-modifiable CUri8 or NULL if IsSIPURI()==ETrue
		*/
		IMPORT_C const CUri8* Uri8() const;

		/**
		* Compares this object to another instance of CURIContainer
		* @param  aURI a URI to compare to
		* @return ETrue, if the objects are equal otherwise EFalse
		*/
		IMPORT_C TBool operator==(const CURIContainer& aURI) const;
	
		/**
		* Encodes the object into a textual representation.
		* @return a textual representation of the object,
		*         the ownership is transferred
		*/
		IMPORT_C HBufC8* ToTextL() const;	
	
		/**
		* Constructs an instance of a CURIContainer from a RReadStream
		*
		* @param aReadStream a stream containing the externalized object
		* @return an instance of a CURIContainer
		*/ 
		IMPORT_C static CURIContainer* InternalizeL(RReadStream& aReadStream);
	
		/**
		* Externalizes the object to stream
		*
		* @param aWriteStream a stream where the object is to be externalized
		*/
		IMPORT_C void ExternalizeL(RWriteStream& aWriteStream) const;

    public: // New functions, for internal use

        static CUri8* CreateUri8L(const TDesC8& aValue);

	private: // Constructors

		void ConstructL();
		void ConstructL(CUri8* aUri8);
		void ConstructL(const CURIContainer& aURI);

	private: // New functions

		void DoInternalizeL(RReadStream& aReadStream);
        static TBool FindSIP(const TDesC8& aValue);

	private: // Data
	
		CSIPURI* iSIPURI;
		CUri8* iUri8;

    private: // For testing purposes

        UNIT_TEST(CURIContainerTest)
	};

#endif // CURICONTAINER_H

// End of File
