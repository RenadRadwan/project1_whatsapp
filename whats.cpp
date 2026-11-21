#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>
using namespace std;

string getCurrentTime() {
    time_t now = time(nullptr);
    struct tm* local = localtime(&now);
    char buffer[64];
    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local) == 0) {
        return "";
    }
    return string(buffer);
}

// ========================
//       USER CLASS
// ========================
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;
    
public:
    User() {
        username = "";
        password = "";
        phoneNumber = "";
        status = "Offline";
        lastSeen = getCurrentTime();
    }
    
    User(string uname, string pwd, string phone) {
        username = uname;
        password = pwd;
        phoneNumber = phone;
        status = "Offline";
        lastSeen = getCurrentTime();
    }
    
    string getUsername() const {
        return username;
    }
    
    string getPhoneNumber() const {
        return phoneNumber;
    }
    
    string getStatus() const {
        return status;
    }
    
    string getLastSeen() const {
        return lastSeen;
    }
    
    void setStatus(string newStatus) {
        status = newStatus;
    }
    
    void setPhoneNumber(string phone) {
        phoneNumber = phone;
    }
    
    void updateLastSeen() {
        lastSeen = getCurrentTime();
    }
    
    bool checkPassword(string pwd) const {
        return pwd == password;
    }
    
    void changePassword(string newPwd) {
        password = newPwd;
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;
    
public:
    Message() {
        // TODO: Implement default constructor
    }
    
    Message(string sndr, string cntnt) {
        // TODO: Implement parameterized constructor
    }
    
    string getContent() const {
        // TODO: Implement getter
        return "";
    }
    
    string getSender() const {
        // TODO: Implement getter
        return "";
    }
    
    string getTimestamp() const {
        // TODO: Implement getter
        return "";
    }
    
    string getStatus() const {
        // TODO: Implement getter
        return "";
    }
    
    Message* getReplyTo() const {
        // TODO: Implement getter
        return nullptr;
    }
    
    void setStatus(string newStatus) {
        // TODO: Implement setter
    }
    
    void setReplyTo(Message* msg) {
        // TODO: Implement setter
    }
    
    void updateTimestamp() {
        // TODO: Implement timestamp update
    }
    
    void display() const {
        // TODO: Implement message display
    }
    
    void addEmoji(string emojiCode) {
        // TODO: Implement emoji support
    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;
    
public:
    Chat() {
        // TODO: Implement default constructor
    }
    
    Chat(vector<string> users, string name) {
        // TODO: Implement parameterized constructor
    }
    
    void addMessage(const Message& msg) {
        // TODO: Implement message addition
    }
    
    bool deleteMessage(int index, const string& username) {
        // TODO: Implement message deletion
        return false;
    }
    
    virtual void displayChat() const {
        // TODO: Implement chat display
    }
    
    vector<Message> searchMessages(string keyword) const {
        // TODO: Implement message search
        return {};
    }
    
    void exportToFile(const string& filename) const {
        // TODO: Implement export to file
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;
    
public:
    PrivateChat(string u1, string u2) {
        // TODO: Implement constructor
    }
    
    void displayChat() const override {
        // TODO: Implement private chat display
    }
    
    void showTypingIndicator(const string& username) const {
        // TODO: Implement typing indicator
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;
    
public:
    GroupChat(vector<string> users, string name, string creator) {
        // TODO: Implement constructor
    }
    
    void addAdmin(string newAdmin) {
        // TODO: Implement add admin
    }
    
    bool removeParticipant(const string& admin, const string& userToRemove) {
        // TODO: Implement remove participant
        return false;
    }
    
    bool isAdmin(string username) const {
        // TODO: Implement admin check
        return false;
    }
    
    bool isParticipant(string username) const {
        // TODO: Implement participant check
        return false;
    }
    
    void setDescription(string desc) {
        // TODO: Implement set description
    }
    
    void displayChat() const override {
        // TODO: Implement group chat display
    }
    
    void sendJoinRequest(const string& username) {
        // TODO: Implement join request
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
class WhatsApp {
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;
    
    int findUserIndex(string username) const {
        // TODO: Implement user search
        return -1;
    }
    
    bool isLoggedIn() const {
        // TODO: Implement login check
        return false;
    }
    
    string getCurrentUsername() const {
        // TODO: Implement get current user
        return "";
    }
    
public:
    WhatsApp() : currentUserIndex(-1) {}

    ~WhatsApp() {
        for (Chat* chat : chats) {
            delete chat;
        }
    }

    void signUp() {
        // TODO: Implement user registration
    }
    
    void login() {
        // TODO: Implement user login
    }
    
    void startPrivateChat() {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }
        string other;
        cout << "Enter username to chat with: ";
        if (!(cin >> other)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            return;
        }
        if (findUserIndex(other) == -1) {
            cout << "User not found!\n";
            return;
        }
        if (other == getCurrentUsername()) {
            cout << "You cannot chat with yourself!\n";
            return;
        }
        chats.push_back(new PrivateChat(getCurrentUsername(), other));
        cout << "Private chat created successfully!\n";
    }

    void createGroup() {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }
        string groupName;
        cin.ignore(10000, '\n');
        cout << "Enter group name: ";
        getline(cin, groupName);
        if (groupName.empty()) {
            cout << "Group name cannot be empty!\n";
            return;
        }
        int count;
        cout << "Enter number of members to add: ";
        if (!(cin >> count)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid number!\n";
            return;
        }
        vector<string> members;
        members.push_back(getCurrentUsername());
        for (int i = 0; i < count; i++) {
            string name;
            cout << "Enter member " << i + 1 << ": ";
            if (!(cin >> name)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input!\n";
                return;
            }
            if (findUserIndex(name) == -1) {
                cout << "User not found: " << name << "\n";
                return;
            }
            bool alreadyAdded = false;
            for (size_t j = 0; j < members.size(); j++) {
                if (members[j] == name) {
                    alreadyAdded = true;
                }
            }
            if (alreadyAdded) {
                cout << name << " is already in the group.\n";
            } else {
                members.push_back(name);
            }
        }
        if (members.size() < 3) {
            cout << "A group needs at least 2 other members!\n";
            return;
        }
        chats.push_back(new GroupChat(members, groupName, getCurrentUsername()));
        cout << "Group created successfully!\n";
    }

    void viewChats() const {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }
        string me = getCurrentUsername();
        vector<size_t> mine;
        for (size_t i = 0; i < chats.size(); i++) {
            if (chats[i]->isParticipant(me)) {
                mine.push_back(i);
            }
        }
        if (mine.empty()) {
            cout << "You have no chats yet.\n";
            return;
        }
        cout << "\n--- Your Chats ---\n";
        for (size_t i = 0; i < mine.size(); i++) {
            cout << i + 1 << ". Chat\n";
        }
        int choice;
        cout << "Select chat number: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice!\n";
            return;
        }
        cin.ignore(10000, '\n');
        if (choice < 1 || choice > (int)mine.size()) {
            cout << "Invalid choice!\n";
            return;
        }
        Chat* chat = chats[mine[choice - 1]];
        chat->displayChat();
        string text;
        while (true) {
            cout << "Message (/back to return): ";
            if (!getline(cin, text)) {
                cin.clear();
                return;
            }
            if (text == "/back") {
                return;
            }
            if (text.empty()) {
                continue;
            }
            chat->addMessage(Message(me, text));
        }
    }
    
    void logout() {
        // TODO: Implement logout
    }
    
    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;
                
                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;
                
                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
};

void testUser() {
    cout << "=== Member 1 Self-Test (SCRUM-22) ===" << endl;
    string t = getCurrentTime();
    cout << "getCurrentTime() -> \"" << t << "\"" << endl;
    assert(!t.empty() && "getCurrentTime must return non-empty string");
    User def;
    assert(def.getUsername() == "");
    assert(def.getPhoneNumber() == "");
    assert(!def.getLastSeen().empty());
    cout << "Default ctor OK (lastSeen=" << def.getLastSeen() << ")" << endl;
    User alice("alice", "pass123", "555-0101");
    assert(alice.getUsername() == "alice");
    assert(alice.getPhoneNumber() == "555-0101");
    assert(alice.getStatus() == "Offline");
    assert(!alice.getLastSeen().empty());
    cout << "Parameterized ctor OK" << endl;
    alice.setStatus("Online");
    assert(alice.getStatus() == "Online");
    alice.setPhoneNumber("555-9999");
    assert(alice.getPhoneNumber() == "555-9999");
    cout << "Setters OK (FR4)" << endl;
    assert(alice.checkPassword("pass123") == true);
    assert(alice.checkPassword("wrong") == false);
    alice.changePassword("newpass1");
    assert(alice.checkPassword("newpass1") == true);
    assert(alice.checkPassword("pass123") == false);
    cout << "checkPassword/changePassword OK (FR6)" << endl;
    alice.updateLastSeen();
    assert(!alice.getLastSeen().empty());
    cout << "updateLastSeen OK (FR5, lastSeen=" << alice.getLastSeen() << ")" << endl;
    cout << "ALL MEMBER 1 TESTS PASSED" << endl;
}

// ========================
//          MAIN
// ========================
int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--test") {
        testUser();
        return 0;
    }
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}
