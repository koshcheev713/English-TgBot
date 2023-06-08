// clang++ bot.cpp -o tgbot -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread
#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>
using namespace std;
using namespace TgBot;

bool uncorrect(vector<string> &check, string command) {
	bool result;
	for (auto k : check) {
		if (command == k) {
			result = true;
		} else {
			result = false;
		}
	}
	return result;
}

int main() {
	srand(time(0));
	static string usrWord;
	static vector<string> newEn;
	static vector<string> newTrans;
	static vector<string> checkList {"/start", "/list", "/rand"};
	
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	Bot bot("TOKEN HERE");
	// Start message and ignore any message
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hello\nRemember english words bot");
    });

    // Add new english words
    bot.getEvents().onCommand("newEn", [&bot](Message::Ptr message) {
        usrWord = message->text;
        usrWord.erase(0, 7);
        newEn.push_back(usrWord);
        bot.getApi().sendMessage(message->chat->id, "Translate of " + usrWord);
		// Add translate of new word
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text;
        	usrWord.erase(0, 4);
			newTrans.push_back(usrWord);
    	});
    });

	// Send random word
	static long randIndx = 0;
    bot.getEvents().onCommand("rand", [&bot](Message::Ptr message) {
		if (newEn.size() > 0 && newTrans.size() > 0) {
		randIndx = rand() % newEn.size();
		cout << newEn.size() << " - " << randIndx << endl;
        bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx));
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text;
			usrWord.erase(0, 4);
			if (usrWord == newEn.at(randIndx)) {
				bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
			} else {
				bot.getApi().sendMessage(message->chat->id, "No!");
			}
    	});
		}
    });
    
	// Show all words
    bot.getEvents().onCommand("list", [&bot](Message::Ptr message) {
    	if (newEn.size() > 0 && newTrans.size() > 0) {
    		for (int i = 0; i < newEn.size(); ++i) {
    			bot.getApi().sendMessage(message->chat->id, newEn.at(i) + " - " + newTrans.at(i));
    		}
    	}
    });

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            cout << "Long poll started\n";
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        cout << "error: " << e.what() << '\n';
    }
    return 0;
}
