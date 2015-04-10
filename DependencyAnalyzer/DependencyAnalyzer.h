#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace analyzer {

	class DependencyAnalyzer final
	{
	public:
		using DirectoryPath = std::wstring;

		using DirectoryPathList = std::vector<DirectoryPath>;

		DependencyAnalyzer();

		DependencyAnalyzer(DependencyAnalyzer const&) = delete;

		auto operator =(DependencyAnalyzer const&) -> DependencyAnalyzer& = delete;

		auto update(DirectoryPath const& source, DirectoryPathList const& include) -> void;

		auto printDependencyTree(std::ostream& out) -> void;

		auto printIncludeFrequencies(std::ostream& out) -> void;
	private:
	};
}