// clang++ bot.cpp -o tgbot -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread
// V 1.0
#include <iostream>
#include <tgbot/tgbot.h>
#include <fstream>
#include <vector>
using namespace std;
using namespace TgBot;


	
bool uncorrect(vector<string> &check, string command) {
	bool result;
	for (auto k : check) {
		result = ((command == k) ? true : false);
	}
	return result;
}

void writeFile(vector<string> enList, vector<string> ruList) {
	ofstream enFile("EnglishWords.txt"); // open new file
	ofstream ruFile("RussianWords.txt"); // open new file
	for (auto word : enList) { enFile << '"' << word << '"' << ',' << endl; } // write to file
	for (auto word : ruList) { ruFile << '"' << word << '"' << ',' << endl; } // wriet to file
}

int main() {
    srand(time(0));

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	Bot bot("TOKEN HERE"); // bot tocken
	// Settings
	const short timeRandWord = 1; // from 1 to 24
	static int lastWords = 5; // how many last words

    static int  startTime; // function time
	static long indxCount; // size of vector (very importent)
    static long  randIndx; // keep random index 

	static string usrWord;
	static vector<string> newEn {
		"kek",
		"lol",
		"hello",
		"c++",
		"go"
	}; // users new words
	static vector<string> newTrans {
		"кок",
		"хее",
		"привет",
		"aaaaaa",
		"идти"
	}; // translate of word
	static vector<string> checkList {"/start", "/list", "/rand", "/last", "/lastRand"}; // command for ignore

	// Start message
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "/new word - add new word\n/tr translate - add translate of new word\n/list - show all words\n/rand - get random word from list for translate\nGitHub: https://github.com/koshcheev713/English-TgBot\nGood luck!");
                    
                    
        /*time_t now = time(0);
	    tm *realTime = localtime(&now);
        startTime = realTime->tm_hour;
        cout << "Start Time = " << startTime << endl;

        //while (true) {
			if (uncorrect(checkList, message->text)) break;
            now = time(0);
            realTime = localtime(&now);
            if (realTime->tm_hour == 8) startTime = 8;
            
			if (realTime->tm_hour == startTime + timeRandWord) {
				cout << "Clock!!!\n";
				if (!newEn.empty() && !newTrans.empty()) {
					randIndx = rand() % indxCount;
			       	bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx)); //send word
					cout << "Rand indx = " << randIndx << endl;
				    bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {		// get translate
						if (uncorrect(checkList, message->text)) return;
						usrWord = message->text; // get message
						usrWord.erase(0, 4);	 // delete /tr
						if (usrWord == newEn.at(randIndx)) {
							bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
						} else {
							bot.getApi().sendMessage(message->chat->id, "No!");
						}
				   	});
				}
			}
		//}
*/
	});





    // Add new words
    bot.getEvents().onCommand("new", [&bot](Message::Ptr message) {
        usrWord = message->text; // get message
        usrWord.erase(0, 5);	 // delete /new
        newEn.push_back(usrWord);// add new word
		// Add translate of new word
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text; // get message
        	usrWord.erase(0, 4);	 // delete /tr
			newTrans.push_back(usrWord); // add translate
    	});
		indxCount = newEn.size();
		writeFile(newEn, newTrans); // Write new words to files
    });

	// Send random word
    bot.getEvents().onCommand("rand", [&bot](Message::Ptr message) {
	if (!newEn.empty() && !newTrans.empty()) {
		randIndx = rand() % indxCount;
       	bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx)); //send word
		cout << "Rand indx = " << randIndx << endl;
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {		// get translate
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text; // get message
			usrWord.erase(0, 4);	 // delete /tr
			if (usrWord == newEn.at(randIndx)) {
				bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
			} else {
				bot.getApi().sendMessage(message->chat->id, "No!");
			}
    	});
	}
    });

    /*bot.getEvents().onCommand("lastRand", [&bot](Message::Ptr message) {
	if (!newEn.empty() && !newTrans.empty()) {
		randIndx = (indxCount-lastWords)+rand()%indxCount;
       	bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx)); //send word
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {		// get translate
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text; // get message
			usrWord.erase(0, 4);	 // delete /tr
			if (usrWord == newEn.at(randIndx)) {
				bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
			} else {
				bot.getApi().sendMessage(message->chat->id, "No!");
			}
    	});
	}
	});
    */
	// Show all words
    bot.getEvents().onCommand("list", [&bot](Message::Ptr message) {
    	if (!newEn.empty() && !newTrans.empty()) {
    		for (int i = 0; i < indxCount; ++i) {
    			bot.getApi().sendMessage(message->chat->id, newEn.at(i) + " - " + newTrans.at(i));
    		}
    	}
	});

	// Show last words
    bot.getEvents().onCommand("last", [&bot](Message::Ptr message) {
		if (!newEn.empty() && !newTrans.empty()) {
			for (int i = indxCount-lastWords; i < indxCount; ++i) {
				cout << "Last list indx = " << i << endl; 
				bot.getApi().sendMessage(message->chat->id, newEn.at(i) + " - " + newTrans.at(i));
			}
		}
	});

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgLongPoll longPoll(bot);
        while (true) {
            cout << "Long poll started\n";
		    longPoll.start();
        }
    } catch (TgBot::TgException& e) {
		writeFile(newEn, newTrans);
        cout << "error: " << e.what() << '\n';
    }
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    return 0;
}
