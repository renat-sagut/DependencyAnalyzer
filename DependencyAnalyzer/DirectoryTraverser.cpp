#include "DirectoryTraverser.h"

#include <regex>

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

namespace analyzer {

	DirectoryTraverser::DirectoryTraverser() = default;

	auto DirectoryTraverser::findSourceFiles(Path const& searchPath) const -> RelativePathList
	{
		fs::wpath p(searchPath);
		std::wregex matchSource(LR"(.*\.cpp$)");
		PathList result;
		for (fs::recursive_directory_iterator iter(p), end; iter != end; ++iter)
		{
			auto const& currentPath = iter->path();
			if (!fs::is_regular_file(currentPath))
				continue;
			auto const name = std::move(currentPath.leaf().wstring());
			if (std::regex_match(name, matchSource))
			{
				auto s = std::move(currentPath.wstring());
				auto relative = std::move(s.erase(s.find(searchPath), searchPath.length()));
				result.push_back(std::move(relative));
			}
		}

		return std::move(result);
	}

	auto DirectoryTraverser::findFileRelative(Path const& path, Path const& relative) const -> Path
	{
		fs::wpath p(path + relative);

		if (!fs::exists(p))
			return {};

		if (!fs::is_regular_file(p))
			return {};

		return std::move(p.wstring());
	}

	auto DirectoryTraverser::findFileInFolders(PathList const& pathList, Path const& relative) const -> Path
	{
		for (auto const& path : pathList)
		{
			auto const p = std::move(findFileRelative(path, relative));
			if (p.length() > 0)
				return std::move(p);
		}

		return {};
	}

	auto DirectoryTraverser::fileExists(Path const& path) const -> bool
	{
		fs::path p(path);
		if (!fs::exists(p))
			return false;

		if (!fs::is_regular_file(p))
			return false;

		return true;
	}

	auto DirectoryTraverser::findDirectoryWithFile(PathList const& dirs, RelativePath const& relative) const -> Path
	{
		for (auto const& dir : dirs)
		{
			Path path = std::move(dir + relative);
			if (fileExists(path))
				return dir;
		}

		return {};
	}

	auto DirectoryTraverser::findRelativePath(RelativePath const& parent, RelativePath const& child) const -> RelativePath
	{
		auto const slashIdx = parent.rfind('\\');
		if (slashIdx != std::wstring::npos)
			return std::move(parent.substr(0, slashIdx) + child);

		return {};
	}

}