#pragma once

#include <string>
#include <vector>

namespace analyzer {

	using File = std::string;

	using Path = std::wstring;

	using RelativePath = Path;

	using PathList = std::vector<Path>;

	using RelativePathList = PathList;

	using IncludeString = std::string;

	using IncludeStringList = std::vector<IncludeString>;

}