#include "client.h"
#include <vector>
#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPSClientSession.h>

std::vector<Message> Client::GetUpdates(int offset) {
    std::map<std::string, std::string> params;
    if (offset) {
        params["offset"] = std::to_string(offset);
    }

    auto result = ExecuteRequest("getUpdates", params);

    auto result_array = result.getArray("result");

    std::vector<Message> updates;

    for (const auto& value  : *result_array) {
        if (value.isStruct()) {
            auto message_object = value.extract<Poco::JSON::Object::Ptr>();
            updates.emplace_back(Message{*message_object});
        }
    }
    return updates;
}

Poco::JSON::Object Client::ExecuteRequest(const std::string& method, const std::map<std::string, std::string>& params) {
    Poco::URI uri(api_end_point_ + method);
    Poco::URI::QueryParameters query_params;
    for (const auto& param : params){
        query_params.emplace_back(param);
    }
    uri.setQueryParameters(query_params);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
    Poco::Net::HTTPResponse response;
    session_->sendRequest(request);

    std::istream& body = session_->receiveResponse(response);
    auto status = response.getStatus();

    if (status / 100 != 2) {
        throw Poco::Exception( "Something went wrong during url query: " + std::to_string(status));
    }

    Poco::JSON::Parser parser;
    auto result = parser.parse(body).extract<Poco::JSON::Object::Ptr>();

    return *result;
}

void Client::GetMe() {
    ExecuteRequest("getMe");
}

Client::Client(std::string api_end_point) {
    if (!api_end_point.empty()) {
        api_end_point_ = api_end_point;
    }
    Poco::URI uri(api_end_point_);
    std::string scheme(uri.getScheme());
    std::string host(uri.getHost());
    auto port(uri.getPort());
    if (scheme == "http"){
        session_ = std::make_unique<Poco::Net::HTTPClientSession>(host, port);
    } else if (scheme == "https"){
        session_ = std::unique_ptr<Poco::Net::HTTPClientSession>(new Poco::Net::HTTPSClientSession(host, port));
    } else {
        throw Poco::Exception("Unknown scheme");
    }
}

void Client::SendMessage(int64_t chat_id, const std::string& text) {
    std::map<std::string, std::string> params;
    params["chat_id"] = std::to_string(chat_id);
    params["text"] = text;
    ExecuteRequest("sendMessage", params);
}
