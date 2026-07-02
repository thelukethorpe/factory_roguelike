#ifndef CLIENT_APP_H
#define CLIENT_APP_H

class ClientApp
{
  public:
    ClientApp() = default;
    ~ClientApp() = default;
    ClientApp(const ClientApp &) = delete;
    ClientApp(ClientApp &&) = delete;
    ClientApp &operator=(const ClientApp &) = delete;
    ClientApp &operator=(ClientApp &&) = delete;
};

#endif
