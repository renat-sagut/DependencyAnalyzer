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

		using IncludeData = std::pair<std::string, IncludeType>;

		using IncludeList = std::vector<IncludeData>;

		using StringList = std::vector<std::string>;

		FileParser();

		FileParser(FileParser const&) = delete;

		auto operator =(FileParser const&) -> FileParser& = delete;

		auto parseIncludes(std::string const& file) const -> IncludeList;

		auto findIncludeStrings(std::string const& inputString, IncludeType const includeType) const -> StringList;

		auto extractFilePath(std::string const& includeString) const -> std::string;

		auto removeComments(std::string& file) const -> void;

		auto parseIncludes(std::string const& file, IncludeType const includeType, IncludeList& includeList) const -> void;

	private:

	};
}