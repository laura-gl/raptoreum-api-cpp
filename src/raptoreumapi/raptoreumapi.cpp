/**
 * @file    raptoreumapi.cpp
 * @author  Krzysztof Okupski / Jesús Sánchez / Laura Mejía
 * @date    29.10.2014 / 09.10.2022
 * @version 1.0
 *
 * Implementation of a C++ wrapper for communication with
 * a running instance of Raptoreum daemon over JSON-RPC.
 */

#include "raptoreumapi.h"

#include <string>
#include <stdexcept>
#include <cmath>

#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>

using jsonrpc::Client;
using jsonrpc::JSONRPC_CLIENT_V1;

using jsonrpc::HttpClient;
using jsonrpc::JsonRpcException;

using Json::Value;
using Json::ValueIterator;

using std::map;
using std::string;
using std::vector;


RaptoreumAPI::RaptoreumAPI(const string& user, const string& password, const string& host, int port, int httpTimeout)
: httpClient(new HttpClient("https://" + user + ":" + password + "@" + host + ":" + IntegerToString(port))),
  client(new Client(*httpClient, JSONRPC_CLIENT_V1))
{
    httpClient->SetTimeout(httpTimeout);
}

RaptoreumAPI::~RaptoreumAPI()
{
    delete client;
    delete httpClient;
}

Value RaptoreumAPI::sendcommand(const string& command, const Value& params){    
    Value result;

    try{
		result = client->CallMethod(command, params);
	}
	catch (JsonRpcException& e){
		RaptoreumException err(e.GetCode(), e.GetMessage());
		throw err;
	}

	return result;
}


string RaptoreumAPI::IntegerToString(int num){
	std::ostringstream ss;
	ss << num;
	return ss.str();
}

std::string RaptoreumAPI::RoundDouble(double num)
{
	std::ostringstream ss;
	ss.precision(8);

	ss << std::fixed << num;
	return ss.str();
}

/* === Accounting === */

double RaptoreumAPI::getAddressBalance(const string& account) {
	string command = "getaddressbalance";
	Value params, result;
	params.append(account);
	result = sendcommand(command, params);

	return result["balance"].asDouble()/100000000;
}

vector<string> RaptoreumAPI::getAddressOnlyTxs(const string& address) {
	string command = "getaddresstxids";
	Value params, resultRpc;
	params.append(address);
	resultRpc = sendcommand(command, params);
	vector<string> result;

	for(unsigned i = 0; i < resultRpc.size(); ++i) {
		result.push_back(resultRpc[i].asString());
	}

	return result;
}

vector<gettransaction_t> RaptoreumAPI::getAddressTxs(const string& address, int count, int from) {

	vector<string> txIds = getAddressOnlyTxs(address);
	vector<gettransaction_t> result;
	gettransaction_t tx;
	
	for(unsigned i = 0; i < 5; ++i) {
		tx = getTransaction(txIds[i]);
		result.push_back(tx);
	}
	
	return result;
}

/* === Mining functions === */
// Probably dont work fine
mininginfo_t RaptoreumAPI::getMiningInfo() {
	string command = "getmininginfo";
	Value params, result;
	mininginfo_t ret;

	result = sendcommand(command, params);

	ret.blocks = result["blocks"].asInt();
	ret.currentblocksize = result["currentblocksize"].asInt();
	ret.currentblocktx = result["currentblocktx"].asInt();
	ret.difficulty = result["difficulty"].asDouble();
	ret.errors = result["errors"].asString();
	ret.genproclimit = result["genproclimit"].asInt();
	ret.networkhashps = result["networkhashps"].asDouble();
	ret.pooledtx = result["pooledtx"].asInt();
	ret.testnet = result["testnet"].asBool();
	ret.generate = result["generate"].asBool();
	ret.hashespersec = result["hashespersec"].asInt();

	return ret;
}

/* === Raw transaction calls === */
// Probably dont work fine
getrawtransaction_t RaptoreumAPI::getRawTransaction(const string& txid, int verbose) {
	string command = "getrawtransaction";
	Value params, result;
	getrawtransaction_t ret;

	params.append(txid);
	params.append(verbose);
	result = sendcommand(command, params);

	ret.hex = ((verbose == 0) ? result.asString() : result["hex"].asString());

	if(verbose != 0){
		ret.txid = result["txid"].asString();
		ret.version = result["version"].asInt();
		ret.locktime = result["locktime"].asInt();
		for (ValueIterator it = result["vin"].begin(); it != result["vin"].end();
				it++) {
			Value val = (*it);
			vin_t input;
			input.txid = val["txid"].asString();
			input.n = val["vout"].asUInt();
			input.scriptSig.assm = val["scriptSig"]["asm"].asString();
			input.scriptSig.hex = val["scriptSig"]["hex"].asString();
			input.sequence = val["sequence"].asUInt();
			ret.vin.push_back(input);
		}

		for (ValueIterator it = result["vout"].begin(); it != result["vout"].end();
				it++) {
			Value val = (*it);
			vout_t output;

			output.value = val["value"].asDouble();
			output.n = val["n"].asUInt();
			output.scriptPubKey.assm = val["scriptPubKey"]["asm"].asString();
			output.scriptPubKey.hex = val["scriptPubKey"]["hex"].asString();
			output.scriptPubKey.reqSigs = val["scriptPubKey"]["reqSigs"].asInt();

			output.scriptPubKey.type = val["scriptPubKey"]["type"].asString();
			for(ValueIterator it2 = val["scriptPubKey"]["addresses"].begin(); it2 != val["scriptPubKey"]["addresses"].end(); it2++){
				output.scriptPubKey.addresses.push_back((*it2).asString());
			}

			ret.vout.push_back(output);
		}
		ret.blockhash = result["blockhash"].asString();
		ret.confirmations = result["confirmations"].asUInt();
		ret.time = result["time"].asUInt();
		ret.blocktime = result["blocktime"].asUInt();
	}

	return ret;
}


gettransaction_t RaptoreumAPI::getTransaction(const string& tx) {
	string command = "getrawtransaction";
	Value params, result;
	gettransaction_t ret;
	params.append(tx);
	params.append(true);
	result = sendcommand(command, params);

	ret.amount = result["amount"].asDouble();
	ret.fee = result["fee"].asDouble();
	ret.confirmations = result["confirmations"].asInt();
	ret.blockhash = result["blockhash"].asString();
	ret.blockindex = result["blockindex"].asInt();
	ret.blocktime = result["blocktime"].asInt();
	ret.txid = result["txid"].asString();

	for (ValueIterator it = result["walletconflicts"].begin();
			it != result["walletconflicts"].end(); it++) {
		ret.walletconflicts.push_back((*it).asString());
	}

	ret.time = result["time"].asInt();
	ret.timereceived = result["timereceived"].asInt();

	for (ValueIterator it = result["details"].begin();
			it != result["details"].end(); it++) {
		Value val = (*it);
		transactiondetails_t tmp;
		tmp.account = val["account"].asString();
		tmp.address = val["address"].asString();
		tmp.category = val["category"].asString();
		tmp.amount = val["amount"].asDouble();
		tmp.vout = val["vout"].asInt();
		tmp.fee = val["fee"].asDouble();

		ret.details.push_back(tmp);
	}

	ret.hex = result["hex"].asString();

	return ret;
}
