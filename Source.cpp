
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowLoginScreen();
void ShowMainMenue();
void ShowMangeUserMenue();
void AddNewUser();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct sUser
{
    string UserName;
    string Password;
    int Permation = 0;
    bool MarkForDelete = false;

};

enum enMainMenuePermitions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16,
    pManageUsers = 32
};
enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eMangeUsers = 6, eExit = 7
};

enum enMangeUsersMenueOptions
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUsers = 4,
    eFindUser = 5, eMainMenue = 6
};

sUser CurruntUser;

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

//---------------------Users Functions -----------------------//

void GoBackToMangeMenue()
{
    cout << "\n\nPress any key to go back to Mange Menue...";
    system("pause>0");
    ShowMangeUserMenue();
}

string UserConvertRecordToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permation);

    return stUserRecord;
}

sUser UserConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUserData;
    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permation = stod(vUserData[2]);//cast string to double

    return User;
}

bool UserExistsByUserName(string UserName, string FileName)
{

    vector <sUser> vUser;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = UserConvertLinetoRecord(Line);
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
            vUser.push_back(User);
        }

        MyFile.close();

    }
    return false;
}

short GiveUserPermation() {

    short Permation = 0;

    char access;

    cout << "Do you want to give access to : ";

    cout << "\nshow Client List? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 1;

    cout << "\nAdd New Client? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 2;

    cout << "\nDelete Client ? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 4;

    cout << "\nUpdate Client ? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 8;

    cout << "\nFind Client ? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 16;

    cout << "\nMange Users ? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        Permation += 32;

    cout << "\nUser Added Successfully, do you want? y/n?\n";
    cin >> access;
    if (access == 'Y' || access == 'y')
        AddNewUser();

    else {

        return Permation;
    }
    return Permation;

}

sUser ReadNewUser()
{
    sUser User;
    cout << "Enter User Name? ";
    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "\n User with [" << User.UserName << "] already exists, Enter another User Name? ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, User.Password);

    char access;
    cout << "Do you want to give him full access Y/N ? ";
    cin >> access;

    if (access == 'Y' || access == 'y')
    {
        User.Permation = -1;
    }
    else {

        User.Permation = GiveUserPermation();

    }
    return User;
}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, UserConvertRecordToLine(User));
}

vector <sUser> LoadUserDataFromFile(string FileName)
{
    vector <sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = UserConvertLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permation;
}

void ShowAllUsersScreen()
{
    vector <sUser> vUsers = LoadUserDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\ Users List (" << vUsers.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "UserName";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permitions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sUser User : vUsers)
        {

            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    AddNewUser();
}

bool FindUsertByUserName(string UserName, vector <sUser> vUsers, sUser& User)
{
    for (sUser C : vUsers)
    {
        if (C.UserName == UserName)
        {
            User = C;
            return true;
        }

    }
    return false;
}

bool CheckIfUserNameAndPasswordExists(vector <sUser> vUsers, sUser& User, string UserName, string Password) {

    for (sUser C : vUsers)
    {
        if (C.UserName == UserName && C.Password == Password)
        {
            User = C;
            return true;
        }

    }
    return false;


}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nUser Name   : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermation       : " << User.Permation;
    cout << "\n-----------------------------------\n";

}

vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sUser C : vUsers)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = UserConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vUsers;
}

bool MarkUserForDeleteByUserName(string UserName, vector <sUser>& vUsers)
{

    for (sUser& C : vUsers)
    {

        if (C.UserName == UserName)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

bool DeleteUserByUserName(string UserName, vector <sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUsertByUserName(UserName, vUsers, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadUserDataFromFile(UsersFileName);

            cout << "\n\n User Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\n User with  UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

string ReadUserName()
{
    string UserName = "";

    cout << "\nPlease enter UserName? ";
    cin >> UserName;
    return UserName;

}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\t Delete User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUserDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    while (UserName == "Admin")
    {
        cout << "You Can't delete Admin";
        UserName = ReadUserName();
    }

    DeleteUserByUserName(UserName, vUser);

}

sUser ChangeUserRecord(string UserName)
{
    sUser User;

    User.UserName = UserName;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Password);

    cout << "\n";
    User.Permation = GiveUserPermation();

    return User;

}

bool UpdateClientByAccountNumber(string UserName, vector <sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUsertByUserName(UserName, vUsers, User))
    {

        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& C : vUsers)
            {
                if (C.UserName == UserName)
                {
                    C = ChangeUserRecord(UserName);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with  UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadUserDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    while (UserName == "Admin")
    {
        cout << "You Can't delete Admin";
        UserName = ReadUserName();
    }

    UpdateClientByAccountNumber(UserName, vUsers);

}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUsers = LoadUserDataFromFile(UsersFileName);
    sUser User;
    string UserName = ReadUserName();
    if (FindUsertByUserName(UserName, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with UserName[" << UserName << "] is not found!";
}

void ReadUserNameAndPassword(sUser& User) {

    cout << "\nPlease Enter Your UserName ? \n";
    cin >> User.UserName;
    cout << "\nPlease Enter Your Password ? \n";
    cin >> User.Password;
}

void PerfromMangeMenueOption(enMangeUsersMenueOptions MangeUsersMenueOptions) {

    switch (MangeUsersMenueOptions)
    {
    case enMangeUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        GoBackToMangeMenue();
        break;
    }
    case enMangeUsersMenueOptions::eAddNewUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToMangeMenue();
        break;
    }
    case enMangeUsersMenueOptions::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToMangeMenue();
        break;
    }
    case enMangeUsersMenueOptions::eUpdateUsers:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToMangeMenue();
        break;
    }
    case enMangeUsersMenueOptions::eFindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToMangeMenue();
        break;
    }
    case enMangeUsersMenueOptions::eMainMenue:
    {
        system("cls");
        GoBackToMainMenue();
        break;
    }

    }
}

void ShowMangeUserMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMange Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Users List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromMangeMenueOption((enMangeUsersMenueOptions)ReadMainMenueOption());
}

//-----------------------------------------------------------------//
void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

bool CheckPermation(enMainMenuePermitions PermitionLevel) {

    if ((CurruntUser.Permation) == enMainMenuePermitions::eAll)
    {
        return true;
    }

    if ((CurruntUser.Permation & PermitionLevel) == PermitionLevel)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, short Permation)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        if (CheckPermation((enMainMenuePermitions)1))
        {
            ShowAllClientsScreen();
        }
        else
        {
            ShowAccessDeniedMessage();
        }
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");

        if (CheckPermation((enMainMenuePermitions)2))
        {
            ShowAddNewClientsScreen();
        }
        else
        {
            ShowAccessDeniedMessage();
        }

        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");


        if (CheckPermation((enMainMenuePermitions)4))
        {
            ShowDeleteClientScreen();
        }
        else
        {
            ShowAccessDeniedMessage();
        }

        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        if (CheckPermation((enMainMenuePermitions)8))
        {

            ShowUpdateClientScreen();
        }
        else
        {
            ShowAccessDeniedMessage();
        }

        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        if (CheckPermation((enMainMenuePermitions)16))
        {
            ShowFindClientScreen();
        }
        else
        {
            ShowAccessDeniedMessage();
        }


        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eMangeUsers:
        system("cls");
        if (CheckPermation((enMainMenuePermitions)32))
        {
            ShowMangeUserMenue();
        }
        else
        {
            ShowAccessDeniedMessage();
        }
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eExit:
        system("cls");
        ShowLoginScreen();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Mange Users.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), CurruntUser.Permation);
}

void ShowLoginScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\Login Screen\n";
    cout << "===========================================\n";


    vector <sUser> vUsers = LoadUserDataFromFile(UsersFileName);
    ReadUserNameAndPassword(CurruntUser);

    while (!CheckIfUserNameAndPasswordExists(vUsers, CurruntUser, CurruntUser.UserName, CurruntUser.Password))
    {
        cout << "ivalid UserName OR Password Please Try Agian...";
        ReadUserNameAndPassword(CurruntUser);
    }

    ShowMainMenue();
}

int main()

{
    ShowLoginScreen();
    system("pause>0");
    return 0;
}