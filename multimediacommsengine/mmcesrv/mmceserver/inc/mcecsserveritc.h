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




#ifndef CMCECSSERVERITC_H
#define CMCECSSERVERITC_H

//  INCLUDES
#include <e32base.h>
#include <bamdesca.h>
#include "mceclientserver.h"

// CLASS DECLARATION

/**
*  ITC helper class for MCE server.
*
*  @lib 
*  @since
*/
class CMceCsServerITC : public CBase
	{
	public: // Constructors and destructor
    
		/**
		* Two-phased constructor.
		*/
		static CMceCsServerITC* NewL();

		/**
		* Two-phased constructor.
		*/
		static CMceCsServerITC* NewLC();

		/**
		* Destructor.
		*/
		~CMceCsServerITC ();
        
	public: // New

		/**
		* Writes desctructor to client.
		* @param aMessage, message of the client.
		* @param aDes, descriptor to be written.
		* @param aItcArgIndex, index of ITC argument.
		*/
		void WriteL (const RMessage2& aMessage,
					 const TDesC8& aDes,
					 TMceItcArguments aItcArgIndex) const;

		/**
		* Writes ids to client.
		* @param aMessage, message of the client.
		* @param aIds, ids to be written.
		*/
		void WriteL (const RMessage2& aMessage,
					 const TMceIds& aIds) const;

		/**
		* Writes sizes to client.
		* @param aMessage, message of the client.
		* @param aSizes, sizes to be written.
		*/
		void WriteL (const RMessage2& aMessage,
                     const TMceMessageBufSizes aSizes) const;
		    
		/**
		* Reads ids from the client.
		* @param aMessage, message of the client.
		* @return ids from the client.
		*/
		TMceIds ReadMCEIdsL (const RMessage2& aMessage) const;

		/**
		* Reads TUint32 from the client.
		* @param aMessage, message of the client.
		* @param aItcArgIndex, index of ITC argument.
		* @return value from the client.
		*/
		TUint32 ReadTUint32L (const RMessage2& aMessage,
					   TMceItcArguments aItcArgIndex) const;

		/**
		* Reads Tint from the client.
		* @param aMessage, message of the client.
		* @param aItcArgIndex, index of ITC argument.
		* @return value from the client.
		*/
		TInt ReadIntL (const RMessage2& aMessage,
					   TMceItcArguments aItcArgIndex) const;
                   
		/**
		* Reads descriptor from the client.
		* @param aMessage, message of the client.
		* @param aItcArgIndex, index of ITC argument.
		* @return value from the client.
		*/
		HBufC8* ReadLC (const RMessage2&  aMessage,
						TMceItcArguments aItcArgIndex) const;

		/**
		* Reads function of the message.
		* @param aMessage, message of the client.
		* @return function value from the message.
		*/
		TInt Function (const RMessage2& aMessage) const;

		/**
		* Completes the message.
		* @param aMessage, message of the client.
		* @param aCompletionCode, code used for completing.
		*/
		void Complete (const RMessage2& aMessage,
                       TInt aCompletionCode) const;

		/**
		* Panics the client.
		* @param aMessage, message of the client.
		* @param aPanic, pacic code used.
		*/
		void PanicClient(const RMessage2& aMessage, TInt aPanic) const;

	private:

		CMceCsServerITC ();

		const TAny* ITCArgPtr (TMceItcArguments aItcArgIndex,
							   const RMessage2&  aMessage) const;

		TInt ITCArgInt (TMceItcArguments aItcArgIndex,
						const RMessage2&  aMessage) const;

		void WriteL (TMceItcArguments aItcArgIndex,
					 const RMessage2&  aMessage,
					 const TDesC8&    aDes) const;

		HBufC8* ReadL (const RMessage2&  aMessage,
					   TMceItcArguments aItcArgIndex) const;

	};

#endif 

// End of File
