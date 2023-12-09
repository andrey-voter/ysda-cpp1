#include <tests/test_scenarios.h>
#include "telegram/client.h"

#include <catch2/catch_test_macros.hpp>

void TestSingleGetMe(std::string_view url) {
    auto api = Client{"123", std::string(url) + "bot"};
    auto me = api.GetMe();
    REQUIRE(me.id == 1234567);
    REQUIRE(me.is_bot);
    REQUIRE(me.username == "test_bot");
    REQUIRE(me.first_name == "Test Bot");
}

void TestGetMeErrorHandling(std::string_view url) {
    auto api = Client{"123", std::string(url) + "bot"};
    REQUIRE_THROWS_AS(api.GetMe(), Poco::Exception);
    REQUIRE_THROWS_AS(api.GetMe(), Poco::Exception);
}

void TestSingleGetUpdatesAndSendMessages(std::string_view url) {
    auto api = Client{"123", std::string(url) + "bot"};
    auto messages = api.GetUpdates();
    REQUIRE(messages.size() == 4);
    api.SendMessage(messages[1].GetChatId(), "Hi!");
    api.ReplyToMessage(messages[1].GetChatId(), "Reply", messages[1].GetMessageId());
    api.ReplyToMessage(messages[1].GetChatId(), "Reply", messages[1].GetMessageId());
}

void TestHandleGetUpdatesOffset(std::string_view url) {
    auto api = Client{"123", std::string(url) + "bot"};
    auto messages = api.GetUpdates(0, 5);
    REQUIRE(messages.size() == 2);

    auto offset = messages[1].GetUpdateId() + 1;

    messages = api.GetUpdates(offset, 5);
    REQUIRE(messages.empty());

    messages = api.GetUpdates(offset, 5);
    REQUIRE(messages.size() == 1);
}
