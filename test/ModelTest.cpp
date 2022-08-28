#include <gtest/gtest.h>
#include <cstdio>
#include "sqlite3.h"
#include "../app/models/Book.h"
#include "../app/models/History.h"
#include "../app/models/Member.h"
#include "../app/models/register.h"

struct ModelTest : public ::testing::Test
{
    sqlite3 *db;

    virtual void SetUp() override
    {
        sqlite3_open("test_database.db", &db);
        RegisterModels(db);
    }

    virtual void TearDown() override
    {
        sqlite3_close(db);
        remove("test_database.db");
    }

};

TEST_F(ModelTest, TestBookWithNegetiveCount) {
    EXPECT_THROW(
        Book book("The Fastlane Millionare", "M.J. DeMarco", "Viperion Corporation", -100),
        std::invalid_argument
    );
}

// TODO: atomize the test
TEST_F(ModelTest, TestBook) {
    int count = 10;

    // Test saving book
    for (int i = 0; i < count; i++)
    {
        string _id = std::to_string(i + 1);
        string name = "Book " + _id;
        string author = "Author " + _id;
        string publisher = "Publisher " + _id;
        int count = i * 10;

        Book book(name, author, publisher, count);
        book.Save();
        ASSERT_NE(book.id, -1) << "Book " << i << " could not be created";
    }

    ASSERT_EQ(Book::GetAll().size(), count);

    vector<Book> books = Book::GetAll();

    // Make sure data is saved in the database correctly
    for (int i = 0; i < count; i++)
    {
        ASSERT_EQ(books[i].name, "Book " + std::to_string(i + 1));
        ASSERT_EQ(books[i].author, "Author " + std::to_string(i + 1));
        ASSERT_EQ(books[i].publisher, "Publisher " + std::to_string(i + 1));
        ASSERT_NE(books[i].id, -1);
    }

    // Test updating book
    for (int i = 0; i < count; i++)
    {
        books[i].count += 1;
        books[i].Save();
    }

    books = Book::GetAll();
    
    for (int i = 0; i < count; i++)
    {
        ASSERT_EQ(books[i].count, i * 10 + 1);
    }

    // Test deleting books
    for (int i = 0; i < count; i++)
    {
        books[i].Delete();
    }

    ASSERT_EQ(Book::GetAll().size(), 0);
}

// TODO: atomize the tests
TEST_F(ModelTest, TestMember) {
    int count = 10;

    // Test creating member
    for (int i = 0; i < count; i++)
    {
        string _id = std::to_string(i + 1);
        string firstName = "fname " + _id;
        string lastName = "lname " + _id;
        string address = "Address " + _id;
        string phone = "12345678" + _id;
        string email = "example" + _id + "@gmail.com";

        Member member(firstName, lastName, address, phone, email);
        member.Save();
        ASSERT_NE(member.id, -1) << "Member " << i << " could not be created";
    }

    ASSERT_EQ(Member::GetAll().size(), count);

    vector<Member> members = Member::GetAll();

    // Make sure data is saved in the database correctly
    for (int i = 0; i < count; i++)
    {
        ASSERT_EQ(members[i].firstName, "fname " + std::to_string(i + 1));
        ASSERT_EQ(members[i].lastName, "lname " + std::to_string(i + 1));
        ASSERT_EQ(members[i].address, "Address " + std::to_string(i + 1));
        ASSERT_EQ(members[i].phonenumber, "12345678" + std::to_string(i + 1));
        ASSERT_EQ(members[i].email, "example" + std::to_string(i + 1) + "@gmail.com");
        ASSERT_NE(members[i].id, -1);
    }

    // Test updating member
    for (int i = 0; i < count; i++)
    {
        members[i].firstName += "updated";
        members[i].Save();
    }

    members = Member::GetAll();
    
    for (int i = 0; i < count; i++)
    {
        ASSERT_EQ(members[i].firstName, "fname " + std::to_string(i + 1) + "updated");
    }

    // Test deleting books
    for (int i = 0; i < count; i++)
    {
        members[i].Delete();
    }

    ASSERT_EQ(Member::GetAll().size(), 0);
}

TEST_F(ModelTest, TestHistory) {
    // create some sample users and books for transactions
    Member user1 = Member("Srijan", "Mukherjee", "KP5", "9775053592", "emailofsrijan@gmail.com");
    Member user2 = Member("Bizare", "Jojo", "anime", "0098123012", "bizarejojo@animeworld.com");
    user1.Save();
    user2.Save();
    ASSERT_EQ(Member::GetAll().size(), 2);

    Book book0 = Book("How to put your dick in the vagina", "Srijan Mukherjee", "Tera Porno Production", 10);
    Book book1 = Book("Python basics", "dont know", "someone", 1);
    book0.Save();
    book1.Save();
    ASSERT_EQ(Book::GetAll().size(), 2);

    // create history :p
    History history0 = History(book0, user1, 7);
    History history1 = History(book0, user2, 1);
    history0.Save();
    history1.Save();
    ASSERT_EQ(History::GetAll().size(), 2);

    vector<History> historiesUser0 = History::GetHistoryOfMember(user1);
    ASSERT_EQ(historiesUser0.size(), 1);

    vector<History> historiesBook0 = History::GetHistoryOfBook(book0);
    ASSERT_EQ(historiesBook0.size(), 2);
}