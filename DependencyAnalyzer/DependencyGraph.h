#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

using boost::adjacency_list;
using boost::vecS;
using boost::bidirectionalS;
using boost::graph_traits;

namespace analyzer {

	class DependencyGraph final
	{
	public:
		using Path = std::wstring;

		using RelativePath = Path;

		class Vertex final
		{
		public:
			auto toString() const -> std::wstring;

			auto valid() const -> bool;

			Path directory;
			RelativePath relative;
		};

		using Graph = adjacency_list<vecS, vecS, bidirectionalS>;

		//using Vertex = graph_traits<Graph>::vertex_descriptor;

		DependencyGraph();

		DependencyGraph(DependencyGraph const&) = delete;

		auto operator =(DependencyGraph const&) -> DependencyGraph& = delete;

		auto reset() -> void;

		auto update() -> void;

		auto addEdge(Vertex const& parent, Vertex const& child) -> void;
	private:
		Graph graph;

	};

}