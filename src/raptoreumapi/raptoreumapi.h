/**
 * @file    raptoreumapi.h
 * @author  Krzysztof Okupski / Jesús Sánchez / Laura Mejía
 * @date    29.10.2014 / 09.10.2022
 * @version 1.0
 *
 * Declaration of a C++ wrapper for communication with
 * a running instance of Raptoreum daemon over JSON-RPC.
 */

#ifndef RAPTOREUM_API_H
#define RAPTOREUM_API_H

#include "types.h"
#include "exception.h"

namespace jsonrpc { class HttpClient; class Client; }

class RaptoreumAPI
{

private:
    jsonrpc::HttpClient * httpClient;
    jsonrpc::Client * client;

public:
    /* === Constructor and Destructor === */
    
    RaptoreumAPI(const std::string& user, const std::string& password, const std::string& host, int port, int httpTimeout = 50000);
    ~RaptoreumAPI();

    /* === Auxiliary functions === */
    
    Json::Value sendcommand(const std::string& command, const Json::Value& params);

    std::string IntegerToString(int num);    
    std::string RoundDouble(double num);

    /* === Accounting === */
    
    // Address balance
    double getAddressBalance(const std::string& account);

	// Vector with all Tx Ids (without more info)
    std::vector<std::string> getAddressOnlyTxs(const std::string& address);
    
    // Vector with all TxIds + advanced info
    std::vector<gettransaction_t> getAddressTxs(const std::string& address, int count = 10, int from = 0);
    
    // Get details from one tx
    gettransaction_t getTransaction(const std::string& tx);
    
    /* === Mining functions === */
    mininginfo_t getMiningInfo();
    

    /* === Low level calls === */
    getrawtransaction_t getRawTransaction(const std::string& txid, int verbose = 0);
};


#endif
