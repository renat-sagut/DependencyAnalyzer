#pragma once

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

		using IncludeList = std::vector<std::string>;

		using StringList = std::vector<std::string>;

		FileParser();

		FileParser(FileParser const&) = delete;

		auto operator =(FileParser const&) -> FileParser& = delete;

		auto parseIncludes(std::string const& file, IncludeList& bracketIncludes, IncludeList& quoteIncludes) const -> void;

		auto findIncludeStrings(std::string const& inputString, IncludeType const includeType) const -> StringList;

		auto extractFilePath(std::string const& includeString) const -> std::string;

		auto removeComments(std::string& file) const -> void;

		auto parseIncludes(std::string const& file, IncludeType const includeType, IncludeList& includeList) const -> void;

	private:

	};
}