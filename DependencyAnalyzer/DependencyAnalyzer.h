#pragma once

#include "CommonTypes.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

namespace analyzer {

	class DependencyAnalyzer final
	{
	public:
		class Vertex final
		{
		public:
			auto operator =(Vertex const& v) -> Vertex&;

			auto operator ==(Vertex const& v) const -> bool;

			auto operator <(Vertex const& v) const -> bool;

			auto toString() const->std::wstring;

			auto valid() const -> bool;

			Path directory;
			RelativePath relative;
		};

		class PrintTree : public boost::default_dfs_visitor
		{
		public:
			PrintTree(std::wostream& wout);

			template <typename TVertex, typename TGraph>
			auto discover_vertex(TVertex u, TGraph& g) -> void;

			template <typename TVertex, typename TGraph>
			auto finish_vertex(TVertex u, TGraph& g) -> void;

		private:
			std::wostream& wout;
			int level = -1;
		};

		using Edge = std::pair<Vertex, Vertex>;

		using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex>;

		using VertexDescriptor = boost::graph_traits<Graph>::vertex_descriptor;

		DependencyAnalyzer();

		DependencyAnalyzer(DependencyAnalyzer const&) = delete;

		auto operator =(DependencyAnalyzer const&) -> DependencyAnalyzer& = delete;

		auto update(Path const& sourceFolder, PathList const& includeFolders) -> void;

		auto printDependencyTree(std::wostream& out) -> void;

		auto printIncludeFrequencies(std::wostream& out) -> void;

	private:
		Path sourceDirectory;
		PathList includeDirectories;
		Graph graph;
	};
}