namespace messaging
{
    class receiver
    {
    public:
        operator sender()
        {
            return sender(&q);
        }
        dispatcher wait()
        {
            return dispatcher(&q);
        }

    private:
        queue q;
    };
}
