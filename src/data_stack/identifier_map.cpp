/*
 * Copyright (c) [2025] [TopHarp]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "identifier_map.h"
#include "hash.h"
#include "error_code.h"
#include "debug_print.h"


std::unordered_map<int, IdentifierToken>::iterator 
        CIdentifierMap::FindID(int hash)
{
    std::unordered_map<int, IdentifierToken>::iterator it;
    it = identifier_map_.find(hash);
    if (it == identifier_map_.end())
    {
        Debug_Error("not found identifier.\n");
        return identifier_map_.end();
    }

    return it;
}

std::unordered_map<int, IdentifierToken>::iterator 
        CIdentifierMap::FindID(std::string id_token_name)
{
    int hash = 
        Common::TokenHash(id_token_name.c_str(), 
                            id_token_name.length());
    if (RETURN_FAIL == hash)
    {
        Debug_Error("hash error.\n");
        return identifier_map_.end();
    }

    // printf("hassh %d name %s ", hash, id_token_name.c_str());

    std::unordered_map<int, IdentifierToken>::iterator it;
    it = identifier_map_.find(hash);
    if (it == identifier_map_.end())
    {
        Debug_Error("not found identifier.\n");
        return identifier_map_.end();
    }

    // printf("findd %s %%hash %d num_tk %d hash %d ", it->second.name.c_str(), 
    //                                     hash, it->second.type,
    //                                     it->second.hash);

    return it;
}

void CIdentifierMap::AddID(int &hash, IdentifierToken &id)
{
    identifier_map_[hash] = id;
    // printf("add %s %%hash %d num_tk %d %d |", id.name.c_str(), 
    //                                     hash, id.type
    //                                     , identifier_map_[hash].type);
    return;
}

std::unordered_map<int, IdentifierToken>::iterator 
        CIdentifierMap::GetEnd()
{
    return identifier_map_.end();
}

void CIdentifierMap::UnwindIdentifierTable(const int &token_type)
{
    for (IdentifierMapItr itr = identifier_map_.begin();
            itr != identifier_map_.end(); itr++)
    {
        if (itr->second.var_class == token_type)
        {
            itr->second.var_class = itr->second.hclass;     ///< 离开函数时出栈 进入函数时入的栈
            itr->second.var_type = itr->second.htype;
            itr->second.value = itr->second.hval;
        }
    }

    return;
}


