#include "bot.h"
#include <iostream>
#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>
#include <fstream>

Bot::Bot() {
}

void Bot::Run() {
    try {
        while (true) {
            for (const auto &message : GetUpdates()) {
                OnMsg(message);
            }
        }
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        std::cout << "Oops, bpt failed... Starting bot again" << std::endl;
        Run();
    }
}

std::vector<Message> Bot::GetUpdates() {
    int offset = GetOffset();
    auto messages = client_->GetUpdates(offset + 1);
    return messages;
}

int Bot::GetOffset() {
    std::ifstream file;
    file.open("offset.txt");
    int offset = 0;
    if (file.peek() != EOF) {
        file >> offset;
    }
    file.close();
    return offset;
}

void Bot::SetOffset(int offset) {
    std::ofstream file;
    file.open("offset.txt");
    file << offset;
    file.close();
}

void Bot::OnMsg(Message message) {
    auto text = message.GetText();

    if (text == "/random") {
        client_->SendMessage(message.GetChatId(), std::to_string(std::rand()));
        return;
    }
    if (text == "/weather") {
        client_->SendMessage(message.GetChatId(), "Winter is coming");
        return;
    }
    if (text == "/styleguide") {
        std::string joke("Тут может быть ваша шутка.");
        client_->SendMessage(message.GetChatId(), "Funny joke");
        return;
    }
    if (text == "/stop") {
        // TODO
        abort();
    }
    if (text == "/crash") {
        abort();
    }
}