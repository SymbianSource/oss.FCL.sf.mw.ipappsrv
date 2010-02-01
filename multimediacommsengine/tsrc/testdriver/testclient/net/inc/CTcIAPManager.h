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
* Description:  See class definition below.
*
*/

#ifndef __CTCIAPMANAGER_H__
#define __CTCIAPMANAGER_H__

// INCLUDES
#include <e32base.h>
#include <bamdesca.h>
#include <cdblen.h>

// CLASS DEFINITION
/**
 * CTcIAPManager provides easy access to the Internet Access Points available
 * in the system.
 */
class CTcIAPManager
	: public CBase,
	  public MDesC8Array
	{
	private: // Nested classes

		// CLASS DEFINITION
		/**
		 * Container for IAP entries having an ID and a name.
		 */
		class TEntry
			{
			public: // Data

				/// IAP Id
				TUint32 iId;

				/// IAP Name
				TBuf8< KCommsDbSvrMaxFieldLength > iName;

			};

	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @return An initialized instance of this class.
		 */
		IMPORT_C static CTcIAPManager* NewL();

		/// Destructor
		IMPORT_C ~CTcIAPManager();

	private:	// Constructors and destructor

		/// Default constructor.
		CTcIAPManager();

		/// 2nd phase constructor
		void ConstructL();

	public: // From MDesC8Array

		TInt MdcaCount() const;
		TPtrC8 MdcaPoint( TInt aIndex ) const;

	public: // New methods

		/// @return Number of IAPs available
		IMPORT_C TInt Count() const;

		/**
		 * Return the CommDb ID of an IAP
		 *
		 * @param aIndex IAP entry index (0..Count()-1)
		 * @return IAP id
		 */
		IMPORT_C TUint32 Id( TInt aIndex ) const;

		/**
		 * Return the CommDb Name of an IAP
		 *
		 * @param aIndex IAP entry index (0..Count()-1)
		 * @return Reference to IAP name
		 */
		IMPORT_C const TDesC8& Name( TInt aIndex ) const;

		/**
		 * Searches for an IAP entry with a matching id.
		 *
		 * @param aId The id to be searched for.
		 * @return Reference to IAP name
		 * @exceptions Leaves with KErrNotFound if no match is found.
		 */
		IMPORT_C const TDesC8& NameForIdL( TUint32 aId ) const;

		/**
		 * Searches for an IAP entry with a matching name.
		 *
		 * @param aName The name to be searched for.
		 * @return IAP id
		 * @exceptions Leaves with KErrNotFound if no match is found.
		 */
		IMPORT_C TUint32 IdForNameL( const TDesC8& aName ) const;

	private: // data

		/// Internet Access Point list. Owned.
		CArrayFixSeg< TEntry > iEntries;

	};

#endif // __CTCIAPMANAGER_H__
