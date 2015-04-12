#include "DependencyGraph.h"

namespace analyzer {


	auto DependencyGraph::Vertex::toString() const -> std::wstring
	{
		return std::move(this->directory + this->relative);
	}

	auto DependencyGraph::Vertex::valid() const -> bool
	{
		return this->directory.length() > 0;
	}

	DependencyGraph::DependencyGraph()
	{

	}

	auto DependencyGraph::reset() -> void
	{

	}

	auto DependencyGraph::update() -> void
	{

	}

	auto DependencyGraph::addEdge(Vertex const& parent, Vertex const& child) -> void
	{

	}

}