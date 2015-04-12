#pragma once

#include "DependencyGraph.h"
using analyzer::DependencyGraph;
#include "CommonTypes.h"

#include <iostream>
#include <string>
#include <vector>

namespace analyzer {

	class DependencyAnalyzer final
	{
	public:
		DependencyAnalyzer();

		DependencyAnalyzer(DependencyAnalyzer const&) = delete;

		auto operator =(DependencyAnalyzer const&) -> DependencyAnalyzer& = delete;

		auto update(Path const& sourceFolder, PathList const& includeFolders) -> void;

		auto printDependencyTree(std::ostream& out) -> void;

		auto printIncludeFrequencies(std::ostream& out) -> void;

	private:
		Path sourceDirectory;
		PathList includeDirectories;
		DependencyGraph graph;
	};
}