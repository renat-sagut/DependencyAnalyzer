#pragma once

#include <vector>
#include <string>

namespace analyzer {
	class FileParser final
	{
	public:
		using DependencyList = std::vector<std::string>;

		FileParser();

		FileParser(FileParser const&) = delete;

		auto operator =(FileParser const&) -> FileParser& = delete;

		auto getDependencies() const -> DependencyList;
	private:

	};
}