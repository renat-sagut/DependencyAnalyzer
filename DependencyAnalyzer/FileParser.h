#pragma once

#include "CommonTypes.h"

#include <vector>
#include <string>

namespace analyzer {
	class FileParser final
	{
	public:
		enum class IncludeType
		{
			BRACKETS,
			QUOTES
		};

		FileParser();

		FileParser(FileParser const&) = delete;

		auto operator =(FileParser const&) -> FileParser& = delete;

		auto readFile(Path const& path) const -> File;

		auto parseIncludes(File const& file, RelativePathList& bracketIncludes, RelativePathList& quoteIncludes) const -> void;

		auto findIncludeStrings(File const& file, IncludeType const includeType) const -> IncludeStringList;

		auto extractRelativePath(IncludeString const& includeString) const -> RelativePath;

		auto removeComments(File& file) const -> void;

		auto parseIncludes(File const& file, IncludeType const includeType, RelativePathList& includeList) const -> void;

	private:

	};
}