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

			auto toString() const -> std::wstring;

			auto toStringRelative() const -> std::wstring;

			auto valid() const -> bool;

			Path directory;
			RelativePath relative;
		};

		using Edge = std::pair<Vertex, Vertex>;

		using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, Vertex>;

		using VertexDescriptor = boost::graph_traits<Graph>::vertex_descriptor;

		using EdgeDescriptor = boost::graph_traits<Graph>::edge_descriptor;

		using VertexDescriptors = std::vector<VertexDescriptor>;

		using SourceVertexDescriptors = VertexDescriptors;

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

		class CycleDetector : public boost::default_dfs_visitor
		{
		public:
			CycleDetector(VertexDescriptors& descriptors);

			template <typename TEdge, typename TGraph>
			auto back_edge(TEdge e, TGraph& g) -> void;

		private:
			VertexDescriptors& descriptors;
		};

		DependencyAnalyzer();

		DependencyAnalyzer(DependencyAnalyzer const&) = delete;

		auto operator =(DependencyAnalyzer const&) -> DependencyAnalyzer& = delete;

		auto update(Path const& sourceDirectory, PathList const& includeDirectories) -> void;

		auto printDependencyTree(std::wostream& out) -> void;

		auto printIncludeFrequencies(std::wostream& out) -> void;

	private:
		Path sourceDirectory;
		PathList includeDirectories;
		Graph graph;
		SourceVertexDescriptors sourceDescriptors;
	};
}