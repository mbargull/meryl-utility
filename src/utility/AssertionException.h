/**************************************************************************
 * This file is part of Celera Assembler, a software program that
 * assembles whole-genome shotgun reads into contigs and scaffolds.
 * Copyright (C) 2005, J. Craig Venter Institute. All rights reserved.
 * Author: Brian Walenz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received (LICENSE.txt) a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *************************************************************************/

#ifndef ASSERTIONEXCEPTION_H
#define ASSERTIONEXCEPTION_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string>

using namespace std;

#include "AssertionType.h"
#include "ExceptionUtils.h"
#include "RuntimeException.h"
#include "StringUtils.h"

static const char* RCSID_ASSERTIONEXCEPTION_H = "$Id: AssertionException.h,v 1.3 2011-08-04 14:34:41 mkotelbajcvi Exp $";

#define UNKNOWN_LOCATION "<unknown>"

class AssertionException : public RuntimeException
{
public:
	AssertionException(AssertionType type, const char* message = NULL, const char* file = NULL, int line = 0, const char* function = NULL) throw();
	
	const char* what() const throw();

protected:
	AssertionType type;
	char* file;
	int line;
	char* function;
	
	static const char* assertionTypeToString(AssertionType type);
};

#endif
