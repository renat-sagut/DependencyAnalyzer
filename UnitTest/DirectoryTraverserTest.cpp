#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(DirectoryTraverserTest)
	{
	public:
		TEST_METHOD(findSourceFiles)
		{
			RelativePathList sourcesCorrect = {
				L"\\DependencyAnalyzer.cpp",
				L"\\DependencyGraph.cpp",
				L"\\DirectoryTraverser.cpp",
				L"\\FileParser.cpp"
			};

			DirectoryTraverser traverser;
			auto sources = std::move(traverser.findSourceFiles(LR"(..\DependencyAnalyzer)"));
			Assert::AreEqual(sources.size(), sourcesCorrect.size());
			for (int i = 0; i < sourcesCorrect.size(); ++i)
			{
				Assert::AreEqual(sources[i], sourcesCorrect[i]);
			}
		}

	};
}