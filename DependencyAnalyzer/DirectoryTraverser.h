#pragma once

#include <vector>
#include <string>

namespace analyzer {
	class DirectoryTraverser final
	{
	public:
		using Path = std::wstring;

		using RelativePath = Path;

		using PathList = std::vector<Path>;

		DirectoryTraverser();

		DirectoryTraverser(DirectoryTraverser const&) = delete;

		auto operator =(DirectoryTraverser const&) -> DirectoryTraverser& = delete;

		auto findSourceFiles(Path const& searchPath) const -> PathList;

		auto findFileRelative(Path const& path, Path const& relative) const -> Path;

		auto findFileInFolders(PathList const& pathList, Path const& relative) const -> Path;

		auto fileExists(Path const& path) const -> bool;

		auto findDirectoryWithFile(PathList const& dirs, RelativePath const& relative) const -> Path;

		auto findRelativePath(RelativePath const& parent, RelativePath const& child) const -> RelativePath;
	private:
	};
}