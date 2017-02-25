// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0,     uint256("0x100944391b1edc3090600c5748a9373665500e80dc2a2e4b74e2168c3252ee47") )
        (  2,     uint256("0x098fa0d430756621e3b23791bb99375fc5d79315ca345c8eef1a69330c56750e") ) // Premine
        ( 50,     uint256("0x1594c1e798b00785772c947672bf10b82f4196e5da8940cedb52a900120bf728") )
        ( 100,    uint256("0x98ae397ba57b54b1b2bf89a63b63bf27f3f15c150eda7aba1b7eff652ab1a78d") )
        ( 150,    uint256("0x4184c1c4aa0c242673f9b82ba70df4fcc1c72e587d78f0fa52c019c066967f6a") )
        ( 1000,   uint256("0xbb784863aeec0d2427d081326305c8e9987a9fed0d678cfdbd8ecc683caf622f") )
        ( 2000,   uint256("0x3172bd74e9a0bea9fdac59a95588b1ba7904b389d8b541fc25eeb1f13738137a") )
        ( 5000,   uint256("0x302626b8a362826614b93ba89993071003bf0e220d2f5c9c3062b8a5fad5de49") )
        ( 6758,   uint256("0x125d4fe9cf6775a1ff75b8b716269b4dad6b9930e48a51b3e6ddaa4351a7cad9") ) // Hardfork to v2.0.0 - hash 4kB
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
        //Sindren
//        return true;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        //Sindren
        return checkpoints.rbegin()->first;
//        return 0;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                //Sindren
                return t->second;
//                return NULL;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
