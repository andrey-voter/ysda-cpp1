#include <string>
#include <vector>
#include <memory>
#include "client.h"

class Bot {
public:
    Bot();
    void OnMsg(Message message);
    void Run();

private:
    std::string apy_key_ = "6410806161:AAEPAqtJ0CO4-f1Dzo-qK3OJmAuFIWjMFDU";
    std::string api_end_point_ = "https://api.telegram.org/bot";
    std::shared_ptr<Client> client_;
    std::vector<Message> GetUpdates();
    int GetOffset();
    void SetOffset(int offset);
};