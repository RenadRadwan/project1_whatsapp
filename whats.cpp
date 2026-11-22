#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cassert>
#include <fstream>
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
        sender = "";
        content = "";
        timestamp = "";
        status = "";
        replyTo = nullptr;
    }

    Message(string sndr, string cntnt) {
        sender = sndr;
        content = cntnt;
        timestamp = getCurrentTime();
        status = "sent";
        replyTo = nullptr;
    }

    string getContent() const {
        return content;
    }

    string getSender() const {
        return sender;
    }

    string getTimestamp() const {
        return timestamp;
    }

    string getStatus() const {
        return status;
    }

    Message* getReplyTo() const {
        return replyTo;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void setReplyTo(Message* msg) {
        replyTo = msg;
    }

    void updateTimestamp() {
        timestamp = getCurrentTime();
    }

    void display() const {
        if (replyTo != nullptr) {
            cout << "Reply to " << replyTo->getSender() << ": \""
                 << replyTo->getContent() << "\"" << endl;
        }
        cout << "[" << timestamp << "] " << sender << ": " << content;
        if (!status.empty()) {
            cout << " (" << status << ")";
        }
        cout << endl;
    }

    void addEmoji(string emojiCode) {
        string emoji = "";
        if (emojiCode == ":)") {
            emoji = "🙂";
        } else if (emojiCode == ":(") {
            emoji = "🙁";
        } else if (emojiCode == ":D") {
            emoji = "😀";
        } else if (emojiCode == "<3") {
            emoji = "❤️";
        } else if (emojiCode == ":thumbsup:") {
            emoji = "👍";
        }
        if (emoji.empty()) {
            return;
        }
        size_t pos = content.find(emojiCode);
        if (pos != string::npos) {
            while (pos != string::npos) {
                content.replace(pos, emojiCode.size(), emoji);
                pos = content.find(emojiCode, pos + emoji.size());
            }
        } else {
            if (!content.empty()) {
                content += " ";
            }
            content += emoji;
        }
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
        chatName = "";
        participants = {};
        messages = {};
        messages.reserve(1000);
    }

    Chat(vector<string> users, string name) {
        participants = users;
        chatName = name;
        messages.reserve(1000);
    }

    virtual ~Chat() {}

    string getChatName() const {
        return chatName;
    }

    bool hasParticipant(const string& username) const {
        for (size_t i = 0; i < participants.size(); i++) {
            if (participants[i] == username) {
                return true;
            }
        }
        return false;
    }

    void addMessage(const Message& msg) {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username) {
        if (index < 0 || index >= (int)messages.size()) {
            return false;
        }
        if (messages[index].getSender() != username) {
            return false;
        }
        Message* target = &messages[index];
        for (size_t i = 0; i < messages.size(); i++) {
            Message* reply = messages[i].getReplyTo();
            if (reply == target) {
                messages[i].setReplyTo(nullptr);
            } else if (reply != nullptr && !messages.empty() &&
                       reply >= &messages.front() && reply <= &messages.back() &&
                       reply > target) {
                messages[i].setReplyTo(reply - 1);
            }
        }
        messages.erase(messages.begin() + index);
        return true;
    }

    virtual void displayChat() const {
        cout << "Chat: " << chatName << endl;
        if (messages.empty()) {
            cout << "No messages yet." << endl;
            return;
        }
        for (size_t i = 0; i < messages.size(); i++) {
            messages[i].display();
        }
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> result;
        for (size_t i = 0; i < messages.size(); i++) {
            if (messages[i].getContent().find(keyword) != string::npos) {
                result.push_back(messages[i]);
            }
        }
        return result;
    }

    void exportToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            return;
        }
        out << "Chat: " << chatName << "\n";
        out << "Participants: ";
        for (size_t i = 0; i < participants.size(); i++) {
            if (i > 0) {
                out << ", ";
            }
            out << participants[i];
        }
        out << "\n";
        for (size_t i = 0; i < messages.size(); i++) {
            out << "[" << messages[i].getTimestamp() << "] "
                << messages[i].getSender() << ": "
                << messages[i].getContent() << " ("
                << messages[i].getStatus() << ")\n";
        }
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
    PrivateChat(string u1, string u2)
        : Chat(vector<string>{u1, u2}, "Chat between " + u1 + " and " + u2),
          user1(u1), user2(u2) {}

    void displayChat() const override {
        cout << "Chat between " << user1 << " and " << user2 << endl;
        if (messages.empty()) {
            cout << "No messages yet." << endl;
            return;
        }
        for (size_t i = 0; i < messages.size(); i++) {
            messages[i].display();
        }
    }

    void showTypingIndicator(const string& username) const {
        cout << "[" << username << "] is typing..." << endl;
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
    GroupChat(vector<string> users, string name, string creator) : Chat(users, name) {
        description = "";
        if (!isParticipant(creator)) {
            participants.push_back(creator);
        }
        admins.push_back(creator);
    }

    void addAdmin(string newAdmin) {
        if (isParticipant(newAdmin) && !isAdmin(newAdmin)) {
            admins.push_back(newAdmin);
        }
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        if (!isAdmin(admin)) {
            return false;
        }
        int index = -1;
        for (int i = 0; i < (int)participants.size(); i++) {
            if (participants[i] == userToRemove) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return false;
        }
        participants.erase(participants.begin() + index);
        for (int i = 0; i < (int)admins.size(); i++) {
            if (admins[i] == userToRemove) {
                admins.erase(admins.begin() + i);
                break;
            }
        }
        return true;
    }

    bool isAdmin(string username) const {
        for (size_t i = 0; i < admins.size(); i++) {
            if (admins[i] == username) {
                return true;
            }
        }
        return false;
    }

    bool isParticipant(string username) const {
        for (size_t i = 0; i < participants.size(); i++) {
            if (participants[i] == username) {
                return true;
            }
        }
        return false;
    }

    void setDescription(string desc) {
        description = desc;
    }

    void displayChat() const override {
        cout << "Group: " << chatName << endl;
        if (!description.empty()) {
            cout << "Description: " << description << endl;
        }
        cout << "Participants: ";
        for (size_t i = 0; i < participants.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << participants[i];
        }
        cout << endl;
        cout << "Admins: ";
        for (size_t i = 0; i < admins.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << admins[i];
        }
        cout << endl;
        if (messages.empty()) {
            cout << "No messages yet." << endl;
            return;
        }
        for (size_t i = 0; i < messages.size(); i++) {
            messages[i].display();
        }
    }

    void sendJoinRequest(const string& username) {
        if (isParticipant(username)) {
            cout << username << " is already a participant in " << chatName << "." << endl;
            return;
        }
        cout << username << " requested to join " << chatName << "." << endl;
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
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                return (int)i;
            }
        }
        return -1;
    }

    bool isLoggedIn() const {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const {
        if (isLoggedIn()) {
            return users[currentUserIndex].getUsername();
        }
        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    ~WhatsApp() {
        for (size_t i = 0; i < chats.size(); i++) {
            delete chats[i];
        }
    }

    void signUp() {
        string username, password, phone;
        cout << "\n--- Sign Up ---\n";
        cout << "Enter username: ";
        cin >> username;
        if (findUserIndex(username) != -1) {
            cout << "Username already exists!\n";
            return;
        }
        cout << "Enter password: ";
        cin >> password;
        if (password.size() < 6) {
            cout << "Password must be at least 6 characters!\n";
            return;
        }
        cout << "Enter phone number: ";
        cin >> phone;
        users.push_back(User(username, password, phone));
        cout << "Account created successfully!\n";
    }

    void login() {
        string username, password;
        cout << "\n--- Login ---\n";
        cout << "Enter username: ";
        cin >> username;
        int index = findUserIndex(username);
        if (index == -1) {
            cout << "User not found!\n";
            return;
        }
        cout << "Enter password: ";
        cin >> password;
        if (users[index].checkPassword(password)) {
            currentUserIndex = index;
            users[currentUserIndex].setStatus("Online");
            users[currentUserIndex].updateLastSeen();
            cout << "Login successful! Welcome " << username << "\n";
        } else {
            cout << "Incorrect password!\n";
        }
    }

    void startPrivateChat() {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }
        string otherUsername;
        cout << "Enter username to chat with: ";
        if (!(cin >> otherUsername)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n";
            return;
        }
        if (findUserIndex(otherUsername) == -1) {
            cout << "User not found!\n";
            return;
        }
        string currentUsername = getCurrentUsername();
        if (currentUsername == otherUsername) {
            cout << "You cannot chat with yourself!\n";
            return;
        }
        chats.push_back(new PrivateChat(currentUsername, otherUsername));
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
        int numberOfUsers;
        cout << "Enter number of other participants: ";
        if (!(cin >> numberOfUsers)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid number!\n";
            return;
        }
        if (numberOfUsers < 1) {
            cout << "A group needs at least 2 participants!\n";
            return;
        }
        vector<string> participants;
        string creator = getCurrentUsername();
        participants.push_back(creator);
        for (int i = 0; i < numberOfUsers; i++) {
            string username;
            cout << "Enter participant " << i + 1 << ": ";
            cin >> username;
            if (findUserIndex(username) == -1) {
                cout << "User does not exist. Try again.\n";
                i--;
                continue;
            }
            bool duplicate = false;
            for (size_t j = 0; j < participants.size(); j++) {
                if (participants[j] == username) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) {
                cout << "This user is already in the group!\n";
                i--;
                continue;
            }
            participants.push_back(username);
        }
        if (participants.size() < 2) {
            cout << "A group needs at least 2 participants!\n";
            return;
        }
        chats.push_back(new GroupChat(participants, groupName, creator));
        cout << "Group created successfully!\n";
    }

    void viewChats() const {
        if (!isLoggedIn()) {
            cout << "Please login first!\n";
            return;
        }
        string username = getCurrentUsername();
        vector<size_t> mine;
        for (size_t i = 0; i < chats.size(); i++) {
            if (chats[i]->hasParticipant(username)) {
                mine.push_back(i);
            }
        }
        if (mine.empty()) {
            cout << "You have no chats yet.\n";
            return;
        }
        cout << "\n--- Your Chats ---\n";
        for (size_t i = 0; i < mine.size(); i++) {
            cout << i + 1 << ". " << chats[mine[i]]->getChatName() << "\n";
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
            chat->addMessage(Message(username, text));
        }
    }

    void logout() {
        if (!isLoggedIn()) {
            cout << "No user is logged in!\n";
            return;
        }
        users[currentUserIndex].updateLastSeen();
        users[currentUserIndex].setStatus("Offline");
        cout << "Goodbye, " << getCurrentUsername() << "!\n";
        currentUserIndex = -1;
    }

    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n===== WhatsApp =====\n";
                cout << "1. Login\n";
                cout << "2. Sign Up\n";
                cout << "3. Exit\n";
                cout << "Choice: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid choice!\n";
                    continue;
                }
                if (choice == 1) {
                    login();
                } else if (choice == 2) {
                    signUp();
                } else if (choice == 3) {
                    cout << "Exiting WhatsApp...\n";
                    break;
                } else {
                    cout << "Invalid choice!\n";
                }
            } else {
                cout << "\n===== WhatsApp Menu =====\n";
                cout << "1. Start Private Chat\n";
                cout << "2. Create Group\n";
                cout << "3. View Chats\n";
                cout << "4. Logout\n";
                cout << "Choice: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid choice!\n";
                    continue;
                }
                if (choice == 1) {
                    startPrivateChat();
                } else if (choice == 2) {
                    createGroup();
                } else if (choice == 3) {
                    viewChats();
                } else if (choice == 4) {
                    logout();
                } else {
                    cout << "Invalid choice!\n";
                }
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
