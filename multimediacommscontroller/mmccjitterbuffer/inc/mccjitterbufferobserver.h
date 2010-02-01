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
* Description:    Observer for Jitter buffer.
*
*/




#ifndef Mcc_JITTERBUFFER_OBS_H
#define Mcc_JITTERBUFFER_OBS_H


class MJitterBufferObserver
    {
    public:
        enum TJitterBufferError
            {
            EBufferUnderflow,
            EBufferOverflow            
            };
    
    	/**
         * Callback function to receive jitter buffer error events.
         */
         virtual void JitterBufferError( TInt aError ) = 0;
         
         /**
          * Callback function for codec reconfiguration for dynamic
          * buffer size adjustment.
          * @since S60 v3.2
          * @param aBufferSize Buffer size request in samples.
          */
         virtual void DynamicBufferChangeRequest( TInt aBufferSize ) = 0;

    };

#endif	// Mcc_RTP_INACTIVITY_OBS_H

