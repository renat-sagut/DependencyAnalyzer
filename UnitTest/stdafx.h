// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// TODO: reference additional headers your program requires here
#include "FileParser.h"
using analyzer::FileParser;
#include "DirectoryTraverser.h"
using analyzer::DirectoryTraverser;

#include "CommonTypes.h"
using analyzer::File;
using analyzer::IncludeString;
using analyzer::IncludeStringList;
using analyzer::Path;
using analyzer::PathList;
using analyzer::RelativePath;
using analyzer::RelativePathList;

#include <string>