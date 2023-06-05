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
	for (auto k : check)
		result = ((command == k) ? true : false);

	return result;
}

void writeFile(vector<string> enList, vector<string> ruList) {
	fstream enFile("EnglishWords.txt"); // open new file
	fstream ruFile("RussiaWords.txt");  // open new file
	for (auto word : enList) { enFile << word << endl; } // write to file
	for (auto word : ruList) { ruFile << word << endl; } // wriet to file
}

int main() {
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	srand(time(0));
	Bot bot("6271982519:AAGKd8lOWDXIFN1xZejCOb3rQAUKzmF1dqE"); // bot tocken

	// Settings
	static int lastWords = 5; // how many last words
	static long indxCount;	  // size of vector (very importent)
	static string usrWord;
	static vector<string> newEn; 	// users new words
	static vector<string> newTrans; // translate of word
	static vector<string> checkList {"/start", "/list", "/rand", "/last", "/lastRand"}; // command for ignore

	// READ DATA FROM FILES
    ifstream enFile("EnglishWords.txt");
	ifstream ruFile("RussiaWords.txt");
	string str;
 	// read from file words
	while (!enFile.eof())
	{
		getline(enFile, str);
		newEn.push_back(str);
	}
	 // read from file translate
	while (!ruFile.eof())
	{
		getline(ruFile, str);
		newTrans.push_back(str);
	}

	newEn.pop_back();
	newTrans.pop_back();
	indxCount = newEn.size();

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) { // Welcome message
        bot.getApi().sendMessage(message->chat->id, "/new word - add new word\n/tr translate - add translate of new word\n/list - show all words\n/rand - get random word from list for translate\nGitHub: https://github.com/koshcheev713/English-TgBot\nGood luck!");
    });

    // Add new words
    bot.getEvents().onCommand("new", [&bot](Message::Ptr message)
	{
        usrWord = message->text; // get message
        usrWord.erase(0, 5);	 // delete /new
        newEn.push_back(usrWord);// add new word
		// Get traslate
    	bot.getEvents().onCommand("tr", [&bot](Message::Ptr message)
		{
			if (uncorrect(checkList, message->text)) return;
			usrWord = message->text; 	 // get message
        	usrWord.erase(0, 4);	 	 // delete /tr
			newTrans.push_back(usrWord); // add translate
    	});
		++indxCount;
		writeFile(newEn, newTrans); // Write new words to files
    });

	// RANDOM WORDS
	static long randIndx = 0;
    bot.getEvents().onCommand("rand", [&bot](Message::Ptr message)
	{
		if (!newEn.empty() && !newTrans.empty())
		{
			randIndx = rand() % indxCount;
			bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx)); //send word
			cout << "Rand indx = " << randIndx << endl;
			// get translate
			bot.getEvents().onCommand("tr", [&bot](Message::Ptr message)
			{
				if (uncorrect(checkList, message->text)) return;
				usrWord = message->text; // get message
				usrWord.erase(0, 4);	 // delete /tr
				if (usrWord == newEn.at(randIndx))
				{
					bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
				}
				else
				{
					bot.getApi().sendMessage(message->chat->id, "No!");
				}
			});
		}
    });

	// RANDOM WORDS FROM LAST
    bot.getEvents().onCommand("lastRand", [&bot](Message::Ptr message)
	{
		if (!newEn.empty() && !newTrans.empty())
		{
			randIndx = (indxCount-lastWords) + rand() % (indxCount-1);
	       	bot.getApi().sendMessage(message->chat->id, newTrans.at(randIndx)); //send word
			// get translate
			bot.getEvents().onCommand("tr", [&bot](Message::Ptr message) {
				if (uncorrect(checkList, message->text)) return;
				usrWord = message->text; // get message
				usrWord.erase(0, 4);	 // delete /tr
				if (usrWord == newEn.at(randIndx))
				{
					bot.getApi().sendMessage(message->chat->id, "Yes! Good!");
				}
				else 
				{
					bot.getApi().sendMessage(message->chat->id, "No!");
				}
	    	});
		}
	});

	// Show all words
    bot.getEvents().onCommand("list", [&bot](Message::Ptr message)
	{
    	if (!newEn.empty() && !newTrans.empty())
		{
    		for (int i = 0; i < indxCount; ++i)
			{
    			bot.getApi().sendMessage(message->chat->id, newEn.at(i) + " - " + newTrans.at(i));
    		}
    	}
	});

	// Show last words
    bot.getEvents().onCommand("last", [&bot](Message::Ptr message)
	{
		if (!newEn.empty() && !newTrans.empty())
		{
			for (int i = indxCount-lastWords; i < indxCount; ++i)
			{
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
