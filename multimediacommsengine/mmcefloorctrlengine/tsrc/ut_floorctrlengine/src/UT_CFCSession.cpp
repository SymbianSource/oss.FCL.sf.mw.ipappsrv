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




//  CLASS HEADER
#include "ut_cfcsession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <SDPDocument.h>
#include <SDPFmtAttributefield.h>
#include <SDPMediaField.h>

//  INTERNAL INCLUDES
#include "fcsession.h"
#include "FCInterface.h"
#include <e32base.h>


const TUint KTBCPLocalPort = 2000;

_LIT8(KTBCP, "TestPlugIn");
_LIT8(KSpace, " ");

_LIT8(Kfmt, "queuing=1; tb_priority=2; timestamp=1");
_LIT8(KHello, "Hello");
_LIT8(KSDPMessage, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp wb\r\n\
a=orient:portrait\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");



_LIT8(KFmtAttribute,"a=fmtp:TBCP format parameters\r\n");

_LIT8(KFmtAttribute2,"a=fmtp:TestPlugIn format parameters\r\n");

_LIT8( KInvalid4, "a=fmtp:format \r\n" );

_LIT8(KSDPMessage2, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp TestPlugIn\r\n\
a=fmtp:TestPlugIn format parameters\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");

// No media line
_LIT8(KSDPMessageNoMedia, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");

// media line, but no fmt attribute
_LIT8(KSDPMessageNoFmtAttr, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp TestPlugIn\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");


// CONSTRUCTION
UT_CFCSession* UT_CFCSession::NewL()
    {
    UT_CFCSession* self = UT_CFCSession::NewLC(); 
    CleanupStack::Pop( self );

    return self;
    }

UT_CFCSession* UT_CFCSession::NewLC()
    {
    UT_CFCSession* self = new( ELeave ) UT_CFCSession();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CFCSession::~UT_CFCSession()
    {
    }

// Default constructor
UT_CFCSession::UT_CFCSession()
    {
    }

// Second phase construct
void UT_CFCSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCSession::SetupL(  )
    {
    iTestPlugInEngine = CTestPlugInEngine::NewL(*this);
    CFCInterface* plugin  = ( CFCInterface* ) iTestPlugInEngine->PlugInByNameL(KTBCP);
    CleanupStack::PushL( plugin );
	iFCSession = CFCSession::NewL( plugin, 0, KFCDefaultPort );
	CleanupStack::Pop( plugin );
    SdpCodecStringPool::OpenL();   
	iErr=0;
	iData = NULL;
    } 

void UT_CFCSession::Teardown(  )
    {
    delete iFCSession;
    delete iTestPlugInEngine;
    delete iData;
    iData = NULL;
    SdpCodecStringPool::Close();
    }

void UT_CFCSession::UT_CFCSession_EmptyTestL(  )
    {
    // first test fails for some reason
    
    }

void UT_CFCSession::UT_CFCSession_DestructorL(  )
    {
    // for coverage, delete fmtp attributes
    
    CFCInterface* plugin  = ( CFCInterface* ) iTestPlugInEngine->PlugInByNameL(KTBCP);
    CleanupStack::PushL( plugin );
        
    CFCSession* session = CFCSession::NewL( plugin, 0, KFCDefaultPort );
    CleanupStack::Pop( plugin );

    delete session->iSdpFmtAttributeFields;
    session->iSdpFmtAttributeFields = NULL;
    delete session;

    }

    
void UT_CFCSession::UT_CFCSession_NewLL(  )
    {
    EUNIT_ASSERT( iFCSession->NegotiatedFCPlugIn()==KTBCP );
    
    }

void UT_CFCSession::UT_CFCSession_NewLCL(  )
    {
    CFCInterface* plugin  = ( CFCInterface* ) iTestPlugInEngine->PlugInByNameL(KTBCP);
    CleanupStack::PushL( plugin );
    CFCSession* temp= CFCSession::NewL( plugin, 0, KFCDefaultPort );
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( temp );
    EUNIT_ASSERT( temp->NegotiatedFCPlugIn()== iFCSession->NegotiatedFCPlugIn() );
    CleanupStack::PopAndDestroy( temp );
    
    
    
    }
    
void UT_CFCSession::UT_CFCSession_PortL(  )
    {
    
    iFCSession->SetPort( KTBCPLocalPort );
	
	HBufC8* aSdptest = KSDPMessage().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*aSdptest);
	CleanupStack::PushL(sdpDocument);
	
    RPointerArray<CSdpFmtAttributeField>* atts = 
        new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	User::LeaveIfError(atts->Append(fmtatt));
	CleanupStack::Pop(fmtatt);//fmtatt
	
	iFCSession->SetFormatAttributeFieldsL(atts);
	CleanupStack::Pop(atts);//atts
	
	
	iFCSession->UpdateSDPL(*sdpDocument);
	
	EUNIT_ASSERT(iFCSession->Port()==KTBCPLocalPort);
   	EUNIT_ASSERT(iErr==KTBCPLocalPort);
   	   	
	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
	
   	CleanupStack::PopAndDestroy(aSdptest);//aSdptest
    }

void UT_CFCSession::UT_CFCSession_IsReceiverL()
    {
    iFCSession->Update( KFCDefaultPort );
    EUNIT_ASSERT( !iFCSession->IsReceiver() );
    iFCSession->Update( KTBCPLocalPort );
    EUNIT_ASSERT( iFCSession->IsReceiver() );
    }

void UT_CFCSession::UT_CFCSession_NegotiatedFCPlugInL(  )
    {
    EUNIT_ASSERT(iFCSession->NegotiatedFCPlugIn()==KTBCP);
   	}

void UT_CFCSession::UT_CFCSession_FormatAttributeFieldsLL(  )
    {	
	RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	iFCSession->SetFormatAttributeFieldsL(atts);
	RPointerArray<CSdpFmtAttributeField>& temparray=iFCSession->FormatAttributeFields();

	TInt temp;
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	if (fmtatt == temparray[ aCount ])
    	{
    		temp=aCount;
    	}
    	
    	}
	EUNIT_ASSERT(temparray[temp]==fmtatt);
	CleanupStack::PopAndDestroy(fmtatt);//fmtatt
	temparray.Close();
	CleanupStack::Pop(atts);//atts
    }

void UT_CFCSession::UT_CFCSession_SetFormatAttributeFieldsL(  )
    {
    // For coverage, with NULL,
    
    iFCSession->SetFormatAttributeFieldsL( NULL );
        
    // Then empty 
    RPointerArray<CSdpFmtAttributeField>* firstatts = 
        new( ELeave ) RPointerArray<CSdpFmtAttributeField>(1);
        
    CleanupStack::PushL( TCleanupItem( CFCSession::ArrayCleanup, firstatts ) );    
    iFCSession->SetFormatAttributeFieldsL( firstatts);
    CleanupStack::Pop();
        
    // Not empty then    
    RPointerArray<CSdpFmtAttributeField>* atts = 
    	new( ELeave ) RPointerArray<CSdpFmtAttributeField>(1);; 
    CleanupStack::PushL(atts);
    
    CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
    atts->AppendL( fmtatt );
    CleanupStack::Pop( fmtatt );
    
    CleanupStack::Pop(); // atts        
	iFCSession->SetFormatAttributeFieldsL( atts );
	
	RPointerArray<CSdpFmtAttributeField>& temparray=iFCSession->FormatAttributeFields();
	
	TInt temp;
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	if (fmtatt == temparray[ aCount ])
    	{
    		temp=aCount;
    	}
    	
    	}
    	
	EUNIT_ASSERT(temparray[temp]==fmtatt);

	//temparray.Close();
	}

void UT_CFCSession::UT_CFCSession_UpdateSDPL2L(  )
    {
    // update answer
    HBufC8* aSdptest = KSDPMessage2().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*aSdptest);
	CleanupStack::PushL(sdpDocument);
	iFCSession->iUpdateNeeded = ETrue;
	iFCSession->UpdateSDPL(*sdpDocument);
	
	RPointerArray<CSdpMediaField>&  temparray =sdpDocument->MediaFields();
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	CSdpMediaField* mediaAnnouncement = temparray[ aCount ];
       	const TDesC8& aPlugInType= mediaAnnouncement->FormatList();
			
		if(KTBCP().Compare(aPlugInType)==0)
            {
    		EUNIT_ASSERT(aPlugInType==KTBCP);
    		}
    	}
    

	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
	
   	CleanupStack::PopAndDestroy(aSdptest);//aSdptest
   	}

void UT_CFCSession::UT_CFCSession_StoreInfoLL(  )
    {
    HBufC8* sdpTest = KSDPMessage2().AllocLC();
    CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*sdpTest);
	CleanupStack::PushL(sdpDocument);
	
	iFCSession->StoreInfoL(*sdpDocument);
	
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute2);
	RPointerArray<CSdpFmtAttributeField>& temparray=iFCSession->FormatAttributeFields();
	
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	CSdpFmtAttributeField* tempatt=temparray[ aCount ];
	    	if (fmtatt == temparray[ aCount ])
		    	{
		    	EUNIT_ASSERT(temparray[aCount]==fmtatt);
		    	}
    	}
	CleanupStack::PopAndDestroy(fmtatt);//fmtatt
	
	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
	
   	CleanupStack::PopAndDestroy(sdpTest);//sdpTest
   	}


void UT_CFCSession::UT_CFCSession_StoreInfoL2L(  )
    {
    // With no FC media line
    EUNIT_ASSERT( iFCSession->iSdpFmtAttributeFields->Count() == 0 );
   	HBufC8* sdpTest = KSDPMessageNoMedia().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL( *sdpTest );
	CleanupStack::PopAndDestroy( sdpTest );
	CleanupStack::PushL( sdpDocument );
	
	iFCSession->StoreInfoL( *sdpDocument );
	CleanupStack::PopAndDestroy( sdpDocument );
	EUNIT_ASSERT( iFCSession->iSdpFmtAttributeFields->Count() == 0 );
	
	// With no Fmt attribute
	HBufC8* sdpTest2 = KSDPMessageNoFmtAttr().AllocLC();
	CSdpDocument* sdpDocument2 = CSdpDocument::DecodeL( *sdpTest2 );
	CleanupStack::PopAndDestroy( sdpTest2 );
	CleanupStack::PushL( sdpDocument2 );
	
	iFCSession->StoreInfoL( *sdpDocument2 );
	CleanupStack::PopAndDestroy( sdpDocument2 );
	EUNIT_ASSERT( iFCSession->iSdpFmtAttributeFields->Count() == 0 );
	}
    

void UT_CFCSession::UT_CFCSession_DecodeSDPMediaPortL(  )
    {
    // Success
	HBufC8* sdpTest = KSDPMessage2().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*sdpTest);
	CleanupStack::PopAndDestroy(sdpTest);//sdpTest
	TInt port= iFCSession->DecodeSDPMediaPort( sdpDocument->MediaFields() );
	delete sdpDocument;
    EUNIT_ASSERT(port==32416);
    
	// media field not found
	HBufC8* sdpTest2 = KSDPMessageNoMedia().AllocLC();
	CSdpDocument* sdpDocument2 = CSdpDocument::DecodeL(*sdpTest2);
	CleanupStack::PopAndDestroy( sdpTest2 );
	TInt port2 = iFCSession->DecodeSDPMediaPort( sdpDocument2->MediaFields() );
	delete sdpDocument2;
    EUNIT_ASSERT(port2==0);
    }

void UT_CFCSession::UT_CFCSession_DecodeConnectionAddrL(  )
    {
    // failing
    EUNIT_ASSERT ( iFCSession->DecodeConnectionAddrL( NULL ) == NULL );

    // success
	HBufC8* aSdptest = KSDPMessage2().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*aSdptest);
	CleanupStack::PopAndDestroy(aSdptest);//aSdptest
	CleanupStack::PushL(sdpDocument);
    HBufC* temp = iFCSession->DecodeConnectionAddrL( sdpDocument->ConnectionField() );
    CleanupStack::PushL( temp );

    
    EUNIT_ASSERT(*temp==_L("224.2.17.12"));
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy(sdpDocument);
	
	}

void UT_CFCSession::UT_CFCSession_UpdateSDPLL(  )
    {
    // update offer
    iFCSession->SetPort( KTBCPLocalPort );
	
	HBufC8* aSdptest = KSDPMessage().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*aSdptest);
	CleanupStack::PushL(sdpDocument);


	RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	
	iFCSession->SetFormatAttributeFieldsL(atts);
	CleanupStack::Pop(fmtatt);//fmtatt
	CleanupStack::Pop(atts);//atts

	
	iFCSession->UpdateSDPL(*sdpDocument);

   	EUNIT_ASSERT(iFCSession->Port()==KTBCPLocalPort);
   	EUNIT_ASSERT(iErr==KTBCPLocalPort);
   
   	
   	RPointerArray<CSdpMediaField>& mediaDescriptions = sdpDocument->MediaFields();
      	
    EUNIT_ASSERT( mediaDescriptions.Count() == 4 )
    CSdpMediaField* mediaAnnouncement = NULL;
    TInt attrCount = 0;
    for (TInt i=0; !mediaAnnouncement && i < mediaDescriptions.Count(); i++)
    	{
    	mediaAnnouncement = mediaDescriptions[ i ];
    	if ( mediaAnnouncement->FormatList().Compare( KTBCP ) == 0 )
    	    {
    	    attrCount = mediaAnnouncement->FormatAttributeFields().Count();
    	    }
    	else
    	    {
    	    mediaAnnouncement = NULL;
    	    }
    	}
    EUNIT_ASSERT( mediaAnnouncement );
    EUNIT_ASSERT( attrCount == 1 );
    
    CleanupStack::PopAndDestroy(sdpDocument);
	CleanupStack::PopAndDestroy(aSdptest);//aSdptest
   	}

void UT_CFCSession::UT_CFCSession_ReleaseSessionL(  )
    {
    iFCSession->ReleaseSession();
    EUNIT_ASSERT(iErr==3);
    }

void UT_CFCSession::UT_CFCSession_ConnectionReadyLL(  )
    {
    iFCSession->iIsReceiver = EFalse;
    iFCSession->ConnectionReadyL();
    
    EUNIT_ASSERT(iErr==4);
    }

void UT_CFCSession::UT_CFCSession_SendToNetLL(  )
    {
    HBufC8* temp=KHello().AllocLC();
    iFCSession->SendToNetL(temp);
    
    EUNIT_ASSERT(iData==temp);
    CleanupStack::Pop(temp);
    }
    
void UT_CFCSession::UT_CFCSession_IsFmtAttributeValidL()
	{
    RPointerArray<CSdpFmtAttributeField>* atts = 
        new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	CleanupStack::Pop(fmtatt);//fmtatt
	iFCSession->SetFormatAttributeFieldsL(atts);
	EUNIT_ASSERT(iFCSession->IsFmtAttributeValid()==true)
	EUNIT_PRINT(_L( "test is fmtAttributeValid"));
	
	atts->ResetAndDestroy();
	atts->Close();
	CleanupStack::Pop(atts);//atts
	}
	
void UT_CFCSession::UT_CFCSession_IsFmtAttributeValid2L()
	{
	RPointerArray<CSdpFmtAttributeField>* atts = 
	    new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
  
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KInvalid4);
	atts->AppendL(fmtatt);
	
	iFCSession->SetFormatAttributeFieldsL(atts);
	RPointerArray<CSdpFmtAttributeField>& temparray=iFCSession->FormatAttributeFields();

	TInt temp;
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    		if (fmtatt == temparray[ aCount ])
    		{
    			temp=aCount;
    		}
    	}
    EUNIT_ASSERT(iFCSession->IsFmtAttributeValid()==false)
	EUNIT_PRINT(_L( "test is fmtAttributeValid"));
	
	temparray.Close();
	CleanupStack::Pop(fmtatt);//fmtatt
	atts->Close();
	CleanupStack::Pop(atts);//atts
    }

void UT_CFCSession::UT_CFCSession_IsFmtAttributeValid3()
	{
	EUNIT_ASSERT(iFCSession->IsFmtAttributeValid()==false)
	EUNIT_PRINT(_L( "test is fmtAttributeValid"));
    }
		
void UT_CFCSession::UT_CFCSession_ArrayCleanupL(  )
    {
    // First with NULL
    CleanupStack::PushL( TCleanupItem( CFCSession::ArrayCleanup, NULL ) );
    CleanupStack::PopAndDestroy();
    
    // Real test case, empty
	RPointerArray< CSdpFmtAttributeField >  emptyArray;
	CleanupStack::PushL( TCleanupItem( CFCSession::ArrayCleanup, &emptyArray ) );	
    CleanupStack::PopAndDestroy();
    
    	
	// Real test case
	RPointerArray< CSdpFmtAttributeField >  array;
	CleanupStack::PushL( TCleanupItem( CFCSession::ArrayCleanup, &array ) );
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC( KFmtAttribute );
	array.AppendL( fmtatt );
	CleanupStack::Pop( fmtatt );
    CleanupStack::PopAndDestroy();
    }


void UT_CFCSession::ReceivedData(HBufC8* aData, MFCPlugIn* /*aSession*/)
{
	iData=aData;
}

void UT_CFCSession::ErrorNotify(TInt aErrCode, MFCPlugIn* /*aSession*/)
{
	iErr=aErrCode;
}




//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCSession,
    "CFCSession Test",
    "UNIT" )

EUNIT_TEST(
    "Empty test ",
    "CFCSession",
    "None",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_EmptyTestL, Teardown)

EUNIT_TEST(
    "~CFCSession test ",
    "CFCSession",
    "~CFCSession",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_DestructorL, Teardown)
    
EUNIT_TEST(
    "NewL - test ",
    "CFCSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CFCSession",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_NewLCL, Teardown)

EUNIT_TEST(
    "Port - test ",
    "CFCSession",
    "Port",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_PortL, Teardown)

EUNIT_TEST(
    "IsReceiver - test ",
    "CFCSession",
    "IsReceiver",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_IsReceiverL, Teardown)

EUNIT_TEST(
    "NegotiatedFCPlugIn - test ",
    "CFCSession",
    "NegotiatedFCPlugIn",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_NegotiatedFCPlugInL, Teardown)

EUNIT_TEST(
    "FormatAttributeFields - test ",
    "CFCSession",
    "FormatAttributeFields",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_FormatAttributeFieldsLL, Teardown)

EUNIT_TEST(
    "SetFormatAttributeFields - test ",
    "CFCSession",
    "SetFormatAttributeFields",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_SetFormatAttributeFieldsL, Teardown)

EUNIT_TEST(
    "StoreInfoL - test ",
    "CFCSession",
    "StoreInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_StoreInfoLL, Teardown)

EUNIT_TEST(
    "StoreInfoL - test 2",
    "CFCSession",
    "StoreInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_StoreInfoL2L, Teardown)


EUNIT_TEST(
    "DecodeSDPMediaPort - test ",
    "CFCSession",
    "DecodeSDPMediaPort",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_DecodeSDPMediaPortL, Teardown)

EUNIT_TEST(
    "DecodeConnectionAddr - test ",
    "CFCSession",
    "DecodeConnectionAddr",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_DecodeConnectionAddrL, Teardown)

EUNIT_TEST(
    "UpdateSDPL - test ",
    "CFCSession",
    "UpdateSDPL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_UpdateSDPLL, Teardown)

EUNIT_TEST(
    "UpdateSDPL - test 2",
    "CFCSession",
    "UpdateSDPL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_UpdateSDPL2L, Teardown)

EUNIT_TEST(
    "ReleaseSession - test ",
    "CFCSession",
    "ReleaseSession",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_ReleaseSessionL, Teardown)

EUNIT_TEST(
    "ConnectionReadyL - test ",
    "CFCSession",
    "ConnectionReadyL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_ConnectionReadyLL, Teardown)

EUNIT_TEST(
    "SendToNetL - test ",
    "CFCSession",
    "SendToNetL",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_SendToNetLL, Teardown)

EUNIT_TEST(
    "IsFmtAttributeValid - test ",
    "CFCSession",
    "IsFmtAttributeValid",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_IsFmtAttributeValidL, Teardown)

/*EUNIT_TEST(
    "IsFmtAttributeValid2 - test ",
    "CFCSession",
    "IsFmtAttributeValid-fmtAttribute is not valid",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_IsFmtAttributeValid2L, Teardown)*/
    
EUNIT_TEST(
    "IsFmtAttributeValid3 - test ",
    "CFCSession",
    "IsFmtAttributeValid-No fmtAtt parameter",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_IsFmtAttributeValid3, Teardown)

EUNIT_TEST(
    "ArrayCleanup - test ",
    "CFCSession",
    "ArrayCleanup",
    "FUNCTIONALITY",
    SetupL, UT_CFCSession_ArrayCleanupL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
