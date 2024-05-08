#include <string>
using namespace std;

namespace messaging
{
    class sender
    {
    };
}

using namespace messaging;
struct withdraw
{
    string account;
    unsigned amount;
    mutable sender atm_queue;
    withdraw(string const &account_, unsigned amount_, sender atm_queue_)
        : account(account_), amount(amount_), atm_queue(atm_queue_)
    {
    }
};
struct withdraw_ok
{
};
struct withdraw_denied
{
};
struct cancel_withdrawal
{
    string account;
    unsigned amount;
    cancel_withdrawal(string const &account_, unsigned amount_)
        : account(account_), amount(amount_)
    {
    }
};
struct withdrawal_processed
{
    string account;
    unsigned amount;
    withdrawal_processed(string const &account_, unsigned amount_)
        : account(account_), amount(amount_)
    {
    }
};
struct card_inserted
{
    string account;
    explicit card_inserted(string const &account_)
        : account(account_)
    {
    }
};
struct digit_pressed
{
    char digit;
    explicit digit_pressed(char digit_)
        : digit(digit_)
    {
    }
};
struct clear_last_pressed
{
};
struct eject_card
{
};
struct withdraw_pressed
{
    unsigned amount;
    explicit withdraw_pressed(unsigned amount_)
        : amount(amount_)
    {
    }
};
struct cancel_pressed
{
};
struct issue_money
{
    unsigned amount;
    issue_money(unsigned amount_)
        : amount(amount_)
    {
    }
};
struct verify_pin
{
    string account;
    string pin;
    mutable sender atm_queue;
    verify_pin(string const &account_, string const &pin_, sender atm_queue_)
        : account(account_), pin(pin_), atm_queue(atm_queue_)
    {
    }
};
struct pin_verified
{
};
struct pin_incorrect
{
};
struct display_enter_pin
{
};
struct display_enter_card
{
};
struct display_insufficient_funds
{
};
struct display_withdrawal_cancelled
{
};
struct display_pin_incorrect_message
{
};
struct display_withdrawal_options
{
};
struct get_balance
{
    string account;
    mutable sender atm_queue;
    get_balance(string const &account_, sender atm_queue_)
        : account(account_), atm_queue(atm_queue_)
    {
    }
};
struct balance
{
    unsigned amount;
    explicit balance(unsigned amount_)
        : amount(amount_)
    {
    }
};
struct display_balance
{
    unsigned amount;
    explicit display_balance(unsigned amount_)
        : amount(amount_)
    {
    }
};
struct balance_pressed
{
};

int main()
{
    return 0;
}
