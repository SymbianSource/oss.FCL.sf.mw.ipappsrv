/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Monitors a property
*
*/



#ifndef CMCEICEPROPERTYWATCHER_H
#define CMCEICEPROPERTYWATCHER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <in_sock.h>


// Forwared Declaration

/**
 *  Maintains an up to date value of a specified property.
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class CMceIcePropertyWatcher : public CActive					
    {
	public: // Constructors and destructor

	    /**
	     * A two-phase constructor.
	     * @param aCategory UID that identifies the property category
	     * @param aKey Property sub-key, identifying the specific property
	     */
	    static CMceIcePropertyWatcher* NewL( TUid aCategory, TUint aKey );
	    
	    /**
	     * Destructor.
	     */
	    ~CMceIcePropertyWatcher();

	public: // From CActive

		void DoCancel();
		
		void RunL();

		TInt RunError( TInt aError );

	public: // New functions
	
		/**
	     * Obtain the current value of the property.
	     * @param aValue OUT: value of the property
	     * @return KErrNone if successful, otherwise a system wide error
	     */
		TInt PropertyValue( TInetAddr& aValue ) const;

	protected: // Constructors

	    CMceIcePropertyWatcher( TUid aCategory, TUint aKey );

	    void ConstructL();

	protected: // New virtual functions

		virtual void ParsePropertyL();

	protected: // New functions

		const TDesC8& Value() const;

	private: // New functions

		void ListenForChanges();
		
		void GetPropertyValueL();

	private: // Data

		RProperty iProperty;

		// UID that identifies the property category
 		TUid iCategory;
 		// Property sub-key, identifying the specific property
 		TUint iKey;
 
 		// Has the property value been successfully read
 		TBool iPropertyExists;

		// Property value, owned.
		HBufC8* iValue;

		// Propery value converted to address
		TInetAddr iAddress;

	    // For unit testing
		friend class UT_CMceIceDirector;    
 	};

#endif // CMCEICEPROPERTYWATCHER_H

// end of file
