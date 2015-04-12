#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(FileParserTest)
	{
	public:
		TEST_METHOD(readFile)
		{
			std::string fileCorrect(
				"first\r\n"
				"second\r\n"
				"third"
			);

			FileParser parser;
			auto const file = std::move(parser.readFile(L"..\\UnitTest\\ReadFileTest.txt"));
			Assert::AreEqual(file, fileCorrect);
		}

		TEST_METHOD(parseIncludes)
		{
			std::string file(
				"#include \"foo.h\"\n"
				"#include <test/aa.h>\n"
				"/*\n"
				"#include <c.h>\n"
				"*/\n"
				"//#include \"foo.h\"\n"
				"#include <test/aa.h>\n"
				"/*\n"
				"#include <c.h>\n"
				"*/\n"
			);

			FileParser::IncludeList bracketIncludesCorrect = {
				"test/aa.h",
				"test/aa.h"
			};

			FileParser::IncludeList quoteIncludesCorrect = {
				"foo.h"
			};

			FileParser parser;
			FileParser::IncludeList bracketIncludesFound, quoteIncludesFound;
			parser.parseIncludes(file, bracketIncludesFound, quoteIncludesFound);

			Assert::AreEqual(bracketIncludesFound.size(), bracketIncludesCorrect.size());
			for (int i = 0; i < bracketIncludesCorrect.size(); ++i)
			{
				Assert::AreEqual(bracketIncludesFound[i], bracketIncludesCorrect[i]);
			}

			Assert::AreEqual(quoteIncludesFound.size(), quoteIncludesCorrect.size());
			for (int i = 0; i < quoteIncludesCorrect.size(); ++i)
			{
				Assert::AreEqual(quoteIncludesFound[i], quoteIncludesCorrect[i]);
			}
		}

		TEST_METHOD(removeComments)
		{
			std::string fileWithComments(
				"#include \"foo.h\"\n"
				"#include <test/aa.h>\n"
				"/*\n"
				"#include <c.h>\n"
				"*/\n"
				"//#include \"foo.h\"\n"
				"#include <test/aa.h>\n"
				"/*\n"
				"#include <c.h>\n"
				"*/\n"
			);

			std::string fileWithoutComments(
				"#include \"foo.h\"\n"
				"#include <test/aa.h>\n\n\n"
				"#include <test/aa.h>\n\n"
			);

			FileParser parser;
			std::string fileToProcess = fileWithComments;
			parser.removeComments(fileToProcess);
			Assert::AreEqual(fileToProcess, fileWithoutComments);
		}

		TEST_METHOD(extractFilePath)
		{
			std::string includeString("#include < sources/foo.h > ");
			std::string filePathString("sources/foo.h");

			FileParser parser;
			std::string result = std::move(parser.extractFilePath(includeString));
			Assert::AreEqual(result, filePathString);
		}

		TEST_METHOD(findIncludeStrings)
		{
			std::string fileWithIncludes(
				"#include \"foo.h\"\n"
				"#include <test/aa.h>\n"
				"/*\n"
				"#include <c.h>\n"
				"*/\n"
				"#include \"c.h\"\n"
			);

			FileParser::StringList bracketIncludesCorrect = {
				"#include <test/aa.h>",
				"#include <c.h>"
			};

			FileParser::StringList quoteIncludesCorrect = {
				"#include \"foo.h\"",
				"#include \"c.h\""
			};

			FileParser parser;
			auto bracketIncludesFound = std::move(parser.findIncludeStrings(fileWithIncludes, FileParser::IncludeType::BRACKETS));
			Assert::AreEqual(bracketIncludesFound.size(), bracketIncludesCorrect.size());
			for (int i = 0; i < bracketIncludesCorrect.size(); ++i)
			{
				Assert::AreEqual(bracketIncludesFound[i], bracketIncludesCorrect[i]);
			}

			auto quoteIncludesFound = std::move(parser.findIncludeStrings(fileWithIncludes, FileParser::IncludeType::QUOTES));
			Assert::AreEqual(quoteIncludesFound.size(), quoteIncludesCorrect.size());
			for (int i = 0; i < quoteIncludesCorrect.size(); ++i)
			{
				Assert::AreEqual(quoteIncludesFound[i], quoteIncludesCorrect[i]);
			}
		}

	};
}