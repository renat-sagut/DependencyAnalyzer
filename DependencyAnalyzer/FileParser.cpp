#include "FileParser.h"

#include <vector>
#include <string>
#include <regex>
#include <assert.h>
#include <sstream>

namespace analyzer {

	FileParser::FileParser() = default;

	auto FileParser::parseIncludes(std::string const& file) const -> IncludeList
	{
		std::string parseFile = file;
		removeComments(parseFile);

		IncludeList includeList;

		parseIncludes(parseFile, IncludeType::BRACKETS, includeList);
		parseIncludes(parseFile, IncludeType::QUOTES, includeList);

		return std::move(includeList);
	}

	auto FileParser::parseIncludes(std::string const& file, IncludeType const includeType, IncludeList& includeList) const -> void
	{
		auto includeStrings = std::move(findIncludeStrings(file, includeType));
		for (auto const& includeString : includeStrings)
		{
			auto filePath = std::move(extractFilePath(includeString));
			includeList.push_back({std::move(filePath), includeType});
		}
	}

	auto FileParser::findIncludeStrings(std::string const& inputString, IncludeType const includeType) const -> StringList
	{
		std::string const matchIncludesWithBrackets(R"(#include[[:space:]]*<.*>)");
		std::string const matchIncludesWithQuotes(R"(#include[[:space:]]*".*")");

		std::regex regular;

		switch (includeType)
		{
			case IncludeType::BRACKETS:
			{
				regular = std::regex(matchIncludesWithBrackets);
			}
			break;
			case IncludeType::QUOTES:
			{
				regular = std::regex(matchIncludesWithQuotes);
			}
			break;
			default:
				assert(false);
		}

		std::smatch matchResult;
		std::string searchString = inputString;
		StringList result;
		for (;;)
		{
			if (!std::regex_search(searchString, matchResult, regular))
				break;

			result.push_back(matchResult[0]);
			searchString = matchResult.suffix().str();
		}

		return std::move(result);

	}

	auto FileParser::extractFilePath(std::string const& includeString) const -> std::string
	{
		std::string const matchFilePath(R"(\w\S*\.h)");

		std::smatch matchResult;
		std::regex regular(matchFilePath);
		if (!std::regex_search(includeString, matchResult, regular))
			return {};

		return std::move(matchResult[0]);
	}

	auto FileParser::removeComments(std::string& file) const -> void
	{
		std::string const matchComments(R"(\/\/.*)");
		std::string const matchCommentsMultiLine(R"(\/\*(.*\n)*?\*\/)");

		std::stringstream ss;
		std::smatch matchResult;
		std::string searchString = file;
		std::regex regular(matchComments);
		StringList result;
		for (;;)
		{
			if (!std::regex_search(searchString, matchResult, regular))
				break;

			ss << matchResult.prefix();
			searchString = matchResult.suffix().str();
		}

		searchString = std::move(ss.str());
		ss.clear();
		regular = std::regex(matchCommentsMultiLine);
		for (;;)
		{
			if (!std::regex_search(searchString, matchResult, regular))
				break;

			ss << matchResult.prefix();
			searchString = matchResult.suffix().str();
		}

		file = std::move(ss.str());
	}

}