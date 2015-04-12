#include "DependencyAnalyzer.h"

#include "DirectoryTraverser.h"
using analyzer::DirectoryTraverser;
#include "FileParser.h"
using analyzer::FileParser;

namespace analyzer {

	DependencyAnalyzer::DependencyAnalyzer() = default;

	auto DependencyAnalyzer::printDependencyTree(std::ostream& out) -> void
	{

	}

	auto DependencyAnalyzer::printIncludeFrequencies(std::ostream& out) -> void
	{

	}

	auto DependencyAnalyzer::update(Path const& source, PathList const& include) -> void
	{
		this->sourceDirectory = source;
		this->includeDirectories = include;

		FileParser parser;
		DirectoryTraverser traverser;
		auto const sourceFilePathList = std::move(traverser.findSourceFiles(this->sourceDirectory));
		for (auto const& relative : sourceFilePathList)
		{
			Path path = std::move(source + relative);
			File file = std::move(parser.readFile(path));
			FileParser::IncludeList quoteIncludes, bracketIncludes;
			parser.parseIncludes(file, bracketIncludes, quoteIncludes);
		}

	}

}