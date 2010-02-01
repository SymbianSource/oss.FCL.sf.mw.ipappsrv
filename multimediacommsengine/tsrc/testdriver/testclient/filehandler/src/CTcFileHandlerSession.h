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

#ifndef __CTCFILEHANDLERSESSION_H__
#define __CTCFILEHANDLERSESSION_H__

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class CTcFileHandlerServer;

//  CLASS DEFINITION
/**
 * CTcFileHandlerSession implements the client-server session of the FileHandler.
 * One object instance of this class is created for each connected client.
 */
class CTcFileHandlerSession
#ifdef __IPC_V2_PRESENT__
    : public CSession2
#else
    : public CSession
#endif
    {
    public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aServer Pointer to the server object owning us.
		 * @return An initialised instance of this class.
		 */
		static CTcFileHandlerSession* NewL( CTcFileHandlerServer* aServer );

		/// Destructor
		~CTcFileHandlerSession();

    private: // Constructors

		/**
		 * Constructor.
		 *
		 */
        CTcFileHandlerSession();


		/**
		 * 2nd phase constructor
		 *
		 * @param aServer Pointer to the server object owning us.
		 */
        void ConstructL( CTcFileHandlerServer* aServer );
        
        void DoCreateFileL( const RMessage2& aMessage ) const;
        void DoCopyFileL( const RMessage2& aMessage ) const;
        void DoDeleteFileL( const RMessage2& aMessage ) const;
        
        void ReadFileNameL( TInt aItcArgIndex,
                            const RMessage2& aMessage,
                            TFileName& aFileName ) const;
                        
        HBufC8* ReadLC( TInt aItcArgIndex,
                        const RMessage2& aMessage ) const;

	public: // from CSession

		void ServiceL( const RMessage2& aMessage );

	private: // Data

		/// Pointer to our owner. Not owned.
		CTcFileHandlerServer* iFileHandlerServer;

		/// Client application name and path. Owned.
		TFileName iAppName;

    };

#endif // __CTCFILEHANDLERSESSION_H__
