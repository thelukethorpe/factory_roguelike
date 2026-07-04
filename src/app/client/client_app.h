#ifndef CLIENT_APP_H
#define CLIENT_APP_H

class ClientApp
{
  public:
    struct View
    {
    };

    ClientApp() = default;
    ~ClientApp() = default;
    ClientApp(const ClientApp &) = delete;
    ClientApp(ClientApp &&) = delete;
    ClientApp &operator=(const ClientApp &) = delete;
    ClientApp &operator=(ClientApp &&) = delete;

    [[nodiscard]] View view() const;

    template <typename InputEvent> void input(const typename InputEvent::Args &args);
};

#endif
