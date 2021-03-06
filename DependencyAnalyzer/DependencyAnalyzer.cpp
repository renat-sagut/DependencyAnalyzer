#include "DependencyAnalyzer.h"

#include "DirectoryTraverser.h"
using analyzer::DirectoryTraverser;
#include "FileParser.h"
using analyzer::FileParser;

#include <deque>
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
		this->wout << g[u].relative;
		if (!g[u].valid())
			this->wout << L" (!)";
		this->wout << std::endl;
	}

	template <typename TVertex, typename TGraph>
	auto DependencyAnalyzer::PrintTree::finish_vertex(TVertex u, TGraph& g) -> void
	{
		this->level -= 1;
	}

	DependencyAnalyzer::CycleDetector::CycleDetector(VertexDescriptors& a_descriptors)
		: descriptors{a_descriptors}
	{
		descriptors.clear();
	}

	template <typename TEdge, typename TGraph>
	auto DependencyAnalyzer::CycleDetector::back_edge(TEdge u, TGraph& g) -> void
	{
		auto const s = boost::source(u, g);
		auto const t = boost::target(u, g);

		this->descriptors.push_back(s);
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

	auto DependencyAnalyzer::Vertex::toStringRelative() const -> std::wstring
	{
		if (this->relative.size() > 0)
			return std::move(this->relative.substr(1, this->relative.size() - 1));

		return { L"" };
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
		//PrintTree printTree(out);
		//boost::depth_first_search(this->graph, boost::visitor(printTree));

		VertexDescriptors descriptors;
		CycleDetector cycleDetector(descriptors);
		boost::depth_first_search(this->graph, boost::visitor(cycleDetector));

		if (!descriptors.empty())
		{
			out << L"Cyclic dependencies detected in:\n";
			for (auto const& desc : descriptors)
			{
				out << this->graph[desc].toStringRelative() << std::endl;
			}

			return;
		}

		std::deque<std::pair<VertexDescriptor, int>> nodesToVisit;
		for (auto const& desc : this->sourceDescriptors)
		{
			nodesToVisit.push_back({desc, 0});
		}

		while (!nodesToVisit.empty())
		{
			auto const descLevel = nodesToVisit.front();
			nodesToVisit.pop_front();

			auto const desc = descLevel.first;
			auto const level = descLevel.second;
			for (int i = 0; i < level; ++i)
				out << L"..";
			out << this->graph[desc].toStringRelative();
			if (!this->graph[desc].valid())
				out << L" (!)";
			out << std::endl;

			Graph::adjacency_iterator iter, end;
			boost::tie(iter, end) = boost::adjacent_vertices(desc, this->graph);
			for (; iter != end; ++iter)
			{
				nodesToVisit.push_front({*iter, level + 1});
			}
		}
	}

	auto DependencyAnalyzer::printIncludeFrequencies(std::wostream& out) -> void
	{
		using VertexFrequency = std::pair<VertexDescriptor, int>;
		using VertexFrequencies = std::vector<VertexFrequency>;
		VertexFrequencies frequencies;
		auto frequencyCalculator = [this, &frequencies](VertexDescriptor const& v) -> void {
			frequencies.push_back({v, boost::in_degree(v, this->graph)});
		};
		std::for_each(boost::vertices(this->graph).first, boost::vertices(this->graph).second, frequencyCalculator);
		auto greater = [](VertexFrequency const& a, VertexFrequency const& b) -> bool {
			return a.second > b.second;
		};
		std::sort(frequencies.begin(), frequencies.end(), greater);
		for (auto vertexFrequency : frequencies)
		{
			auto v = vertexFrequency.first;
			auto f = vertexFrequency.second;
			out << this->graph[v].toStringRelative() << L" : " << f;
			out << std::endl;
		}
	}

	auto DependencyAnalyzer::update(Path const& sourceDirectory, PathList const& includeDirectories) -> void
	{
		DirectoryTraverser traverser;
		this->sourceDirectory = std::move(traverser.normalizePath(sourceDirectory));
		this->includeDirectories.clear();
		for (auto const& includeDirectory : includeDirectories)
		{
			this->includeDirectories.push_back(std::move(traverser.normalizePath(includeDirectory)));
		}
		this->graph.clear();
		this->sourceDescriptors.clear();	

		auto const sourceList = std::move(traverser.findSourceFiles(this->sourceDirectory));

		if (sourceList.empty())
			return;

		std::vector<Vertex> sourceVertexList;
		for (auto const& source : sourceList)
		{
			sourceVertexList.push_back({this->sourceDirectory, std::wstring(source.begin(), source.end())});
		}

		std::deque<Vertex> nodesToVisit;
		for (auto const& sourceVertex : sourceVertexList)
		{
			nodesToVisit.push_back(sourceVertex);
		}

		FileParser parser;
		std::map<Edge, int> edges;
		std::map<Vertex, VertexDescriptor> visitedNodes;
		while (!nodesToVisit.empty())
		{
			auto parent = std::move(nodesToVisit.front());
			nodesToVisit.pop_front();
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
				nodesToVisit.push_back(std::move(child));

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
				nodesToVisit.push_back(std::move(child));

				Edge edge{parent, child};
				if (edges.find(edge) == edges.end())
					edges[edge] = 1;

			}

		}

		for (auto const& sourceVertex : sourceVertexList)
		{
			if (visitedNodes.find(sourceVertex) != visitedNodes.end())
				this->sourceDescriptors.push_back(visitedNodes[sourceVertex]);
		}

		for (auto const& edgeKeyValue : edges)
		{
			auto const& edge = edgeKeyValue.first;
			auto const& vertexDescriptorFirst = visitedNodes[edge.first];
			auto const& vertexDescriptorSecond = visitedNodes[edge.second];

			EdgeDescriptor desc;
			bool added;
			boost::tie(desc, added) = boost::add_edge(vertexDescriptorFirst, vertexDescriptorSecond, this->graph);
		}

	}

}