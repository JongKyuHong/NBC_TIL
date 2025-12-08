#include <iostream>
#include <memory>
using namespace std;

class Logger {
private:
    int logCount;

public:
    Logger() : logCount(0) {};
    void logInfo(const string& message) {
        logCount++;
        cout << "[INFO]: " << message << endl;
    };
    void logWarning(const string& message) {
        logCount++;
        cout << "[WARNING]: " << message << endl;
    };
    void logError(const string& message) {
        logCount++;
        cout << "[ERROR]: " << message << endl;
    };
    void showTotalLogs() {
        cout << "Total logs recorded: " << logCount << endl;
    };

    ~Logger() {
        cout << "Logger instance destroyed." << endl;
    };
};

int main() {
    unique_ptr<Logger> LogHistory = make_unique<Logger>();

    LogHistory->logInfo("System is starting.");
    LogHistory->logWarning("Low disk space.");
    LogHistory->logError("Unable to connect to the server.");

    LogHistory->showTotalLogs();

    return 0;
}
