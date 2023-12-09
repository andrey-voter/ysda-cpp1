#include "client.h"
#include <vector>
#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPSClientSession.h>

std::vector<Message> Client::GetUpdates(int offset, int timeout) {

    std::map<std::string, std::string> params;
    if (offset) {
        params["offset"] = std::to_string(offset);
    }

    if (timeout) {
        params["timeout"] = std::to_string(timeout);
    }

    auto result = ExecuteRequest("/getUpdates", params);
    auto result_array = result.getArray("result");
    std::vector<Message> updates;
    for (const auto& value : *result_array) {
        auto message_object = value.extract<Poco::JSON::Object::Ptr>();
        updates.emplace_back(Message{*message_object->getObject("message"),
                                     message_object->getValue<int>("update_id")});
    }
    return updates;
}

Poco::JSON::Object Client::ExecuteRequest(const std::string& method,
                                          const std::map<std::string, std::string>& params) {
    Poco::URI uri(api_end_point_ + api_key_ + method);

    Poco::Net::HTTPRequest request;
    if (method == "/getUpdates" or method == "/getMe") {
        Poco::URI::QueryParameters query_params;
        for (const auto& param : params) {
            query_params.emplace_back(param);
        }
        uri.setQueryParameters(query_params);
        request = {Poco::Net::HTTPRequest::HTTP_GET, uri.getPathAndQuery(),
                   Poco::Net::HTTPMessage::HTTP_1_1};
        session_->sendRequest(request);
    } else {
        request = {Poco::Net::HTTPRequest::HTTP_POST, uri.getPathAndQuery(),
                   Poco::Net::HTTPMessage::HTTP_1_1};

        Poco::JSON::Object::Ptr data = new Poco::JSON::Object();

        if (params.contains("chat_id")) {
            data->set("chat_id", std::stoll(params.at("chat_id")));
        }

        if (params.contains("text")) {
            data->set("text", params.at("text"));
        }

        if (params.contains("reply_to_message_id")) {
            data->set("reply_to_message_id", std::stoll(params.at("reply_to_message_id")));
        }

        if (params.contains("timeout")) {
            data->set("timeout", std::stoll(params.at("timeout")));
        }

        std::ostringstream oss;
        data->stringify(oss, 2);

        request.setContentLength(oss.str().length());
        request.setContentType("application/json");

        session_->sendRequest(request) << oss.str();
        session_->flushRequest();
    }

    Poco::Net::HTTPResponse response;
    std::istream& body = session_->receiveResponse(response);

    auto status = response.getStatus();

    if (status / 100 != 2) {
        throw Poco::Exception("Something went wrong during url query: " + std::to_string(status));
    }

    Poco::JSON::Parser parser;
    auto result = parser.parse(body).extract<Poco::JSON::Object::Ptr>();
    return *result;
}

User Client::GetMe() {
    return User(ExecuteRequest("/getMe"));
}

void Client::SetOffset(int offset) {
    std::ofstream file;
    file.open("offset.txt");
    file << offset;
    file.close();
}

Client::Client(std::string api_key, std::string api_end_point) {
    if (!api_key.empty()) {
        api_key_ = api_key;
    }

    if (!api_end_point.empty()) {
        api_end_point_ = api_end_point;
    }
    Poco::URI uri(api_end_point_);
    std::string scheme(uri.getScheme());
    std::string host(uri.getHost());
    auto port(uri.getPort());
    if (scheme == "http") {
        session_ = std::make_unique<Poco::Net::HTTPClientSession>(host, port);
    } else if (scheme == "https") {
        session_ = std::unique_ptr<Poco::Net::HTTPClientSession>(
            new Poco::Net::HTTPSClientSession(host, port));
    } else {
        throw Poco::Exception("Unknown scheme");
    }
}

void Client::SendMessage(int64_t chat_id, const std::string& text) {
    std::map<std::string, std::string> params;
    params["chat_id"] = std::to_string(chat_id);
    params["text"] = text;
    ExecuteRequest("/sendMessage", params);
}

void Client::ReplyToMessage(int64_t chat_id, const std::string& text, int message_id) {
    std::map<std::string, std::string> params;
    params["chat_id"] = std::to_string(chat_id);
    params["text"] = text;
    params["reply_to_message_id"] = std::to_string(message_id);
    ExecuteRequest("/sendMessage", params);
}
