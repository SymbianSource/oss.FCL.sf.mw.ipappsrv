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





#ifndef CSIPURI_H
#define CSIPURI_H

//  INCLUDES
#include <e32base.h>
#include <s32mem.h>
#include <badesca.h>
#include <uri8.h>
#include <stringpool.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CSIPHostPort;
class CSIPURIParams;
class CSIPURIHeaders;

// CLASS DECLARATION
/**
* Class provides functions for setting and getting fields and parameters
* in the SIP-URI or SIPS-URI structure. 
*
* @lib sipcodec.lib
*/
class CSIPURI : public CBase
	{
	public:	// Constructors and destructor

		/**
		* Constructs a CSIPURI from textual representation 
		* @param aValue SIP-URI or SIPS-URI as text
		* @return a new instance of CSIPURI
		*/
		IMPORT_C static CSIPURI* DecodeL(const TDesC8& aValue);

		/**
		* Destructor, deletes the resources of CSIPURI.
		*/
		IMPORT_C ~CSIPURI();
	

	public: // New functions

		/**
		* Checks the scheme of this CSIPURI
		* @return ETrue if the scheme is "sips:", EFalse if the scheme is "sip:" 
		*/
		IMPORT_C TBool IsSIPSURI() const;

		/**
		* Sets the hostport
		* @pre aHostPort != 0
		* @param aHostPort the ownership is transferred
		*/
		IMPORT_C void SetHostPortL(CSIPHostPort* aHostPort);
	
		/**
		* Gets the hostport as const
		* @return the hostport 
		*/
		IMPORT_C const CSIPHostPort& HostPort() const;

		/**
		* Gets the hostport
		* @return the hostport 
		*/
		IMPORT_C CSIPHostPort& HostPort();

		/**
		* Gets the user part of userinfo structure
		* @return the user
		*/				
		IMPORT_C const TDesC8& User() const;
	
		/**
		* Sets the user part of userinfo structure
		* @param aUser the user to set
		*/	
		IMPORT_C void SetUserL(const TDesC8& aUser);

		/**
		* Deletes the userinfo (user+password)
		*/	
		IMPORT_C void DeleteUserInfo();

		/**
		* Gets the password part of userinfo structure
		* @return the password
		*/	
		IMPORT_C const TDesC8& Password() const;

		/**
		* Sets the password part of userinfo structure
		* @param aPassword the password to set
		*/	
		IMPORT_C void SetPasswordL(const TDesC8& aPassword);

		/**
		* Deletes the password part of userinfo structure
		*/	
		IMPORT_C void DeletePassword();

		/**
		* Checks if any parameters are present
		* @return ETrue if present, otherwise EFalse 
		*/
		IMPORT_C TBool HasParams() const;
	
		/**
		* Gets the value of the "ttl" parameter
		* @return the value of the "ttl" parameter or 
		*         KErrNotFound if the ttl parameter is not present.
		*/	
		IMPORT_C TInt TtlParam() const;

		/**
		* Sets the value of the "ttl" parameter
		* @pre aValue >= 0
		* @param aValue the value of the "ttl" parameter
		*/	
		IMPORT_C void SetTtlParamL(TInt aValue);

		/**
		* Checks if a URI parameter is present
		* @param aName the name of the URI parameter
		* @return ETrue if present, otherwise EFalse 
		*/
		IMPORT_C TBool HasParam(RStringF aName) const;

		/**
		* Gets the value of a URI parameter
		* @pre HasParam(aName) == ETrue
		* @return the value of a URI parameter
		*/	
		IMPORT_C RStringF ParamValue(RStringF aName) const;

		/**
		* Sets a new URI parameter that does not have a value
		* @param aName the name of a URI parameter
		*/	
		IMPORT_C void SetParamL(RStringF aName);

		/**
		* Sets a new URI parameter with a name and a value
		* @param aName the name of a URI parameter
		* @param aValue the value of a URI parameter
		*/	
		IMPORT_C void SetParamL(RStringF aName, RStringF aValue);

		/**
		* Deletes a URI parameter
		*/
		IMPORT_C void DeleteParam(RStringF aName);

		/**
		* Checks if named URI headers are present
		* @param aName the name of the URI headers
		* @return ETrue if present, otherwise EFalse 
		*/
		IMPORT_C TBool HasURIHeaders(const TDesC8& aName) const;

		/**
		* Returns an array of URI headers with the given name.
		* The header values are copies of the actual header values
		* in the SIP-URI
		* thus the actual values cannot be modified through the given array.
		* Note that removing a header value from the array
		* does NOT remove it from the SIP URI.
		*
		* @param aName the name of the URI headers
		* @return 0 if no URI headers with the given name were found.
		*         Otherwise array of URI header values.
		*         The ownership of the array is transferred.
		*/
		IMPORT_C CPtrC8Array* URIHeaderValuesL(const TDesC8& aName) const;

		/**
		* Adds a new URI header
		* @param aName the name of a URI header
		* @param aValue the value of a URI header
		*/	
		IMPORT_C void AddURIHeaderL(const TDesC8& aName, const TDesC8& aValue);

		/**
		* Compares this object to another instance of CSIPURI
		* @param  aURI a SIP URI to compare to
		* @return ETrue, if the objects are equal otherwise EFalse
		*/
		IMPORT_C TBool operator==(const CSIPURI& aURI) const;
	
		/**
		* Encodes the object into a textual representation.
		* @return a textual representation of the object,
		*         the ownership is transferred
		*/
		IMPORT_C HBufC8* ToTextL() const;	


	public: // New functions, for internal use

		void DeleteAllURIHeaders();

	private: // Constructors

        static CSIPURI* NewL();
        static CSIPURI* NewLC();
		static CSIPURI* NewL(const CSIPURI& aSIPURI);
		static CSIPURI* NewLC(const CSIPURI& aSIPURI);
		CSIPURI(TBool aIsSIPSUri=EFalse);
        void ConstructL();
		void ConstructL(const CSIPURI& aSIPURI);

	private: // New functions

		TPtrC8 ParseSipSchemeL(const TDesC8& aValue);
		void ParseL(const TDesC8& aValue);
		void ParseUserInfoL(const TDesC8& aValue);
		void LocateSeparatorL(const TDesC8& aValue,
		                      TChar aSeparator,
		                      TInt& aPosition) const;
		void SetURIParams(CSIPURIParams* aURIParams);
		void SetURIHeaders(CSIPURIHeaders* aURIHeaders);

	private: // Data
	
		TBool iIsSIPSURI;
		CSIPHostPort* iHostPort;
		TBool iHasUser;
		HBufC8* iUser;
		TBool iHasPassword;
		HBufC8* iPassword;
		CSIPURIParams* iURIParams;
		CSIPURIHeaders* iURIHeaders;

	private: // For testing purposes
	
		UNIT_TEST(CSIPURITest)
	};

#endif // CSIPURI_H

// End of File
