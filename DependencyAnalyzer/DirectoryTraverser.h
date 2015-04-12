#pragma once

#include <vector>
#include <string>

namespace analyzer {
	class DirectoryTraverser final
	{
	public:
		using Path = std::wstring;

		using PathList = std::vector<Path>;

		DirectoryTraverser();

		DirectoryTraverser(DirectoryTraverser const&) = delete;

		auto operator =(DirectoryTraverser const&) -> DirectoryTraverser& = delete;

		auto findSourceFiles(Path const& searchPath) const -> PathList;
	private:
	};
}