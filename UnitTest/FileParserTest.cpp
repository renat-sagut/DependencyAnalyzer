#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(FileParserTest)
	{
	public:
		
		TEST_METHOD(removeComments)
		{
			std::string fileWithComments(
				R"(#include "foo.h"\n)"
				R"(#include <test/aa.h>\n)"
				R"(/*\n)"
				R"(#include <c.h>\n)"
				R"(*/\n)");

			std::string fileWithoutComments(
				R"(#include "foo.h"\n)"
				R"(#include <test/aa.h>\n)");

			FileParser parser;
			std::string fileToProcess = fileWithComments;
			parser.removeComments(fileToProcess);
			Assert::AreEqual(fileToProcess, fileWithoutComments);
		}

	};
}