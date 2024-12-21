#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cctype>
using namespace std;

const string ClientsFileName = "ClientsFile.txt";

void ATM();
void Login();

short ReadNumberInRange(short From, short To, string Message) {
	short Choose;
	do {
		cout << Message;
		cin >> Choose;
	} while (Choose < From || Choose > To);
	return Choose;
}

string ReadString(string Message) {
	string String;
	cout << Message;
	cin >> String;
	return String;
}

bool AskYesNo(string Message) {
	char Answer;
	cout << Message;
	cin >> Answer;
	return toupper(Answer) == 'Y' ? true : false;
}

struct stClientData {

	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;

};

stClientData CurrentClient;

vector<string> SplitString(string MyString, string Split = "#//#") {

	vector<string> vStringWords;
	string sWord;
	short pos = 0;

	while ((pos = MyString.find(Split)) != std::string::npos) {

		sWord = MyString.substr(0, pos);
		if (sWord != "") {
			vStringWords.push_back(sWord);
		}

		MyString.erase(0, pos + Split.length());

	}

	if (MyString != "") {
		vStringWords.push_back(MyString);
	}

	return vStringWords;

}

stClientData ConvertDataLineToRecord(string DataLine, string Seperator = "#//#") {

	vector<string> vStringWords = SplitString(DataLine, Seperator);
	stClientData stClient;

	stClient.AccountNumber = vStringWords[0];
	stClient.PinCode = vStringWords[1];
	stClient.Name = vStringWords[2];
	stClient.Phone = vStringWords[3];
	stClient.AccountBalance = stod(vStringWords[4]);

	return stClient;

}

vector<stClientData> LoadClientsDataFromFile(string FileName) {

	vector<stClientData> vClients;
	stClientData stClient;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;

		while (getline(MyFile, Line)) {

			stClient = ConvertDataLineToRecord(Line);
			vClients.push_back(stClient);

		}

		MyFile.close();
	}

	return vClients;

}

string ConvertRecordToLine(stClientData stClient, string Seperator = "#//#") {

	string DataLine = "";

	DataLine += stClient.AccountNumber + Seperator;
	DataLine += stClient.PinCode + Seperator;
	DataLine += stClient.Name + Seperator;
	DataLine += stClient.Phone + Seperator;
	DataLine += to_string(stClient.AccountBalance);

	return DataLine;

}

bool FindClientByAccountNumber(vector<stClientData> vClients, stClientData& Client, string AccountNumber) {

	for (stClientData stClient : vClients) {

		if (stClient.AccountNumber == AccountNumber) {
			Client = stClient;
			return true;
		}

	}

	return false;

}

void SaveDataFromVectorToFile(vector<stClientData> vClients, string FileName) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {

		string DataLine;

		for (stClientData stClient : vClients) {

			if (stClient.MarkForDelete == false) {

				DataLine = ConvertRecordToLine(stClient);
				MyFile << DataLine << endl;

			}
		}

		MyFile.close();

	}


}

void GoBackToMainMenue() {

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ATM();

}

void PrintATMMenue() {

	system("cls");
	cout << "=========================================================\n";
	cout << "\t\t\tATM Main Menue Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] Quick Withdraw.\n";
	cout << "\t\t[2] Normal Withdraw.\n";
	cout << "\t\t[3] Deposit.\n";
	cout << "\t\t[4] Check Balance.\n";
	cout << "\t\t[5] Logout.\n";
	cout << "=========================================================\n";

}

double ReadPositiveAmount(string Message) {

	double Amount = 0.0;
	do {
		cout << Message;
		cin >> Amount;
	} while (Amount < 0);

	return Amount;
}

void Transaction(vector<stClientData>& vClients, string FileName, double TransactionAmount, bool TransactionType = true) {

	if (!TransactionType) {
		TransactionAmount = TransactionAmount * -1;  // if TransactionType false: Withdraw, true: Deposit
	}

	for (stClientData& stClient : vClients) {
		if (stClient.AccountNumber == CurrentClient.AccountNumber) {
			stClient.AccountBalance += TransactionAmount;
			CurrentClient.AccountBalance += TransactionAmount;
			break;
		}
	}

	SaveDataFromVectorToFile(vClients, FileName);

}

void DepositScreen(vector<stClientData>& vClients, string FileName) {

	cout << "=========================================================\n";
	cout << "\t\t\tDeposit Screen\n";
	cout << "=========================================================\n";

	double DepositAmount = ReadPositiveAmount("\nEnter a positive Deposit Amount? ");

	if (AskYesNo("\n\nAre you sure you want to perform this transaction? y/n ? ")) {

		Transaction(vClients, FileName, DepositAmount);
		cout << "\nDone Successfully. New balance is: " << CurrentClient.AccountBalance << endl;

	}


}

void CheckBalanceScreen() {
	cout << "=========================================================\n";
	cout << "\t\t\tCheck Balance Screen\n";
	cout << "=========================================================\n";

	cout << "Your Balance is " << CurrentClient.AccountBalance << '\n';
}

void Withdraw(vector<stClientData> vClients, string FileName, int WithdrawAmount) {


	if (WithdrawAmount > CurrentClient.AccountBalance) {

		cout << "\n\nAmount Exceeds your balance, make another choice.";
		cout << "\nPress Anykey to continue...";
		system("pause>0");
		ATM();
	}


	if (AskYesNo("\n\nAre you sure you want to perform this transaction? y/n ? ")) {

		Transaction(vClients, FileName, WithdrawAmount, false);
		cout << "\nDone Successfully. New balance is: " << CurrentClient.AccountBalance << endl;

	}

}

int ReadMultipleOf5Amount(string Message) {

	int Amount;
	do {
		cout << Message;
		cin >> Amount;
	} while (Amount % 5 != 0);

	return Amount;
}

void NormalWithdrawScreen(vector<stClientData> vClients, string FileName) {

	cout << "=========================================================\n";
	cout << "\t\t\tNormal Withdraw Screen\n";
	cout << "=========================================================\n";

	int Amount = ReadMultipleOf5Amount("\nEnter an amount multiple of 5's ? ");

	Withdraw(vClients, FileName, Amount);


}

void QuickWithdrawScreen(vector<stClientData> vClients, string FileName) {

	cout << "=========================================================\n";
	cout << "\t\t\tQuick Withdraw\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] 20  \t [2] 50\n";
	cout << "\t\t[3] 100 \t [4] 200\n";
	cout << "\t\t[5] 400 \t [6] 600\n";
	cout << "\t\t[7] 800 \t [8] 1000\n";
	cout << "\t\t[9] Exit\n";
	cout << "=========================================================\n";

	cout << "Your Balance is " << CurrentClient.AccountBalance << '\n';

	short Choose = ReadNumberInRange(1, 9, "Choose what to do from [1] to [8] ? ");

	int WithdrawAmount[8] = { 20, 50, 100, 200, 400, 600, 800, 1000 };

	if (Choose == 9) {

		return;
	}
	else {

		Withdraw(vClients, FileName, WithdrawAmount[Choose - 1]);

	}

}

void ATM() {

	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintATMMenue();
	short Choose = ReadNumberInRange(1, 5, "Choose what do you want to do? [1 to 5]? ");

	switch (Choose)
	{
	case 1:
		system("cls");
		QuickWithdrawScreen(vClients, ClientsFileName);
		GoBackToMainMenue();
		break;
	case 2:
		system("cls");
		NormalWithdrawScreen(vClients, ClientsFileName);
		GoBackToMainMenue();
		break;
	case 3:
		system("cls");
		DepositScreen(vClients, ClientsFileName);
		GoBackToMainMenue();
		break;
	case 4:
		system("cls");
		CheckBalanceScreen();
		GoBackToMainMenue();
		break;
	case 5:
		system("cls");
		Login();
		break;
	}

}

void PrintLogin() {

	system("cls");
	cout << "\n----------------------------------------\n";
	cout << "\tLogin Screen";
	cout << "\n----------------------------------------\n";

}

void Login() {

	PrintLogin();
	vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	while (true) {
		string AccountNumber = ReadString("Enter Account Number? ");
		string Pin = ReadString("Enter Pin? ");
		bool AccountNumberFound = FindClientByAccountNumber(vClients, CurrentClient, AccountNumber);

		if (AccountNumberFound && Pin == CurrentClient.PinCode) {

			ATM();
			break;

		}
		else {

			PrintLogin();
			cout << "Invalid Account Number/PinCode!\n";

		}
	}

}

int main() {

	Login();



}

