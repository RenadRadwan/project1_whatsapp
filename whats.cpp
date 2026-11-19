#include <iostream>
#include <vector>
#include <string>
using namespace std;

class GroupChat : public Chat{
    private:
    vector<string>admins;
    string description;

    public:
    GroupChat(vector<string>parts, string name, string creator) : Chat(parts,name){
        description="";
        if(!isParticipant(creator)){
            participants.push_back(creator);

        }
        admins.push_back(creator);
    }
    bool isParticipant(string username){
        for(int i=0;i < (int)participants.size();i++){
            if(participants[i] == username){
                return true;
            }

        }
        return false;
    }
    bool isAdmin(string username){
        for(int i=0;i < (int)admins.size();i++){
            if(admins[i] == username){
                return true;
            }
        }
        return false;
    }
    void addAdmin(string newAdmin){
        if(isParticipant(newAdmin) && !isAdmin(newAdmin)){
            admins.push_back(newAdmin);
        }
    }
    
    bool removeParticipant(string admin, string userToRemove){
        if(!isAdmin(admin)){
            return false;
        }
        int index = -1;
        for(int i=0; i<(int)participants.size();i++){
            if(participants[i] == userToRemove){
                index=i;
                break;
            }
        }
        if(index == -1){
            return false;
        }
        participants.erase(participants.begin()+ index);
        for(int i=0; i <(int)admins.size(); i++){
            if(admins[i] == userToRemove){
                admins.erase(admins.begin()+ i);
                break;
            }
        }
        return true;
    }
};