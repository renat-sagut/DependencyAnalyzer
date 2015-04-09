#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;


// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
	copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
	return os;
}

int main(int ac, char* av[])
{
	try
	{
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("source-path", po::value<string>()->required(), "source path")
			("include-path,I", po::value<vector<string>>(), "include path")
			;

		po::positional_options_description p;
		p.add("source-path", 1);

		po::variables_map vm;
		po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);

		if (vm.count("help")) {
			cout << "Usage: options_description [options]\n";
			cout << desc;
			return 0;
		}

		po::notify(vm);

		if (vm.count("source-path"))
		{
			cout << "Source path is: " << vm["source-path"].as< string >() << "\n";
		}

		if (vm.count("include-path"))
		{
			cout << "Include paths are: " << vm["include-path"].as< vector<string> >() << "\n";
		}
	}
	catch (std::exception& e)
	{
		cout << e.what() << "\n";
		return 1;
	}
	return 0;
}
