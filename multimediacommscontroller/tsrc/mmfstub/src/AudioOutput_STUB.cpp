/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    This is the implementation of the CAudioOutput class.
*
*/





// INCLUDE FILES
#include <mmf/server/sounddevice.h>
#include <AudioOutput.h>

class CAudioOutputImpl : public CAudioOutput
    {

public :    

    TAudioOutputPreference AudioOutput()
        {
        return iAudioOutput;
        }

	TAudioOutputPreference DefaultAudioOutput()
	    {
	    return ENoPreference;
        }

	void RegisterObserverL( MAudioOutputObserver& /*aObserver*/ )
	    {
	    delete HBufC8::NewL( 10 );
        }

	TBool SecureOutput()
	    {
	    return EFalse;
        }

	void SetAudioOutputL( TAudioOutputPreference aAudioOutput = ENoPreference )
	    {
	    delete HBufC8::NewL( 10 );
	    iAudioOutput = aAudioOutput;
        }

	void SetSecureOutputL( TBool /*aSecure = EFalse*/ )
	    {
	    delete HBufC8::NewL( 10 );
        }

	void UnregisterObserver( MAudioOutputObserver& /*aObserver*/ )
	    {
        }
        
    CAudioOutputImpl();

private:            

    TAudioOutputPreference iAudioOutput;
    
    };

CAudioOutputImpl::CAudioOutputImpl() :
    iAudioOutput( ENoPreference )
    {
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMdaAudioPlayerUtility& /*aUtility*/)
    {
    return new ( ELeave ) CAudioOutputImpl;
    }


// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMdaAudioRecorderUtility& /*aUtility*/, TBool /*aRecordStream*/)
    {
    
    return new ( ELeave ) CAudioOutputImpl;
    }


// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMdaAudioOutputStream& /*aUtility*/)
    {
    
    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMdaAudioToneUtility& /*aUtility*/)
    {
    

    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMMFDevSound& /*aDevSound*/)
    {
    

    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(MCustomInterface& /*aUtility*/)
    {
    

    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(MCustomCommand& /*aUtility*/)
    {
    
    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CMidiClientUtility& /*aUtility*/)
    {
    	
    return new ( ELeave ) CAudioOutputImpl;
    }

// Two-phased constructor.
EXPORT_C CAudioOutput* CAudioOutput::NewL(CDrmPlayerUtility& /*aUtility*/)
    {
    
    return new ( ELeave ) CAudioOutputImpl;
    }

EXPORT_C CAudioOutput* CAudioOutput::NewL(CVideoPlayerUtility& /*aUtility*/)
    {
    
    return new ( ELeave ) CAudioOutputImpl;
    }
    
// End of file

