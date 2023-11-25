#include <Poco/URI.h>
#include <Poco/Format.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "weather.h"

class YandexForecaster : public IForecaster {
public:
    YandexForecaster(const std::string& api_key, const std::string& api_endpoint)
        : api_key_(api_key), api_endpoint_(api_endpoint) {
    }

    WeatherForecast ForecastWeather(std::optional<Location> where = std::nullopt) override {
        Poco::URI uri(api_endpoint_);

        if (where) {
            uri.addQueryParameter("lat", Poco::format("%.2f", where->lat));
            uri.addQueryParameter("lon", Poco::format("%.2f", where->lon));
        }

        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

        Poco::Net::HTTPRequest request("GET", uri.toString());
        request.add("X-Yandex-API-Key", api_key_);
        session.sendRequest(request);

        Poco::Net::HTTPResponse response;
        auto& body = session.receiveResponse(response);

        if (response.getStatus() / 100 != 2) {
            throw YandexAPIError(response.getStatus(), "oops");
        }

        Poco::JSON::Parser parser;
        auto reply = parser.parse(body);

        auto fact = reply.extract<Poco::JSON::Object::Ptr>()->getObject("fact");

        WeatherForecast forecast;
        forecast.temp = fact->getValue<double>("temp");
        forecast.feels_like = fact->getValue<double>("feels_like");

        return forecast;
    }

private:
    std::string api_key_;
    std::string api_endpoint_;
};

std::unique_ptr<IForecaster> CreateYandexForecaster(const std::string& api_key,
                                                    const std::string& api_endpoint) {
    return std::make_unique<YandexForecaster>(YandexForecaster(api_key, api_endpoint));
}