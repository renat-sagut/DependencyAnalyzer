#include "DirectoryTraverser.h"

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;
#include "boost/regex.hpp"

namespace analyzer {

	DirectoryTraverser::DirectoryTraverser() = default;

	auto DirectoryTraverser::findSourceFiles(Path const& searchPath) const -> PathList
	{
		fs::wpath p(searchPath);
		boost::wregex matchSource(LR"(.*\.cpp$)");
		PathList result;
		for (fs::recursive_directory_iterator iter(p), end; iter != end; ++iter)
		{
			auto const& currentPath = iter->path();
			if (!fs::is_regular_file(currentPath))
				continue;
			auto const name = std::move(currentPath.leaf().wstring());
			if (boost::regex_match(name, matchSource))
			{
				auto s = std::move(currentPath.wstring());
				auto relative = std::move(s.erase(s.find(searchPath), searchPath.length()));
				result.push_back(std::move(relative));
			}
		}

		return std::move(result);
	}

}