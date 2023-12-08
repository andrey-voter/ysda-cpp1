#include <string>
#include <vector>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>

class Message;

class Client {
public:
    Client(std::string api_end_point = "");
    void GetMe();
    void SendMessage(int64_t chat_id, const std::string& text);
    std::vector<Message> GetUpdates(int offset = 0);

private:
    std::string apy_key_ = "6410806161:AAEPAqtJ0CO4-f1Dzo-qK3OJmAuFIWjMFDU";
    std::string api_end_point_ = "https://api.telegram.org/bot";
    std::unique_ptr<Poco::Net::HTTPClientSession> session_;
    Poco::JSON::Object ExecuteRequest(const std::string& method, const std::map<std::string, std::string>& params = {});
};

class Message {
public:
    Message(Poco::JSON::Object value) : value_(value) {}

    int64_t GetChatId()  const {
        return value_.getObject("chat")->getValue<int64_t>("id");
    }

    int GetMessageId()  const {
        return value_.getValue<int>("message_id");
    }

    std::string GetText() const {
        return value_.getValue<std::string>("text");
    }

private:
    Poco::JSON::Object value_;
};
