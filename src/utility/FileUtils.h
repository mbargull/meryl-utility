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

#ifndef FILEUTILS_H
#define FILEUTILS_H

static const char* rcsid_FILEUTILS_H = "$Id: FileUtils.h,v 1.4 2011-08-30 23:09:51 mkotelbajcvi Exp $";

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "ArgumentException.h"
#include "StringUtils.h"
#include "VarUtils.h"

#define NEWLINE '\n'
#define PATH_DELIMITER '/'

class FileUtils
{
public:
	static char* readLine(FILE* file, char* buffer, size_t bufferSize, bool includeNewline = false);
	
	// TODO: reimplement
	/*
	static const char* getPath(size_t num, ...);
	static const char* getPath(size_t num, const char** pathParts);
	*/
};

#endif
