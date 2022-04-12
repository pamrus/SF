#include "ClassThatUseDb.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class SomeTestSuite : public ::testing::Test
{
protected:
    // Переопределённая фикстура для передачи мок-объекта внутрь исследуемого класса:
    void SetUp(DBConnectionInterface* mockPointer) 
    {
        testObject = new ClassThatUseDb(mockPointer);
    }

    void TearDown()
    {
        delete testObject;
    }

    ClassThatUseDbInterface* testObject;
};


// Случай с корректным именем БД dbName:
// С 50%-й вероятностью проявляется искуственно-заложенная в ClassThatUseDb::useConnection ошибка
TEST_F(SomeTestSuite, CASE_filledName)
{
    std::string dbName = "TestName";
    std::string dbRequest = "DB request";
    std::string dbShouldReply = "DB reply";
    std::string dbReply;

    MockDBConnection mDBConnection;
    SetUp(&mDBConnection);

    EXPECT_CALL(mDBConnection, open).WillOnce(::testing::Return(true)); // dbName не пустое, значит open должен быть вызван
    EXPECT_CALL(mDBConnection, execQuery).WillOnce(::testing::Return(dbShouldReply)); // Может быть и не вызван (искуственная ошибка)
    EXPECT_CALL(mDBConnection, close).Times(1); // Должен быть вызван

    testObject->openConnection(dbName);
    dbReply = testObject->useConnection(dbRequest);
    testObject->closeConnection();

    EXPECT_EQ(dbReply, dbShouldReply);  // Также может проявиться искуственная ошибка
}

// Случай с пустым именем БД dbName:
// Тесты всегда должны быть успешно пройдены
TEST_F(SomeTestSuite, CASE_emptyName)
{
    std::string dbName = "";
    std::string dbRequest = "DB request";
    std::string dbShouldNotReply = "DB reply"; // Так как из-за пустого имени подключение НЕ должно быть установленным, ответ будет пустым
    std::string dbReply;

    MockDBConnection mDBConnection;
    SetUp(&mDBConnection);

    EXPECT_CALL(mDBConnection, open).Times(0); // dbName пустое, значит ни один из методов нижнего уровня не должен быть вызван
    EXPECT_CALL(mDBConnection, execQuery).Times(0);
    EXPECT_CALL(mDBConnection, close).Times(0);

    testObject->openConnection(dbName);
    dbReply = testObject->useConnection(dbRequest);
    testObject->closeConnection();

    EXPECT_NE(dbReply, dbShouldNotReply); // Или проще:
    EXPECT_EQ(dbReply, "");
}
