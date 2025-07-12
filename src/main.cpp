#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <print>
#include <string>
#include <utility>
#include <vector>

// NOTE: Use it as a reference to most of the basics and some of the modern features of the language

class ContactClass {
  private:
    std::string name;
    std::string phone_number;

  public:
    ContactClass(std::string contact_name, std::string contact_number) : name(std::move(contact_name)), phone_number(std::move(contact_number)) {
        std::println("^^ building the object, or shall i say constructing...");
    }

    void print_details() const { // const -- makes a promise not to change the object it's called on
        std::println("Name: {}, Phone: {}", name, phone_number);
    }

    [[nodiscard]] const std::string& get_name() const {
        return name;
    }
    [[nodiscard]] const std::string& get_phone_number() const {
        return phone_number;
    }

    void set_phone_number(std::string new_phone_number) {
        phone_number = std::move(new_phone_number);
    }

    bool operator<=>(const ContactClass&) const = delete;
    bool operator==(const ContactClass&) const  = default;
};

void add_contact(std::vector<ContactClass>& contact_list) {
    std::string name_input, phone_num_input;

    std::print("Please enter name: ");
    std::getline(std::cin >> std::ws, name_input);

    std::print("Please enter phone number: ");
    std::getline(std::cin >> std::ws, phone_num_input);

    contact_list.emplace_back(name_input, phone_num_input);
    std::println("Contact {} added successfully!", name_input);
}

void view_contacts(const std::vector<ContactClass>& contact_list) {
    if (contact_list.empty()) {
        std::println("No contacts yet! :(");
        return;
    }

    for (const auto& contact : contact_list)
        contact.print_details();

    // std::println("{} -> {}", contact.name, contact.phone_number);
    // for (size_t i = 0; i < contact_list.size(); ++i)
    //     std::println("{}. {} -> {}", i + 1, contact_list[i].name, contact_list[i].phone_number);
}

void save_contacts(const std::vector<ContactClass>& contacts, const std::string& filename) {
    if (std::ofstream contacts_file(filename); !contacts_file.is_open()) {
        std::println("Error: Could not open file {} for saving.", filename);
    } else {
        for (const auto& contact : contacts) {
            contacts_file << "Name: " << contact.get_name() << '\n';
            contacts_file << "Number: " << contact.get_phone_number() << '\n';
        }
        contacts_file.close();
    }
}

void load_contacts(std::vector<ContactClass>& contacts, const std::string& filename) {
    std::ifstream contacts_file(filename);

    if (!contacts_file.is_open()) {
        std::println("Error: Could not open file {} for reading.", filename);
        return;
    } else {
        contacts.clear();

        std::string name_line, number_line, name, number;
        while (std::getline(contacts_file, name_line) && std::getline(contacts_file, number_line)) {
            if (name_line.starts_with("Name: ") && number_line.starts_with("Number: ")) {
                name   = name_line.substr(6);
                number = number_line.substr(8);

                contacts.emplace_back(name, number);
            } else {
                std::println("Warning: Malformed data in {}. Stopping load.", filename);
                break;
            }
        }
    }
    std::println("Contacts loaded from {}. Total contacts: {}.", filename, contacts.size());
}

void search_contacts(const std::vector<ContactClass>& contacts) {
    std::string query;
    std::print("Please enter a name or part of it: ");
    std::getline(std::cin, query);

    bool is_found = false;
    for (const auto& contact : contacts) {
        if ((contact.get_name().find(query)) != std::string::npos) {
            is_found = true;
            std::println("Name: {}, Phone: {}", contact.get_name(), contact.get_phone_number());
        }
    }
    if (!is_found)
        std::println("No contacts found!");
}

void edit_contact(std::vector<ContactClass>& contacts) {
    if (contacts.empty()) {
        std::println("No contacts to edit!");
        return;
    }
    std::string contact_to_edit, new_number;
    std::print("Enter the name of the contact you want to edit: ");
    std::getline(std::cin, contact_to_edit);

    bool is_edited = false;
    for (auto& contact : contacts) {
        if (contact.get_name() == contact_to_edit) {
            std::print("Enter the new number of {}: ", contact_to_edit);
            std::getline(std::cin, new_number);
            contact.set_phone_number(new_number);
            is_edited = true;
            break;
        }
    }
    if (!is_edited)
        std::println("No contact edited!");
}

void delete_contact(std::vector<ContactClass>& contacts) {
    if (contacts.empty()) {
        std::println("No contacts to delete!");
        return;
    }
    std::string contact_to_delete;
    std::print("Enter the name of the contact you want to delete: ");
    std::getline(std::cin, contact_to_delete);

    bool is_deleted = false;
    for (size_t i = 0; i < contacts.size(); ++i) {
        if (contacts[i].get_name() == contact_to_delete) {
            contacts.erase(contacts.begin() + static_cast<long>(i));
            is_deleted = true;
            break;
        }
    }
    if (!is_deleted)
        std::println("No contact deleted!");
}

enum class MenuChoice {
    Add = 1,
    View,
    Save,
    Load,
    Edit,
    Delete,
    Search,
    Exit
};

int main() {
    std::vector<ContactClass> contacts;

    load_contacts(contacts, "contacts.txt");

    int choice = 0;
    do {
        std::println(R"(
-----[ contact list menu]----
    1. Add Contact
    2. View Contacts
    3. Save Contacts
    4. Load Contacts
    5. Edit Contact
    6. Delete Contact
    7. Search Contacts
    8. Exit
-----------[ end ]-----------)");

        std::print("Enter your choice: ");
        while (!(std::cin >> choice)) {
            std::println("Invalid input! Please enter a number.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::print("Enter your choice: ");
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (static_cast<MenuChoice>(choice)) {
            case MenuChoice::Add: add_contact(contacts); break;
            case MenuChoice::View: view_contacts(contacts); break;
            case MenuChoice::Save: save_contacts(contacts, "contacts.txt"); break;
            case MenuChoice::Load: load_contacts(contacts, "contacts.txt"); break;
            case MenuChoice::Edit: edit_contact(contacts); break;
            case MenuChoice::Delete: delete_contact(contacts); break;
            case MenuChoice::Search: search_contacts(contacts); break;
            case MenuChoice::Exit: std::println("Exiting ..."); break;
            default: std::println("Unknown choice, please select a number between 1 and 8.");
        }
        std::println();

    } while (choice != 8);

    return 0;
}
