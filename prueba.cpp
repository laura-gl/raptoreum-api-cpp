#include <raptoreumapi/raptoreumapi.h>

int main()
{
    std::string username = "saltyaf4412";
    std::string password = "ds3856hfgtr4";
    std::string address = "saltyaf.saltydragon.io";
    std::string account = "RECWuQy4277G355SoPoNds6xdXFqENHf87";
    int port = 443;

    try
    {
        /* Constructor to connect to the bitcoin daemon */
        RaptoreumAPI rtm(username, password, address, port);
        /* Example method - getbalance */
        //std::cout << "Wallet balance: " << rtm.getaddressbalance(account) << std::endl;
        std::cout << "Wallet balance: " << rtm.getaddressbalance(account) << std::endl;
        //std::cout << "TX0: " << rtm.getaddresstxidsTest(account) << std::endl;
        //std::string txinfo = rtm.getaddresstxidsTest(account);

        std::vector<std::string> restxids = rtm.getaddresstxidsTest(account);

        for(unsigned i = 0; i< restxids.size(); ++i){
            std::cout << "id[" << i << "]: " << restxids[i] << std::endl;
            //std::cout << "amount: " << txinfo[i].amount << std::endl;
        }

    }
    catch(RaptoreumException e)
    {
        std::cerr << "PERROR: " + e.getMessage() << std::endl;
    }
}
