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

#ifndef __CTCWATCHERSESSION_H__
#define __CTCWATCHERSESSION_H__

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class CTcCoroner;
class CTcGrimreaper;
class CTcWatcherServer;

//  CLASS DEFINITION
/**
 * CTcWatcherSession implements the client-server session of the Watcher.
 * One object instance of this class is created for each connected client.
 */
class CTcWatcherSession
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
		 * @param aClient Reference to the client thread that
		 *				  has opened this session.
		 * @param aServer Pointer to the server object owning us.
		 * @return An initialised instance of this class.
		 */
		static CTcWatcherSession* NewL( 
#ifndef __IPC_V2_PRESENT__
										RThread& aClient,
#endif
									    CTcWatcherServer* aServer );

		/// Destructor
		~CTcWatcherSession();

    private: // Constructors

		/**
		 * Constructor.
		 *
		 * @param aClient Reference to the client thread that
		 *				  has opened this session.
		 */
#ifdef __IPC_V2_PRESENT__
        CTcWatcherSession();
#else
        CTcWatcherSession( RThread& aClient );

		/// Default constructor. Not implemented.
		CTcWatcherSession();
#endif

		/**
		 * 2nd phase constructor
		 *
		 * @param aServer Pointer to the server object owning us.
		 */
        void ConstructL( CTcWatcherServer* aServer );

	public: // from CSession

#ifdef __IPC_V2_PRESENT__
		void ServiceL( const RMessage2& aMessage );
#else
		void ServiceL( const RMessage& aMessage );
#endif

	private: // Data

		/// Pointer to our owner. Not owned.
		CTcWatcherServer* iWatcherServer;

		/// Client coroner. Owned.
		CTcCoroner* iCoroner;

		/// Client grimreaper. Owned.
		CTcGrimreaper* iGrimreaper;

		/// Client application name and path. Owned.
		TFileName iAppName;

    };

#endif // __CTCWATCHERSESSION_H__
