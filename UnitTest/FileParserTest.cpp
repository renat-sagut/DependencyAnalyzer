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
				R"(#include "foo.h")"
				R"(#include <test/aa.h>)"
				R"(/*)"
				R"(#include <c.h>)"
				R"(*/)");

			std::string fileWithoutComments(
				R"(#include "foo.h")"
				R"(#include <test/aa.h>)");

			FileParser parser;
			std::string fileToProcess = fileWithComments;
			parser.removeComments(fileToProcess);
			Assert::AreEqual(fileToProcess, fileWithoutComments);
		}

	};
}