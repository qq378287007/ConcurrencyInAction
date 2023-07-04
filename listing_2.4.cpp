#include <thread>
#include <string>
#include <iostream>
using namespace std;

void open_document_and_display_gui(string const &filename) {}

bool done_editing() { return false; }

enum command_type
{
    open_new_document
};

struct user_command
{
    command_type type;

    user_command() : type(open_new_document) {}
};

user_command get_user_input() { return user_command(); }

string get_filename_from_user()
{
    static int n;
    return "file" + to_string(++n) + ".doc";
}

void process_user_input(user_command const &cmd) {}

void edit_document(string const &filename)
{
    cout << filename << endl;

    open_document_and_display_gui(filename);
    // while (!done_editing())
    for (int i = 0; i < 2; i++)
    {
        user_command cmd = get_user_input();
        if (cmd.type == open_new_document)
        {
            string const new_name = get_filename_from_user();
            thread t(edit_document, new_name);
            t.detach();
        }
        else
        {
            process_user_input(cmd);
        }
    }
}

int main()
{
    edit_document("bar.doc");
    this_thread::sleep_for(chrono::milliseconds(1));

    return 0;
}
