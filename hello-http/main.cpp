#include "weather.h"
#include <iostream>

int main() {
    auto a = CreateYandexForecaster("4dedd4ab-23f6-4217-8f58-d3ae32652510");
    auto b = a->ForecastWeather();
    std::cout << b.temp << b.feels_like << std::endl;

    return 0;
}
