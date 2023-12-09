#include "bot.h"
#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <fstream>

Bot::Bot() : client_(std::make_shared<Client>()) {
}

void Bot::Run() {
    try {
        while (true) {
            for (const auto &message : GetUpdates()) {
                OnMsg(message);
            }
        }
    } catch (const std::exception &exception) {
        if (std::string(exception.what()) == "stop") {
            client_->SetOffset(GetOffset() + 1);
            return;
        }
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

void Bot::OnMsg(Message message) {
    auto text = message.GetText();

    if (text == "/random") {
        client_->ReplyToMessage(message.GetChatId(), std::to_string(std::rand()),
                                message.GetMessageId());
        return;
    }
    if (text == "/weather") {
        client_->SendMessage(message.GetChatId(), "Winter is coming");
        return;
    }
    if (text == "/styleguide") {
        client_->SendMessage(message.GetChatId(), "Funny joke");
        return;
    }
    if (text == "/stop") {
        throw std::runtime_error("stop");
    }
    if (text == "/crash") {
        abort();
    }
    client_->SendMessage(message.GetChatId(), "Your text was: " + message.GetText());
}