#include "DependencyAnalyzer.h"

#include "DirectoryTraverser.h"
using analyzer::DirectoryTraverser;
#include "FileParser.h"
using analyzer::FileParser;

#include <queue>
#include <map>

namespace analyzer {

	DependencyAnalyzer::PrintTree::PrintTree(std::wostream& a_wout)
		: wout{a_wout}
	{
	}

	template <typename TVertex, typename TGraph>
	auto DependencyAnalyzer::PrintTree::discover_vertex(TVertex u, TGraph& g) -> void
	{
		this->level += 1;
		for (int i = 0; i < level; ++i)
			this->wout << L"..";
		this->wout << g[u].relative << std::endl;
	}

	template <typename TVertex, typename TGraph>
	auto DependencyAnalyzer::PrintTree::finish_vertex(TVertex u, TGraph& g) -> void
	{
		this->level -= 1;
	}

	auto DependencyAnalyzer::Vertex::operator =(Vertex const& v) -> Vertex&
	{
		this->directory = v.directory;
		this->relative = v.relative;

		return *this;
	}

	auto DependencyAnalyzer::Vertex::operator ==(Vertex const& v) const -> bool
	{
		return toString() == v.toString();
	}

	auto DependencyAnalyzer::Vertex::operator <(Vertex const& v) const -> bool
	{
		return toString() < v.toString();
	}

	auto DependencyAnalyzer::Vertex::toString() const -> std::wstring
	{
		return std::move(this->directory + this->relative);
	}

	auto DependencyAnalyzer::Vertex::valid() const -> bool
	{
		if (this->directory.length() == 0)
			return false;

		if (this->relative.length() == 0)
			return false;

		DirectoryTraverser traverser;
		if (!traverser.fileExists(toString()))
			return false;

		return true;
	}

	DependencyAnalyzer::DependencyAnalyzer() = default;

	auto DependencyAnalyzer::printDependencyTree(std::wostream& out) -> void
	{
		PrintTree printTree(out);
		boost::depth_first_search(this->graph, boost::visitor(printTree));
	}

	auto DependencyAnalyzer::printIncludeFrequencies(std::wostream& out) -> void
	{

	}

	auto DependencyAnalyzer::update(Path const& sourceFolder, PathList const& includeFolders) -> void
	{
		this->sourceDirectory = sourceFolder;
		this->includeDirectories = includeFolders;
		this->graph.clear();

		FileParser parser;
		DirectoryTraverser traverser;
		auto const sourceList = std::move(traverser.findSourceFiles(this->sourceDirectory));

		std::queue<Vertex> nodesToVisit;
		for (auto const& source : sourceList)
		{
			nodesToVisit.push({this->sourceDirectory, std::wstring(source.begin(), source.end())});
		}

		std::map<Vertex, VertexDescriptor> visitedNodes;
		std::map<Edge, int> edges;
		while (!nodesToVisit.empty())
		{
			auto parent = std::move(nodesToVisit.front());
			nodesToVisit.pop();
			if (visitedNodes.find(parent) == visitedNodes.end())
			{
				auto parentDescriptor = boost::add_vertex(this->graph);
				this->graph[parentDescriptor] = parent;
				visitedNodes[parent] = parentDescriptor;
			}
			else
				continue;

			if (!parent.valid())
				continue;

			auto path = std::move(parent.toString());
			auto file = std::move(parser.readFile(path));

			RelativePathList quoteIncludes, bracketIncludes;
			parser.parseIncludes(file, bracketIncludes, quoteIncludes);

			for (auto const& relative : bracketIncludes)
			{
				auto dir = std::move(traverser.findDirectoryWithFile(this->includeDirectories, relative));
				
				Vertex child{dir, relative};
				nodesToVisit.push(std::move(child));

				Edge edge{parent, child};
				if (edges.find(edge) == edges.end())
					edges[edge] = 1;
			}

			for (auto const& relativeToParent : quoteIncludes)
			{
				auto relative = std::move(traverser.findRelativePath(parent.relative, relativeToParent));
				auto directory = parent.directory;
				if (!traverser.fileExists(std::move(directory + relative)))
					directory = Path(L"");

				Vertex child{directory, relative};
				nodesToVisit.push(std::move(child));

				Edge edge{parent, child};
				if (edges.find(edge) == edges.end())
					edges[edge] = 1;

			}

		}

		for (auto const& edgeKeyValue : edges)
		{
			auto const& edge = edgeKeyValue.first;
			auto const& vertexDescriptorFirst = visitedNodes[edge.first];
			auto const& vertexDescriptorSecond = visitedNodes[edge.second];
			boost::add_edge(vertexDescriptorFirst, vertexDescriptorSecond, this->graph);
		}

	}

}