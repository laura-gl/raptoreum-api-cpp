/**
 * @file    types.h
 * @author  Krzysztof Okupski / Jesús Sánchez
 * @date    29.10.2014
 * @version 1.0
 *
 * Type definitions for the JSON-RPC C++ interface.
 */

#ifndef RAPTOREUM_API_TYPES_H
#define RAPTOREUM_API_TYPES_H

#include <string>
#include <vector>

#include <jsoncpp/json/json.h>

	/* === Account, address types === */
	struct accountinfo_t{
		std::string account;
		double amount;
		int confirmations;
	};

	struct addressinfo_t: accountinfo_t{
		std::string address;
		std::vector<std::string> txids;
	};

	struct transactioninfo_t: accountinfo_t{
		std::string address;
		std::string category;
		std::string blockhash;
		int blockindex;
		int blocktime;
		std::string txid;
		std::vector<std::string> walletconflicts;
		int time;
		int timereceived;
	};

	struct validateaddress_t{
		bool isvalid;
		std::string address;
		bool ismine;
		bool isscript;
		std::string pubkey;
		bool iscompressed;
		std::string account;
	};

	struct addressgrouping_t{
		std::string address;
		double balance;
		std::string account;
	};

	/* === Transactions === */
	struct transactiondetails_t{
		std::string account;
		std::string address;
		std::string category;
		double amount;
		int vout;
		double fee;
	};

	struct gettransaction_t{
		double amount;
		double fee;
		int confirmations;
		std::string blockhash;
		int blockindex;
		int blocktime;
		std::string txid;
		std::vector<std::string> walletconflicts;
		int time;
		int timereceived;
		std::vector<transactiondetails_t> details;
		std::string hex;
	};

	struct decodescript_t{
		std::string assm;
		std::string type;
		std::string p2sh;

		int reqSigs;
		std::vector<std::string> addresses;
	};

	/* decoderawtransaction return type */
	struct scriptSig_t{
		std::string assm;
		std::string hex;
	};

	struct scriptPubKey_t{
		std::string assm;
		std::string hex;
		int reqSigs;
		std::string type;
		std::vector<std::string> addresses;
	};

	struct txout_t{
		std::string txid;
		unsigned int n;
	};

	struct vin_t: txout_t{
		scriptSig_t scriptSig;
		unsigned int sequence;
	};

	struct vout_t{
		double value;
		unsigned int n;
		scriptPubKey_t scriptPubKey;
	};

	struct decoderawtransaction_t{
		std::string txid;
		int version;
		int locktime;
		std::vector<vin_t> vin;
		std::vector<vout_t> vout;
	};

	/* getrawtransaction return type */
	struct getrawtransaction_t: decoderawtransaction_t{
		std::string hex;
		std::string blockhash;
		unsigned int confirmations;
		unsigned int time;
		unsigned int blocktime;
	};

	/* signrawtransaction return type */
	struct signrawtxin_t: txout_t{
		std::string scriptPubKey;
		std::string redeemScript;
	};

	/* signrawtransaction return type */
	struct signrawtransaction_t{
		std::string hex;
		bool complete;
	};

	/* === Unused yet === */

	struct mininginfo_t{
		int blocks;
		int currentblocksize;
		int currentblocktx;
		double difficulty;
		std::string errors;
		int genproclimit;
		double networkhashps;
		int pooledtx;
		bool testnet;
		bool generate;
		int hashespersec;
	};

#endif
