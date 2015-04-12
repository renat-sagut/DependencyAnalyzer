#pragma once

#include "CommonTypes.h"

#include <vector>
#include <string>

namespace analyzer {
	class DirectoryTraverser final
	{
	public:
		DirectoryTraverser();

		DirectoryTraverser(DirectoryTraverser const&) = delete;

		auto operator =(DirectoryTraverser const&) -> DirectoryTraverser& = delete;

		auto findSourceFiles(Path const& searchPath) const -> RelativePathList;

		auto findFileRelative(Path const& path, Path const& relative) const -> Path;

		auto findFileInFolders(PathList const& pathList, Path const& relative) const -> Path;

		auto fileExists(Path const& path) const -> bool;

		auto findDirectoryWithFile(PathList const& dirs, RelativePath const& relative) const -> Path;

		auto findRelativePath(RelativePath const& parent, RelativePath const& child) const -> RelativePath;
	private:
	};
}