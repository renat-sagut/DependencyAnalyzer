#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <io.h>
#include <fcntl.h>

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		_setmode(_fileno(stdout), _O_U16TEXT);

		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("source-path", po::wvalue<wstring>()->required(), "source path")
			("include-path,I", po::wvalue<vector<wstring>>(), "include path");

		po::positional_options_description p;
		p.add("source-path", 1);

		po::variables_map vm;
		po::store(po::basic_command_line_parser<wchar_t>(argc, argv).options(desc).positional(p).run(), vm);

		if (vm.count("help"))
		{
			stringstream ss;
			ss << desc;
			string str = std::move(ss.str());
			wstring wdesc(str.begin(), str.end());

			wcout << L"Usage: options_description [options]\n";
			wcout << wdesc;
			return 0;
		}

		po::notify(vm);

		if (vm.count("source-path"))
		{
			wcout << L"Source path is: " << vm["source-path"].as<wstring>() << L"\n";
		}

		if (vm.count("include-path"))
		{
			wcout << L"Include paths are: ";
			for (auto const& path : vm["include-path"].as<vector<wstring>>())
			{
				wcout << path << L" ";
			}
			wcout << L"\n";
		}
	}
	catch (std::exception& e)
	{
		wcout << e.what() << "\n";
		return 1;
	}
	return 0;
}
