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

#include <e32base.h>
#include <bamdesca.h>
#include "mceclientserver.h"


//
// ITC for unit testing purposes!
//
class CMceCsServerITC : public CBase
	{
	public:

		static CMceCsServerITC* NewL();

		static CMceCsServerITC* NewLC();

		~CMceCsServerITC ();
        
	
		void WriteL (const RMessage2& aMessage,
					 const TDesC8& aDes,
					 TMceItcArguments aItcArgIndex) const;

		void WriteL (const RMessage2& aMessage,
					 const TMceIds& aIds) const;
					 
		void WriteL (const RMessage2& aMessage,
                     const TMceMessageBufSizes aSizes) const;
		    
		TMceIds ReadMCEIdsL (const RMessage2& aMessage) const;

		TUint32 ReadTUint32L (const RMessage2& aMessage,
					   TMceItcArguments aItcArgIndex) const;

		TInt ReadIntL (const RMessage2& aMessage,
					   TMceItcArguments aItcArgIndex) const;
                   
		HBufC8* ReadLC (const RMessage2&  aMessage,
						TMceItcArguments aItcArgIndex) const;

		TInt Function (const RMessage2& aMessage) const;

		void Complete (const RMessage2& aMessage,
                       TInt aCompletionCode) const;

		void PanicClient(const RMessage2& aMessage, TInt aPanic) const;

	private:

		void ConstructL ();

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
