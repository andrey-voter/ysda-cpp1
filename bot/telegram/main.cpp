#include "bot.h"

int main() {
    try{
        Bot bot;
        bot.Run();
        return 0;
    } catch (const std::exception& exception){
        std::cerr << "main crashed: " << exception.what() << std::endl;
    }

}
