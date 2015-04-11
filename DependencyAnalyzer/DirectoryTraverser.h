#pragma once

#include <vector>
#include <string>

namespace analyzer {
	class DirectoryTraverser final
	{
	public:
		using FilePathList = std::vector<std::string>;

		DirectoryTraverser();

		DirectoryTraverser(DirectoryTraverser const&) = delete;

		auto operator =(DirectoryTraverser const&) ->DirectoryTraverser& = delete;

		auto findSourceFiles() const -> FilePathList;
	private:
	};
}