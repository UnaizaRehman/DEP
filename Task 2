#include <vector>
#include <iostream>
using namespace std;
class Contacts
{
    string name;
    string number;

public:
    Contacts(string name, string number)
    {
        this->name = name;
        this->number = number;
    }
    string getName()
    {
        return name;
    }
    string getNumber()
    {
        return number;
    }
    void setName(string name)
    {
        this->name = name;
    }
    void setNumber(string number)
    {
        this->number = number;
    }
    void print()
    {
        cout << "Name: " << name << endl;
        cout << "Number: " << number << endl;
    }
};
int main()
{
    std::vector<Contacts> contacts;
    int choice;
    string name, no;
    do
    {
        cout << "1. Add Contact" << endl;
        cout << "2. Delete Contact" << endl;
        cout << "3. Search Contact" << endl;
        cout << "4. Print All Contacts" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter name: ";
            cin >> name;
            cout << "\nEnter number: ";
            cin >> no;
            contacts.push_back(Contacts(name, no));
            break;
        case 2:
            cout << "Enter name of contact to delete: ";
            cin >> name;
            for (int i = 0; i < contacts.size(); i++)
            {
                if (contacts[i].getName() == name)
                {
                    contacts.erase(contacts.begin() + i);
                    break;
                }
                else{
                    cout<<"Contact not found"<<endl;
                }
            }
            break;
        case 3:
            cout << "Enter name of contact to search: ";
            cin >> name;
            for (int i = 0; i < contacts.size(); i++)
            {
                if (contacts[i].getName() == name)
                {
                    contacts[i].print();
                    break;
                }
                else{
                    cout<<"Contact not found"<<endl;
                }
            }
            break;
        case 4:
            for (int i = 0; i < contacts.size(); i++)
            {
                if(contacts.size() == 0){
                    cout<<"No contacts found"<<endl;
                }
                else{
                    contacts[i].print();
                }
            }
            break;
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice" << endl;
            break;
        }
    } while (choice != 5);

    return 0;
}
