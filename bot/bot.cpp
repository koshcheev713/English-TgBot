// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// English TgBot helper
// clang++ bot.cpp -o tgbot -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread
// V 1.0
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <tgbot/tgbot.h>
#include <fstream>
#include <vector>
using namespace std;
using namespace TgBot;
	
bool uncorrect(vector<string> &check, string command)
{
	bool result;
	for (auto k : check)
		result = ((command == k) ? true : false);

	return result;
}

void writeFile(vector<string> &enList, vector<string> &ruList)
{
	fstream enFile("EnglishWords.txt", ios::out); // open new file
	fstream ruFile("RussiaWords.txt",  ios::out); // open new file

	cout << "Write file!\n";
	cout << enList.back() << endl;
	cout << ruList.back() << endl;
	if (!enFile || !ruFile)
	{
		cout << "File open error!\n";
	}	
	for (auto en_words : enList) { enFile << en_words << endl; } // write to file
	for (auto ru_words : ruList) { ruFile << ru_words << endl; } // wriet to file
}

int main() {
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	srand(time(0));
	Bot bot("6271982519:AAGKd8lOWDXIFN1xZejCOb3rQAUKzmF1dqE"); // bot tocken

	// Settings
	static int lastWords = 5; // how many last words
	static int indxCount;	  // size of vector (very importent)
	static string usrWord;
	static vector<string> newEn; 	// users new words
	static vector<string> newTrans; // translate of word
	static vector<string> checkList {"/start", "/list", "/rand", "/last", "/lastRand"}; // command for ignore

	// READ DATA FROM FILES
   	fstream enFile("EnglishWords.txt", ios::in);
	fstream ruFile("RussiaWords.txt",  ios::in);
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

	newEn.erase(newEn.end());
	newTrans.erase(newTrans.end());
	indxCount = newTrans.size();

	// Welcome message
	bot.getEvents().onCommand("start", [&bot](Message::Ptr message)
	{
		bot.getApi().sendMessage(message->chat->id, "Hello! Documentation:\nGitHub: https://github.com/koshcheev713/English-TgBot\nGood luck!");
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
			cout << "New word!\n" << newEn.back() << endl << newTrans.back() << endl;
			writeFile(newEn, newTrans); // Write new words to files
		});
		++indxCount;
	});

	// RANDOM WORDS
	static int randIndx = 0;
	static int lastIndx = 0;
	bot.getEvents().onCommand("rand", [&bot](Message::Ptr message)
	{
		if (!newEn.empty() && !newTrans.empty())
		{
			do {
				randIndx = rand() % indxCount;
			}
			while (lastIndx == randIndx);
			lastIndx = randIndx;

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
	static int indxLast = 0;
	bot.getEvents().onCommand("last_rand", [&bot](Message::Ptr message)
	{
		randIndx = 0;
		if (!newEn.empty() && !newTrans.empty())
		{
			while (true)
			{
				randIndx = (indxCount-lastWords) + rand() % (indxCount-1);
				if (randIndx < indxCount && indxLast != randIndx) break; 
			}
			indxLast = randIndx;
			
			cout << "Start index: " << indxCount - lastWords << endl;
			cout << "Finish Indx: " << indxCount - 1 << endl;
			cout << "Random indx: " << randIndx << endl;
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
			//Message::Ptr message;
			//bot.getApi().sendMessage(message->chat->id, "Bot working!");
		}
	} catch (TgBot::TgException& e) {
		writeFile(newEn, newTrans);
		cout << "error: " << e.what() << '\n';
	}
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= TELEGRAM BOT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return 0;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// End the program
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
