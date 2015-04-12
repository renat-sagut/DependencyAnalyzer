#include "DependencyAnalyzer.h"

#include "DirectoryTraverser.h"
using analyzer::DirectoryTraverser;
#include "FileParser.h"
using analyzer::FileParser;

#include <queue>

namespace analyzer {

	DependencyAnalyzer::DependencyAnalyzer() = default;

	auto DependencyAnalyzer::printDependencyTree(std::ostream& out) -> void
	{

	}

	auto DependencyAnalyzer::printIncludeFrequencies(std::ostream& out) -> void
	{

	}

	auto DependencyAnalyzer::update(Path const& sourceFolder, PathList const& includeFolders) -> void
	{
		this->sourceDirectory = sourceFolder;
		this->includeDirectories = includeFolders;
		this->graph.reset();

		FileParser parser;
		DirectoryTraverser traverser;
		auto const sourceList = std::move(traverser.findSourceFiles(this->sourceDirectory));

		std::queue<DependencyGraph::Vertex> nodesToVisit;
		for (auto const& source : sourceList)
		{
			nodesToVisit.push({this->sourceDirectory, std::wstring(source.begin(), source.end())});
		}

		while (!nodesToVisit.empty())
		{
			auto currentNode = std::move(nodesToVisit.front());
			nodesToVisit.pop();

			auto path = std::move(currentNode.toString());
			auto file = std::move(parser.readFile(path));

			RelativePathList quoteIncludes, bracketIncludes;
			parser.parseIncludes(file, bracketIncludes, quoteIncludes);

			for (auto const& relative : bracketIncludes)
			{
				auto dir = std::move(traverser.findDirectoryWithFile(this->includeDirectories, relative));
				
				DependencyGraph::Vertex child{dir, relative};
				this->graph.addEdge(currentNode, child);
				if (child.valid())
					nodesToVisit.push(std::move(child));
			}

			for (auto const& relativeToParent : quoteIncludes)
			{
				auto relative = std::move(traverser.findRelativePath(currentNode.relative, relativeToParent));
				auto directory = currentNode.directory;
				if (!traverser.fileExists(std::move(directory + relative)))
					directory = Path({});

				DependencyGraph::Vertex child{directory, relative};
				this->graph.addEdge(currentNode, child);
				if (child.valid())
					nodesToVisit.push(std::move(child));
			}

		}

	}

}