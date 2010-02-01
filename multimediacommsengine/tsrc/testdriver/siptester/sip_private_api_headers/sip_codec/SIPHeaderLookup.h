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



#ifndef SIPHEADERLOOKUP_H
#define SIPHEADERLOOKUP_H

//  INCLUDES
#include <e32std.h>
#include <s32mem.h>
#include "sipheaderbase.h"
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CSIPHeaderLookupTable;

// CLASS DECLARATION
/**
* A class for generic header construction.
*
* @lib sipcodec.lib
*/
class SIPHeaderLookup
	{
	public:	// Constructors and destructor
	
		/**
		* Opens a SIP header lookup. 
		* Implemented with a reference count.
		*/
		IMPORT_C static void OpenL();

		/**
		* Closes a SIP header lookup.
		* In other words decrements reference count and if it reaches
		* zero, closes the header lookup.	 
		* The user must not call Close() if it has not called OpenL().
		*/
		IMPORT_C static void Close();
	

	public: // New functions

		/**
		* Tells if a SIP header is supported as an explicit class.
		* (Not only as an extension header).
		* @param aName the name of the header
		* @returns ETrue if a given header is supported, otherwise EFalse
		*/
		IMPORT_C static TBool IsSupported(RStringF aName);	
	
		/**
		* Internalizes a header with a given name.
		* @pre aHeaderName.Length()>0
		* @param aName the name of the header
		* @param aReadStream the stream from where to internalize
		* @returns 0, if internalizing for the given header is not supported,
		*          otherwise the internalized header.
		*/
		IMPORT_C static CSIPHeaderBase* 
			InternalizeL(RStringF aName, RReadStream& aReadStream);

		/**
		* Checks if the header is part of the public SIP API as a non-extension.
		* @param aName the name of the header
		* @returns ETrue if supported, otherwise EFalse.
		*/
		IMPORT_C static TBool IsAPIHeader(RStringF aName);

		/**
		* Decodes SIP header(s).
		* @pre aHeaderName.Length()>0
		* @param aName the name. For example "To".
		* @param aValue the value. For example "User <user@host>"
		* @returns An array of headers. Note that the value parameter may contain
		*          multiple comma separated header values for those SIP headers
		*          that allow this format according to SIP BNF.
		*/
		IMPORT_C static RPointerArray<CSIPHeaderBase> 
			CreateHeaderL(RStringF aName, const TDesC8& aValue);


	public: // For internal use

		IMPORT_C static void SetConvertToSIPURI(TBool aValue=ETrue);
		static TBool ConvertToSIPURI();

		
	private: // New functions

		static CSIPHeaderLookupTable* LookupTable();
	};

#endif // SIPHEADERLOOKUP_H

// End of File
