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

		auto fileExists(Path const& path) const -> bool;

		auto findDirectoryWithFile(PathList const& dirs, RelativePath const& relative) const -> Path;

		auto findRelativePath(RelativePath const& parent, RelativePath const& child) const -> RelativePath;
	private:
	};
}