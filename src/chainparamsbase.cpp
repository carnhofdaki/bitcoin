// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparamsbase.h>

#include <tinyformat.h>
#include <util/system.h>
#include <util/memory.h>

#include <assert.h>

const std::string CBaseChainParams::MAIN = "main";
const std::string CBaseChainParams::TESTNET = "test";
const std::string CBaseChainParams::SIGNET = "signet";
const std::string CBaseChainParams::REGTEST = "regtest";

void SetupChainParamsBaseOptions()
{
    gArgs.AddArg("-chain=<chain>", "Use the chain <chain> (default: main). Reserved values: main, test, regtest. With any other value, a custom chain is used. All regtest-only options are available in custom chains too.", ArgsManager::ALLOW_ANY, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-regtest", "Enter regression test mode, which uses a special chain in which blocks can be solved instantly. "
                 "This is intended for regression testing tools and app development. Equivalent to -chain=regtest.", ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-segwitheight=<n>", "Set the activation height of segwit. -1 to disable. (regtest-only)", ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY, OptionsCategory::DEBUG_TEST);
    gArgs.AddArg("-testnet", "Use the test chain. Equivalent to -chain=test.", ArgsManager::ALLOW_ANY, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-vbparams=deployment:start:end", "Use given start/end times for specified version bits deployment (regtest-only)", ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-signet", "Use the signet chain. Note that the network is defined by the signet_blockscript parameter", ArgsManager::ALLOW_ANY, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-signet_blockscript", "Blocks must satisfy the given script to be considered valid (only for signet networks)", ArgsManager::ALLOW_STRING, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-signet_enforcescript", "Blocks must satisfy the given script to be considered valid (this replaces -signet_blockscript, and is used for opt-in-reorg mode)", ArgsManager::ALLOW_STRING, OptionsCategory::CHAINPARAMS);
    gArgs.AddArg("-is_test_chain", "Whether it's allowed to set -acceptnonstdtxn=0 for this chain or not. Default: 1 (regtest-only)", ArgsManager::ALLOW_ANY | ArgsManager::DEBUG_ONLY, OptionsCategory::CHAINPARAMS);
}

static std::unique_ptr<CBaseChainParams> globalChainBaseParams;

const CBaseChainParams& BaseParams()
{
    assert(globalChainBaseParams);
    return *globalChainBaseParams;
}

std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return MakeUnique<CBaseChainParams>("", 8332);
    else if (chain == CBaseChainParams::TESTNET)
        return MakeUnique<CBaseChainParams>("testnet3", 18332);
    else if (chain == CBaseChainParams::REGTEST)
        return MakeUnique<CBaseChainParams>("regtest", 18443);
    else if (chain == CBaseChainParams::SIGNET)
        return MakeUnique<CBaseChainParams>("signet", 38332);

    return MakeUnique<CBaseChainParams>(chain, 18553);
}

void SelectBaseParams(const std::string& chain)
{
    globalChainBaseParams = CreateBaseChainParams(chain);
    gArgs.SelectConfigNetwork(chain);
}
