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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdWriteSdpToFile.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngineSession.h"
#include <MCESession.h>
#include <mcemediastream.h>
#include <mcertpsource.h>
#include <f32file.h>

_LIT( KSdpFile, "c:\\system\\data\\sdp.txt" );

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::TMCETestUIEngineCmdFCSendSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdWriteSdpToFile::TMCETestUIEngineCmdWriteSdpToFile( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdWriteSdpToFile::ExecuteL()
	{
    RFs fs;
	if( fs.Connect() != KErrNone )
		{
		return;
		}
	RFile out;
	if( !out.Replace( fs, KSdpFile, EFileWrite | EFileShareAny ) )
		{
		MDesC8Array* sdp = iSession.Session().SessionSDPLinesL();
		User::LeaveIfNull( sdp );
		for( int i = 0; i < sdp->MdcaCount(); i++ )
		    {
		    TInt err = out.Write( sdp->MdcaPoint(i) );
		    User::LeaveIfError( err );
		    }
		delete sdp;
		
		const RPointerArray<CMceMediaStream>& streams = iSession.Session().Streams();
        
        for( int i = 0; i < streams.Count(); i++ )
		    {
		    MDesC8Array* sdpmedia = streams[i]->MediaAttributeLinesL();
	    	User::LeaveIfNull( sdpmedia );
	    	
	    	if( streams[i]->Source()->Type() == KMceRTPSource )
	    	    {
	    	    TInt err = out.Write( _L8("Writing main stream media DL:\r\n") );
    		    User::LeaveIfError( err );
	    	    }
	        else
	            {
	            TInt err = out.Write( _L8("Writing main stream media UL:\r\n") );
    		    User::LeaveIfError( err );
	            }
	            
		    for( int i = 0; i < sdpmedia->MdcaCount(); i++ )
    		    {
    		    TInt err = out.Write( sdpmedia->MdcaPoint(i) );
    		    User::LeaveIfError( err );
    		    }
    		delete sdpmedia; 
    		    
    		if( streams[i]->BoundStream() )
                {
                CMceMediaStream* boundStream = &streams[i]->BoundStreamL();
                TInt err = out.Write( _L8("Writing bound stream media:\r\n") );
    		    User::LeaveIfError( err );
    		    
    		    MDesC8Array* sdpmediabound = boundStream->MediaAttributeLinesL();
	    	    User::LeaveIfNull( sdpmediabound );
    		    
    		    for( int i = 0; i < sdpmediabound->MdcaCount(); i++ )
        		    {
        		    TInt err = out.Write( sdpmediabound->MdcaPoint(i) );
        		    User::LeaveIfError( err );
        		    }
        		delete sdpmediabound;
                }
		    }
		out.Close();
		}
	fs.Close();
	iEngine.EngineStateChangedL();    
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdWriteSdpToFile::Caption() const
	{
	return KCommandCaptionWriteSdpToFile;
	}




// End of File
