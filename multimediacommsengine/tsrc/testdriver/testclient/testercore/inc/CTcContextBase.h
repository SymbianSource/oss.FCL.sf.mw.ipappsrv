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

#ifndef __CTCCONTEXTBASE_H__
#define __CTCCONTEXTBASE_H__

// INCLUDES
#include "CTcParameterList.h"
#include "CTcRegistry.h"
#include "MTcTestContext.h"

// FORWARD DECLARATIONS
class TTcCommandBase;

// DEFINES

// For command registration array
typedef TBool (*CommandMatch)( const TTcIdentifier& aId );
typedef TTcCommandBase* (*CommandNew)( MTcTestContext& aContext );
struct TcCommand
	{
	CommandMatch iMatchFunc;
	CommandNew iNewFunc;
	};
#define AddCmd( cmdClass ) { cmdClass::Match, cmdClass::CreateL }

// CLASS DEFINITION
/**
 * CTcContextBase implements a generic test context base class
 * and an implementation of MTcTestContext.
 */
class CTcContextBase
	: public CBase,
	  public MTcTestContext
	{
	public:	// Constructors and destructor

		/// Destructor
		IMPORT_C ~CTcContextBase();

	protected:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aCodec Reference to CTRL codec object,
		 *				 initialized to a valid request.
		 */
		IMPORT_C CTcContextBase( CTcCTRLCodec& aCodec );

	private:	// Constructors and destructor

		/// Default constructor. Not implemented.
		CTcContextBase();

	public: // From MTcTestContext

		IMPORT_C void ExecuteL();
		IMPORT_C void ResetL( const TTcIdentifier& aId );
		IMPORT_C CTcCTRLCodec& Codec();
		IMPORT_C CTcRegistry& Registry();

	public: // New methods

		/// @return Reference to the request parameter list.
		IMPORT_C CTcParameterList& List();

		/// @return Reference to the response parameter list.
		IMPORT_C CTcParameterList& ReturnList();

	protected: // Abstract methods

		/**
		 * Factory method for creating tester commands. Creates a command
		 * instance that matches the specified function identifier.
		 *
		 * @param aId Function/command identifier.
		 * @param aContext Parent test context.
		 * @return An initialized command instance.
		 * @exceptions Leaves with KErrNotSupported if no matching command
		 *			   is found.
		 */
		virtual TTcCommandBase* CreateCommandL( TTcIdentifier& aId,
												MTcTestContext& aContext ) = 0;

	protected: // New methods

		IMPORT_C TTcCommandBase* CreateCommandL(
											const TcCommand* aCommandArray,
											TInt aSizeOfArray,
											TTcIdentifier& aId,
											MTcTestContext& aContext );

	protected: // Data

		/// Reference to the CTRL codec. Not owned.
		CTcCTRLCodec& iCodec;

		/// Tester object registry. Owned.
		CTcRegistry iRegistry;

		/// Request parameter list. Owned.
		CTcParameterList iList;

		/// Response parameter list. Owned.
		CTcParameterList iReturnList;

	};

#endif // __CTCCONTEXTBASE_H__
