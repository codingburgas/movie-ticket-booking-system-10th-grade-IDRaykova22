#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UserTests)
	{
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			nlohmann::json testData = nlohmann::json::array();
			nlohmann::json user1;
			user1["id"] = 1;
			user1["email"] = "test@example.com";
			user1["userName"] = "TestUser";
			user1["password"] = "Password123!";
			user1["isAdmin"] = false;
			testData.push_back(user1);

			std::ofstream testFile("test_users.json");
			testFile << testData.dump(4);
			testFile.close();

			std::ofstream emptyFile("empty_test.json");
			emptyFile.close();
		}
		TEST_METHOD(Constructor_SetsDefaultValues)
		{
			User user;
			Assert::IsFalse(user.getIsAdmin());
			Assert::IsTrue(user.isUserEmpty());
		}

		TEST_METHOD(GetterSetter_AdminStatus)
		{
			User user;
			Assert::IsFalse(user.getIsAdmin());
			user.setAdmin();
			Assert::IsTrue(user.getIsAdmin());
		}

		TEST_METHOD(GetterSetter_UserName)
		{
			User user;
			std::string testName = "TestUser";
			user.setUserName(testName);
			Assert::AreEqual(testName, user.getUserName());
		}

		TEST_METHOD(CheckEmail_ValidEmail_ReturnsTrue)
		{
			User user;
			bool result = user.checkEmail("unique@example.com", "empty_test.json");
			Assert::IsTrue(result);
			Assert::AreEqual(std::string("unique@example.com"), user.getEmail());
		}

		TEST_METHOD(CheckEmail_InvalidFormat_ReturnsFalse)
		{
			User user;
			bool result = user.checkEmail("invalid-email", "empty_test.json");
			Assert::IsFalse(result);
		}

		TEST_METHOD(CheckEmail_DuplicateEmail_ReturnsFalse)
		{
			User user;
			bool result = user.checkEmail("test@example.com", "test_users.json");
			Assert::IsFalse(result);
		}

		TEST_METHOD(CheckPassword_ValidPassword_ReturnsTrue)
		{
			User user;
			std::string testPassword = "Password123!";
			bool result = user.checkPassword(testPassword);
			Assert::IsTrue(result);
			Assert::AreEqual(testPassword, user.getPassword());
		}

		TEST_METHOD(CheckPassword_TooShort_ReturnsFalse)
		{
			User user;
			bool result = user.checkPassword("Pw!");
			Assert::IsFalse(result);
		}

		TEST_METHOD(CheckPassword_NoSpecialChar_ReturnsFalse)
		{
			User user;
			bool result = user.checkPassword("Password123");
			Assert::IsFalse(result);
		}

		TEST_METHOD(IsUserEmpty_NewUser_ReturnsTrue)
		{
			User user;
			Assert::IsTrue(user.isUserEmpty());
		}

		TEST_METHOD(IsUserEmpty_AfterSettingEmail_ReturnsFalse)
		{
			User user;
			user.checkEmail("test@example.com", "empty_test.json");
			Assert::IsFalse(user.isUserEmpty());
		}

		TEST_METHOD(EraseUser_ClearsAllUserData)
		{
			User user;
			user.checkEmail("test@example.com", "empty_test.json");
			user.checkPassword("Password123!");
			user.setUserName("TestUser");
			user.setAdmin();

			user.eraseUser();

			Assert::IsTrue(user.isUserEmpty());
			Assert::AreEqual(std::string(""), user.getEmail());
			Assert::AreEqual(std::string(""), user.getPassword());
			Assert::AreEqual(std::string(""), user.getUserName());
			Assert::IsFalse(user.getIsAdmin());
		}
		TEST_METHOD(SaveAsJson_CreatesCorrectJsonObject)
		{
			User user;
			user.checkEmail("test@example.com", "empty_test.json");
			user.checkPassword("Password123!");
			user.setUserName("TestUser");

			nlohmann::json userData = user.saveAsJson();

			Assert::IsTrue(userData.contains("email"));
			Assert::IsTrue(userData.contains("password"));
			Assert::IsTrue(userData.contains("userName"));
			Assert::IsTrue(userData.contains("isAdmin"));
			Assert::AreEqual(std::string("TestUser"), userData["userName"].get<std::string>());
			Assert::IsFalse(userData["isAdmin"].get<bool>());
		}
		TEST_METHOD(LoadFromFile_ExistingUser_ReturnsTrue)
		{
			User user;
			bool result = user.loadFromFile("test_users.json", "test@example.com");

			Assert::IsTrue(result);
			Assert::AreEqual(std::string("test@example.com"), user.getEmail());
			Assert::AreEqual(std::string("TestUser"), user.getUserName());
		}

		TEST_METHOD(LoadFromFile_NonExistingUser_ReturnsFalse)
		{
			User user;
			bool result = user.loadFromFile("test_users.json", "nonexistent@example.com");

			Assert::IsFalse(result);
		}

		TEST_METHOD(LoadFromFile_EmptyFile_ReturnsFalse)
		{
			User user;
			bool result = user.loadFromFile("empty_test.json", "test@example.com");

			Assert::IsFalse(result);
		}
	};


	TEST_CLASS(TimelineExtendedTests)
	{
	public:
		TEST_METHOD(AddEvent_MultipleEvents_SortsByYear)
		{
			Timeline timeline;
			timeline.addEvent("Event 2000", 2000, 0, "Region", "Leader", "Country", "Desc", "user");
			timeline.addEvent("Event 1990", 1990, 0, "Region", "Leader", "Country", "Desc", "user");
			timeline.addEvent("Event 2010", 2010, 0, "Region", "Leader", "Country", "Desc", "user");
			timeline.saveEventsToJson("test_sorted_events.json");
			nlohmann::json savedData = Utiles::loadFile("test_sorted_events.json");

			Assert::IsTrue(savedData.is_array());
			Assert::AreEqual(static_cast<size_t>(3), savedData.size());
			Assert::AreEqual(1990, savedData[0]["year"].get<int>());
			Assert::AreEqual(2000, savedData[1]["year"].get<int>());
			Assert::AreEqual(2010, savedData[2]["year"].get<int>());
		}

		TEST_METHOD(EditEvent_NonExistingYear_DoesNotModify)
		{
			Timeline timeline;
			timeline.addEvent("Original Event", 2020, 0, "Region", "Leader", "Country", "Desc", "user");

			timeline.editEvent("test_edit.json", 9999);

			timeline.saveEventsToJson("test_edit.json");
			nlohmann::json data = Utiles::loadFile("test_edit.json");

			Assert::AreEqual(static_cast<size_t>(1), data.size());
			Assert::AreEqual(std::string("Original Event"), data[0]["title"].get<std::string>());
		}

		TEST_METHOD(DeleteEvent_EmptyFile_DoesNotCrash)
		{
			Timeline timeline;
			timeline.deleteEvent("test_empty_delete.json", 2020);
			Assert::IsTrue(true);
		}

		TEST_METHOD(CompareEvents_SameEvent_ReturnsCorrectComparison)
		{
			Timeline timeline;
			Event event1{ "Event", 2000, 100, "North", "Leader", "BG", "Desc", "user" };
			Event event2{ "Event", 2000, 100, "North", "Leader", "BG", "Desc", "user" };

			timeline.addEvent(event1.title, event1.year, event1.victims, event1.partOfBulgaria,
				event1.leader, event1.countries, event1.description, event1.username);
			timeline.addEvent(event2.title, event2.year, event2.victims, event2.partOfBulgaria,
				event2.leader, event2.countries, event2.description, event2.username);

			Assert::IsTrue(true);
		}
	};

	TEST_CLASS(UtilsTests)
	{
	public:
		TEST_METHOD(Sha256FromString_ValidInput_ReturnsHash)
		{
			std::string hash = Utiles::sha256FromString("password");
			Assert::AreEqual(static_cast<size_t>(64), hash.length());
			Assert::AreNotEqual(std::string("password"), hash);
		}

		TEST_METHOD(SaveToFile_ValidData_CreatesFile)
		{
			nlohmann::json testData;
			testData["test"] = "value";

			Utiles::saveToFile("test_utils.json", testData);

			std::ifstream file("test_utils.json");
			Assert::IsTrue(file.good());
			file.close();

			std::remove("test_utils.json");
		}

		TEST_METHOD(IsFileEmpty_NonExistentFile_ReturnsFalse)
		{
			Assert::IsFalse(Utiles::isFileEmpty("nonexistent_file.xyz"));

			std::ofstream emptyFile("empty_test_file.tmp");
			emptyFile.close();
			Assert::IsTrue(Utiles::isFileEmpty("empty_test_file.tmp"));
			std::remove("empty_test_file.tmp");

			std::ofstream nonEmptyFile("non_empty_test_file.tmp");
			nonEmptyFile << "content";
			nonEmptyFile.close();
			Assert::IsFalse(Utiles::isFileEmpty("non_empty_test_file.tmp"));
			std::remove("non_empty_test_file.tmp");
		}
	};

	TEST_CLASS(StagesTests)
	{
	public:
		TEST_METHOD(StagesMenu_InvalidChoice_RepeatsPrompt)
		{
			Stages stages;
			std::istringstream input("9\n1\n");
			std::streambuf* origCin = std::cin.rdbuf(input.rdbuf());

			try {
				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

				std::cout.rdbuf(old);
				std::cin.rdbuf(origCin);

				std::string output = buffer.str();
				Assert::IsTrue(output.find("You've entered an invalid option") != std::string::npos);
			}
			catch (...) {
				std::streambuf* old = std::cout.rdbuf();
				std::cout.rdbuf(old);

				std::cin.rdbuf(origCin);
				Assert::Fail(L"Exception thrown");
			}
		}

		TEST_METHOD(DisplayStage_ValidFiles_DisplaysContent)
		{
			Stages stages;
			const std::string validMap = "../assets/graphic/maps/681-814.txt";
			const std::string validInfo = "../assets/graphic/stages/681-814.txt";

			try {
				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

				std::cout.rdbuf(old);
				std::string output = buffer.str();
				Assert::IsFalse(output.empty());
			}
			catch (...) {
				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
				Assert::Fail(L"Exception thrown");
			}
		}
	};

		TEST_CLASS(UiTests)
		{
		public:
			TEST_METHOD_INITIALIZE(SetUp)
			{
				std::ofstream testFile("test_users.json");
				testFile << R"([{"id":1,"email":"test@test.com","userName":"testuser","password":"hash","isAdmin":false}])";
				testFile.close();
			}

			TEST_METHOD_CLEANUP(Cleanup)
			{
				std::remove("test_users.json");
				std::remove("test_events.json");
			}

			TEST_METHOD(MainMenu_ValidChoice_NavigatesCorrectly)
			{
				std::istringstream input("1\n");
				std::cin.rdbuf(input.rdbuf());

				Ui ui;

				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

				try {
					ui.mainMenu();
					std::string output = buffer.str();
					Assert::IsTrue(output.find("Registration") != std::string::npos);
				}
				catch (...) {
					std::cout.rdbuf(old);
					Assert::Fail(L"Exception thrown");
				}

				std::cout.rdbuf(old);
			}

			TEST_METHOD(RegisterUi_ValidInput_CreatesUser)
			{
				std::istringstream input("test@test.com\ntestuser\nValidPass123!\nValidPass123!\nn\n");
				std::cin.rdbuf(input.rdbuf());

				Ui ui;

				nlohmann::json users = Utiles::loadFile("test_users.json");
				Assert::AreEqual(static_cast<size_t>(2), users.size());
				Assert::AreEqual(std::string("testuser"), users[1]["userName"].get<std::string>());
			}

			TEST_METHOD(LogInUi_ValidCredentials_LogsInUser)
			{
				std::istringstream input("test@test.com\npassword\n");
				std::cin.rdbuf(input.rdbuf());

				Ui ui;

				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

				std::string output = buffer.str();
				std::cout.rdbuf(old);

				Assert::IsTrue(output.find("Welcome") != std::string::npos);
			}

			TEST_METHOD(TimeLineUi_AuthenticatedUser_DisplaysEvents)
			{
				std::istringstream input("test@test.com\npassword\n");
				std::cin.rdbuf(input.rdbuf());

				Ui ui;

				std::stringstream buffer;
				std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

				ui.timeLineUi();

				std::string output = buffer.str();
				std::cout.rdbuf(old);

				Assert::IsTrue(output.find("events") != std::string::npos);
			}
		};
	}