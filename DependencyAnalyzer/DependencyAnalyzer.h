#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace analyzer {

	class DependencyAnalyzer final
	{
	public:
		using File = std::string;

		using Path = std::wstring;

		using PathList = std::vector<Path>;

		DependencyAnalyzer();

		DependencyAnalyzer(DependencyAnalyzer const&) = delete;

		auto operator =(DependencyAnalyzer const&) -> DependencyAnalyzer& = delete;

		auto update(Path const& source, PathList const& include) -> void;

		auto printDependencyTree(std::ostream& out) -> void;

		auto printIncludeFrequencies(std::ostream& out) -> void;

	private:
		Path sourceDirectory;
		PathList includeDirectories;
	};
}