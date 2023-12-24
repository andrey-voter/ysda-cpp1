#include <string>
#include <vector>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <fstream>

class Message;
class User;

class Client {
public:
    Client(std::string api_key = "", std::string api_end_point = "");
    User GetMe();
    void SendMessage(int64_t chat_id, const std::string& text);
    void ReplyToMessage(int64_t chat_id, const std::string& text, int message_id);
    void SendSticker(int64_t chat_id, const std::string& sticker_id);
    void SendGif(int64_t chat_id, const std::string& file_id);
    std::vector<Message> GetUpdates(int offset = 0, int timeout = 0);
    void SetOffset(int offset);

private:
    std::string api_key_ = "6410806161:AAEPAqtJ0CO4-f1Dzo-qK3OJmAuFIWjMFDU";
    std::string api_end_point_ = "https://api.telegram.org/bot";
    std::unique_ptr<Poco::Net::HTTPClientSession> session_;
    Poco::JSON::Object ExecuteRequest(const std::string& method,
                                      const std::map<std::string, std::string>& params = {});
};

class Message {
public:
    Message(Poco::JSON::Object value, int64_t update_id) : value_(value), update_id_(update_id) {
    }

    int64_t GetChatId() const {
        return value_.getObject("chat")->getValue<int64_t>("id");
    }

    int64_t GetUpdateId() const {
        return update_id_;
    }

    int GetMessageId() const {
        return value_.getValue<int>("message_id");
    }

    std::string GetText() const {
        return value_.getValue<std::string>("text");
    }

private:
    Poco::JSON::Object value_;
    int64_t update_id_;
};

class User {
public:
    User(Poco::JSON::Object obj) {
        auto res = obj.getObject("result");
        id = res->getValue<int64_t>("id");
        is_bot = res->getValue<bool>("is_bot");
        username = res->getValue<std::string>("username");
        first_name = res->getValue<std::string>("first_name");
    }

    int64_t id;
    bool is_bot;
    std::string username;
    std::string first_name;
};
