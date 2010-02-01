#
# Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

use File::Copy;

# Form parameters
#

my $epocroot = $ENV{EPOCROOT};
my $srcfile = "dummy_bldvariant.hrh";
my $dstpath = $epocroot . "epoc32\\include\\oem";
my $dstfile = $dstpath . "\\bldvariant.hrh";

# Sanity checking
#
if (!-e $srcfile)
	{
	print STDERR "$srcfile does not exist\n";
	exit 1;
	}

# Perform copy if $dstfile does not exist
#
if (!-e $dstfile )
	{
	# Create destination directory if it does not exist
	#
	if( !-d $dstpath )
		{
		mkdir( $dstpath, 077 );
		}
	# Copy file
	#
	if (!copy ($srcfile, $dstfile))
		{
		print STDERR "Failed to copy file $dstfile\n";
		unlink $dstfile;
		exit 1;
		}
	}
	
# Done
#
exit 0;