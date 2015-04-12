#include "FileParser.h"

#include <vector>
#include <string>
#include <regex>
#include <assert.h>
#include <sstream>

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;
#include <boost/iostreams/device/mapped_file.hpp>
namespace io = boost::iostreams;

namespace analyzer {

	FileParser::FileParser() = default;

	auto FileParser::readFile(Path const& path) const -> File
	{
		if (!fs::exists(path))
			return {};

		if (!fs::is_regular_file(path))
			return {};

		fs::wpath p(path);
		io::mapped_file file(p);
		std::stringstream ss;
		ss << file.data();

		return std::move(ss.str());
	}

	auto FileParser::parseIncludes(File const& file, IncludeList& bracketIncludes, IncludeList& quoteIncludes) const -> void
	{
		std::string parseFile = file;
		removeComments(parseFile);

		parseIncludes(parseFile, IncludeType::BRACKETS, bracketIncludes);
		parseIncludes(parseFile, IncludeType::QUOTES, quoteIncludes);
	}

	auto FileParser::parseIncludes(std::string const& file, IncludeType const includeType, IncludeList& includeList) const -> void
	{
		auto includeStrings = std::move(findIncludeStrings(file, includeType));
		for (auto const& includeString : includeStrings)
		{
			auto filePath = std::move(extractFilePath(includeString));
			includeList.push_back(std::move(filePath));
		}
	}

	auto FileParser::findIncludeStrings(std::string const& inputString, IncludeType const includeType) const -> StringList
	{
		std::string const matchIncludesWithBrackets(R"(#include[[:space:]]*<.*?>)");
		std::string const matchIncludesWithQuotes(R"(#include[[:space:]]*".*?")");

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
		std::string matchComments(R"((/\*(.|[\r\n])*?\*/)|(//.*))");
		file = std::regex_replace(file, std::regex(matchComments), "");
	}

}